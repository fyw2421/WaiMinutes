---
title: "AndroidNotes15 : 协程与 Flow"
weight: 16
description: "Android 侧的协程与 Flow：Dispatchers 注入与作用域选择、collectAsStateWithLifecycle 的正确用法、stateIn 与 shareIn 的语义差异与参数选择、Flow 的异常处理与重试、并发原语 Mutex 与 Flow 的背压，以及协程测试的调度控制。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、Dispatcher 注入

`kotlinx-coroutines-android` 提供三个主调度器，**不要在代码里硬写 `Dispatchers.IO`**：

| 调度器 | 用途 |
| --- | --- |
| `Dispatchers.Main` | UI 更新 |
| `Dispatchers.IO` | 阻塞 IO（网络、磁盘） |
| `Dispatchers.Default` | CPU 密集（纯计算、JSON 解析） |

```kotlin
// 定义限定注解
@Retention(AnnotationRetention.BINARY)
@Qualifier
annotation class IoDispatcher

@Retention(AnnotationRetention.BINARY)
@Qualifier
annotation class DefaultDispatcher

// 注入
@Singleton
class ArticleRepositoryImpl @Inject constructor(
    private val api: ArticleApi,
    @IoDispatcher private val io: CoroutineDispatcher
) {
    // ★这里没有 override——上面这个类没有实现任何接口，
    //   写 override 会报「overrides nothing」
    suspend fun parse(text: String): Article = withContext(io) {
        Json.decodeFromString(text)
    }
}
```

**为什么必须注入**：单元测试里把 `io` 换成 `StandardTestDispatcher`，协程就能在虚拟时间里跑，不需要真的等网络。

⚠ **`Dispatchers.Unconfined` 不要用**。它会让代码的执行顺序变得难以预测，调试时非常痛苦。

# 二、作用域的三种

```kotlin
// 1) ViewModel 里：viewModelScope（随 onCleared 取消）
class ArticleViewModel : ViewModel() {
    fun load() = viewModelScope.launch { /* ... */ }
}

// 2) Composable 里：rememberCoroutineScope（随组合离开取消）
@Composable
fun Detail() {
    val scope = rememberCoroutineScope()
    Button(onClick = { scope.launch { load() } }) { Text("加载") }
}

// 3) 应用级：应用自己的 Application scope（活到进程结束）
@Singleton
class AppScope @Inject constructor() {
    val scope = CoroutineScope(SupervisorJob() + Dispatchers.Default)
    fun track(event: String) { scope.launch { analytics.send(event) } }
}
```

⚠ **`rememberCoroutineScope()` 里的协程会在 Composable 离开组合时取消**——用它做「点一下发起请求」是对的，拿它做「界面没了也要完成的操作」是错的。

# 三、collectAsStateWithLifecycle

```kotlin
// ✅ 正确：生命周期感知，后台不收集
val state by viewModel.uiState.collectAsStateWithLifecycle(
    lifecycle = lifecycle,
    minActiveState = Lifecycle.State.STARTED
)

// ❌ 后台也在收集，白耗电，且可能在后台更新 UI
val state by viewModel.uiState.collectAsState()
```

**`minActiveState` 的选择**：

| 值 | 行为 |
| --- | --- |
| `STARTED`（默认） | 进入前台即收集 |
| `RESUMED` | 只有真正在交互时才收集 |
| `CREATED` | 尽早收集，适合需要预取数据的状态 |

## (一) 生命周期感知流

```kotlin
private val _isOnline = MutableStateFlow(true)
val isOnline: Flow<Boolean> = _isOnline.asStateFlow()
```

```kotlin
// 组合进生命周期里
class NetworkObserver @Inject constructor(
    private val cm: ConnectivityManager
) {
    fun isOnline(): Flow<Boolean> = callbackFlow {
        val callback = object : ConnectivityManager.NetworkCallback() {
            override fun onAvailable(n: Network) { trySend(true) }
            override fun onLost(n: Network) { trySend(false) }
        }
        val request = NetworkRequest.Builder()
            .addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
            .build()

        cm.registerNetworkCallback(request, callback)
        trySend(cm.activeNetwork != null)          // ★先发一次当前值

        awaitClose { cm.unregisterNetworkCallback(callback) }   // ★必须注销
    }
}
```

⚠ **`callbackFlow` 里必须 `awaitClose` 并反注册**，否则回调会一直持有，回调对象泄漏。

# 四、stateIn 与 shareIn

这是 Android 状态管理里最容易搞混的两个操作符。

| | `stateIn` | `shareIn` |
| --- | --- | --- |
| 返回 | `StateFlow<T>` | `SharedFlow<T>` |
| 有初始值 | 必须给 | 可不给 |
| 去重 | 会（按 `equals`） | 不去重 |
| 重订阅时 | 立刻发当前值 | 不一定重发 |
| 典型用途 | **界面状态** | 事件流 |

```kotlin
// ✅ 界面状态：StateFlow
val uiState: StateFlow<ArticleUiState> = flow
    .map { toUiState(it) }
    .stateIn(
        scope = viewModelScope,
        started = SharingStarted.WhileSubscribed(5_000),
        initialValue = ArticleUiState()
    )

// ✅ 事件流：SharedFlow
val events: SharedFlow<ArticleEvent> = _events.asSharedFlow()
```

## (一) SharingStarted 的三种取值

| 值 | 行为 | 用在哪 |
| --- | --- | --- |
| `WhileSubscribed(5000)` | 最后一个订阅者取消后 5 秒才停 | **界面状态的标准选择** |
| `WhileSubscribed(0)` | 立刻停 | 需要严格省资源时 |
| `Eagerly` | 从不停止 | 全局事件总线 |
| `Lazily` | 启动后一直跑 | 后台持续任务 |

```kotlin
// ★5 秒的缓冲能挡掉配置变更造成的取消-重订阅
.stateIn(viewModelScope, SharingStarted.WhileSubscribed(5_000), ArticleUiState())
```

⚠ **缓冲时长要略大于重组的抖动**。写成 0 会在每次旋转时重启上游，白白重发网络请求。

⚠ **`WhileSubscribed` 不是 `Lazy`**：如果没有任何订阅者，上游根本不会启动——这正是它的意义（没人看就不干活）。

## (二) 上游会重启

```kotlin
val uiState = flowOf(1, 2, 3)
    .stateIn(scope, SharingStarted.WhileSubscribed(5_000), 0)

// 有订阅者时会收到 1, 2, 3
// 全部取消订阅后，上游取消
// 再次订阅 → ★★ 上游重新执行，又发一遍 1, 2, 3
```

⚠ **上游含副作用（发请求、写库）时要格外小心**。要么把副作用放在 `onStart` 之外，要么改用 `Eagerly`。

# 五、Flow 的异常处理

**上游异常会终止整个流**，必须显式处理：

```kotlin
// ★ 注意 emit(emptyList()) 发射的是 List，stateIn 的初始值也必须是同一个类型
val articles: StateFlow<List<Article>> = repository.observeArticles()
    .catch { e ->
        if (e is IOException) {
            emit(emptyList())              // 降级为空数据
        } else {
            throw e                        // 其他异常继续抛
        }
    }
    .stateIn(
        scope = viewModelScope,
        started = SharingStarted.WhileSubscribed(5_000),
        initialValue = emptyList()         // ★必须是 List<Article>，不能是 ArticleUiState()
    )
```

要输出 `ArticleUiState` 的话，`map` 要放在 `catch` **之前**，这样 `catch` 降级发射的 `emptyList()` 也会被转成状态：

```kotlin
val uiState: StateFlow<ArticleUiState> = repository.observeArticles()
    .map { toUiState(it) }
    .catch { e -> if (e is IOException) emit(ArticleUiState()) else throw e }
    .stateIn(viewModelScope, SharingStarted.WhileSubscribed(5_000), ArticleUiState())
```

⚠ **`catch` 只捕获它「上面」的异常**，也就是链上排在它之前的操作符：

```kotlin
flow { emit(1) }
    .map { throw RuntimeException("上游") }    // ★在 catch 之前 —— 会被捕获
    .catch { e -> /* ✅ 能接住 */ }

// ❌ 想演示「下游捕获不到」，异常必须放在 catch 之后
flow { emit(1) }
    .catch { e -> /* 抓不到下面这个 */ }
    .map { throw RuntimeException("下游") }
```

⚠ **下游抛出的异常会直接终止整个收集过程**，不会流回 `catch`。要兜住它，得在 `collect` 处用 `try/catch`。

重试要自己写：

```kotlin
fun <T> Flow<T>.retryWithBackoff(
    retries: Int = 3,
    initialDelay: Long = 1000
): Flow<T> = retryWhen { cause, attempt ->
    if (attempt >= retries || cause !is IOException) return@retryWhen false
    delay(initialDelay * (1L shl attempt.toInt()))   // 指数退避
    true
}
```

⚠ **别无限重试**。服务端返回 500 时无脑重试会打垮自己的服务，也耗尽用户流量。

# 六、并发原语

## (一) Mutex

```kotlin
class Counter {
    private val mutex = Mutex()
    private var count = 0

    suspend fun increment() {
        mutex.withLock {          // ★互斥进入
            count++
        }
    }
}
```

⚠ **`Mutex` 是挂起函数**，必须配 `withLock`。`@Synchronized` 在协程里能用，但会阻塞线程。

⚠ **只在真正有竞争时用**。`Mutex` 本身有开销，单线程场景（如都在 `viewModelScope` 里）不需要。

## (二) Flow 的背压

Flow 默认是**顺序执行**的（`collect` 会等上游 `emit` 完再继续），这天然提供了背压。但 `flowOn` 会引入缓冲：

```kotlin
flow {
    emit(compute())      // 慢
}.flowOn(Dispatchers.Default)      // ★这里有缓冲
 .collect {
    send(result)         // 快
 }
```

⚠ **`flowOn` 会切换上游的上下文并引入通道缓冲**。上游极快、下游极慢时（比如批量写库），缓冲会堆积——这时考虑 `buffer(capacity)` 显式控制。

## (三) 分段并行

```kotlin
// 无序并发，只要速度
coroutineScope {
    ids.map { id -> async { repository.fetch(id) } }.awaitAll()
}

// 保序并发（awaitAll 本身就按 Deferred 的顺序返回）
coroutineScope {
    val tasks = ids.map { id -> async { repository.fetch(id) } }
    tasks.awaitAll()
}
```

⚠ **裸的 `async {}` 必须有接收者**——它不是顶层函数，得包在 `coroutineScope { }`、`viewModelScope` 或某个 scope 里，否则编译不过。

⚠ **真正让并行退化成串行的是 `CoroutineStart.LAZY`**：

```kotlin
// ❌ LAZY 的 async 启动后不执行，等到 await 才开始 → 全串行
coroutineScope {
    ids.map { id -> async(start = CoroutineStart.LAZY) { fetch(id) } }
        .map { it.await() }
}

// ✅ 默认的 DEFAULT 是立即启动的
coroutineScope {
    ids.map { id -> async { fetch(id) } }.awaitAll()
}
```

⚠ **`map { it.await() }` 编译不过**——`await()` 是挂起函数，而 `map` 的 lambda 不是挂起上下文。要么用 `awaitAll()`，要么在挂起作用域里 `forEach { it.await() }`。

# 七、协程测试

```kotlin
@Test
fun `上游重试两次后成功`() = runTest {
    var attempt = 0
    val flow = flow {
        attempt++
        if (attempt < 3) throw IOException("失败")
        emit("成功")
    }.retryWithBackoff(retries = 3, initialDelay = 1000)

    val result = flow.toList()

    assertEquals("成功", result.first())
    assertEquals(3, attempt)
    // ★虚拟时间，initialDelay 的 1 秒没真花时间
    assertEquals(1000 + 2000, currentTime)    // 1s + 2s 的退避
}
```

**关键点**：

| API | 作用 |
| --- | --- |
| `runTest` | 虚拟时钟，自动跳过 `delay` |
| `advanceUntilIdle()` | 跑完所有挂起协程 |
| `advanceTimeBy(ms)` | 推进指定时间 |
| `runCurrent()` | 只跑当前就该执行的任务 |
| `StandardTestDispatcher` | 默认，按虚拟时间排队 |
| `UnconfinedTestDispatcher` | 立即执行，用于测试启动顺序 |

⚠ **`Dispatchers.setMain()` 只在 UI 测试里需要**。纯 ViewModel 测试用 `runTest` 就行——`viewModelScope` 用 `Dispatchers.Main.immediate`，`runTest` 会自动接管。

# 八、易错点

| 误区 | 事实 |
| --- | --- |
| 硬编码 `Dispatchers.IO` | 注入它，否则无法在测试里替换 |
| `Dispatchers.Default` 干所有活 | Default 是 CPU 密集，IO 是阻塞调用，混用会浪费线程池 |
| `collectAsState` 就够了 | 要用 `collectAsStateWithLifecycle`，后台不收集 |
| `WhileSubscribed(0)` 更快 | 缓冲要大于重组抖动，否则配置变更就重发请求 |
| `catch` 能捕获下游异常 | 只能捕获上游 |
| `List.map { async {} }` 是并行的 | 必须先建好所有 async 再 `awaitAll` |
| `Mutex` 到处都要加 | 没竞争的地方加了纯是开销 |
| `callbackFlow` 不用 `awaitClose` | 会泄漏回调注册 |

# 九、参考资料

- [Android 协程最佳实践](https://developer.android.com/kotlin/coroutines) — 官方推荐的协程用法
- [Kotlin Flow](https://developer.android.com/kotlin/flow) — 流在 Android 中的用法
- [协程测试](https://developer.android.com/kotlin/coroutines/test) — `runTest` 与调度控制
