---
title: "AndroidNotes03 : Compose 基础"
weight: 3
description: "Jetpack Compose 的声明式思维：@Composable 与组合、重组机制与跳过规则、remember 与派生状态、Modifier 链的顺序语义与自定义、副作用 API 的取舍，以及 @Preview 实时预览。"
featureimage: "covers/android-basics.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、从命令式到声明式

传统 Android 写 UI 是命令式的：**先创建视图，再一步步修改它**。

```java
// 传统写法
TextView tv = findViewById(R.id.text);
tv.setText("Hello");
tv.setVisibility(View.VISIBLE);
tv.setTextColor(Color.RED);
```

Compose 是声明式的：**描述「UI 在当前状态下应该长什么样」**，框架负责算出差异并更新。

```kotlin
// Compose 写法
Text(
    text = "Hello",
    color = Color.Red,
    modifier = Modifier.padding(16.dp)
)
```

核心区别：命令式需要你**记住每个控件的当前状态并手动同步**，声明式只需**改状态，UI 自动跟上**。

| 维度 | 命令式 | 声明式 |
| --- | --- | --- |
| UI 与逻辑 | 分离（XML + Activity） | 同函数内 |
| 更新 | 手动 `setXxx()` | 改状态 → 自动重组 |
| 查找控件 | `findViewById` | 直接用参数 |
| 空指针 | 运行时崩溃 | 编译期拦截 |

# 二、@Composable

## (一) 基本形式

```kotlin
@Composable                       // 标注后编译器才能处理
fun Greeting(name: String) {
    Text(text = "Hello $name")
}
```

带 `@Composable` 的函数**只能在另一个 `@Composable` 函数里调用**——这是编译期强制的，也保证了调用链上每一环都是声明式的。

**约定**：返回 `Unit` 的省略不写；有返回值的要显式 `remember`，否则每次重组都会重算（见下一节）。

```kotlin
@Composable
fun CounterButton(onClick: () -> Unit) { ... }   // 不写返回类型

@Composable
fun rememberSelectedId(): String {
    return remember { "" }                      // 有返回值必须 remember
}
```

## (二) 一个完整的例子

```kotlin
@Composable
fun ArticleCard(article: Article, onClick: () -> Unit) {
    Column(
        modifier = Modifier
            .fillMaxWidth()
            .clickable(onClick = onClick)
            .padding(16.dp)
    ) {
        Text(
            text = article.title,
            style = MaterialTheme.typography.titleMedium
        )
        Spacer(Modifier.height(4.dp))
        Text(
            text = article.summary,
            style = MaterialTheme.typography.bodyMedium,
            color = MaterialTheme.colorScheme.onSurfaceVariant,
            maxLines = 2,
            overflow = TextOverflow.Ellipsis
        )
    }
}
```

三个值得注意的点：

1. **样式来自 `MaterialTheme.typography`，不写死字号**——这是主题能生效的前提
2. **`maxLines` + `overflow` 组合才是完整的截断策略**，只写 `maxLines` 文字会被硬切
3. **整个卡片是参数**，父组件怎么传、什么时候传，全在外部决定

## (三) 组合与重组

Compose 维护一棵**组合树**（Composition），记录每个 `@Composable` 的调用位置。状态变化时，只重新执行**受影响的那些函数**，这个过程叫**重组**。

```kotlin
@Composable
fun Profile(name: String, avatarUrl: String?) {
    println("Profile 被调用")          // 每次重组都会打印
    Text(text = name)
    AsyncImage(model = avatarUrl)
}
```

⚠ **关键误解：跳过粒度是函数级，不是参数级。** 上例中即使只改了 `avatarUrl`，整个 `Profile` 仍会重跑。这是 Compose 性能问题最常见的根源。

编译器能跳过的前提是：**重新执行的结果与上次完全一致**（参数相同、没有读取任何变化的状态）。

# 三、状态与重组

## (一) mutableStateOf

```kotlin
@Composable
fun Counter() {
    var count by remember { mutableStateOf(0) }

    Column {
        Text(text = "点击了 $count 次")
        Button(onClick = { count++ }) { Text("加一") }
    }
}
```

`mutableStateOf` 返回的类型带**读写拦截**：写入时通知 Compose 重算依赖它的函数。用 `by` 委托可以省掉 `.value`。

```kotlin
// 等价的非委托写法
val count = remember { mutableStateOf(0) }
Text(text = "${count.value}")        // 注意 .value
```

## (二) remember 的含义

`remember` 让值**在重组之间存活**。没有它，每次重组都重新初始化：

```kotlin
// ❌ 每次重组都归零
var count = 0

// ✅ 重算但保留
var count by remember { mutableStateOf(0) }

// remember 支持多个 key，key 变才重算
val formatted by remember(date) { mutableStateOf(format(date)) }
```

⚠ **四个 key 是 `remember` 的默认值**：`remember(key1, key2, key3, key4)`。漏传依赖是 Compose 里极隐蔽的 bug——值"看起来"该更新却没更新。

⚠ **`remember` 不是持久化**。配置变更（旋转屏幕）导致 Activity 重建，状态就没了。要跨配置变更用 `rememberSaveable`：

```kotlin
var count by rememberSaveable { mutableStateOf(0) }   // 旋转后仍在
```

`rememberSaveable` 要求值能被放进 `Bundle`，所以只支持基本类型与 `Parcelable`。复杂对象要用 `listSaver` / `mapSaver` 自定义存取。

## (三) 派生状态

```kotlin
// ❌ 每次重组都重新计算，即便依赖没变
val filtered = orders.filter { it.paid }

// ✅ 只有 orders 变了才重算
val filtered by remember(orders) { derivedStateOf { orders.filter { it.paid } } }
```

**⚠ **`derivedStateOf` 的计算块是在读取时于组合线程（主线程）同步执行的**——它只做缓存与去重，**不会**把重活挪到后台。想离线程只能用 `LaunchedEffect` + `withContext`。

`derivedStateOf` 的真正用途是「输出比依赖窄」**。如果计算量很小（几行代码），直接算比套一层 `remember` 更快——`derivedStateOf` 本身有簿记开销。

它的典型场景是**高频变化但不频繁影响输出**：

```kotlin
// 滚动位置每帧都在变，但只有跨过阈值时才需要触发埋点
val shouldTrack by remember {
    derivedStateOf { listState.firstVisibleItemIndex / 10 }
}
LaunchedEffect(shouldTrack) { analytics.scrollDepth(shouldTrack) }
```

# 四、Modifier

`Modifier` 是 Compose 里**唯一的样式入口**，不存在 View 那样的一堆 `layout_*` 属性。

```kotlin
Text(
    text = "Hello",
    modifier = Modifier
        .fillMaxWidth()
        .padding(16.dp)
        .background(Color.Gray)
        .clickable { }
)
```

## (一) 链式顺序有讲究

**修饰符按书写顺序从外到内生效**。下面两段不等价：

```kotlin
// A：先 padding 再 background —— 背景只盖住内容区
Modifier.padding(16.dp).background(Color.Gray)

// B：先 background 再 padding —— 背景包含 padding 区域
Modifier.background(Color.Gray).padding(16.dp)
```

需要给带 `padding` 的区域加背景时用 `clip().background()`，保证圆角与背景一致：

```kotlin
// 圆角要盖住 padding，否则四角会漏出方角
Modifier
    .clip(RoundedCornerShape(12.dp))
    .background(MaterialTheme.colorScheme.surfaceVariant)
    .padding(16.dp)
```

## (二) offset 与 padding 的区别

这是高频考点：

| | `padding` | `offset` |
| --- | --- | --- |
| 影响测量 | 是，后续元素会被推开 | 否 |
| 影响绘制 | 是 | 是 |
| 后续兄弟的位置 | 被推移 | **按原位置布局，会重叠** |

```kotlin
Column {
    Box(Modifier.size(100.dp).background(Color.Red))
    Box(Modifier.offset(x = 20.dp).background(Color.Blue))   // 与红色重叠 20dp
    Box(Modifier.size(100.dp).background(Color.Green))       // 绿色位置不变
}
```

`zIndex()` 可以调整**绘制顺序**，让蓝色盖在红色上面：

```kotlin
Box(Modifier.offset(x = 20.dp).zIndex(1f).background(Color.Blue))
```

## (三) 自定义 Modifier

```kotlin
fun Modifier.underlineIf(enabled: Boolean) = if (enabled) {
    this.then(
        Modifier.drawBehind {
            val line = size.height * 0.05f
            drawLine(
                color = Color.Red,
                start = Offset(0f, size.height - line),
                end = Offset(size.width, size.height - line),
                strokeWidth = 2f
            )
        }
    )
} else this
```

`Modifier` 的设计要点是**不可变、返回新实例、可链式组合**——每个修饰符都是一个 `Modifier.Element`，Compose 按顺序把它们应用到布局节点上。

⚠ **自定义 Modifier 的顺序陷阱**：`drawBehind` 画在内容**之下**。要在文字上面画（下划线、删除线），要用 `drawWithContent`：

```kotlin
fun Modifier.strikethrough() = drawWithContent {
    drawContent()                    // 先画原内容
    val y = size.height - 2f
    drawLine(Color.Red, Offset(0f, y), Offset(size.width, y), 2f)   // 再画线
}
```

## (四) 常用修饰符

| 修饰符 | 作用 |
| --- | --- |
| `fillMaxWidth()` / `fillMaxSize()` | 填满父容器 |
| `padding()` | 外边距（会改变测量结果） |
| `size()` / `width()` / `height()` | 固定尺寸 |
| `background()` / `clip()` | 背景与裁剪 |
| `clickable()` | 点击响应 |
| `weight()` | 在 `Row`/`Column` 中分配剩余空间 |
| `offset()` | 绘制偏移，不影响布局 |
| `zIndex()` | 调整绘制顺序 |
| `combinedClickable()` | 长按 / 双击 |
| `clearAndSetSemantics {}` | 替换整个无障碍语义（自绘控件用） |

# 五、副作用

重组会频繁发生，把网络请求、日志写在里面会重复执行。需要用**副作用 API** 明确标记。

## (一) 选哪个

| API | 何时触发 | key 变化时 | 典型用途 |
| --- | --- | --- | --- |
| `LaunchedEffect(key)` | 首次组合 + key 变 | 取消旧协程、启新协程 | 网络请求、订阅 |
| `DisposableEffect(key)` | 同上 | 清理旧的、建新的 | 注册监听、绑定生命周期 |
| `SideEffect` | **每次重组成功后** | — | 转发非 Compose 状态 |

## (二) LaunchedEffect

```kotlin
@Composable
fun UserProfile(userId: String) {
    var user by remember { mutableStateOf<User?>(null) }
    var loading by remember { mutableStateOf(true) }

    LaunchedEffect(userId) {              // ★userId 变化时重启协程
        loading = true
        runCatching { repository.fetch(userId) }
            .onSuccess { user = it }
            .onFailure { Log.e(TAG, "load failed", it) }
        loading = false
    }

    when {
        loading -> CircularProgressIndicator()
        user != null -> ProfileContent(user!!)
        else -> Text("加载失败")
    }
}
```

⚠ **不要用 `remember { GlobalScope.launch { } }`**。`remember` 的 lambda 不是挂起函数，这样写协程不受组合生命周期管理——屏幕切走后请求仍在跑，还会持有已销毁的 UI 状态。这是最常见的 Compose 协程泄漏。

⚠ **`LaunchedEffect(Unit)` 和 `LaunchedEffect { }` 只在首次组合时跑一次**。想在状态变化时执行，`Unit` 必须换成那个状态。

## (三) 其他副作用

```kotlin
// 滚动监听：用 snapshotFlow 把 Compose 状态转成流
LaunchedEffect(listState) {
    snapshotFlow { listState.firstVisibleItemIndex }
        .distinctUntilChanged()                 // 去重，避免每次微滚都触发
        .collect { analytics.scrollTo(it) }
}

// 注册 / 注销：onDispose 必须成对
DisposableEffect(lifecycleOwner) {
    val observer = LifecycleEventObserver { _, event ->
        if (event == Lifecycle.Event.ON_RESUME) startTimer()
    }
    lifecycleOwner.lifecycle.addObserver(observer)

    onDispose {
        lifecycleOwner.lifecycle.removeObserver(observer)   // ★必须清理，否则泄漏
    }
}

// 每次重组后执行：把非 Compose 的状态同步进来
SideEffect {
    legacyViewModel.text = uiState.query       // 双向桥接老代码
}
```

`DisposableEffect` 的 `onDispose` 与 try-with-resources 的 `finally` 是同一回事：**申请了资源就必须释放**。

# 六、@Preview

```kotlin
@Preview(showBackground = true, widthDp = 360, heightDp = 640)
@Composable
private fun GreetingPreview() {
    MyAppTheme {          // ★必须用主题包起来，否则颜色/字号都不对
        Greeting(name = "Alice")
    }
}

// 夜间模式
@Preview(uiMode = Configuration.UI_MODE_NIGHT_YES)
// 小屏适配
@Preview(name = "小屏", widthDp = 320)
// 字体放大（无障碍）
@Preview(name = "大字体", fontScale = 1.5f)
// 不同语言
@Preview(name = "德语", locale = "de")

@Composable
private fun MultiplePreviews() {
    MyAppTheme { Greeting(name = "Alice") }
}
```

⚠ **不包 `MyAppTheme` 的预览是错的**——Material 3 的颜色和字号都来自主题，直接调用会看到一套不属于你 App 的样式，预览就失去参考价值。

预览在 Android Studio 里支持 **Live Edit**——改代码时界面实时更新，不必重新编译。这是 Compose 相对 XML 最大的开发体验优势。

# 七、性能：重组范围

重组本身很便宜，但**大列表项里做重活就会掉帧**。三个常用手段：

```kotlin
// 1) 传入稳定类型：lambda 本身是引用稳定的，函数引用尤其稳
LazyColumn {
    items(orders, key = { it.id }) { order ->
        OrderRow(order, onClick = { nav(order.id) })   // lambda 每次都是新对象
    }
}

// 2) 用派生状态缩小影响面
val visible by remember(orders, filter) {
    derivedStateOf { orders.filter(filter) }
}

// 3) 重活要丢到后台线程 —— derivedStateOf 做不到
val parsed by remember(raw) { mutableStateOf<Article?>(null) }
LaunchedEffect(raw) {
    parsed = withContext(Dispatchers.Default) {
        Json.decodeFromString<Article>(raw)     // ★真正的后台执行
    }
}
```

**大列表项的第一原则是给 `key`**（第 5 篇详述），第二原则是**别在 item 内部做 `remember` 之外的初始化**。

# 八、易错点

| 误区 | 事实 |
| --- | --- |
| 重组会重建整个界面 | 只重跑受影响的 `@Composable`，参数全相同才会跳过 |
| 跳过是参数级的 | **函数级**，一个参数变了整个函数都重跑 |
| `remember` 能跨配置变更 | 不能，要用 `rememberSaveable` |
| `derivedStateOf` 总是更快 | 计算量小时反而更慢，它有簿记开销 |
| `GlobalScope.launch` 在 Compose 里能用 | 不能，它不受生命周期管理，会泄漏 |
| `padding` 和 `offset` 等价 | 不等价：前者影响测量，后者只影响绘制 |
| `remember` 只需要记得写 key | 有 4 个 key，漏传依赖是隐蔽 bug 的常见来源 |
| `@Preview` 直接调函数就行 | 不包主题的话颜色和字号都不是真实值 |
| `drawBehind` 能画在文字上面 | 不能，它画在内容之下，要用 `drawWithContent` |

# 九、参考资料

- [Jetpack Compose 官方指南](https://developer.android.com/develop/ui/compose) — 文档入口
- [Compose 编程思想](https://developer.android.com/develop/ui/compose/mental-model) — 组合、重组与状态的核心模型
- [Modifier 参考](https://developer.android.com/develop/ui/compose/modifiers) — 修饰符的顺序语义与自定义写法
- [副作用 API](https://developer.android.com/develop/ui/compose/side-effects) — `LaunchedEffect`、`DisposableEffect`、`SideEffect` 的取舍
- [Compose 性能最佳实践](https://developer.android.com/develop/ui/compose/performance/bestpractices) — 官方总结的性能陷阱与对策
- [Compose API 参考](https://developer.android.com/reference/kotlin/androidx/compose/runtime/package-summary) — `remember`、`mutableStateOf` 等完整签名
- [Kotlin 官方文档](https://kotlinlang.org/docs/home.html) — 语言参考
