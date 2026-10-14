---
title: "AndroidNotes06 : Material 3 与主题"
weight: 6
description: "Jetpack Compose 的 Material 3 主题体系：ColorScheme 与语义色槽位、动态取色、自定义配色方案、Typography 排版体系、Shapes 形状与 Elevation 层级，以及 Theme 的正确组织方式与深色模式。"
featureimage: "covers/android-ui.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、为什么用语义色

Material 3 的颜色不是「红/蓝/灰」，而是**语义色槽位**：`primary`、`surface`、`onSurface`、`error`……组件只声明「我要主色」「我要错误色」，具体值由主题决定。

这是深色模式与动态取色能自动生效的前提：

```kotlin
// ✅ 组件只依赖语义
Button(colors = ButtonDefaults.buttonColors()) { Text("提交") }

// ❌ 写死颜色，深色模式下必然要改代码
Button(colors = ButtonDefaults.buttonColors(
    containerColor = Color(0xFF6750A4)
)) { Text("提交") }
```

## (一) 色槽位的成对关系

每个容器色都有一个 `on*` 前景色，**两者必须成对使用**：

| 槽位 | 含义 | 配套 |
| --- | --- | --- |
| `primary` | 主要动作的容器色 | `onPrimary` |
| `primaryContainer` | 次要强调的容器色 | `onPrimaryContainer` |
| `surface` | 默认背景 | `onSurface` |
| `surfaceVariant` | 次级背景（卡片、表头） | `onSurfaceVariant` |
| `error` | 错误状态 | `onError` |
| `outline` | 边框、分隔线 | — |

⚠ **`surface` 与 `onSurface` 是一组，`surface` 与 `primary` 不是**。写 `Text(color = MaterialTheme.colorScheme.primary)` 放在 `surface` 背景上，对比度可能不达标——文本颜色默认应该用 `onSurface`。

## (二) 取用方式

```kotlin
// 推荐：Composable 里
Text(
    text = "标题",
    color = MaterialTheme.colorScheme.onSurface,
    style = MaterialTheme.typography.titleLarge
)

// 非 Composable 里（如自定义 View 的绘制代码）
val primary = MaterialTheme.colorScheme.primary   // 只能在 @Composable 中调用
```

⚠ **`MaterialTheme.colorScheme` 只能在 `@Composable` 函数里访问**。在普通类的方法里取会编译报错——需要的话用 `CompositionLocal` 提前取出再传进去。

# 二、ColorScheme 的来源

## (一) 三种取值方式

```kotlin
// 1) 手工定义：品牌色一致性最强
private val LightColors = lightColorScheme(
    primary = Color(0xFF6750A4),
    onPrimary = Color(0xFFFFFFFF),
    primaryContainer = Color(0xFFEADDFF),
    onPrimaryContainer = Color(0xFF21005D),
    secondary = Color(0xFF625B71),
    onSecondary = Color(0xFFFFFFFF),
    error = Color(0xFFB3261E),
    onError = Color(0xFFFFFFFF),
    background = Color(0xFFFFFBFE),
    onBackground = Color(0xFF1C1B1F),
    surface = Color(0xFFFFFBFE),
    onSurface = Color(0xFF1C1B1F),
    surfaceVariant = Color(0xFFE7E0EC),
    onSurfaceVariant = Color(0xFF49454F),
    outline = Color(0xFF79747E)
)

private val DarkColors = darkColorScheme(
    primary = Color(0xFFD0BCFF),
    onPrimary = Color(0xFF381E72),
    // ... 深色下同一语义通常用更浅的色
)

// ★isSystemInDarkTheme() 是 @Composable，不能在普通属性初始化里调
@Composable
private fun myColors(): ColorScheme =
    if (isSystemInDarkTheme()) DarkColors else LightColors
```

**不用把所有槽位都写出来**——没写的会从 Material 默认配色派生，只覆盖品牌相关的几个即可。

## (二) 动态取色

Android 12+ 可以从用户壁纸取色：

```kotlin
@Composable
fun MyAppTheme(
    darkTheme: Boolean = isSystemInDarkTheme(),
    // ★API 31+ 才有，低版本会返回 false
    dynamicColor: Boolean = true,
    content: @Composable () -> Unit
) {
    val context = LocalContext.current
    val colorScheme = when {
        dynamicColor && Build.VERSION.SDK_INT >= Build.VERSION_CODES.S -> {
            val colors = if (darkTheme) dynamicDarkColorScheme(context)
                         else dynamicLightColorScheme(context)
            // 品牌色优先：在品牌主色上叠加，不完全跟随壁纸
            colors.copy(primary = BrandPrimary, onPrimary = BrandOnPrimary)
        }
        darkTheme -> DarkColors
        else -> LightColors
    }

    MaterialTheme(
        colorScheme = colorScheme,
        typography = AppTypography,
        shapes = AppShapes,
        content = content
    )
}
```

⚠ **动态取色会破坏品牌识别**。主流做法是「动态底色 + 品牌主色」——`copy(primary = ...)` 把关键色槽换回品牌色，其余跟随壁纸。

# 三、Typography

Material 3 定义了 15 种排版样式，覆盖从 `displayLarge` 到 `labelSmall`：

| 用途 | 样式 |
| --- | --- |
| 页面主标题 | `headlineLarge` / `headlineMedium` |
| 区块标题 | `titleLarge` / `titleMedium` |
| 列表项主文案 | `titleMedium` |
| 正文 | `bodyLarge` / `bodyMedium` |
| 辅助说明 | `bodySmall` |
| 按钮与标签 | `labelLarge` / `labelMedium` |

```kotlin
Text(text = "文章标题", style = MaterialTheme.typography.headlineSmall)
Text(text = "正文内容", style = MaterialTheme.typography.bodyLarge)
Text(text = "2026-10-13", style = MaterialTheme.typography.labelSmall,
     color = MaterialTheme.colorScheme.onSurfaceVariant)
```

## (一) 自定义排版

```kotlin
val AppTypography = Typography(
    headlineMedium = TextStyle(
        fontFamily = FontFamily.Default,
        fontWeight = FontWeight.Bold,
        fontSize = 28.sp,
        lineHeight = 36.sp
    ),
    bodyLarge = TextStyle(
        fontFamily = FontFamily.Default,
        fontWeight = FontWeight.Normal,
        fontSize = 16.sp,
        lineHeight = 24.sp,
        letterSpacing = 0.5.sp
    )
)
```

**只覆盖要改的样式**，其余沿用 Material 默认。字体文件放 `res/font/`，注意用 `FontFamily` 而不是在 `TextStyle` 里写死 `fontFamily`。

## (二) 中文排版的注意点

```kotlin
Text(
    text = "这是一段中文",
    style = MaterialTheme.typography.bodyLarge,
    lineHeight = 26.sp            // ★中文行高要比英文大 1.3~1.5 倍
)
```

⚠ **Material 默认的行高是按西文调的**，中文直接用会显得拥挤。中文正文的 `lineHeight` 建议设为 `fontSize × 1.6`。

# 四、Shapes

Material 3 有五个形状级别：

```kotlin
val AppShapes = Shapes(
    extraSmall = RoundedCornerShape(4.dp),
    small = RoundedCornerShape(8.dp),
    medium = RoundedCornerShape(12.dp),
    large = RoundedCornerShape(16.dp),
    extraLarge = RoundedCornerShape(28.dp)
)

// 使用
Card(shape = MaterialTheme.shapes.medium) { ... }
```

⚠ **`clip()` 要与 `background()` 配对**。只设了圆角但没 clip，内容和背景会溢出到圆角之外：

```kotlin
// ✅ 顺序：先 clip 再 background
Modifier
    .clip(MaterialTheme.shapes.medium)
    .background(MaterialTheme.colorScheme.surfaceVariant)

// ❌ 背景画出来了，内容还是方的
Modifier
    .background(MaterialTheme.colorScheme.surfaceVariant)
    .clip(MaterialTheme.shapes.medium)     // ★clip 在 background 之后不生效
```

# 五、Elevation

Material 3 用阴影表达层级：

```kotlin
Card(tonalElevation = 2.dp) { ... }      // 同色系内更明显（用色调而非阴影）
Surface(shadowElevation = 8.dp) { ... } // 传统阴影
```

⚠ **elevated 元素在浅色与深色下的视觉效果不同**。深色模式下阴影几乎不可见，Material 3 因此推荐用 `tonalElevation`（在容器色上叠加色调）而不是单纯加大 `shadowElevation`。

# 六、Theme 的组织方式

## (一) 目录结构

标准做法是把主题拆成三个文件：

```text
ui/theme/
├── Color.kt        颜色定义
├── Type.kt         排版定义
├── Theme.kt        主题组合 + Theme 函数
└── Shape.kt        形状定义
```

```kotlin
// Color.kt
val BrandPrimary = Color(0xFF00658A)
val BrandOnPrimary = Color(0xFFFFFFFF)

// Type.kt
val AppTypography = Typography(/* ... */)

// Shape.kt
val AppShapes = Shapes(/* ... */)

// Theme.kt
@Composable
fun MyAppTheme(
    darkTheme: Boolean = isSystemInDarkTheme(),
    dynamicColor: Boolean = true,
    content: @Composable () -> Unit
) {
    MaterialTheme(
        colorScheme = /* ... */,
        typography = AppTypography,
        shapes = AppShapes,
        content = content
    )
}
```

## (二) 在应用入口套一次

```kotlin
class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        enableEdgeToEdge()
        super.onCreate(savedInstanceState)
        setContent {
            MyAppTheme {
                App()
            }
        }
    }
}
```

**主题只在这里套一次**，所有屏幕都从这里继承。组件内部**不要**再套 `MaterialTheme`。

## (三) @Preview 也要套

```kotlin
@Preview(showBackground = true)
@Composable
private fun CardPreview() {
    MyAppTheme { MyCard("标题", "正文") }
}
```

不套主题的预览看到的颜色与字号都不是真实值，预览就失去了参考意义。

## (四) 局部覆盖

某个子树要换配色，用嵌套的 `MaterialTheme`：

```kotlin
// 深色背景的底部弹窗
MaterialTheme(
    colorScheme = MaterialTheme.colorScheme.copy(
        surface = Color(0xFF1C1B1F),
        onSurface = Color(0xFFE6E1E5)
    )
) {
    ModalBottomSheet(onDismissRequest = { }) { ... }
}
```

⚠ **嵌套 MaterialTheme 会替换整个 colorScheme**，被省略的槽位会回落到默认值而不是父级。所以覆盖时要 `copy()` 而不是新建 `lightColorScheme()`。

# 七、Compose 颜色与资源颜色的分工

XML 时代颜色放在 `res/values/colors.xml`，Compose 里大多写在 Kotlin 常量。两者的取舍：

| | Kotlin 常量 | `res/values/colors.xml` |
| --- | --- | --- |
| 读取方式 | `Color(0xFF6750A4)` | `ColorResource(R.color.brand)` |
| 适用 | **主题色板** | 需要按配置切换的颜色 |
| 夜间模式 | 手动写两套 `ColorScheme` | `values-night/` 自动切换 |

**主题色板应该用 Kotlin 常量**，因为它要和深浅两套 `ColorScheme` 配对，拆到资源文件反而绕。以下这几种才适合放资源：

```xml
<!-- res/values/colors.xml -->
<resources>
    <color name="brand">#00658A</color>
    <color name="scrim">#99000000</color>
</resources>
<!-- res/values-night/colors.xml -->
<resources>
    <color name="brand">#7FD3FF</color>
</resources>
```

```kotlin
val brand = ColorResource(R.color.brand)
Box(Modifier.background(brand))
```

⚠ **别为了「统一」把所有颜色都塞进资源文件**。主题色板是 Compose 的核心机制，拆出去会丢掉类型安全和 IDE 补全。

# 八、主题常见问题

| 现象 | 原因 |
| --- | --- |
| 深色模式下文字看不清 | 用了 `primary` 当正文色，应换 `onSurface` |
| 圆角卡片四角漏出方角 | `background` 写在 `clip` 前面 |
| 动态取色后品牌色消失 | 需要 `copy(primary = 品牌色)` 覆盖 |
| 深色模式阴影看不见 | 改用 `tonalElevation` |
| 局部换色后其他控件颜色变了 | 嵌套 `MaterialTheme` 用了新建的 scheme，应 `copy()` |
| 预览里颜色和真机不一致 | `@Preview` 没套 `MyAppTheme` |

# 九、易错点

| 误区 | 事实 |
| --- | --- |
| Material 3 的色板就是一组固定颜色 | 它是语义槽位，值由主题决定 |
| `surface` 上的文字可以用 `primary` | 文本默认该用 `onSurface`，否则对比度不达标 |
| 动态取色能直接用 | 它会覆盖品牌色，需 `copy()` 保住关键槽位 |
| 写了 `shapes` 圆角就一定生效 | 还要 `clip()`，且必须在 `background()` 之前 |
| Material 默认行高适合中文 | 不适合，中文要放大到 1.5 倍以上 |
| 可以在任意组件里再套一层主题 | 主题只在应用入口套一次 |

# 十、参考资料

- [Material 3 设计系统](https://developer.android.com/develop/ui/compose/designsystems/material3) — 色彩、排版、形状的官方说明
- [Compose 中的 Material 3 主题](https://developer.android.com/develop/ui/compose/designsystems/material3#theming) — Theme 函数的组织方式
- [Material Theme Builder](https://material-foundation.github.io/material-theme-builder/) — 在线生成 ColorScheme 代码
- [Material Design 3 配色](https://m3.material.io/styles/color/roles/) — 每个色槽的官方定义与用途
