---
title: "AndroidNotes04 : 布局与状态提升"
weight: 4
description: "Compose 布局体系与状态提升：Column/Row/Box 与权重分配、对齐与自适应尺寸、IntrinsicMeasurement 与自定义 Layout、状态的所有权与提升原则、回调命名规范，以及无状态与有状态组件的分工。"
featureimage: "covers/android-basics.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、布局基础

## (一) 三个基本容器

```kotlin
Column(                       // 竖直排列
    verticalArrangement = Arrangement.spacedBy(8.dp),    // 主轴
    horizontalAlignment = Alignment.CenterHorizontally   // 交叉轴
) {
    Text("第一项")
    Text("第二项")
}

Row(                          // 水平排列
    horizontalArrangement = Arrangement.SpaceBetween,    // 主轴
    verticalAlignment = Alignment.CenterVertically       // 交叉轴
) {
    Text("左")
    Text("右")
}

Box(                          // 叠放
    modifier = Modifier.size(200.dp)
) {
    Box(Modifier.fillMaxSize().background(Color.Gray))   // 被盖住
    Text("在最上层", modifier = Modifier.align(Alignment.Center))
}
```

`Arrangement` 管**主轴**、`Alignment` 管**交叉轴**，是两套独立参数：Column 用 `verticalArrangement` + `horizontalAlignment`，Row 正好反过来。

`Arrangement.spacedBy(8.dp)` 是列表间距的标准写法——比给每个子项加 `padding` 更好，因为第一个和最后一个不会多出外边距。

## (二) weight 的分配规则

```kotlin
Column(Modifier.fillMaxWidth()) {
    Text("固定高度", modifier = Modifier.height(60.dp))
    Box(Modifier.weight(1f).background(Color.LightBlue))    // 占满剩余
    Text("固定高度", modifier = Modifier.height(60.dp))
}
```

**规则**：先扣掉**所有非权重**子项占的空间，剩下的按权重比例分配。

```kotlin
Row(Modifier.fillMaxWidth()) {
    Text("自适应", modifier = Modifier.weight(1f, fill = false))
    Text("撑满剩余", modifier = Modifier.weight(1f))          // fill = true（默认）
}
```

⚠ 三个坑：

1. **`weight` 只能在 `ColumnScope` / `RowScope` 内用**，写在外层编译报错——它是这两个容器的扩展函数
2. **`fillMaxWidth()` + `weight(1f)` 会得到零宽度**，因为前者已经占满、没有剩余可分
3. **`fill = false` 是「按内容所需分配」**，`fill = true`（默认）是「强行占满可用空间」。文字自适应必须显式写 `fill = false`

## (三) 三个常见布局模式

```kotlin
// 1) 列表项：主内容自适应，操作按钮固定
Row(
    modifier = Modifier.fillMaxWidth().padding(16.dp),
    verticalAlignment = Alignment.CenterVertically
) {
    Column(Modifier.weight(1f)) {
        Text(title, style = MaterialTheme.typography.titleMedium)
        Text(subtitle, style = MaterialTheme.typography.bodySmall)
    }
    IconButton(onClick = onDelete) {          // 不给权重，宽度由内容定
        Icon(Icons.Default.Delete, contentDescription = "删除")
    }
}

// 2) 表单：标签固定宽度，输入框自适应
Row(Modifier.fillMaxWidth()) {
    Text("用户名", Modifier.width(80.dp))
    OutlinedTextField(value = name, onValueChange = onNameChange, Modifier.weight(1f))
}

// 3) 居中：Box + align
Box(Modifier.fillMaxSize(), contentAlignment = Alignment.Center) {
    CircularProgressIndicator()
}
```

# 二、自适应尺寸

Compose 一切尺寸默认是 `Dp`，**屏幕适配靠约束自适应，不靠多套布局文件**。

```kotlin
Modifier.widthIn(max = 480.dp)          // 内容不超 480dp（表单在平板上很关键）
Modifier.aspectRatio(16f / 9f)          // 宽高比
Modifier.fillMaxHeight(0.5f)            // 占 50% 高度
Modifier.heightIn(min = 48.dp)          // 最小高度
Modifier.defaultMinSize(minWidth = 48.dp, minHeight = 48.dp)   // 无障碍最小可点区域
Modifier.verticalScroll(rememberScrollState())
```

⚠ **不要写死 `px`**。`dp` 会按屏幕密度换算，`px` 是物理像素，在不同设备上尺寸完全不同。

## (一) insets

`targetSdk 35` 起应用默认铺满全屏，内容会被状态栏和导航栏盖住。必须显式处理：

```kotlin
Scaffold(modifier = Modifier.fillMaxSize()) { innerPadding ->
    Column(
        modifier = Modifier
            .fillMaxSize()
            .padding(innerPadding)      // ★Scaffold 已经算好了，直接用
    ) {
        TopAppBar(title = { Text("文章列表") })
        ArticleList()
    }
}
```

`Scaffold` 的 `innerPadding` 参数就是 insets 的标准解法。不想用 `Scaffold` 时用 `WindowInsets` 直接取：

```kotlin
val bars = WindowInsets.systemBars.asPaddingValues()
Column(Modifier.padding(bars)) { ... }
```

# 三、自定义布局

当 `Column`/`Row` 组合表达不了时，实现 `Layout` 接口。它要回答两个问题：**要多大**、**子项摆哪**。

```kotlin
@Composable
fun StackedLayout(
    modifier: Modifier = Modifier,
    content: @Composable () -> Unit
) {
    Layout(modifier = modifier, content = content) { measurables, constraints ->
        // 1. 测量所有子项
        val placeables = measurables.map { it.measure(constraints) }

        // 2. 决定自身尺寸
        val width = placeables.maxOf { it.width }
        val height = placeables.sumOf { it.height }

        // 3. 摆放每个子项
        layout(width, height) {
            var y = 0
            placeables.forEach { placeable ->
                placeable.placeRelative(0, y)
                y += placeable.height
            }
        }
    }
}
```

**测量与摆放是两个独立阶段**，Compose 允许「先测量完所有子项，再决定父容器多大」。这比 View 的「边测量边布局」更灵活，也是 `weight` 能工作的基础。

## (一) 更简洁的写法

多数自定义布局用 `Layout` + 委托能省掉重复代码：

```kotlin
@Composable
fun StackedLayout(
    modifier: Modifier = Modifier,
    content: @Composable () -> Unit
) {
    Layout(content = content, modifier = modifier) { measurables, constraints ->
        val placeables = measurables.map { it.measure(constraints) }
        val width = placeables.maxOfOrNull { it.width } ?: constraints.minWidth
        val height = placeables.sumOf { it.height }

        layout(width, height) {
            placeables.forEachIndexed { index, p ->
                p.placeRelative(0, placeables.take(index).sumOf { it.height })
            }
        }
    }
}
```

## (二) 内在尺寸测量

需要「根据内容决定自身大小」时（比如自动换行的标签流），用 `IntrinsicMeasurement`：

```kotlin
Layout(content = content) { measurables, constraints ->
    // minIntrinsicWidth：内容「最希望」多宽
    val width = measurables.maxOf { it.minIntrinsicWidth }
    layout(width, constraints.maxHeight) { ... }
}
```

⚠ **内在测量很贵**（每项要测 2–4 次），只在小元素上用。Compose 1.5 后的 `FlowRow` 已经内置好，不需要自己实现。

# 四、状态提升

## (一) 问题的提出

```kotlin
@Composable
fun Counter() {
    var count by remember { mutableStateOf(0) }    // 状态私有持有

    Column {
        Text("$count")
        Button(onClick = { count++ }) { }           // 组件自己改
    }
}
```

状态藏在组件内部，带来两个问题：**无法从外部控制**、**无法复用**（两个计数器想联动就必须拆组件）。

## (二) 提升的本质

**把状态移到调用方，组件只负责渲染与上报事件**——这就是状态提升。

```kotlin
// 无状态组件：只读状态、只发事件
@Composable
fun Counter(count: Int, onIncrement: () -> Unit) {
    Column {
        Text("点击了 $count 次")
        Button(onClick = onIncrement) { Text("加一") }
    }
}

// 有状态组件：持有状态，向下传递
@Composable
fun CounterApp() {
    var count by rememberSaveable { mutableStateOf(0) }

    Column {
        Counter(count = count, onIncrement = { count++ })
        Button(onClick = { count = 0 }) { Text("重置") }
    }
}
```

改造后 `Counter` 既能被外部控制，也能复用——因为它不再依赖任何私有状态。

## (三) 判断标准

一个组件该不该提升状态，问三个问题：

| 问题 | 是 → |
| --- | --- |
| 父组件是否需要读它？ | 提升 |
| 多个子组件是否共享它？ | 提升 |
| 它是否会随时间自己变？ | 提升 |

**状态应该放在「所有需要它的组件的最近公共祖先」**——放高了会不必要地扩大重组范围，放低了别的组件够不着。

## (四) 回调的命名规范

Google 的 Compose 规范：**状态参数用名词、回调参数用动词短语**。

```kotlin
// ✅ 标准写法
fun UserCard(name: String, onClick: () -> Unit, onDismiss: () -> Unit)

// ❌ 含糊
fun UserCard(name: String, click: () -> Unit, close: () -> Unit)
```

传 `() -> Unit` 还是 `T -> Unit` 取决于是否携带参数：

```kotlin
fun ArticleRow(onSelect: (Article) -> Unit)     // 需要 id
fun ConfirmDialog(onConfirm: () -> Unit)        // 不需要参数
```

**单事件回调用 `onXxx: () -> Unit`；多事件回调用 `onXxx: (Event) -> Unit` 携带密封类**。多事件时后者可扩展性更好：

```kotlin
sealed interface ArticleEvent {
    data object Delete : ArticleEvent
    data class Share(val article: Article) : ArticleEvent
    data class Open(val id: Long) : ArticleEvent
}

fun ArticleList(onEvent: (ArticleEvent) -> Unit) { ... }
```

## (五) 提升到哪一层：State 包装

状态提升后，最外层通常需要一个可观察容器。传 `T` 还是 `State<T>`，差别在于**谁在读它**：

```kotlin
// ❌ 在这里读 .value —— 状态一变，本函数照样被标脏，没有任何收益
@Composable
fun CounterRow(count: State<Int>, name: String) {
    Text("${count.value}")               // 父级仍会重组
    Text(name)
}

// ✅ 把 State 交给真正需要它的子组件，中间的父组件就能跳过
@Composable
fun CounterRow(count: State<Int>, name: String) {
    Text(name)                           // 只读 name：name 不变就跳过
    CountText(count)                     // 把 State 传下去
}

@Composable
private fun CountText(count: State<Int>) {
    Text("${count.value}")               // 只有 count 变才重组这里
}
```

**跳过判定永远是函数级的**，不存在「把判定推迟到读取处」这一步。`State<T>` 的真正价值是**状态所有者与读取者解耦**——父组件不读就不会被牵连。

# 五、有状态与无状态

| 类型 | 特征 | 用途 |
| --- | --- | --- |
| **无状态** | 只收参数、只发回调 | 通用组件，可复用、易预览 |
| **有状态** | 内部 `remember` | 包装层，负责持有状态 |

工程上通常成对出现——有状态组件是唯一入口：

```kotlin
// 有状态：唯一入口，负责把状态接进来
@Composable
fun ArticleRoute(
    viewModel: ArticleViewModel = hiltViewModel()
) {
    val uiState by viewModel.uiState.collectAsStateWithLifecycle()
    ArticleScreen(
        state = uiState,
        onEvent = viewModel::onEvent
    )
}

// 无状态：可预览、可测试，不碰 ViewModel
@Composable
fun ArticleScreen(state: UiState, onEvent: (ArticleEvent) -> Unit) {
    if (state.loading) {
        CircularProgressIndicator()
    } else {
        LazyColumn {
            items(state.articles, key = { it.id }) { ArticleRow(it) }
        }
    }
}
```

这样拆分的好处是**预览 `Screen` 不需要 ViewModel，测试也不必构造依赖**——给个 `UiState` 就能测。

⚠ **用 `collectAsStateWithLifecycle()` 而不是 `collectAsState()`**：后者在 App 进入后台后仍会收集，白白耗电且可能在后台更新 UI 报错。

# 六、重组范围的取舍

状态提升不是越高越好。放得太高，一个字段变化会连带重组整棵子树。

```kotlin
// ❌ 顶层一个 state 变化，全树重算
var query by remember { mutableStateOf("") }
Column {
    SearchBar(query) { query = it }
    ResultList(query)
}

// ✅ 拆成两个状态容器，各自独立重组
SearchSection(query, onQueryChange)     // 自己的局部 state
ResultList(query)
```

**传递基本类型（String、Int）比传递复杂对象更容易跳过重组**——因为相等判断更便宜。传递 `State<T>` 而非 `T` 可以把跳过判断推迟到读取处。

判断是否需要优化的指标：**用 Layout Inspector 看重组计数**。稳态下同一个组件每帧重组多次，就该拆了。

# 七、易错点

| 误区 | 事实 |
| --- | --- |
| `weight` 可以写在任何地方 | 只能在 `ColumnScope`/`RowScope` 内 |
| `fillMaxWidth()` + `weight` 一起用 | 前者已占满，权重分不到空间 |
| `weight(1f)` 就是「占一半」 | 是占**剩余空间**的一半，固定尺寸的子项要先扣掉 |
| `offset` 会推开其他元素 | 不会，它只改绘制位置，会产生重叠 |
| 状态提升得越高越好 | 不是，会无谓扩大重组范围 |
| 传 `Modifier` 参数忘了设命名 | 应写作 `modifier: Modifier = Modifier`，放在**参数列表最后** |
| `Arrangement` 管主轴还是交叉轴 | 主轴；交叉轴用 `Alignment` |
| 自定义布局直接在测量里摆放 | 测量和摆放是两阶段，摆放必须在 `layout {}` 里 |
| 用 `collectAsState()` 就行 | 用 `collectAsStateWithLifecycle()`，后台不收集 |
| 自己实现 `FlowRow` | 用 Compose 内置的 `FlowRow`，内在测量很贵 |

# 八、参考资料

- [Compose 布局官方指南](https://developer.android.com/develop/ui/compose/layouts) — 布局基础与修饰符
- [Compose 状态](https://developer.android.com/develop/ui/compose/state-hoisting) — 状态提升的官方说明与设计原则
- [Compose 自定义布局](https://developer.android.com/develop/ui/compose/layouts/custom) — `Layout` 接口与内在测量
- [Compose 编程思想](https://developer.android.com/develop/ui/compose/mental-model) — 组合与重组模型
- [边到边显示](https://developer.android.com/develop/ui/compose/layouts/insets) — insets 的标准处理方式
- [Material Design 布局原则](https://m3.material.io/foundations/layout/) — Google 设计规范对布局的指导
- [Kotlin 官方文档](https://kotlinlang.org/docs/home.html) — 语言参考
