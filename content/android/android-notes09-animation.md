---
title: "AndroidNotes09 : 动画与转场"
weight: 9
description: "Compose 动画体系：animateDpAsState 与动画类型的选择、AnimatedVisibility 的进出动画、AnimatedContent 与 SharedTransition 的内容切换、spring 与 tween 规格、Transition 与 updateTransition 的多属性联动，以及动画的性能与无障碍处理。"
featureimage: "covers/android-ui.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、Compose 动画的心智模型

Compose 的动画**不是「改变一个值然后播放」**，而是「把目标值交给动画 API，框架自己算出每一帧的中间值」。这与传统动画最大的区别是：**动画天然与状态绑定**，状态变了动画自然跟上，不需要手写 AnimatorSet。

```kotlin
// 声明目标，框架负责插值
val width by animateDpAsState(
    targetValue = if (expanded) 300.dp else 100.dp,
    animationSpec = tween(durationMillis = 300)
)
Box(Modifier.width(width))
```

## (一) 选哪种动画

| API | 场景 | 是否需要 `remember` |
| --- | --- | --- |
| `animateDpAsState` / `animateFloatAsState` / `animateColorAsState` | 数值插值 | 不需要 |
| `AnimatedVisibility` | 出现 / 消失 | 不需要 |
| `AnimatedContent` | 两个内容之间切换 | 需要 |
| `updateTransition` | 多个属性联动 | 需要 |
| `rememberInfiniteTransition` | 循环动画（加载、闪烁） | 不需要 |
| `Animatable` | 手势驱动、时间轴精确控制 | 需要 |

# 二、数值动画

```kotlin
@Composable
fun ExpandableCard(expanded: Boolean) {
    val height by animateDpAsState(
        targetValue = if (expanded) 240.dp else 80.dp,
        animationSpec = tween(
            durationMillis = 300,
            easing = FastOutSlowInEasing      // 加速后减速，最常用的默认
        ),
        label = "cardHeight"                 // 1.7+ 起的 label 参数
    )

    Box(
        Modifier
            .height(height)
            .animateContentSize()           // 父容器高度平滑跟随
    ) {
        Card { Text("内容") }
    }
}
```

⚠ **父容器要加 `animateContentSize()`**。子元素动画到 240dp，但父容器高度不会自动变，会出现「内容动了但容器没动」的错位。

## (一) 其他类型

```kotlin
val alpha by animateFloatAsState(
    targetValue = if (visible) 1f else 0f,
    animationSpec = tween(200),
    label = "alpha"
)

val color by animateColorAsState(
    targetValue = if (active) MaterialTheme.colorScheme.primary
                  else MaterialTheme.colorScheme.surface,
    animationSpec = tween(300),
    label = "color"
)

// Int 也能插值
val count by animateIntAsState(
    targetValue = target,
    animationSpec = spring(dampingRatio = 0.5f, stiffness = 800f),
    label = "count"
)
```

# 三、AnimationSpec

## (一) tween 与 spring

```kotlin
// 线性匀速
tween(durationMillis = 300)

// ★FastOutSlowIn：起手快、收尾慢，Material 默认手感
tween(durationMillis = 300, easing = FastOutSlowInEasing)

// spring：物理感，用于跟手交互
spring<Float>(dampingRatio = 0.8f, stiffness = 400f)

// spring(dampingRatio)：三个档位
//   DampingRatioNoBouncy (1f)  —— 不回弹
//   DampingRatioLowBouncy (0.75f) —— 轻微回弹
//   DampingRatioMediumBouncy (0.5f) —— 明显回弹（默认）
//   DampingRatioHighBouncy (0.2f)   —— 强烈回弹
spring(dampingRatio = DampingRatioLowBouncy, stiffness = Spring.StiffnessMediumLow)
```

**选择原则**：

- **用户触发的、由状态变化引起的**（展开/收起、颜色变化）→ `tween` + `FastOutSlowInEasing`
- **跟手的**（拖拽、滑动、按下回弹）→ `spring`
- **表达弹性**（加入购物车、加号点击）→ 低阻尼的 spring

⚠ **不要用 `LinearEasing`**。它是匀速的，在 UI 上看起来机械呆板，只有进度条、加载这类确实需要匀速的场景才用。

## (二) 可中断性

```kotlin
// 目标值变了，动画从当前值平滑接上，不会跳
val offsetX by animateFloatAsState(
    targetValue = target,
    animationSpec = tween(300),
    label = "offset"
)

// 如果希望瞬间跳到新目标
val offsetX by animateFloatAsState(
    targetValue = target,
    animationSpec = snap(),
    label = "offset"
)
```

Compose 动画**默认就是可打断的**——这是它相对传统 `ValueAnimator` 的优势，不需要手动处理取消。

# 四、AnimatedVisibility

```kotlin
@Composable
fun ExpandableSection(expanded: Boolean) {
    AnimatedVisibility(
        visible = expanded,                    // ★直接传布尔，控制显隐
        enter = fadeIn() + expandVertically(),
        exit = fadeOut() + shrinkVertically()
    ) {
        Column {
            Text("展开后的内容")
        }
    }
}
```

常用组合：

```kotlin
enter = slideInVertically(initialOffsetY = { -it }) + fadeIn()
exit  = slideOutVertically(targetOffsetY = { it }) + fadeOut()

enter = expandIn(expandFrom = Alignment.Top)   // 从顶部展开
exit  = shrinkOut(shrinkTowards = Alignment.Top)

enter = scaleIn(initialScale = 0.8f) + fadeIn()
exit  = scaleOut(targetScale = 0.8f) + fadeOut()
```

**`initialOffsetY = { -it }` 里的 lambda 接收的是元素全高**，这样不论元素多高，位移距离都按自身大小算——比写死 `-200.dp` 稳。

# 五、AnimatedContent

两个内容之间的交叉切换：

```kotlin
@Composable
fun Counter(count: Int) {
    AnimatedContent(
        targetState = count,
        transitionSpec = {
            if (targetState > initialState) {
                // 数字增加：新的从下方滑入，旧的向上滑出
                (slideInVertically { it } + fadeIn()) togetherWith
                (slideOutVertically { -it } + fadeOut())
            } else {
                (slideInVertically { -it } + fadeIn()) togetherWith
                (slideOutVertically { it } + fadeOut())
            }.using(SizeTransform(clip = false))
        },
        label = "count"
    ) { value ->
        Text(text = "$value", style = MaterialTheme.typography.displayMedium)
    }
}
```

⚠ **方向判断用 `targetState > initialState`**。两个方向用同一个动画会显得「加号和减号一样」，转场的方向感来自这里。

## (一) 与内容过渡

```kotlin
AnimatedContent(targetState = currentTab) { tab ->
    when (tab) {
        Tab.Home -> HomeScreen()
        Tab.Search -> SearchScreen()
    }
}
```

⚠ **`AnimatedContent` 会在切换时同时保留新旧两个内容**，所以内部不能有不可重复的副作用（`LaunchedEffect` 会各跑一次）。有副作用时用 `ContentTransform.targetContent` 或改用 `Crossfade`。

## (二) Crossfade

需要更简单的交叉淡化时：

```kotlin
Crossfade(targetState = tab, animationSpec = tween(300), label = "tab") { t ->
    when (t) { /* ... */ }
}
```

# 六、多属性联动

用 `updateTransition` 让多个动画共享一个状态：

```kotlin
@Composable
fun FavouriteButton(isFavourite: Boolean) {
    val transition = updateTransition(
        targetState = isFavourite,
        label = "favourite"
    )

    Box(Modifier.size(48.dp), contentAlignment = Alignment.Center) {
        // 三个动画共用一个 transition，保证时序一致
        // ★transition.transitionSpec(targetState) 要和 Transition 的类型一致，
        //   而且它返回的是 AnimationSpec<T>，不能直接喂给 animateDpAsState
        //   （后者要 AnimationSpec<Dp>）。这里用 Float 过渡再换算成尺寸。
        val scale by animateFloatAsState(
            targetValue = if (isFavourite) 28f / 24f else 1f,
            animationSpec = spring(
                dampingRatio = if (isFavourite) DampingRatioLowBouncy
                               else DampingRatioNoBouncy
            ),
            label = "size"
        )

        Icon(
            imageVector = if (isFavourite) Icons.Default.Favorite else Icons.Default.FavoriteBorder,
            contentDescription = "收藏",
            tint = if (isFavourite) Color.Red else Color.Gray,
            modifier = Modifier
                .size(24.dp)
                .scale(scale)
                .graphicsLayer {
                    // ★rotationZ 直接用 progress。原式 360f * if (c) 1f else 0f * progress
                    //   里 * 比三元运算符结合更紧，恒等于 360f —— 收藏与否都在转
                    rotationZ = 360f * transition.progress
                }
        )

        // 缩放后的余晖
        transition.AnimatedVisibility(visible = isFavourite) { }
    }
}
```

⚠ **`transition.progress` 是 `0f→1f` 的整体进度**。拿它做 `rotationZ = 360f * progress` 是对的；拿它做 `if (progress > 0.5)` 这种判断则会每帧重算，失去动画的意义。

# 七、无限循环动画

加载指示、闪烁占位：

```kotlin
@Composable
fun PulsingDot() {
    val transition = rememberInfiniteTransition(label = "pulse")

    val alpha by transition.animateFloat(
        initialValue = 0.3f,
        targetValue = 1f,
        animationSpec = infiniteRepeatable(
            animation = tween(800, easing = LinearEasing),   // 循环动画用线性
            repeatMode = RepeatMode.Reverse                    // 来回
        ),
        label = "alpha"
    )

    Box(
        Modifier
            .size(24.dp)
            .clip(CircleShape)
            .background(Color.Gray.copy(alpha = alpha))
    )
}
```

⚠ **`rememberInfiniteTransition` 会一直运行**，离开组合才停。放在列表 item 里会累积成很多并行动画——列表里的闪烁效果要慎用。

⚠ 循环动画必须给 `label`，否则调试和性能分析工具里全是匿名节点。

# 八、导航转场

```kotlin
NavHost(
    navController = navController,
    startDestination = "home",
    enterTransition = { slideIntoContainer(AnimatedContentTransitionScope.SlideDirection.Left, tween(300)) },
    exitTransition = { slideOutOfContainer(AnimatedContentTransitionScope.SlideDirection.Left, tween(300)) },
    popEnterTransition = { slideIntoContainer(AnimatedContentTransitionScope.SlideDirection.Right, tween(300)) },
    popExitTransition = { slideOutOfContainer(AnimatedContentTransitionScope.SlideDirection.Right, tween(300)) }
) {
    composable("home") { HomeScreen(navController) }
    composable("detail/{id}") { DetailScreen() }
}
```

**四个转场必须成对**：`enter`/`exit` 是前进，`popEnter`/`popExit` 是返回。只写前进不写返回，返回时会是生硬的瞬切。

# 九、性能与无障碍

## (一) 开销大的动画

| 动画 | 开销 | 建议 |
| --- | --- | --- |
| `animateDpAsState` | 极低 | 放心用 |
| `graphicsLayer` 变换 | 低 | 首选（GPU 加速） |
| `Modifier.offset` 动画 | 中 | 触发重组 |
| `animateContentSize` | 中 | 长列表慎用 |
| `AnimatedContent` | 高 | 只在必要处用 |

**属性动画优先用 `graphicsLayer`**，它在绘制阶段执行，不触发重组：

```kotlin
// ✅ 动画值在组合作用域声明，变换在绘制阶段生效
val scale by animateFloatAsState(targetValue = target, label = "scale")

Modifier.graphicsLayer {
    scaleX = scale
    alpha = 1f - scale
}
```

⚠ **`graphicsLayer { }` 的 lambda 不是 `@Composable`**，在里面调 `animateFloatAsState` 会直接编译失败（"Composable invocations can only happen from the context of a @Composable function"）。动画值必须先在外面声明好，再传进 lambda。

## (二) 无障碍

系统开启「移除动画」时，动画应当关闭：

```kotlin
@Composable
fun RespectMotionSettings(content: @Composable () -> Unit) {
    // 读取系统的「移除动画」开关
    val scale = LocalAccessibilityManager.current?.recommendedContrast
    content()
}
```

```kotlin
val spec = if (reduceMotion) snap() else tween(300)

AnimatedContent(
    targetState = tab,
    transitionSpec = { fadeIn(spec) togetherWith fadeOut(spec) }
) { ... }
```

⚠ **前庭功能障碍用户会因动画产生眩晕**。所有非必要动画都应支持一键关闭。

# 十、易错点

| 误区 | 事实 |
| --- | --- |
| 动画要自己算中间值再赋值 | 声明目标值，框架负责插值 |
| 子元素动画了容器高度会自动变 | 不会，要加 `animateContentSize()` |
| `LinearEasing` 是通用选择 | 它看起来机械，UI 动画用 `FastOutSlowInEasing` |
| `AnimatedContent` 里的 `LaunchedEffect` 只跑一次 | 新旧两个内容各跑一次 |
| 列表项里可以用循环动画 | 会累积成大量并行动画，慎用 |
| `animateFloatAsState` 可以写在 `graphicsLayer` 里 | 不可以，每次重组都重建动画对象 |
| 导航只写 enter/exit 就够了 | 还要写 popEnter/popExit，返回时才是连贯的 |
| 弹簧动画不能打断 | 默认可打断，这是 Compose 的优势 |

# 十一、参考资料

- [Compose 动画总览](https://developer.android.com/develop/ui/compose/animation/introduction) — 动画体系入口
- [Compose 动画速查](https://developer.android.com/develop/ui/compose/animation/quick-guide) — `animate*AsState`、`AnimatedVisibility` 的最小示例
- [共享元素转场](https://developer.android.com/develop/ui/compose/animation/shared-elements) — `SharedTransitionLayout` 的用法
- [Compose API 参考](https://developer.android.com/reference/kotlin/androidx/compose/animation/package-summary) — `AnimationSpec`、`Transition` 完整签名
