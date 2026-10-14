---
title: "AndroidNotes12 : 自适应与大屏"
weight: 12
description: "Compose 的自适应布局：window size class 的五档宽度断点与 currentWindowAdaptiveInfoV2、list-detail 与双栏导航、三种导航形态的自动切换、折叠屏姿态与铰链处理，以及响应式栅格与可拖拽窗口的适配要点。"
featureimage: "covers/android-ui.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、按窗口而不是按设备

自适应的核心原则：**根据应用窗口的可用宽度决策，而不是根据设备类型**。

```kotlin
// ❌ 老做法：判断是不是平板
if (resources.configuration.smallestScreenWidthDp >= 600) {
    showTwoPane()
}

// ✅ 根据窗口宽度
val widthClass = currentWindowAdaptiveInfoV2().windowSizeClass.windowWidthSizeClass
if (widthClass != WindowWidthSizeClass.COMPACT) {
    showTwoPane()
}
```

**为什么后者更正确**：分屏模式下手机窗口可能只有 400dp 宽（该用单栏），ChromeOS 上平板窗口可能被拉到 900dp（该用双栏）。设备类型判断在这两种场景下都会出错。

⚠ **`calculateWindowSizeClass()` 与不带 V2 的 `currentWindowAdaptiveInfo()` 都已弃用**，现在用 `currentWindowAdaptiveInfoV2()`。

# 二、五档宽度断点

| 宽度类 | 断点 | 典型场景 |
| --- | --- | --- |
| `Compact` | < 600dp | 手机竖屏（99.96%） |
| `Medium` | 600dp ≤ w < 840dp | 平板竖屏（93.73%） |
| `Expanded` | 840dp ≤ w < 1200dp | 平板横屏（97.22%） |
| `Large` | 1200dp ≤ w < 1600dp | 大平板 |
| `ExtraLarge` | w ≥ 1600dp | 桌面 |

高度类另有三档：`Compact` < 480dp、`Medium` 480–900dp、`Expanded` ≥ 900dp。

## (一) 取用

```kotlin
dependencies {
    implementation("androidx.compose.material3.adaptive:adaptive")
    implementation("androidx.compose.material3.adaptive:adaptive-layout")
    // NavigationSuiteScaffold 在这个独立 artifact 里，不在 adaptive-navigation
    implementation("androidx.compose.material3:material3-adaptive-navigation-suite")
}

@Composable
fun App() {
    val windowSizeClass = currentWindowAdaptiveInfoV2().windowSizeClass

    when (windowSizeClass.windowWidthSizeClass) {
        WindowWidthSizeClass.COMPACT -> PhoneLayout()
        WindowWidthSizeClass.MEDIUM -> TwoPaneLayout()
        else -> ThreePaneLayout()
    }
}
```

⚠ **`androidx.window.core.layout.WindowWidthSizeClass` 只有三个常量**：`COMPACT` / `MEDIUM` / `EXPANDED`（全大写）。上表里的 `Large`（≥1200dp）和 `ExtraLarge`（≥1600dp）两档是**断点表里的概念**，这个类型并不单独枚举它们——要区分得自己比较 `windowSizeClass.minWidthDp`。

⚠ **只处理宽度就够了**。绝大多数 App 不需要区分高度类——竖向滚动让高度的影响远小于宽度。只有横屏手机（高度 < 480dp）这类特殊场景才需要看高度。

⚠ **别用 `isTablet` 之类的逻辑**。窗口大小类明确不是为了「判断设备类型」设计的。

# 三、双栏布局

## (一) 手工分栏

```kotlin
@Composable
fun ArticlePane(articles: List<Article>, selectedId: Long?, onSelect: (Long) -> Unit) {
    Row(Modifier.fillMaxSize()) {
        // 左栏：列表
        LazyColumn(Modifier.width(320.dp).fillMaxHeight()) {
            items(articles, key = { it.id }) { ArticleRow(it, onSelect) }
        }
        VerticalDivider()

        // 右栏：详情
        Box(Modifier.weight(1f).fillMaxHeight()) {
            if (selectedId == null) {
                EmptyPlaceholder("选择一篇文章查看")
            } else {
                ArticleDetail(articles.first { it.id == selectedId })
            }
        }
    }
}
```

## (二) NavigationSuiteScaffold

让导航形态自动跟随宽度切换，省去手写判断：

```kotlin
NavigationSuiteScaffold(
    navigationSuiteItems = {
        item(selected = current == Home, onClick = { nav(Home) },
             icon = { Icon(Icons.Default.Home, null) },
             label = { Text("首页") })

        item(selected = current == Search, onClick = { nav(Search) },
             icon = { Icon(Icons.Default.Search, null) },
             label = { Text("搜索") })

        item(selected = current == Settings, onClick = { nav(Settings) },
             icon = { Icon(Icons.Default.Settings, null) },
             label = { Text("设置") })
    }
) {
    NavHost(navController, startDestination = "home") { /* ... */ }
}
```

它会这样切换：

| 宽度 / 姿态 | 导航形态 |
| --- | --- |
| `COMPACT` | 底部 `NavigationBar` |
| `MEDIUM`，或高度为 `COMPACT` 的桌面模式（Tabletop） | 短款 `NavigationRail` |
| 其余（`EXPANDED` 及以上） | 宽款 `NavigationRail`（展开式导航栏） |

⚠ **全程没有 `NavigationDrawer`**。想要抽屉得自己用 `ModalDrawerSheet` 实现。

⚠ **这是需要自适应时最省事的选择**——一行代码换来三种形态的自动适配。

# 四、List-Detail 折叠

`ListDetailPaneScaffold` 是「列表 + 详情」的官方折叠组件：

```kotlin
@OptIn(ExperimentalMaterial3AdaptiveApi::class)
@Composable
fun ArticleApp(list: List<Article>) {
    // ★rememberListDetailPaneNavigator / ListDetailPaneNavigator 已被
    //   rememberListDetailPaneScaffoldNavigator / ThreePaneScaffoldNavigator 取代
    val navigator = rememberListDetailPaneScaffoldNavigator<Article>()

    ListDetailPaneScaffold(
        // ★现行签名必须传 directive 与 scaffoldState
        directive = navigator.scaffoldDirective,
        scaffoldState = navigator.scaffoldState,
        listPane = {
            // 窄屏时是全屏列表，宽屏时是左栏
            ArticleList(
                articles = list,
                onItemClick = { article ->
                    navigator.navigateTo(article)      // ★窄屏：跳到详情
                }
            )
        },
        detailPane = {
            ArticleDetail(navigator.currentPane?.key)
        }
    )
}
```

**同一份代码**在窄屏上是「列表 → 详情」两个页面，在宽屏上是「列表 | 详情」并排——不需要写任何 `if (isWide)`。

## (一) 三种状态

| 状态 | 窄屏 | 宽屏 |
| --- | --- | --- |
| 未选中 | 只显示列表 | 列表 + 空占位 |
| 选中一项 | 列表 → 详情（单栏） | 列表 + 详情（双栏） |
| 返回 | 详情 → 列表 | 详情收起为占位 |

# 五、折叠屏与铰链

```kotlin
val windowAdaptiveInfo = currentWindowAdaptiveInfoV2()

// ★windowPosture 的类型是 Posture（不是 WindowAdaptiveInfo），
//   也没有 WindowAdaptiveInfo.WindowPosture.Folded / HalfOpened 这些枚举。
//   Posture 用 isTabletop、hingeList 这类属性表达
val posture = windowAdaptiveInfo.windowPosture

when {
    posture.isTabletop -> TabletopLayout()      // 半开（支架模式），上下分栏
    posture.hingeList.any { !it.isFlat } -> FoldedLayout()   // 有非平整铰链，对折合上
    else -> FlatLayout()
    }
}
```

## (一) 铰链位置

```kotlin
val hingeBounds = windowAdaptiveInfo.windowPosture.separatingVerticalHingeBounds
    ?.firstOrNull()

if (hingeBounds != null) {
    // 让内容避开铰链区域
    Row {
        Box(Modifier.width(with(hingeBounds) { left }).weight(1f))
        // ★hingeBounds 是 Rect，width() 是扩展函数；也可直接相减
        Spacer(Modifier.width(hingeBounds.width()))
        Box(Modifier.weight(1f))
    }
}
```

⚠ **铰链位置可能每帧都变**（折叠/展开动画中）。把它做成 state 传下去，不要在 Composable 里直接算。

## (二) 布局建议

| 姿态 | 布局 |
| --- | --- |
| 平铺 | 正常自适应 |
| 对折合上（书本式） | 左右分栏，铰链做分隔 |
| 半开（支架式） | 上下分栏，上半屏内容、下半屏控件 |

# 六、响应式栅格

```kotlin
// 自适应列数：每格至少 160dp，能放几列放几列
LazyVerticalGrid(columns = GridCells.Adaptive(minSize = 160.dp)) {
    items(categories) { CategoryCard(it) }
}
```

配合窗口宽度限制最大宽度，避免超宽屏上元素被拉得过散：

```kotlin
Box(Modifier.fillMaxWidth(), contentAlignment = Alignment.TopCenter) {
    LazyVerticalGrid(
        columns = GridCells.Adaptive(minSize = 160.dp),
        modifier = Modifier.widthIn(max = 1200.dp)    // ★超宽时居中收窄
    ) { ... }
}
```

⚠ **超宽屏上不加 `widthIn(max = ...)` 会很难看**——一行卡片拉到 2000dp 宽，每个卡片里只有一行字。

# 七、不要锁死的东西

自适应设计里有几条硬性禁止项：

```kotlin
// ❌ 锁定方向：窗口被拖宽时不跟着变
requestedOrientation = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT

// ❌ 锁定宽高比
android:screenOrientation="portrait"
```

| 禁止 | 后果 |
| --- | --- |
| 锁定屏幕方向 | 平板与折叠屏上旋转时布局崩掉 |
| 限制宽高比 | 分屏与可拖拽窗口下被强制变形 |
| 假设全屏 | 分屏时窗口只有一半 |
| 只支持触摸 | 外接键盘/鼠标/手柄用户无法操作 |
| 用 `getRealSize()` / `getMetrics()` | 已弃用，返回的是整屏尺寸而非窗口 |

⚠ **这两个是 `Display` 的实例方法**，不是 `Window` 或任意 Context 上的方法：`Display#getRealSize(Point)`、`Display#getRealMetrics(DisplayMetrics)`。而且它们在多窗口下返回的是**整屏**尺寸，对窗口来说是错的。

拿窗口尺寸的正确做法是 `WindowMetrics`：

```kotlin
// Activity 里
val bounds = windowManager.currentWindowMetrics.bounds   // API 30+
// 兼容低版本用 androidx
val bounds = WindowMetricsCalculator.getOrCreate()
    .computeCurrentWindowMetrics(activity).bounds
```

`getRealSize()` / `getMetrics()` 自 Android 11 起弃用，`getRealMetrics()` 自 Android 12 起弃用。

# 八、自适应检查清单

- [ ] 所有布局基于 `currentWindowAdaptiveInfoV2()`，不判断设备类型
- [ ] 窄屏与宽屏各有明确的信息架构，不是简单拉伸
- [ ] 文本行宽有上限（`widthIn(max = ...)`）
- [ ] 触摸目标不小于 48×48dp
- [ ] 支持鼠标悬停与键盘导航
- [ ] 未锁定方向与宽高比
- [ ] 折叠屏上铰链区域被避开
- [ ] 在 Android Studio 的多设备预览里验证过

```kotlin
@Preview(showBackground = true, widthDp = 411, heightDp = 891)    // 手机
@Preview(showBackground = true, widthDp = 1280, heightDp = 800)   // 平板
@Preview(showBackground = true, widthDp = 1920, heightDp = 1080)  // 超宽
@Composable
private fun ArticleAppPreview() {
    MyAppTheme { ArticleApp(sampleArticles) }
}
```

# 九、易错点

| 误区 | 事实 |
| --- | --- |
| `isTablet` 判断够用 | 不够，分屏与可拖拽窗口下会错 |
| `calculateWindowSizeClass()` 是新的 | 已弃用，用 `currentWindowAdaptiveInfoV2()` |
| `androidx.window.core.layout.WindowWidthSizeClass` 有五档 | 它只有**三个**常量：`COMPACT` / `MEDIUM` / `EXPANDED`（全大写）。Large（≥1200dp）与 ExtraLarge（≥1600dp）是**断点表里的概念**，要区分得自己比 `minWidthDp` |
| 高度类也要认真处理 | 多数场景只看宽度，高度类只在横屏等特殊情况用 |
| 超宽屏上把卡片拉满就行 | 会很散，要 `widthIn(max = ...)` |
| `ListDetailPaneScaffold` 需要手写宽度判断 | 不需要，它自己处理 |
| 折叠屏不用管铰链 | 铰链会切掉内容，必须避开 |
| 锁定竖屏能简化开发 | 代价是平板、折叠屏、分屏全部体验劣化 |

# 十、参考资料

- [自适应布局总览](https://developer.android.com/develop/ui/compose/layouts/adaptive) — 自适应设计的官方入口
- [窗口大小类](https://developer.android.com/develop/ui/compose/layouts/adaptive/use-window-size-classes) — 五档断点与取值方式
- [自适应布局实践](https://developer.android.com/develop/ui/compose/layouts/adaptive/adaptive-dos-and-donts) — 官方列出的该做与不该做
- [Compose Material 3 Adaptive](https://developer.android.com/jetpack/androidx/releases/compose-material3-adaptive) — 依赖与版本
- [Material Design 3 响应式布局](https://m3.material.io/foundations/layout/) — 断点与布局的设计规范
