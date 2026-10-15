---
title: "AndroidNotes07 : 组件与交互"
weight: 8
description: "Compose 常用组件与交互反馈：按钮与图标按钮、文本输入与表单校验、对话框与底部弹窗、Snackbar 与 FAB、下拉菜单与卡片，以及状态、禁用与加载态的正确处理方式。"
featureimage: "covers/android-ui.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、按钮

Material 3 提供五个按钮变体，按语义选而不是按好看选：

| 变体 | 用途 | 同时能出现几个 |
| --- | --- | --- |
| `Button` | 页面主操作 | **一个** |
| `FilledTonalButton` | 次要操作 | 少量 |
| `OutlinedButton` | 与主操作平级 | 少量 |
| `TextButton` | 取消、跳过等低强调 | 多个 |
| `FloatingActionButton` | 页面级主动作 | 一个 |

```kotlin
// 主操作：Filled
Button(onClick = onSubmit) { Text("提交") }

// 次要：Tonal
FilledTonalButton(onClick = onSave) { Text("存草稿") }

// 低强调：Text
TextButton(onClick = onCancel) { Text("取消") }
```

⚠ **一个屏幕最多一个 `Button`**。两个填满色按钮会让用户不知道该点哪个——这是 Material 规范里最常被违反的一条。

## (一) 图标与状态

```kotlin
// 图标按钮必须有 contentDescription，否则无障碍读不出来
IconButton(onClick = onBack) {
    Icon(Icons.AutoMirrored.Filled.ArrowBack, contentDescription = "返回")
}

// 加载态：替换内容并禁用
Button(
    onClick = onSubmit,
    enabled = !isSubmitting,           // ★提交中禁用，防止重复提交
    contentPadding = ButtonDefaults.ContentPadding
) {
    if (isSubmitting) {
        CircularProgressIndicator(
            modifier = Modifier.size(18.dp),
            strokeWidth = 2.dp,
            color = MaterialTheme.colorScheme.onPrimary
        )
    } else {
        Text("提交")
    }
}
```

⚠ **按钮的尺寸会随内容变化**。加载指示器和文字宽度不同，会导致按钮在点击时「跳一下」——给按钮固定宽度或给加载指示器与文字预留相同尺寸。

# 二、文本输入

## (一) 基础与状态

```kotlin
var name by rememberSaveable { mutableStateOf("") }
val isValid = name.isNotBlank()

OutlinedTextField(
    value = name,
    onValueChange = { name = it },
    label = { Text("昵称") },
    isError = !isValid && name.isNotEmpty(),      // ★空值不算错
    supportingText = {
        if (!isValid && name.isNotEmpty()) Text("昵称不能为空")
    },
    singleLine = true,
    keyboardOptions = KeyboardOptions(
        keyboardType = KeyboardType.Email,
        imeAction = ImeAction.Next
    ),
    modifier = Modifier.fillMaxWidth()
)
```

**`value` + `onValueChange` 是受控组件的写法**：状态在上层，TextField 只是渲染。这也是状态提升的典型场景。

## (二) 密码与错误提示

```kotlin
var password by remember { mutableStateOf("") }
var visible by remember { mutableStateOf(false) }

OutlinedTextField(
    value = password,
    onValueChange = { password = it },
    label = { Text("密码") },
    visualTransformation = if (visible) VisualTransformation.None
                           else PasswordVisualTransformation(),
    trailingIcon = {
        IconButton(onClick = { visible = !visible }) {
            Icon(
                imageVector = if (visible) Icons.Default.VisibilityOff
                              else Icons.Default.Visibility,
                contentDescription = if (visible) "隐藏密码" else "显示密码"
            )
        }
    },
    supportingText = { Text("至少 8 位") }
)
```

⚠ **`KeyboardOptions` 在 `androidx.compose.foundation.text`**，不是 `ui.text`。写错包名会找不到符号。

# 三、表单校验

校验逻辑不要写在 onValueChange 里，单独成一个纯函数便于测试：

```kotlin
sealed interface FormState {
    data object Idle : FormState
    data object Validating : FormState
    data class Error(val message: String) : FormState
    data object Success : FormState
}

fun validateName(name: String, taken: Set<String>): FormState = when {
    name.isBlank() -> FormState.Idle
    name.length < 2 -> FormState.Error("昵称至少 2 个字符")
    name in taken -> FormState.Error("该昵称已被占用")
    else -> FormState.Success
}
```

**表单整体用 `rememberSaveable` 持有**，旋转屏幕不丢输入：

```kotlin
@Composable
fun ProfileForm(onSubmit: (String) -> Unit) {
    var name by rememberSaveable { mutableStateOf("") }
    var email by rememberSaveable { mutableStateOf("") }

    val nameState = validateName(name, emptySet())

    Column(Modifier.verticalScroll(rememberScrollState()).padding(16.dp)) {
        OutlinedTextField(
            value = name,
            onValueChange = { name = it },
            label = { Text("昵称") },
            isError = nameState is FormState.Error,
            supportingText = { (nameState as? FormState.Error)?.let { Text(it.message) } },
            modifier = Modifier.fillMaxWidth()
        )
        Spacer(Modifier.height(16.dp))
        Button(
            onClick = { onSubmit(name) },
            enabled = nameState is FormState.Success,      // ★只有通过校验才可提交
            modifier = Modifier.fillMaxWidth()
        ) { Text("保存") }
    }
}
```

⚠ **按钮的 `enabled` 不要只看「表单有没有全填」**，用校验函数的返回值直接驱动——校验规则变了，UI 不用动。

# 四、对话框

## (一) 确认框

```kotlin
var showDelete by remember { mutableStateOf(false) }

// ...
if (showDelete) {
    AlertDialog(
        onDismissRequest = { showDelete = false },     // ★点外面/返回键必须能关
        title = { Text("删除这条评论？") },
        text = { Text("删除后无法恢复。") },
        confirmButton = {
            TextButton(onClick = {
                viewModel.delete(id)
                showDelete = false
            }) { Text("删除") }
        },
        dismissButton = {
            TextButton(onClick = { showDelete = false }) { Text("取消") }
        }
    )
}
```

⚠ **`onDismissRequest` 是必填的**。漏掉会导致返回键关不掉对话框，用户被卡住——这是 Compose 对话框最常见的 bug。

## (二) 自定义内容

```kotlin
AlertDialog(
    onDismissRequest = { show = false },
    title = { Text("选择排序方式") },
    text = {
        Column {
            listOf("最新", "最热", "最早").forEach { option ->
                RadioButton(
                    selected = current == option,
                    onClick = { current = option },
                    label = { Text(option) }       // ★label 用 Text，text 只放选中态
                )
            }
        }
    },
    confirmButton = {
        TextButton(onClick = { show = false }) { Text("确定") }
    }
)
```

# 五、底部弹窗

```kotlin
@OptIn(ExperimentalMaterial3Api::class)
@Composable
fun ShareSheet(onDismiss: () -> Unit) {
    ModalBottomSheet(onDismissRequest = onDismiss) {
        // ★不要自己加 navigationBarsPadding()：ModalBottomSheet 默认已对内容列
        //   应用了 contentWindowInsets = safeDrawing.only(Bottom + Top)，
        //   而且会【消费】这些 inset，重复加在竖屏下是 0 额外 padding
        Column(Modifier.padding(24.dp)) {
            Text("分享到", style = MaterialTheme.typography.titleMedium)
            Spacer(Modifier.height(16.dp))
            ShareTarget("微信", R.drawable.ic_wechat, onDismiss)
            ShareTarget("微博", R.drawable.ic_weibo, onDismiss)
            ShareTarget("复制链接", R.drawable.ic_link) {
                clipboard.setText(Uri.parse(url).toString())
                onDismiss()
            }
        }
    }
}
```

⚠ **想改底部避让，用 `contentWindowInsets` 参数覆盖默认行为**，不要在内容里手动加 padding（默认的已消费，再加会重复）：

```kotlin
ModalBottomSheet(
    onDismissRequest = onDismiss,
    contentWindowInsets = { WindowInsets.safeDrawing.only(WindowInsetsSides.Bottom) }
) { /* ... */ }
```

**`ModalBottomSheet` 会抢占返回键**并自动处理动画，不要再手动 `AnimatedVisibility` 包一层。

# 六、Snackbar

Snackbar 用于展示「已完成的操作」的结果提示：

```kotlin
val snackbarHostState = remember { SnackbarHostState() }
val scope = rememberCoroutineScope()

Scaffold(
    snackbarHost = { SnackbarHost(snackbarHostState) }      // ★挂在 Scaffold 上
) { padding ->
    Button(onClick = {
        scope.launch {
            val result = snackbarHostState.showSnackbar(
                message = "已复制到剪贴板",
                actionLabel = "撤销",
                duration = SnackbarDuration.Short
            )
            if (result == SnackbarResult.ActionPerformed) {
                // 用户点了「撤销」
                restore()
            }
        }
    }) { Text("复制链接") }
}
```

⚠ **Snackbar 用 `showSnackbar`（挂起），不要用 `SnackbarHostState.currentSnackbarData`**。后者已被弃用，且拿不到点击结果。

⚠ **在挂起函数里调用**——必须包在 `scope.launch` 里，否则会抛异常。

# 七、FAB

```kotlin
Box(Modifier.fillMaxSize()) {
    LazyColumn(Modifier.fillMaxSize()) { /* ... */ }

    FloatingActionButton(
        onClick = onAdd,
        modifier = Modifier
            .align(Alignment.BottomEnd)
            .padding(16.dp)
            .navigationBarsPadding()          // ★不被系统条压住
    ) {
        Icon(Icons.Default.Add, contentDescription = "新建")
    }
}
```

⚠ **FAB 会遮挡列表最后几项**。要给列表留出底部空间，否则最后一条永远点不到：

```kotlin
LazyColumn(contentPadding = PaddingValues(bottom = 96.dp)) { /* ... */ }
```

# 八、卡片与列表项

```kotlin
// 卡片：分组信息
Card(
    onClick = { onClick(item.id) },       // 可点击的卡片直接用 onClick 重载
    modifier = Modifier.fillMaxWidth()
) {
    Column(Modifier.padding(16.dp)) {
        Text(item.title, style = MaterialTheme.typography.titleMedium)
        Spacer(Modifier.height(4.dp))
        Text(
            text = item.summary,
            style = MaterialTheme.typography.bodyMedium,
            color = MaterialTheme.colorScheme.onSurfaceVariant,
            maxLines = 2,
            overflow = TextOverflow.Ellipsis
        )
    }
}
```

⚠ **点击区域要够**。Material 规范要求可点击元素不小于 48×48dp：

```kotlin
IconButton(modifier = Modifier.size(48.dp)) { ... }     // 显式保证
IconButton(modifier = Modifier.defaultMinSize(minWidth = 48.dp, minHeight = 48.dp)) { ... }
```

# 九、下拉菜单

```kotlin
var expanded by remember { mutableStateOf(false) }

Box {
    IconButton(onClick = { expanded = true }) {
        Icon(Icons.Default.MoreVert, contentDescription = "更多")
    }
    DropdownMenu(expanded = expanded, onDismissRequest = { expanded = false }) {
        DropdownMenuItem(
            text = { Text("编辑") },
            leadingIcon = { Icon(Icons.Default.Edit, null) },
            onClick = {
                onEdit()
                expanded = false        // ★选完必须收起
            }
        )
        DropdownMenuItem(
            text = { Text("删除") },
            leadingIcon = { Icon(Icons.Default.Delete, null) },
            onClick = {
                onDelete()
                expanded = false
            }
        )
    }
}
```

⚠ **`DropdownMenu` 必须放在触发它的组件外面一层 `Box`**——它靠这个 `Box` 的位置定位。直接放进 `Row` 会跑到屏幕边缘。

# 十、组件状态对照

| 状态 | 做法 | 别用 |
| --- | --- | --- |
| 加载中 | 显示进度、禁用触发 | 什么反应都没有 |
| 空数据 | 显示空态视图与引导 | 空白屏幕 |
| 出错 | 显示错误与重试按钮 | 崩溃或空白 |
| 禁用 | `enabled = false` | 隐藏按钮 |

```kotlin
@Composable
fun StateHost<T>(
    state: LoadState<T>,
    onRetry: () -> Unit,
    content: @Composable (T) -> Unit
) {
    when (state) {
        is LoadState.Loading -> Box(Modifier.fillMaxSize(), Alignment.Center) {
            CircularProgressIndicator()
        }
        is LoadState.Error -> Box(Modifier.fillMaxSize(), Alignment.Center) {
            Column(horizontalAlignment = Alignment.CenterHorizontally) {
                Text(state.message)
                Spacer(Modifier.height(8.dp))
                Button(onClick = onRetry) { Text("重试") }
            }
        }
        is LoadState.Success -> if (state.data.isEmpty()) {
            EmptyState(message = "还没有内容")
        } else {
            content(state.data)
        }
    }
}
```

# 十一、易错点

| 误区 | 事实 |
| --- | --- |
| 一页可以有两个 `Button` | 不行，最多一个填满色主操作 |
| `AlertDialog` 漏掉 `onDismissRequest` 没问题 | 必填，漏了返回键关不掉 |
| `SnackbarHostState.currentSnackbarData` 还能用 | 已弃用，用挂起的 `showSnackbar` |
| 下拉菜单放进 `Row` 里就行 | 需要外套 `Box` 才能正确定位 |
| 表单提交按钮只看字段非空 | 应该由校验函数的返回值驱动 |
| FAB 直接放屏幕上 | 会挡住列表末项，要留 `contentPadding` |
| 加载时用 `disabled` 视觉代替 | 要同时 `enabled = false`，否则还能点 |
| 图标按钮不用写 `contentDescription` | 必须写，否则无障碍服务读不出来 |

# 十二、参考资料

- [Compose 组件总览](https://developer.android.com/develop/ui/compose/components) — 各组件的官方文档入口
- [Material 3 Compose 依赖](https://developer.android.com/develop/ui/compose/designsystems/material3) — Material 3 组件与主题
- [Material Design 3 组件规范](https://m3.material.io/components/overview/) — 每个组件的尺寸、间距、状态定义
