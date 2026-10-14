---
title: "AndroidNotes14 : ViewModel"
weight: 14
description: "ViewModel 的完整用法：viewModelScope 与生命周期、参数注入与 SavedStateHandle、状态容器与配置变更的保存时机、onEvent 事件入口、ViewModel 的测试方法，以及 Factory 与 assisted injection 的取舍。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、ViewModel 是什么

`ViewModel` 是一个**与界面生命周期解耦的状态容器**。它活到界面销毁之后，配置变更（旋转）不会重建它。

```kotlin
class ArticleViewModel(
    private val repository: ArticleRepository
) : ViewModel() {

    private val _uiState = MutableStateFlow(ArticleUiState())
    val uiState: StateFlow<ArticleUiState> = _uiState.asStateFlow()

    init {
        refresh()
    }
}
```

## (一) 为什么旋转屏幕不会丢

| 事件 | ViewModel | Composable 里的 `remember` | `rememberSaveable` |
| --- | --- | --- | --- |
| 重组 | 保留 | 保留 | 保留 |
| 旋转屏幕 | **保留** | **丢失** | 保留 |
| 切到后台再回来 | 保留 | 保留 | 保留 |
| 进程被杀后重建 | 丢失 | 丢失 | 保留 |

⚠ **ViewModel 挡不住进程被杀**。用户从最近任务里划掉 App、或系统内存紧张回收进程，ViewModel 也会没。需要跨进程重启保存的东西（搜索词、滚动位置、滚动到的 id）要用 `SavedStateHandle`。

# 二、viewModelScope

**所有 ViewModel 里的协程都应该用 `viewModelScope`**：

```kotlin
class ArticleViewModel(private val repository: ArticleRepository) : ViewModel() {

    private val _uiState = MutableStateFlow(ArticleUiState())
    val uiState = _uiState.asStateFlow()

    fun onEvent(event: ArticleEvent) {
        when (event) {
            is ArticleEvent.Refresh -> viewModelScope.launch {   // ★viewModelScope
                _uiState.update { it.copy(loading = true) }
                runCatching { repository.refresh() }
                    .onSuccess { _uiState.update { s -> s.copy(loading = false) } }
                    .onFailure { e -> _uiState.update { s -> s.copy(loading = false, error = e.message) } }
            }
        }
    }

    override fun onCleared() {
        super.onCleared()
        // ViewModel 销毁时 viewModelScope 自动取消，这里不用手动 cancel
    }
}
```

⚠ **不要自己造 `CoroutineScope()`**。`viewModelScope` 在 `onCleared()` 时自动取消所有子协程，自建的 scope 不会——这正是协程泄漏的常见来源。

⚠ **进程被杀时 `onCleared()` 不会被调用**。所以别把「重要数据落盘」只放在 `onCleared()` 里，要主动存。

# 三、参数注入

## (一) SavedStateHandle

```kotlin
@HiltViewModel
class ArticleDetailViewModel @Inject constructor(
    private val repository: ArticleRepository,
    private val savedState: SavedStateHandle          // ★框架自动注入
) : ViewModel() {

    val articleId: Long = checkNotNull(savedState["articleId"])

    init {
        viewModelScope.launch {
            repository.getArticle(articleId)
        }
    }

    fun rememberScroll(offset: Int) {
        savedState["scrollOffset"] = offset         // ★旋转后仍在
    }
}
```

⚠ **`SavedStateHandle` 只能存 `Bundle` 能装的东西**：基本类型、`Parcelable`、`Serializable`、`ArrayList`。存不可 Bundle 化的类型会抛 **`IllegalArgumentException`**（"Cannot put object with type ... into saved state"），部分路径会静默丢弃。

`TransactionTooLargeException` 是 Binder 事务超过 1MB 时的异常，与 `SavedStateHandle` 无关。

## (二) 从导航参数取

```kotlin
// Navigation 3 里把参数直接放进 backStack key
@Serializable
data class ArticleDetail(val id: Long)

@Composable
fun ArticleDetailRoute(id: Long) {
    val vm: ArticleDetailViewModel = hiltViewModel()
    // ★Navigation 3 没有 NavBackStackEntry，也没有 arguments，
    //   所以 hiltViewModel() 拿不到导航参数。id 要么从 key 显式传进 Composable，
    //   要么走 assisted injection（第 254 节）
}
```

⚠ **`hiltViewModel()` 的默认 key 是 `null`**，最终落到 `ViewModelProvider` 的默认 key（ViewModel 的类名）；`viewModelStoreOwner` 参数（默认 `LocalViewModelStoreOwner.current`）只决定「存进哪个 store」，**不是 key**。

所以：**同一个 store 内**重复取同一类型才会复用。要在同一 store 里放多份实例才需要显式传 key：

```kotlin
val vm: ArticleDetailViewModel = hiltViewModel(key = "article_$id")
```

⚠ 多个界面拿到同一个 ViewModel 实例，根因通常不是「忘了传 key」，而是 **store 是共享的**——比如 Navigation 3 里忘了加 `rememberViewModelStoreNavEntryDecorator()`（第 16 篇），所有 entry 共用 Activity 的 store，这时传不传 key 都一样。

# 四、状态容器选哪种

| 容器 | 保留重组 | 保留配置变更 | 观察者 |
| --- | --- | --- | --- |
| `mutableStateOf` + `remember` | 是 | 否 | 1 个界面 |
| `mutableStateOf` + `rememberSaveable` | 是 | 是 | 1 个界面 |
| `ViewModel` + `StateFlow` | 是 | **是**（进程未死） | 多处 |

**规则：界面本地状态（弹窗开关、输入框内容）用 `remember`；业务状态用 ViewModel**。

```kotlin
// ✅ ViewModel 持有业务状态
val state by viewModel.uiState.collectAsStateWithLifecycle()

// ✅ Composable 只持有纯 UI 状态
var showFilter by rememberSaveable { mutableStateOf(false) }
```

⚠ **业务状态放 Composable 里用 `rememberSaveable` 是个陷阱**——旋转不丢，但跨界面跳转就不共享了，重复加载的问题还在。

# 五、事件入口

事件统一从 `onEvent` 进，别给每个动作开一个方法：

```kotlin
// ❌ 方法会无限膨胀
fun onRefresh() {}
fun onTabSelected(tab: Tab) {}
fun onDelete(id: Long) {}
fun onRetry() {}

// ✅ 一个密封类做入口
sealed interface ArticleEvent {
    data object Refresh : ArticleEvent
    data class SelectTab(val tab: Tab) : ArticleEvent
    data class Delete(val id: Long) : ArticleEvent
    data object Retry : ArticleEvent
    data class Search(val query: String) : ArticleEvent
}
```

这样好处是：能在一个地方打日志、能穷举检查、测试时只需测一个方法。

⚠ **事件用一次性消费（如 Toast、导航）时不要放进 State**。State 是持久状态，反复消费会重复触发：

```kotlin
// ✅ 一次性副作用用 Channel
private val _messages = Channel<String>(Channel.BUFFERED)
val messages = _messages.receiveAsFlow()

fun onDeleteConfirmed() {
    viewModelScope.launch { _messages.send("已删除") }
}
```

# 六、ViewModel 的测试

ViewModel 不需要任何 Android 依赖就能测——这是它最大的价值。

```kotlin
@Test
fun `首次加载时显示加载中`() = runTest {
    val repo = FakeArticleRepository(delay = 500)
    val vm = ArticleViewModel(repo)

    assertEquals(LoadState.Loading, vm.uiState.value.listState)
}

@Test
fun `加载成功后状态为 Success`() = runTest {
    val repo = FakeArticleRepository(articles = sampleArticles)
    val vm = ArticleViewModel(repo)
    advanceUntilIdle()

    val state = vm.uiState.value.listState
    assertTrue(state is LoadState.Success)
    assertEquals(sampleArticles.size, (state as LoadState.Success).data.size)
}

@Test
fun `加载失败时保留已有数据`() = runTest {
    val repo = FakeArticleRepository(error = SocketTimeoutException())
    val vm = ArticleViewModel(repo)
    advanceUntilIdle()

    assertTrue(vm.uiState.value.listState is LoadState.Error)
}
```

**三个测试要点**：

1. 用 `runTest` 而不是 `runBlocking`——它是虚拟时钟，不真的等待
2. 用 `advanceUntilIdle()` 让挂起的协程跑完
3. **不要 mock ViewModel 自己**，mock 它依赖的 Repository

# 七、Factory 与 Assisted Injection

## (一) 多参数时用 assisted injection

```kotlin
// ★Hilt 的 assisted ViewModel 必须标 @HiltViewModel，否则不进 ViewModelComponent
@HiltViewModel
class ArticleDetailViewModel @Inject constructor(
    private val repository: ArticleRepository
) : ViewModel() {

    @AssistedInject
    constructor(repository: ArticleRepository, private val articleId: Long) : this(repository) {
        load(articleId)
    }

    @AssistedFactory
    interface Factory {
        fun create(articleId: Long): ArticleDetailViewModel
    }
}
```

```kotlin
@Composable
fun ArticleDetailRoute(id: Long) {
    // ★不能这么写：hiltViewModel() 是 @Composable，不能在 remember {} 的
    //   非 suspend lambda 里调用；而且它走 @Inject 构造器，
    //   永远不会用 @AssistedFactory。remember 创建的实例也不受 ViewModelStore
    //   管理，onCleared() 不会被调
    val vm: ArticleDetailViewModel = viewModel(
        factory = viewModelFactory {
            initializer {
                val hiltEntryPoint = EntryPointAccessors.fromApplication(
                    LocalContext.current.applicationContext as Application,
                    HiltAssistedFactoryEntryPoint::class.java
                )
                hiltEntryPoint.assistedFactory().create(id)
            }
        }
    )
}
```

⚠ **能用 `SavedStateHandle` 就别用 assisted**。多写一个工厂类的收益远小于复杂度增加，`SavedStateHandle` 还能顺带解决进程恢复。

## (二) 老式 Factory

```kotlin
class MyViewModelFactory(private val repo: ArticleRepository) : ViewModelProvider.Factory {
    // ★create(modelClass: Class<T>) 在 lifecycle 2.9+ 已废弃，
    //   改实现带 CreationExtras 的重载
    override fun <T : ViewModel> create(
        modelClass: Class<T>,
        extras: CreationExtras
    ): T {
        return ArticleViewModel(repo) as T
    }
}
```

⚠ **这是 KAPT 时代的写法**。用 Hilt 就用 `hiltViewModel()`，别再手写 Factory——除非你完全不用 Hilt。

# 八、常见问题

| 现象 | 原因 |
| --- | --- |
| 旋转后数据没了 | 状态放进了 Composable 的 `remember`，应放 ViewModel |
| 从后台回来数据没了 | 进程被杀了，用 `SavedStateHandle` |
| 导航到第二篇还是第一篇的内容 | `hiltViewModel()` 没传 key |
| ViewModel 里的协程泄漏 | 用了自建 `CoroutineScope` 而非 `viewModelScope` |
| Toast 弹了两次 | 一次性事件放进了 State，被重复消费 |
| 单元测试跑不起来 | ViewModel 里用了 Android 依赖，越界了 |
| 构造函数报「没有无参构造」 | 用 Hilt 的 `@HiltViewModel` + `@Inject`，别手写 |

# 九、易错点

| 误区 | 事实 |
| --- | --- |
| ViewModel 能扛住进程被杀 | 不能，被杀就丢，重要状态用 `SavedStateHandle` |
| `onCleared()` 一定会被调 | 进程被杀时不调，别只在那里存数据 |
| ViewModel 里可以持有 View | 会泄漏，ViewModel 必须能在无 View 时存活 |
| `hiltViewModel()` 每次都是新实例 | 默认按 ViewModelStoreOwner 复用，同图内共享 |
| 事件应该放进 UiState | 一次性副作用用 Channel，否则会重复触发 |
| 测试要 mock ViewModel | 应该 mock 它依赖的 Repository |
| 多参数就得写 Factory | 先考虑 `SavedStateHandle` |

# 十、参考资料

- [ViewModel API 参考](https://developer.android.com/reference/kotlin/androidx/lifecycle/ViewModel) — 官方 API 与生命周期说明
- [Android 架构蓝图中的 ViewModel](https://github.com/android/nowinandroid) — 官方参考实现里的实际用法
- [Lifecycle 组件发布记录](https://developer.android.com/jetpack/androidx/releases/lifecycle) — `viewModelScope`、`SavedStateHandle`、`collectAsStateWithLifecycle` 所在库的版本与变更
