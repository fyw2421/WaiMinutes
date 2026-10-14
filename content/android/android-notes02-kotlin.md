---
title: "AndroidNotes02 : Kotlin 速览"
weight: 2
description: "面向 Android 的 Kotlin 要点：空安全与 elvis、字符串模板与默认参数、数据类与密封类、作用域函数与返回值对照、集合操作式写法、协程与 Flow、扩展函数与内联、SAM 转换，以及与 Java 互操作的注解桥接。"
featureimage: "covers/android-basics.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、空安全

Kotlin 把「可能为 null」编码进类型里。编译器强制你处理它，**`NullPointerException` 从运行时崩溃变成了编译期错误**。

```kotlin
var name: String? = null      // 显式可空
var age: Int = 30             // 默认不可空

// val a: Int = null          // 编译错误：类型不匹配
```

## (一) elvis 与安全调用

```kotlin
val name: String? = loadName()

val len1 = name?.length               // 安全调用：name 为 null 时整个表达式为 null
val len2 = name?.length ?: 0           // elvis：为 null 时取右边
val len3 = requireNotNull(name).length // 断言非空，否则抛 IllegalArgumentException
```

实际工程里最常用的是**早返回**——把可空参数在函数开头一次收窄，之后整个函数体都不用再判空：

```kotlin
fun submit(user: User?, draft: Draft): Boolean {
    if (user == null) return false        // 此处 user 已收窄为非空
    if (draft.title.isBlank()) return false

    api.upload(user.token, draft)          // 放心用
    return true
}
```

四种「断言非空」的取舍：

| 写法 | null 时 | 用途 |
| --- | --- | --- |
| `!!` | 抛 `NullPointerException` | **几乎不要用**，它关掉了检查 |
| `checkNotNull(x)` | 抛 `IllegalStateException` | 状态不符合预期 |
| `requireNotNull(x)` | 抛 `IllegalArgumentException` | 入参不合法 |
| `x ?: return` | 正常返回 | 最常用，交给调用方兜底 |

## (二) 智能转换

编译器能推断「此处一定非空」，从而免掉强转：

```kotlin
fun describe(x: Any) {
    if (x !is String) return
    println(x.length)               // 此分支内 x 已是 String，无需 as
}
```

Kotlin 1.7 起支持**复杂条件**的智能转换，这是实际代码里最省事的一处：

```kotlin
fun normalize(raw: String?): String {
    // 复合条件：x 非空 且 长度 ≥ 3
    if (raw != null && raw.length >= 3) {
        return raw.lowercase()      // 此处 raw 已自动收窄
    }
    return ""
}
```

⚠ **智能转换不跨越会修改状态的调用**。下面这段编译不过：

```kotlin
// ❌ properties 可以被别处改掉，编译器无法保证转换后仍是同一个值
if (x !is String) return
println(x.length)
val y = doSomething()      // 这一行之后，x 的智能转换失效
println(x.length)          // ★ 这里需要重新判定

// ✅ 提升到局部变量，转换就此固定
val s = x as? String ?: return
val y = doSomething()
println(s.length)          // 安全
```

# 二、字符串与函数

## (一) 字符串模板

```kotlin
val name = "Alice"
val count = 3

"$name 有 $count 条消息"                    // Alice 有 3 条消息
"${name.length} 个字符"                     // 嵌套表达式要写 ${}
"余额：${price * 0.8}"                      // 算术也放进去
```

**模板里不要写复杂逻辑**——可读性和性能都会掉。取出来算好再拼：

```kotlin
// ❌ 模板里塞三元和函数调用
val text = "$name ${if (count > 0) "有新消息" else "暂无消息"}"

// ✅ 先算好
val status = if (count > 0) "有新消息" else "暂无消息"
val text = "$name $status"
```

原始字符串用 `"""`，适合写 JSON、SQL、正则——**不用转义引号**：

```kotlin
val sql = """
    SELECT id, title
    FROM articles
    WHERE author = ?
    ORDER BY created_at DESC
    LIMIT 20
""".trimIndent()
```

## (二) 默认参数与命名参数

Android 的 Composable 全靠这个特性，读起来才顺：

```kotlin
fun loadData(
    userId: String,
    page: Int = 1,               // 有默认值
    pageSize: Int = 20,
    forceRefresh: Boolean = false
)

// 调用时可以跳过中间参数
loadData("u1")                                    // page=1, pageSize=20
loadData("u1", pageSize = 50)                     // ★命名参数：任意顺序、任意组合
loadData(userId = "u1", forceRefresh = true)
```

**规则**：一旦开始用命名参数，后面的参数**必须全用命名**。这是硬性限制，也是为什么 Composable 的参数习惯把 `modifier` 放最后。

# 三、数据类与密封类

## (一) data class

```kotlin
data class User(val id: Long, val name: String, val age: Int)

val u = User(1L, "Alice", 30)
println(u)                          // User(id=1, name=Alice, age=30)
u.copy(name = "Bob")                 // User(id=1, name=Bob, age=30)
val (id, name, age) = u              // 解构：按位置取组件
```

编译器自动生成 `equals`、`hashCode`、`toString`、`copy`、`componentN`。**反序列化必须配插件**，否则生成的 `equals` 拿不到数据：

```kotlin
@Serializable
data class User(val id: Long, val name: String)
```

⚠ **data class 的限制**：至少要有一个构造参数；十个以上字段会让编译器报错，字段多时改用普通 class + 自己写 `equals`。

## (二) sealed 与枚举

`sealed` 的子类必须在同一文件内声明（Kotlin 1.5 起可跨文件，需 `sealed interface`），编译器因此能穷举所有分支：

```kotlin
sealed interface LoadState<out T> {
    data object Loading : LoadState<Nothing>
    data class Success<T>(val data: T) : LoadState<T>
    data class Error(val message: String, val cause: Throwable?) : LoadState<Nothing>
}
```

关键价值在**穷举检查**：新加一个分支，所有没处理的 `when` 立刻编译报错。

```kotlin
// 作为表达式：必须穷举，少一支就编译不过
// ★T 加上 Collection 约束才能调 .size，否则编译器在 T 上找不到这个成员
fun <T : Collection<*>> toMessage(state: LoadState<T>): String = when (state) {
    LoadState.Loading -> "加载中…"          // data object 用 == 比，is 会触发编译器警告
    is LoadState.Success -> "共 ${state.data.size} 条"
    is LoadState.Error -> "失败：${state.message}"
}

// 作为语句：允许不穷举
fun <T> log(state: LoadState<T>) {
    when (state) {
        is LoadState.Error -> Log.e(TAG, state.message, state.cause)
        else -> Unit
    }
}
```

**`when` 是表达式时必须穷举，是语句时不必**——这个区别决定了你能不能漏写分支。

对比 `enum class`：enum 的每个常量只能是无数据对象，`sealed` 可以携带任意数据。这是 Compose 里表达 UI 状态用 `sealed` 而非 `enum` 的原因。

# 四、作用域函数

五个函数的差别只在两点：**函数名里那个字母（`it` 还是 `this`）指的是谁**，以及**返回什么**。

| 函数 | 接收者 | 返回 | 典型场景 |
| --- | --- | --- | --- |
| `let` | `it` | lambda 的结果 | 判空后取值 |
| `run` | `this` | lambda 的结果 | 计算并返回结果 |
| `apply` | `this` | **接收者自身** | 配置对象 |
| `also` | `it` | **接收者自身** | 附加副作用 |
| `with` | `this` | lambda 的结果 | 非扩展，批量操作 |
| `takeIf` | `it` | 满足条件则自身，否则 null | 过滤 |
| `takeUnless` | `it` | 不满足条件则自身，否则 null | 守卫式校验 |

## (一) apply：配置对象

```kotlin
// 返回自身，所以可以直接 .build()
val client = OkHttpClient.Builder().apply {
    connectTimeout(10, TimeUnit.SECONDS)
    readTimeout(15, TimeUnit.SECONDS)
    retryOnConnectionFailure(true)
}.build()
```

## (二) let：判空后取值

```kotlin
val avatarUrl: String? = user.avatarUrl
val len = avatarUrl?.let { it.length }        // null 时结果是 null
val safe = avatarUrl?.let { it.ifBlank { "default" } } ?: "default"
```

## (三) run：需要 this 且要返回值

```kotlin
// this 是 viewModel.state，返回值是 lambda 最后的表达式
val uiState = viewModel.state.run {
    if (isLoading) copy(loading = false, data = emptyList()) else this
}
```

## (四) also：附加副作用

```kotlin
// 返回自身，所以不影响主链
val user = repo.findById(id)
    ?.also { Log.d(TAG, "loaded user: $it") }   // 记日志
    ?.also { analytics.track("user_load", it.id) }
```

## (五) with：批量操作非扩展对象

```kotlin
val sb = StringBuilder()
with(sb) {
    append("id=").append(1)
    append(", name=").append("Alice")
}
val query = sb.toString()      // id=1, name=Alice
```

## (六) takeIf / takeUnless

```kotlin
// 条件满足才保留
val nonBlank = input.takeIf { it.isNotBlank() }

// 条件不满足才保留 —— 守卫式校验很顺手
fun submit(name: String) {
    name.takeUnless { it.isBlank() }
        ?.let { api.submit(it) }
        ?: Log.w(TAG, "name 为空，已忽略")
}
```

# 五、集合

```kotlin
val orders = listOf(
    Order(1, "iPhone",  5999.0, paid = true),
    Order(2, "键盘",     299.0, paid = false),
    Order(3, "显示器",  1299.0, paid = true),
    Order(4, "鼠标",     199.0, paid = false),
)
```

| 操作 | 结果 |
| --- | --- |
| `orders.filter { it.paid }` | 只要已付款的 |
| `orders.map { it.name }` | 转成名称列表 |
| `orders.sortedBy { it.amount }` | 按金额升序 |
| `orders.sumOf { it.amount }` | 合计 `7796.0` |
| `orders.groupBy { it.paid }` | 按是否付款分成两组 |
| `orders.maxByOrNull { it.amount }` | 最贵的（`null` 安全版） |
| `orders.associateBy { it.id }` | 转成 `Map<Long, Order>` |
| `orders.partition { it.paid }` | 一分为二，返回 `Pair` |

几个容易踩的点：

```kotlin
// ✅ 空集合安全
val first = orders.firstOrNull()
val max = orders.maxByOrNull { it.amount }     // 不是 maxBy（后者在空集合上抛异常）

// ✅ 可能为空的属性要先过滤，再转换
val names = orders.mapNotNull { it.name.takeIf(String::isNotBlank) }

// ✅ 需要扁平化时用 flatMap，不是 map + flatten
val tags = orders.flatMap { it.tags }

// ✅ 去重要注意：distinct() 用的是 equals，对 data class 有效
val uniqueNames = orders.map { it.name }.distinct()

// ✅ 分组后常配 firstOrNull
val unpaid = orders.filterNot { it.paid }

// ✅ 关联两个列表
val pairs = ids.zip(names)

// ✅ 分批处理（如批量上传）
orders.chunked(10).forEach { batch -> api.uploadBatch(batch) }
```

⚠ **`maxBy` 与 `maxByOrNull`**：集合非空时 `maxBy` 能用，但一旦数据来自网络/数据库就可能为空，一律用 `OrNull` 版本。

# 六、协程与 Flow

协程是 Kotlin 的语言级特性，也是 Compose 的基石——Compose 的 API 本身就建立在协程之上。

## (一) 结构化并发

`suspend` 不等于「开新线程」，它只是**允许挂起**。不切换线程时，后续代码仍在原线程执行。

```kotlin
// 串行：等第一个完成才发第二个，慢
val user = api.loadUser(id)
val posts = api.loadPosts(id)

// 并行：两个同时发，总耗时取决于较慢的那个
val user = async { api.loadUser(id) }
val posts = async { api.loadPosts(id) }
UserDetail(user.await(), posts.await())
```

⚠ **`await` 的位置很关键**。写成下面这样就退化成串行了：

```kotlin
val user = async { api.loadUser(id) }.await()   // ★立刻等待，并行失效
val posts = async { api.loadPosts(id) }.await()
```

正确写法是用 `coroutineScope` 整体包起来（任一失败则其余取消）：

```kotlin
suspend fun loadDetail(id: String): UserDetail = coroutineScope {
    val user = async { api.loadUser(id) }
    val posts = async { api.loadPosts(id) }
    UserDetail(user.await(), posts.await())
}
```

## (二) 异常与取消

```kotlin
// 默认：任一子协程失败，兄弟全部取消，异常向上抛
coroutineScope {
    launch { api.a() }
    launch { api.b() }      // a 失败时，b 被取消
}

// SupervisorScope：兄弟互不影响
supervisorScope {
    launch { runCatching { api.a() } }   // 每个自己兜住
    launch { runCatching { api.b() } }
}
```

**`CancellationException` 不是错误**。捕获异常时必须重新抛出它，否则协程取消机制会失效：

```kotlin
try {
    api.load()
} catch (e: CancellationException) {
    throw e            // ★必须再抛，否则协程无法正确取消
} catch (e: IOException) {
    // 真正的网络错误才在这里处理
}
```

## (三) 线程切换

```kotlin
// 阻塞调用放 IO
val data = withContext(Dispatchers.IO) {
    database.query("select * from articles")     // 阻塞 API
}

// 主线程才碰 UI
withContext(Dispatchers.Main) {
    textView.text = data
}
```

⚠ **`Dispatchers.Default` 是 CPU 密集（纯计算），`Dispatchers.IO` 是阻塞 IO**。搞混不会崩，但会让线程池用错——纯计算丢给 IO 是常见性能问题。

## (四) Flow

冷流：没有收集者就不执行。

```kotlin
val flow = flow {
    emit(1)
    emit(2)
}

flowOf(1, 2, 3)
    .map { it * 2 }
    .filter { it > 2 }
    .collect { println(it) }        // 输出 4、6
```

三种流容器，选错是常见坑：

| 类型 | 有初始值 | 会去重 | 典型用途 |
| --- | --- | --- | --- |
| `Flow<T>` | 否 | 否 | 一次性流、事件流 |
| `StateFlow<T>` | 是 | 是（`equals`） | **有状态**的当前值 |
| `SharedFlow<T>` | 否 | 否 | 广播，多个收集者 |

```kotlin
// StateFlow：Compose 的状态容器用它
private val _uiState = MutableStateFlow(UiState())
val uiState: StateFlow<UiState> = _uiState.asStateFlow()   // ★对外只暴露只读版

fun onQueryChange(q: String) {
    _uiState.update { it.copy(query = q, loading = true) }  // update 是原子操作
}
```

`MutableStateFlow` 直接对外暴露会让外部随意 `value = ` 绕过业务逻辑。用 `asStateFlow()` 包一层是硬性习惯。

## (五) 作用域与取消

```kotlin
val scope = CoroutineScope(Dispatchers.Main + SupervisorJob())
val job = scope.launch { flow.collect { } }

job.cancel()          // 必须取消，否则协程泄漏
```

**规则**：谁创建协程，谁负责取消。ViewModel 里用 `viewModelScope`（随 `onCleared()` 自动取消），Compose 里用 `rememberCoroutineScope()`（随组合离开自动取消）。自己 `CoroutineScope(...)` 造出来的必须手动管。

# 七、扩展函数与内联

```kotlin
// 扩展函数：给已有类加方法，不改源码
fun String.toTitleCase(): String =
    replaceFirstChar { it.uppercase() }

"hello".toTitleCase()            // "Hello"
```

**`inline` 的两个实际收益**：lambda 参数不需要装箱（列表场景省掉对象分配），以及**允许非局部返回**——在传入的 lambda 里直接 `return` 会从外层函数返回：

```kotlin
inline fun requireNotNullOrThrow(value: String?, msg: String): String {
    if (value.isNullOrBlank()) throw IllegalArgumentException(msg)
    return value
}

val token = run {
    requireNotNullOrThrow(input, "缺少 token")   // 编译期把内联的代码贴进来
    loadToken()
}
```

⚠ **`Modifier.then` 不是内联函数**——它是 `Modifier` 接口上带默认实现的普通成员。Compose 里「内联」指的是编译器插件对 `@Composable` 做的转换，与 Kotlin 的 `inline` 关键字是两回事。

# 八、SAM 转换

Java 的单抽象方法接口在 Kotlin 里可以直接写成 lambda，这是写 Android 回调代码的基础：

```kotlin
// Java 接口：interface OnClickListener { void onClick(View v); }
// Kotlin 可以写成 lambda，不必写 object : OnClickListener { ... }
button.setOnClickListener { view -> navigate(view) }

// 需要引用接口自身时才用显式写法
button.setOnClickListener(object : View.OnClickListener {
    override fun onClick(v: View) { navigate(v) }
})
```

**Kotlin 的 `fun interface` 是同一个机制**——写自己的回调接口时用 `fun interface`，调用方就能直接传 lambda：

```kotlin
fun interface OnSwipe {
    fun onSwipe(direction: Int)
}
onSwipe { dir -> animate(dir) }
```

# 九、与 Java 互操作

虽然本笔记全面用 Kotlin，但工程上仍会遇到 Java 代码。

```kotlin
// Kotlin 调 Java：平台类型 String!（带感叹号）不强制判空，
// 因为编译器不知道那个 Java 方法会不会返回 null
val len = javaUtil.trim(s).length      // s 可能为 null，长度是运行时的
```

平台类型是 Kotlin 与 Java 互操作唯一的**不安全缺口**。处理 Java API 时不要因为「没报错」就以为它安全。

Kotlin 侧要给 Java 调用时，需要桥接注解：

| 注解 | 作用 |
| --- | --- |
| `@JvmStatic` | 把成员函数变成 Java 的静态方法 |
| `@JvmField` | 把属性变成 Java 的字段（省掉 getter） |
| `@JvmOverloads` | 为带默认参数的方法生成全部重载 |
| `@JvmName` | 改生成的方法名（顶层函数会被加 `Kt` 后缀） |

```kotlin
object Config {
    @JvmField
    val VERSION = "1.0.0"          // Java: Config.VERSION
}

class Api {
    companion object {
        @JvmStatic
        fun create(baseUrl: String): Api = Api(baseUrl)   // Java: Api.create(url)
    }
}

fun helper() = 1                    // Java: HelperKt.helper()

// Java: new Api()  —— 默认参数生成的构造函数不存在，必须加 @JvmOverloads
class Page @JvmOverloads constructor(
    val size: Int,
    val offset: Int = 0
)
```

⚠ **只在有 Java 调用方时才需要加这些注解**，纯 Kotlin 工程里加了纯属噪音。

# 十、易错点

| 误区 | 事实 |
| --- | --- |
| `suspend` 会开新线程 | 不会，它只是允许挂起，不切换线程 |
| `await()` 写在 `async` 后面就是并行 | 立刻 await 会退化成串行，要等全部都启动完再 await |
| `Dispatchers.Default` 和 `IO` 可以随便用 | Default 是 CPU 密集，IO 是阻塞调用 |
| 捕获异常时统一 catch Exception | 必须重抛 `CancellationException`，否则协程无法取消 |
| `Flow` 会自动在后台跑 | 不会，冷流要 `collect` 才执行，且需要正确的 scope |
| `data class` 天然可序列化 | 不行，需要 `@Serializable` 插件 |
| `!!` 是 Kotlin 推荐写法 | 它关掉了空安全，优先用 `let` / `checkNotNull` |
| `first()` 和 `firstOrNull()` 等价 | 空集合上行为不同，前者抛异常 |
| `MutableStateFlow` 可以直接暴露给 UI | 用 `asStateFlow()` 包一层，外部只能读 |
| Kotlin 调 Java 不用判 null | Java 返回值是平台类型 `String!`，仍可能为 null |

# 十一、参考资料

- [Kotlin 官方文档](https://kotlinlang.org/docs/home.html) — 语言参考与标准库
- [Kotlin 协程](https://kotlinlang.org/docs/coroutines-overview.html) — 结构化并发与 Flow 的完整说明
- [Kotlin 与 Android](https://developer.android.com/kotlin) — Google 的 Kotlin 最佳实践
- [Kotlin 常见问题](https://kotlinlang.org/docs/faq.html) — 官方整理的高频坑
- [Android 协程最佳实践](https://developer.android.com/kotlin/coroutines) — 官方推荐的协程用法
- [Kotlin Flow](https://developer.android.com/kotlin/flow) — 流在 Android 中的定位与用法
- [Kotlin 互操作参考](https://kotlinlang.org/docs/java-interop.html) — 平台类型与注解的完整列表
