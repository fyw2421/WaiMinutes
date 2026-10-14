---
title: "AndroidNotes10 : 手势"
weight: 10
description: "Compose 手势处理：pointerInput 与 awaitPointerEventScope、点击与长按与双击、拖拽与滑动的自定义识别、与 Modifier.clickable 的冲突处理、anchoredDraggable 抽屉，以及手势与滚动的手势竞争。"
featureimage: "covers/android-ui.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、两套手势 API

Compose 有两套手势机制，用错会导致手势互相打架：

| | `Modifier.clickable` 等 | `Modifier.pointerInput` |
| --- | --- | --- |
| 适用 | 简单点击、长按、双击 | 自定义拖拽、滑动、多指 |
| 写法 | 声明式，一行 | 命令式，自己识别 |
| 开销 | 低 | 较高 |
| 典型 | 按钮、列表项 | 滑拽删除、下拉刷新、缩放 |

**能用 `clickable` 就别用 `pointerInput`**——简单手势用声明式更短、更不容易错。

# 二、clickable 家族

```kotlin
Modifier.clickable { onClick() }                       // 点击

Modifier.combinedClickable(                           // ★需要 foundation 的实验 API
    onClick = { onClick() },
    onLongClick = { onLongClick() },                   // 长按
    onDoubleClick = { onDoubleClick() },               // 双击
    onLabel = { "文章标题，双击收藏，长按更多" }         // 无障碍朗读文案
)

Modifier.clickable(
    enabled = !isProcessing,                           // 禁用态
    role = Role.Switch,                                // 无障碍角色
    onClick = { onToggle() }
)
```

⚠ **`combinedClickable` 会吞掉点击**，和 `clickable` 不能同时挂在一个元素上。

⚠ **`role` 要写对**——`Role.Switch`、`Role.Checkbox`、`Role.Tab` 会让 TalkBack 读出正确的状态，漏写只会被当成普通按钮。

## (一) 自定义点击

```kotlin
Modifier.pointerInput(Unit) {
    detectTapGestures { offset ->
        onTap(offset)                     // 点击时的位置
    }
}
```

# 三、pointerInput 的结构

```kotlin
Modifier.pointerInput(key1 = state) {     // ★key 变了会重启手势协程
    awaitPointerEventScope {
        // 这里是一个无限循环，监听指针事件
        while (true) {
            val event = awaitPointerEvent()
            // 处理
        }
    }
}
```

⚠ **`pointerInput` 的 key 一定要给对**。写 `Unit` 表示永不重启；如果协程里读了会变的外部状态，那些值永远是第一次的快照。

```kotlin
// ❌ key 写 Unit，enabled 变化不会重启协程
Modifier.pointerInput(Unit) {
    detectDragGestures { change, offset -> if (enabled) { ... } }
}

// ✅ key 包含依赖的变量；三个 detect*Gestures 都没有 enabled 参数，
//    要禁用就在 pointerInput 块里提前返回
Modifier.pointerInput(enabled) {
    if (!enabled) return@pointerInput
    detectDragGestures { change, dragAmount -> ... }
}
```

# 四、拖拽

```kotlin
@Composable
fun DraggableCard(onDismiss: () -> Unit) {
    var offsetX by remember { mutableFloatStateOf(0f) }
    val density = LocalDensity.current

    Box(
        Modifier
            .fillMaxWidth()
            .padding(16.dp)
            .offset { IntOffset(offsetX.roundToInt(), 0) }    // ★offset lambda 不触发重组
            .pointerInput(Unit) {
                detectDragGestures(
                    onDragEnd = {
                        if (offsetX > 300.dp.toPx()) onDismiss()
                    },
                    onDragCancel = { offsetX = 0f },
                    onDrag = { change, dragAmount ->
                        change.consume()                         // ★消费掉，防止冒泡
                        offsetX += dragAmount.x
                    }
                )
            }
    ) {
        Card { Text("左滑删除") }
    }
}
```

## (一) 三个关键点

```kotlin
// 1) 累加的是增量，不是绝对位置
offsetX += dragAmount.x        // ✅ 增量累加
offsetX = position.x            // ❌ 绝对坐标，手指变化会抖

// 2) change.consume() 消费事件
change.consume()               // 不消费的话父容器会同时收到
change.consumePositionChange() // 只消费位置变化

// 3) offset 用 lambda 形式
Modifier.offset { IntOffset(offsetX.roundToInt(), 0) }   // ✅ 绘制阶段，不重组
Modifier.offset(x = offsetX.dp)                          // ❌ 每次重组
```

⚠ **`Modifier.offset(x = dp)` 写在拖拽里会导致每帧重组**。拖拽这种高频操作必须用 `offset { }` 的 lambda 重载。

# 五、滑动识别

```kotlin
Modifier.pointerInput(Unit) {
    detectHorizontalDragGestures(
        onDragEnd = {
            if (abs(offset) > 100) {
                if (offset > 0) onSwipeRight() else onSwipeLeft()
            }
            offset = 0f
        },
        onHorizontalDrag = { change, amount ->
            change.consume()
            offset += amount
        }
    )
}
```

⚠ **只挂一个方向就够了，而且推荐这么做**。`detectHorizontalDragGestures` / `detectVerticalDragGestures` 的 KDoc 明确说明二者会内建协调：「ensure only vertical or horizontal dragging is locked, but not both」。

往另一个方向拖时，方向不符的 slop 检测返回 null 并**不消费事件**，父级滚动照常发生——不会阻止滚动。同时挂两个反而会让它们抢事件。

# 六、多指手势

```kotlin
Modifier.pointerInput(Unit) {
    awaitEachGesture {
        awaitFirstDown(requireUnconsumed = false)          // 等待第一指按下
        var zoom = 1f

        do {
            val event = awaitPointerEvent()
            val zoomChange = event.calculateZoom()
            zoom *= zoomChange

            val panChange = event.calculatePan()
            val centroid = event.calculateCentroid()

            offset = centroid - panChange
        } while (event.changes.any { it.pressed })

        // 手势结束
    }
}
```

`awaitPointerEventScope` 之外还有 `awaitEachGesture`、`awaitPointerEvent` 两层封装，`awaitEachGesture` 会在手势结束后自动复位状态，是更常用的入口。

# 七、anchoredDraggable：抽屉

Material 3 的抽屉不用自己写拖拽：

```kotlin
val drawerState = rememberDrawerState(DrawerValue.Closed)

ModalDrawer(
    drawerState = drawerState,
    drawerContent = {
        ModalDrawerSheet {
            NavigationDrawerItem(
                label = { Text("收件箱") },
                selected = current == Inbox,
                onClick = { scope.launch { drawerState.close() } }
            )
        }
    }
) {
    Scaffold(topBar = {
        TopAppBar(
            title = { Text("文章") },
            navigationIcon = {
                IconButton(onClick = { scope.launch { drawerState.open() } }) {
                    Icon(Icons.Default.Menu, contentDescription = "打开菜单")
                }
            }
        )
    }) { /* 内容 */ }
}
```

⚠ **`drawerState` 要 `remember`**。每次重组新建会让抽屉状态丢失、动画失效。

# 八、手势与滚动的竞争

横向滑动和纵向滚动在同一个容器里会冲突。三种处理方式：

```kotlin
// 1) 滚动组件自己处理（最省事）
LazyRow { /* ... */ }     // 横向滚动天然与纵向滚动不冲突

// 2) 自定义连接（跨容器精细控制）
//    ★没有叫 NestedScroll 的 Composable，入口只有 Modifier.nestedScroll(connection)
Modifier.nestedScroll(
    connection = object : NestedScrollConnection {
        // 只拦垂直分量，横滑放行
        override fun onPreScroll(
            available: Offset,
            source: NestedScrollSource
        ): Offset = available.copy(y = 0f)

        // 不接fling，父容器剩余的惯性距离会被丢弃
        override fun onPreFling(available: Offset): Offset = Offset.Zero
        override fun onPostFling(consumed: Offset, available: Offset): Offset = available
    }
) { ... }

// 3) 手动决定拦截时机（精细控制）
Modifier.nestedScroll(
    connection = object : NestedScrollConnection {
        override fun onPreScroll(
            available: Offset,
            source: NestedScrollSource
        ): Offset {
            // 只在垂直方向且源是用户触摸时才拦截
            return if (source == NestedScrollSource.UserInput && available.y != 0f) {
                available.copy(y = 0f)       // ★只吃垂直分量
            } else {
                Offset.Zero
            }
        }
    }
)
```

⚠ **子组件默认拿到手势**（子优先于父）。父容器想优先拿到，必须用 `NestedScrollSource` 显式指定优先级。

# 九、点击与滚动的区分

Compose 自动处理「滑动时不要触发点击」：

```kotlin
LazyColumn {
    items(data) { item ->
        Box(
            Modifier.clickable { onClick(item) }
                .combinedClickable(
                    onClick = { onClick(item) },
                    onLongClick = { onLongClick(item) }
                )
        )
    }
}
```

⚠ **`clickable` 与 `combinedClickable` 挂在同一元素上会互相覆盖**，只保留后者。

⚠ **自定义拖拽时要注意**：`detectDragGestures` 一旦识别出拖拽就会 `consume` 掉事件，此时松手不会触发 `click`。这是期望行为；若需要「轻微移动也算点击」，自己用 `totalMovement` 判断距离。

# 十、性能提示

```kotlin
// 1) 拖拽位置存 mutableFloatStateOf，用 offset lambda 读
var offsetX by remember { mutableFloatStateOf(0f) }
Modifier.offset { IntOffset(offsetX.roundToInt(), 0) }

// 2) 手势计算放到 DrawScope 里，完全不重组
Modifier.drawBehind {
    translate(offsetX) { drawContent() }
}
```

⚠ **拖拽时每帧都重组会让列表明显掉帧**。一个 `LazyColumn` 里 20 项同时可拖拽，用错写法帧率能掉一半以上。

# 十一、易错点

| 误区 | 事实 |
| --- | --- |
| `pointerInput(Unit)` 就行 | key 必须包含协程里读到的外部变量，否则读到的是首次快照 |
| `offsetX = position.x` 正确 | 应累加**增量** `dragAmount`，用绝对坐标会抖 |
| `change.consume()` 可选 | 不消费，事件会冒泡给父容器 |
| `Modifier.offset(x = x.dp)` 够快 | 拖拽场景会每帧重组，用 `offset { }` |
| `clickable` 和 `combinedClickable` 能共存 | 不能，会互相覆盖 |
| `drawBehind` 里不能用 `drawContent` | 可以，`drawContent()` 正是用来画原内容的 |
| 抽屉状态可以每次新建 | 必须 `rememberDrawerState` |
| 自定义手势不用管滚动冲突 | 横向与纵向会打架，需要 `NestedScroll` |

# 十二、参考资料

- [Compose 手势检测](https://developer.android.com/develop/ui/compose/touch-input/pointer-input) — `pointerInput` 与各种检测器
- [Compose 手势 API 参考](https://developer.android.com/reference/kotlin/androidx/compose/foundation/gestures/package-summary) — 全部手势检测器签名
- [嵌套滚动](https://developer.android.com/develop/ui/compose/touch-input/pointer-input/nested-scroll) — 手势与滚动的协调
- [Material 3 抽屉](https://m3.material.io/components/navigation-drawer/overview/) — 抽屉的设计与交互规范
