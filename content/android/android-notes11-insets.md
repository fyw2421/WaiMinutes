---
title: "AndroidNotes11 : 边缘到边缘与 insets"
weight: 11
description: "targetSdk 35+ 的边缘到边缘要求：enableEdgeToEdge 的调用时机与参数、WindowInsets 的四类来源与消费、Scaffold 的 innerPadding 机制、手势导航条与显示切口适配，以及 InputModifier 键盘避让的标准写法。"
featureimage: "covers/android-ui.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、什么是边缘到边缘

`targetSdk 35` 起，Android 强制应用**绘制到系统栏之下**：状态栏、导航栏不再是系统窗口，应用内容铺满整个屏幕。

| targetSdk | 变化 |
| --- | --- |
| **35（Android 15）** | 默认强制 edge-to-edge。还可以用 `windowOptOutEdgeToEdgeEnforcement` 临时退出 |
| **36（Android 16）** | 该 opt-out 属性被**弃用并禁用**，彻底无法退出 |

```kotlin
class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        enableEdgeToEdge()        // ★必须在 super.onCreate 之前
        super.onCreate(savedInstanceState)
        setContent { App() }
    }
}
```

⚠ **`enableEdgeToEdge()` 要在 `super.onCreate(savedInstanceState)` 之前调用**。这是官方 CodeLab 明确要求的顺序，写反了可能不生效。

⚠ **既然 35 起已经是默认行为，为什么还要显式调 `enableEdgeToEdge()`**：它是为了让**低版本设备**（Android 10–14）也走同一套绘制逻辑，从而不必写两套布局。

## (一) 系统栏图标样式

默认的 `enableEdgeToEdge()` 会跟随系统深浅色切换图标颜色。大多数情况够用：

```kotlin
enableEdgeToEdge()        // 跟随系统
```

⚠ **`enableEdgeToEdge(...)` 整个函数（含无参形式）在现行 AndroidX 已整体 `@Deprecated`**，`SystemBarStyle` 同样弃用——不只是带参重载。所以推荐写法是 `WindowCompat` + `WindowInsetsControllerCompat`：

```kotlin
// 弃用写法（还能编译，但不建议新代码再用）
enableEdgeToEdge(
    statusBarStyle = SystemBarStyle.dark(Color.TRANSPARENT),
    navigationBarStyle = SystemBarStyle.light(Color.TRANSPARENT, Color.TRANSPARENT)
)

// 推荐写法
WindowCompat.enableEdgeToEdge(window)
val controller = WindowCompat.getInsetsController(window, window.decorView)
controller.isAppearanceLightStatusBars = !isDarkTheme      // 深色主题用浅色图标
controller.isAppearanceLightNavigationBars = !isDarkTheme
```

| `SystemBarStyle`（已弃用） | 含义 |
| --- | --- |
| `dark(scrim)` | 浅色背景 + 深色图标 |
| `light(scrim, darkScrim)` | 深色背景 + 浅色图标 |
| `auto(lightScrim, darkScrim)` | 跟随系统深浅色（默认） |

⚠ **在 Compose 里通常根本不用管**。Material 3 的 `Scaffold` + 主题已经会处理，手动设置容易与主题打架。

## (二) 边到边之后要做什么

开启后内容会延伸到系统栏之下，**被盖住的部分必须自己让出来**：

```kotlin
// ❌ 标题被状态栏盖住
Column(Modifier.fillMaxSize()) {
    TopBar()
    Content()
}

// ✅ 顶部避开状态栏，底部避开导航条
Column(
    Modifier
        .fillMaxSize()
        .windowInsetsPadding(WindowInsets.systemBars)
) {
    TopBar()
    Content()
}
```

# 二、四类 Insets

| 来源 | 常量 | 覆盖什么 |
| --- | --- | --- |
| 系统栏 | `WindowInsets.systemBars` | 状态栏 + 导航栏 |
| 显示切口 | `WindowInsets.displayCutout` | 刘海、挖孔 |
| 软键盘 | `WindowInsets.ime` | 输入法 |
| 手势导航 | `WindowInsets.safeGestures` | 手势区域 |

```kotlin
// 全部系统栏
Modifier.windowInsetsPadding(WindowInsets.systemBars)

// 只避状态栏
Modifier.statusBarsPadding()

// 只避导航条
Modifier.navigationBarsPadding()

// 只避刘海（横屏时才有值）
Modifier.displayCutoutPadding()

// 只避底部手势条
Modifier.safeGesturePadding()

// 合并
Modifier.windowInsetsPadding(WindowInsets.systemBars.union(WindowInsets.displayCutout))
```

⚠ **`statusBarsPadding()` 在状态栏透明时仍会占位**。如果状态栏是透明的、且内容应该延伸到它下面（如视频播放页），就不该加这个 padding。

# 三、Scaffold 的 innerPadding

Material 3 的 `Scaffold` 会**自动计算需要避让的 insets**，通过 `innerPadding` 传给内容：

```kotlin
Scaffold(
    topBar = { TopAppBar(title = { Text("文章") }) },
    bottomBar = { NavigationBar { /* ... */ } },
    snackbarHost = { SnackbarHost(hostState) }
) { innerPadding ->
    // ★必须用 padding(innerPadding)，否则内容会被 topBar / bottomBar 盖住
    LazyColumn(
        modifier = Modifier
            .fillMaxSize()
            .padding(innerPadding)          // 一次性处理所有避让
    ) {
        items(articles) { ArticleRow(it) }
    }
}
```

`innerPadding` 是**累加**的：`topBar` 的高度 + 状态栏 inset，`bottomBar` 的高度 + 导航条 inset。

## (一) 常见错误

```kotlin
// ❌ 手动再加一次系统栏 padding，和 innerPadding 重复了
Scaffold(topBar = { TopAppBar(...) }) { padding ->
    LazyColumn(Modifier.padding(padding).statusBarsPadding())   // 多了一层空白
}

// ✅ 只用 innerPadding
Scaffold(topBar = { TopAppBar(...) }) { padding ->
    LazyColumn(Modifier.padding(padding))
}
```

⚠ **`innerPadding` 是要「消费」掉的**。不应用它，padding 值白算了（浪费）；应用两次，内容会多缩进一层。

## (二) TopAppBar 自己会处理状态栏

```kotlin
@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun MyTopBar() {
    TopAppBar(title = { Text("标题") })     // ★不需要自己加 statusBarsPadding
}
```

`TopAppBar`、`NavigationBar` **内部已经处理了状态栏与导航条的 inset**。再加一次就会多出空白——这是最常见的边缘到边缘 bug。

# 四、软键盘避让

输入法弹出时，内容不能被遮住：

```kotlin
@Composable
fun CommentInput(onSend: (String) -> Unit) {
    var text by remember { mutableStateOf("") }

    // 键盘高度作为 padding，动画跟随
    val imePadding by animateDpAsState(
        targetValue = if (isFocused) WindowInsets.ime.getBottom(LocalDensity.current).toDp()
                       else 0.dp,
        animationSpec = tween(200),
        label = "ime"
    )

    Column(
        Modifier
            .fillMaxSize()
            .imePadding()          // ★推荐：与系统动画同步
    ) {
        LazyColumn(Modifier.weight(1f)) {
            items(messages) { MessageRow(it) }
        }
        CommentBar(text, onTextChange = { text = it }, onSend = onSend)
    }
}
```

⚠ **`imePadding()` 会自动跟随键盘的显示/隐藏动画**，比手写 `animateDpAsState` + 手动测量更平滑。

⚠ **别用 `WindowInsets.ime` 配 `asPaddingValues()` 硬算高度**——不同厂商 ROM 的键盘动画曲线不一致，硬算容易跳变。

## (一) 焦点与键盘

```kotlin
// 弹出键盘
val focusManager = LocalFocusManager.current
focusManager.requestFocus(textFieldFocusRequester)

// 收起键盘
focusManager.clearFocus()
```

⚠ **`ScrollState` 不会自动跟随键盘**。输入框在列表底部时，键盘弹出会遮住它，需要手动 `bringIntoViewRequester.bringIntoView()`。

# 五、沉浸式与布局适配

```kotlin
// 全屏（隐藏系统栏）——播放器、阅读器常用
WindowInsetsControllerCompat(window, view).apply {
    hide(WindowInsetsCompat.Type.systemBars())
    systemBarsBehavior = WindowInsetsControllerCompat
        .BEHAVIOR_SHOW_TRANSIENT_BARS_BY_SWIPE
}
```

## (一) 显示切口

```kotlin
@Composable
fun FullScreenPlayer() {
    Box(
        Modifier
            .fillMaxSize()
            // ★不要加 displayCutoutPadding()：它是把内容【移出】刘海区，
            //   与「播放器铺满整屏包括刘海区」正好相反
            .background(Color.Black)
    ) {
        // 视频铺满，包括刘海区域
    }
}
```

⚠ **刘海屏上 `fillMaxSize()` 不等于「安全区」**——它会铺满包括刘海在内的整屏，内容可能被挖孔或状态栏盖住。这正是 `displayCutoutPadding()` 存在的原因：它是**把内容移开**，不是让内容铺得更满。

⚠ **想让内容铺满整屏（播放器、视频），就什么 inset 修饰符都不要加**，包括不要写 `windowInsetsPadding(WindowInsets(0, 0, 0, 0))`——那个构造是 `@Composable` 且依赖实测密度，拿到的是当前真实值，并不是「清零」。

## (二) 横屏与折叠屏

```kotlin
// 横屏时导航条在侧边
Row(Modifier.fillMaxSize()) {
    SidePanel(Modifier.navigationBarsPadding())
    Content(Modifier.weight(1f))
}
```

⚠ **`navigationBarsPadding()` 在横屏时会作用在侧边**。固定格式的布局要注意别把侧栏挤没。

# 六、系统栏图标控制

```kotlin
// Compose 里控制图标深浅
val view = LocalView.current
if (!view.isInEditMode) {
    SideEffect {
        val window = (view.context as Activity).window
        WindowCompat.enableEdgeToEdge(window)
        WindowCompat.getInsetsController(window, view).apply {
            isAppearanceLightStatusBars = !isDarkTheme      // 深色主题用浅色图标
            isAppearanceLightNavigationBars = !isDarkTheme
        }
    }
}
```

⚠ **不要硬编码 `Color.RED` 之类的系统栏颜色**。`window.statusBarColor` 与 `navigationBarColor` 在 Android 15 起已弃用并移除，系统栏透明正是边缘到边缘的前提。

# 七、检查清单

开启边缘到边缘后，逐项确认：

- [ ] `enableEdgeToEdge()` 在 `super.onCreate` 之前
- [ ] 用了 `Scaffold` 的 `innerPadding`，且只应用一次
- [ ] `TopAppBar` / `NavigationBar` 没有额外加 padding
- [ ] 列表的 `contentPadding` 考虑了底部导航条
- [ ] 输入框所在区域有 `imePadding()`
- [ ] 悬浮按钮（FAB）有 `navigationBarsPadding()`
- [ ] 横屏时侧边导航条不会挤压布局
- [ ] 深浅色主题下状态栏图标都清晰可读

# 八、易错点

| 误区 | 事实 |
| --- | --- |
| `enableEdgeToEdge()` 可以放在 `super` 之后 | 必须在 `super.onCreate` 之前 |
| `Scaffold` 的 padding 和状态栏 padding 要都加 | 只用 `innerPadding`，重复加会多一层空白 |
| `TopAppBar` / `NavigationBar` 需要自己加 inset padding | 都已内置：TopAppBar 处理 `systemBars ∪ displayCutout` 的 `Horizontal + Top`，NavigationBar 处理 `Horizontal + Bottom`（不含状态栏） |
| `fillMaxSize()` 就是全屏 | 刘海屏上会被挖孔切掉 |
| 系统栏可以设成品牌色 | 系统栏透明是边缘到边缘的前提，`statusBarColor` 在 Android 15 已移除 |
| 键盘弹出会自动把内容顶上去 | 不会，要用 `imePadding()` |
| `navigationBarsPadding` 永远作用在底部 | 横屏时作用在侧边 |
| 用了 `WindowInsets.systemBars` 就不用 `displayCutout` | 两者要 `union` 起来才完整 |

# 九、参考资料

- [应用边到边缘显示](https://developer.android.com/develop/ui/compose/layouts/insets) — `enableEdgeToEdge` 与 insets 的官方说明
- [Insets Compose API 参考](https://developer.android.com/reference/kotlin/androidx/compose/foundation/layout/package-summary) — `WindowInsets`、`PaddingValues` 完整签名
- [Android 15 边缘到边缘强制](https://developer.android.com/about/versions/15/summary) — 官方变更清单中「Edge-to-edge enforcement」一节
- [边缘到边缘实战 CodeLab](https://developer.android.com/codelabs/edge-to-edge) — 官方动手教程，逐步改造现有 App
- [Material 3 Scaffold](https://m3.material.io/components/app-bars/overview/) — 顶部栏与底部导航的设计规范
