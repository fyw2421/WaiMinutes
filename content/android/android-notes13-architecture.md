---
title: "AndroidNotes13 : 分层与 UDF"
weight: 14
description: "Android 推荐架构：UI 层与数据层的职责边界、UDF 单向数据流的输入输出与事件处理、状态建模与不可变数据结构、Repository 模式与数据源分层、依赖倒置的真实收益，以及各层之间该传什么不该传什么。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、官方推荐的分层

Google 官方推荐的两层架构（不是 MVC，也不是 MVVM）：

```text
┌─────────────────────────────────────────────┐
│  UI 层（View / Composable）                  │
│  Compose Composable + ViewModel               │
│  职责：把状态渲染成界面，把用户操作变成事件    │
└──────────────────┬──────────────────────────┘
                   │  数据流向上 / 事件向下
┌──────────────────▼──────────────────────────┐
│  数据层（Repository）                         │
│  职责：提供应用需要的数据，屏蔽数据来源细节    │
│  内部：网络 API、数据库、本地存储             │
└─────────────────────────────────────────────┘
```

**只有两层**。官方文档里明确说过，**不要给单 App 应用强加「领域层」**——只有存在多个 App 共享同一套业务逻辑时，领域层才值得。

## (一) 每层的判断标准

| 层 | 该做 | 不该做 |
| --- | --- | --- |
| UI 层 | 渲染状态、处理用户事件、格式化显示 | 发网络请求、碰数据库 |
| ViewModel | 持有 UI 状态、调用 Repository、启动协程 | 操作 View、解析 JSON |
| Repository | 聚合数据源、做业务判断、返回领域模型 | 知道任何 Compose/View 的事 |

⚠ **ViewModel 里出现 `TextView`、`Context`、`Uri` 就说明越界了**。ViewModel 应当能在没有 Android 依赖的情况下被单元测试。

# 二、UDF：单向数据流

UDF（Unidirectional Data Flow）的核心只有一句话：**状态向下流，事件向上流**。

```text
        ┌──────────────┐
事件 ──→│              │
        │  ViewModel   │──→ 状态 ──→ Composable
        │              │
        └──────┬───────┘
               │
               ↓
          Repository
```

```kotlin
// ViewModel：持有状态，接收事件
class ArticleViewModel(
    private val repository: ArticleRepository
) : ViewModel() {

    private val _uiState = MutableStateFlow(ArticleUiState())
    val uiState: StateFlow<ArticleUiState> = _uiState.asStateFlow()

    // ★事件统一入口
    fun onEvent(event: ArticleEvent) {
        when (event) {
            is ArticleEvent.Refresh -> refresh()
            is ArticleEvent.SelectTab -> _uiState.update { it.copy(tab = event.tab) }
            is ArticleEvent.Delete -> delete(event.id)
        }
    }

    private fun refresh() {
        viewModelScope.launch {
            _uiState.update { it.copy(loading = true, error = null) }
            // ★不要用 runCatching：它会把 CancellationException 一并捕获，
            //   协程被取消后仍会继续执行 onFailure，破坏结构化并发
            try {
                val list = repository.getArticles()
                _uiState.update {
                    it.copy(loading = false, articles = list, error = null)
                }
            } catch (e: CancellationException) {
                throw e                        // ★取消必须原样抛出
            } catch (e: Exception) {
                _uiState.update { it.copy(loading = false, error = e.message) }
            }
        }
    }
}
```

```kotlin
// Composable：只渲染状态、上报事件
@Composable
fun ArticleRoute(viewModel: ArticleViewModel = hiltViewModel()) {
    val state by viewModel.uiState.collectAsStateWithLifecycle()
    ArticleScreen(state = state, onEvent = viewModel::onEvent)
}

@Composable
fun ArticleScreen(state: ArticleUiState, onEvent: (ArticleEvent) -> Unit) {
    // 只读 state，不持有自己的业务状态
}
```

## (一) 为什么不是双向绑定

双向绑定（数据变更自动写回 UI，UI 变更自动写回数据）在小项目里省事，但规模一大就崩：

| | 双向绑定 | UDF |
| --- | --- | --- |
| 数据流向 | 双向，难追踪 | 单向，可追踪 |
| 状态来源 | 散落在各处 | 集中在 ViewModel |
| 调试 | 不知道是谁改的 | `Log` 一个入口 |
| 测试 | 需要模拟整个视图 | 只测 ViewModel |

**Compose 本身就是单向的**——它没有 `setText` 回写模型这种机制，这是设计使然。

# 三、状态建模

## (一) 一个状态类装所有

```kotlin
data class ArticleUiState(
    val articles: List<Article> = emptyList(),
    val loading: Boolean = false,
    val error: String? = null,
    val tab: Tab = Tab.LATEST,
    val query: String = "",
    val selectedId: Long? = null
)
```

**把所有 UI 相关状态放进一个类**，不要散成一堆 `State`。这样重组时只读一个对象，也便于整体比较是否变化。

## (二) 加载状态用 sealed 而不是三个字段

```kotlin
// ❌ 三个布尔量组合出 8 种状态，其中一半是非法的
data class State(
    val loading: Boolean,
    val data: List<Article>?,
    val error: String?
)

// ✅ 合法状态只有三种，且必须穷举处理
sealed interface LoadState<out T> {
    data object Loading : LoadState<Nothing>
    data class Success<T>(val data: T) : LoadState<T>
    data class Error(val message: String, val retryable: Boolean) : LoadState<Nothing>
}
```

**这就是 `sealed` 的价值**——把「非法状态」在类型层面消灭掉。

但注意：**页面级的异步状态**用 sealed，**可编辑的表单状态**用扁平类。整页用一个 `ArticleUiState` data class，异步加载部分嵌套一个 sealed：

```kotlin
data class ArticleUiState(
    val listState: LoadState<List<Article>> = LoadState.Loading,
    val tab: Tab = Tab.LATEST,
    val query: String = ""
)
```

⚠ **别把所有东西都塞进 sealed**。用户正在输入的 `query` 不需要「加载中/失败」三态，硬套会让代码很难写。

## (三) 状态必须是不可变的

```kotlin
data class ArticleUiState(
    val articles: List<Article> = emptyList()
)

// ✅ 整体替换
_state.value = _state.value.copy(articles = newList)

// ❌ 原地修改——Compose 感知不到
_state.value.articles.add(article)
```

⚠ **`List` 本身是不可变的但内容可以变**。存 `mutableListOf()` 然后 `.add()`，Compose 的快照系统不会记录到这个修改（除非通过 SnapshotStateList）。统一用 `List<T>` + 整体替换。

# 四、Repository

## (一) 接口定在数据层

```kotlin
interface ArticleRepository {
    suspend fun getArticles(forceRefresh: Boolean = false): List<Article>
    suspend fun getArticle(id: Long): Article
    suspend fun delete(id: Long)
    suspend fun refresh()                        // ★显式声明，下面 Impl 才覆写得动
    fun observeArticles(): Flow<List<Article>>   // 持续观察（本地为数据源时）
}
```

```kotlin
@Singleton
class ArticleRepositoryImpl @Inject constructor(
    private val api: ArticleApi,
    private val dao: ArticleDao,
    @IoDispatcher private val io: CoroutineDispatcher
) : ArticleRepository {

    override suspend fun getArticles(forceRefresh: Boolean): List<Article> =
        withContext(io) {
            if (forceRefresh) {
                val fresh = api.fetchArticles()
                dao.replaceAll(fresh)       // 写本地
                fresh
            } else {
                dao.getAll()               // 读本地
            }
        }

    override fun observeArticles(): Flow<List<Article>> =
        dao.observeAll()                    // Room 的 Flow 会随数据变化自动推送

    // ★接口有 5 个方法，实现类必须全覆写，否则编译不过
    override suspend fun getArticle(id: Long): Article =
        withContext(io) { dao.findById(id) }

    override suspend fun delete(id: Long) = withContext(io) { dao.deleteById(id) }

    override suspend fun refresh() = withContext(io) {
        dao.replaceAll(api.fetchArticles())
    }
}
```

## (二) 界面只依赖接口

```kotlin
class ArticleViewModel @Inject constructor(
    private val repository: ArticleRepository      // ★接口，不是 Impl
) : ViewModel()
```

⚠ **依赖注入的是接口，不影响单测**：

```kotlin
@Test
fun `加载失败时状态为 Error`() = runTest {
    val fakeRepo = FakeArticleRepository(error = IOException("网络异常"))
    val vm = ArticleViewModel(fakeRepo)

    vm.onEvent(ArticleEvent.Refresh)
    advanceUntilIdle()

    val state = vm.uiState.value.listState
    assertTrue(state is LoadState.Error)
}
```

这就是「依赖倒置」的真实收益——**不是为了架构好看，是为了可测试**。

## (三) 网络为缓存还是缓存为网络

两种模式，要按业务选：

| | 网络优先 | 本地优先（Room 为单一数据源） |
| --- | --- | --- |
| 流程 | 请求网络 → 返回结果 → 更新缓存 | 读本地 → 同时请求网络 → 写本地推送 |
| 首屏速度 | 慢 | 快 |
| 离线可用 | 否 | 是 |
| 实现复杂度 | 低 | 高 |
| 适合 | 管理后台、数据实时性高 | 内容类 App、需要离线 |

**内容类 App 应该用本地优先**——用户看到的是数据库里的数据，网络回来后 Flow 自动推送更新，这是 Compose + Flow 最舒服的场景。

```kotlin
override fun observeArticles(): Flow<List<Article>> =
    dao.observeAll()                       // UI 永远观察数据库

override suspend fun refresh() = withContext(io) {
    val fresh = api.fetchArticles()
    dao.replaceAll(fresh)                  // 写入后 Flow 自动推送
}
```

# 五、各层传什么

| 内容 | 该在哪 | 不该在哪 |
| --- | --- | --- |
| `Article`（领域模型） | 数据层定义 | UI 不该 import 数据库实体 |
| `ArticleEntity`（表实体） | 数据层内部 | 不出 Repository |
| `ArticleUiState` | ViewModel 定义 | Composable 不该定义 |
| `Json` / `Cursor` / `Response` | 数据层内部 | 绝对不出数据层 |
| `Context` | UI 层 | ViewModel、Repository |

## (一) 映射放哪

```kotlin
// 在 Repository 内部把 Entity 映射成领域模型
private fun ArticleEntity.toDomain(): Article = Article(
    id = id,
    title = title,
    author = author,
    publishedAt = publishedAt
)
```

⚠ **不要用同一个类当数据库实体和界面模型**。数据库表加了 `sync_status`、`local_version` 字段，界面就会被迫处理这些它不关心的东西。分层隔离掉。

# 六、依赖倒置的取舍

三层（UI / ViewModel / Repository）之间，**依赖应该指向内层**：

```text
UI → ViewModel → Repository 接口 ← Repository 实现
```

但**不是所有东西都值得抽接口**。判断标准：

| 情况 | 抽接口？ |
| --- | --- |
| 需要在测试里替换 | 抽 |
| 可能有第二个实现 | 抽 |
| 只有一个实现且不会变 | **别抽** |

⚠ **过度抽象的代价是真实的**：几十个只有一行实现的接口，改一处需求要动五个文件。**Repository 该抽（因为要测），工具类不该抽**。

# 七、常见反模式

| 反模式 | 问题 | 正确做法 |
| --- | --- | --- |
| ViewModel 里 `view.findViewById` | 泄漏 View | 状态提升到 Composable |
| Composable 里直接调 Retrofit | 逻辑散落、无法复用 | 经 ViewModel |
| Composable 里 `remember { MutableStateFlow(...) }` 存业务状态 | 旋转屏幕丢、跨屏不同步 | 放 ViewModel |
| Repository 返回 `Result<T>` | Kotlin 有内置 `Result`，且异常被吞 | 抛异常，在 ViewModel 捕获 |
| 一个 ViewModel 管整个 App | 状态巨大、重组范围失控 | 按屏幕拆 |
| ViewModel 持有 `Activity` 引用 | 内存泄漏 | 只用 `ViewModel` |

⚠ **Composable 里的业务状态必须提升**。这是 Compose 架构的第一原则——一旦在 Composable 里 `remember` 了业务状态，它就脱离了 ViewModel 的生命周期管理。

# 八、易错点

| 误区 | 事实 |
| --- | --- |
| MVVM 是官方推荐架构 | 官方推荐的是「UI 层 + 数据层」两层，不强推 MVVM 术语 |
| 每个 App 都该有领域层 | 单App 不需要，官方明确说过 |
| 状态用 sealed 更好 | 页面级异步状态适合，可编辑表单状态不适合 |
| 数据类默认浅拷贝 | `List` 字段共用同一个引用，要整体替换而非改内容 |
| Repository 必须抽接口 | 为了可测性抽；只有一个实现的工具类不必抽 |
| ViewModel 能碰 View | 持有 View 引用必然泄漏 |
| `Result<T>` 比抛异常好 | Kotlin 有内置 `Result`，且自定义的容易吞异常 |

# 九、参考资料

- [应用架构推荐](https://developer.android.com/topic/architecture/recommendations) — 官方架构原则：UDF、状态容器、Repository
- [应用架构指南](https://developer.android.com/topic/architecture) — 分层、最佳实践、示例应用
- [Android 架构蓝图](https://github.com/android/nowinandroid) — 官方参考实现，完整展示了各层的实际写法
