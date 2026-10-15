---
title: "AndroidNotes08 : 图片与资源"
weight: 9
description: "Compose 的图片与资源处理：painterResource 与资源目录、网络图片 Coil 3 的用法与缓存、ContentScale 的选择、矢量图标与 ImageVector 的自绘，以及深色模式与多语言资源目录的组织。"
featureimage: "covers/android-ui.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、三种图片来源

Compose 里显示一张图有三条路，选哪条取决于图的来源：

```kotlin
// 1) 本地资源（PNG / WebP / 矢量图）
Image(
    painter = painterResource(R.drawable.ic_notification),
    contentDescription = "通知"
)

// 2) 网络地址（需要 Coil 之类的加载库）
AsyncImage(
    model = "https://example.com/avatar.jpg",
    contentDescription = "用户头像"
)

// 3) Compose 自绘（Canvas 画出来的）
Canvas(Modifier.size(48.dp)) {
    drawCircle(color = Color.Red, radius = size.minDimension / 2)
}
```

## (一) 本地资源

```kotlin
// ★painterResource 而不是 R.drawable.xxx
Image(
    painter = painterResource(R.drawable.logo),
    contentDescription = "应用 Logo",
    modifier = Modifier.size(64.dp)
)
```

⚠ **Compose 里必须用 `painterResource()`**，直接传 `R.drawable.logo`（Drawable）编译不过——Compose 需要的是 `Painter`。

`painterResource` 能同时处理位图和矢量图，**不需要知道资源类型**，这也是官方推荐它的原因。

# 二、资源目录

## (一) drawable 的分类

| 目录 | 放什么 | 用法 |
| --- | --- | --- |
| `res/drawable/` | 位图、XML drawable | `painterResource(R.drawable.x)` |
| `res/drawable-night/` | 夜间模式的不同外观 | 自动切换 |
| `res/mipmap-anydpi-v26/` | 自适应图标 | 启动图标专用 |
| `res/font/` | 字体文件 | `FontFamily` |

**位图要按密度分目录**，否则在小屏设备上会模糊：

```text
res/drawable/
├── icon.png           (mdpi，1x)
├── drawable-hdpi/     1.5x
├── drawable-xhdpi/    2x
├── drawable-xxhdpi/   3x
├── drawable-xxxhdpi/  4x
```

⚠ **只放一张不带密度限定的图，它会被当作 mdpi 缩放到所有屏幕**——高密度屏上发虚，低密度屏上白白占内存。要按密度提供多份。

## (二) 限定符目录

资源目录支持按条件覆盖，同名资源放不同目录，系统按优先级挑选：

| 目录 | 条件 |
| --- | --- |
| `values-night/` | 深色模式 |
| `values-en/` | 英文 |
| `values-zh-rCN/` | 简体中文 |
| `values-land/` | 横屏 |
| `values-sw600dp/` | 最小宽度 600dp（平板） |
| `values-sw720dp/` | 最小宽度 720dp |

```kotlin
// 主题色随语言变化
Text(
    text = stringResource(
        id = R.string.welcome,
        stringResource(R.string.app_name)
    )
)

// 深浅两套颜色
val bg = colorResource(if (isDark) R.color.bg_dark else R.color.bg_light)
```

⚠ **限定符不匹配时不会报错，只会静默回落到默认目录**。某个语言下显示空白，八成是 `values-en/` 里漏了某个 key——用 `tools:ignore="MissingTranslation"` 配合 lint 可以查出来。

# 三、网络图片

## (一) 引入 Coil

```kotlin
// app/build.gradle.kts
dependencies {
    implementation("io.coil-kt.coil3:coil-compose:3.6.2")
    implementation("io.coil-kt.coil3:coil-network-okhttp:3.6.2")   // 用 OkHttp 加载
}
```

Coil 3 的包名是 **`coil3`**（Coil 2 是 `coil`），导入路径：

```kotlin
import coil3.compose.AsyncImage
import coil3.request.ImageRequest
import coil3.request.crossfade
```

## (二) 基本用法

```kotlin
AsyncImage(
    model = user.avatarUrl,
    contentDescription = "${user.name} 的头像",
    modifier = Modifier
        .size(48.dp)
        .clip(CircleShape),
    contentScale = ContentScale.Crop,
    placeholder = painterResource(R.drawable.placeholder),   // 加载中
    error = painterResource(R.drawable.error),             // 加载失败
    fallback = painterResource(R.drawable.default_avatar)  // model 为 null
)
```

⚠ **`contentDescription` 必须写**。装饰性图片（纯背景、分隔线）才写 `null`，不要用空字符串。

## (三) 自定义请求

```kotlin
val request = ImageRequest.Builder(LocalContext.current)
    .data(user.avatarUrl)
    .crossfade(true)                       // 淡入
    .memoryCacheKey(user.avatarUrl)        // 显式缓存键
    .placeholderMemoryCacheKey("avatar_placeholder")
    .build()

AsyncImage(model = request, contentDescription = null)
```

## (四) 全局配置

```kotlin
class App : Application(), ImageLoaderFactory {
    override fun newImageLoader(): ImageLoader =
        ImageLoader.Builder(this)
            .memoryCache {
                MemoryCache.Builder(this)
                    .maxSizePercent(0.25)      // ★占可用内存的 25%
                    .build()
            }
            .diskCache {
                DiskCache.Builder()
                    .directory(cacheDir.resolve("image_cache"))
                    .maxSizeBytes(100L * 1024 * 1024)    // 100MB
                    .build()
            }
            .crossfade(true)
            .build()
}
```

```kotlin
// AndroidManifest.xml 里注册
<application
    android:name=".App"
    android:hardwareAccelerated="true">
```

⚠ **不设缓存上限会让图片缓存无限增长**，最终吃掉大量存储。内存缓存按百分比、磁盘缓存按绝对字节数，都必须设上限。

# 四、ContentScale 的选择

`ContentScale` 决定图片如何填满给定空间，选错会导致变形或留白：

| 值 | 效果 | 用在哪 |
| --- | --- | --- |
| `Fit` | 完整显示，可能留白（**默认**） | Logo、插图 |
| `Crop` | 填满，裁掉多余部分 | 头像、封面图 |
| `FillBounds` | **拉伸变形** | 几乎不用 |
| `Inside` | 完整显示，不放大 | 小图标 |
| `None` | 原尺寸，不缩放 | 调试 |

```kotlin
// 头像：正方形 + 裁剪
AsyncImage(
    model = url,
    contentDescription = null,
    modifier = Modifier.size(56.dp).clip(CircleShape),
    contentScale = ContentScale.Crop
)

// 封面：固定宽高比
AsyncImage(
    model = url,
    contentDescription = null,
    modifier = Modifier
        .fillMaxWidth()
        .aspectRatio(16f / 9f)
        .clip(MaterialTheme.shapes.medium),
    contentScale = ContentScale.Crop
)
```

⚠ **头像必须给容器加尺寸**（`size()`、`aspectRatio()`）。`AsyncImage` 会按约束决定加载多大尺寸，没有约束时可能加载原图（几 MB）只显示几十 dp，白费流量和内存。

# 五、矢量图与 ImageVector

## (一) 使用内置图标

```kotlin
Icon(imageVector = Icons.Default.Add, contentDescription = "新建")
Icon(imageVector = Icons.AutoMirrored.Filled.ArrowBack, contentDescription = "返回")
```

⚠ **RTL 布局要用 `Icons.AutoMirrored.*`**。`Icons.Default.ArrowBack` 在阿拉伯语等从右到左的语言里方向是错的，`AutoMirrored` 版本会自动翻转。

## (二) 自定义 ImageVector

```kotlin
val HeartIcon = ImageVector.Builder(
    name = "Heart",
    defaultWidth = 24.dp,
    defaultHeight = 24.dp,
    viewportWidth = 24f,
    viewportHeight = 24f
).apply {
    // path( fill = SolidColor(Color.Red) ) { moveTo(12f, 21f); ... }
    path(fill = SolidColor(Color.Red)) {
        moveTo(12f, 21f)
        lineTo(3f, 12f)
        lineTo(10.5f, 4.5f)
        lineTo(12f, 6f)
        lineTo(13.5f, 4.5f)
        lineTo(21f, 12f)
        close()
    }
}.build()
```

```kotlin
Icon(imageVector = HeartIcon, contentDescription = "收藏",
     tint = if (isLiked) Color.Red else Color.Gray)
```

⚠ **`tint` 会覆盖 path 里写死的颜色**。想在运行时换色就别把颜色写死在 path 里。

## (三) XML 矢量图转 Compose

```kotlin
Image(
    painter = painterResource(R.drawable.ic_search),
    contentDescription = "搜索",
    tint = MaterialTheme.colorScheme.onSurfaceVariant   // ★XML 图标靠 tint 染色
)
```

XML 矢量图用 `android:fillColor` 定义颜色，Compose 里用 `tint` 覆盖它——两者优先级是 `tint` 高。

# 六、性能提示

```kotlin
// 1) 给位图加上采样尺寸，避免解码超大图
AsyncImage(
    model = ImageRequest.Builder(context)
        .data(url)
        .size(Size(widthPx, heightPx))       // ★按实际显示尺寸解码
        .build(),
    contentDescription = null
)

// 2) 大量静态图片用 drawBehind，不必走 Image
Box(
    Modifier
        .drawBehind { drawImage(image, dstSize = size.toIntSize()) }
        .size(64.dp)
)
```

⚠ **位图解码占用内存是宽×高×4 字节**。一张 4000×3000 的图解码后约 48MB——即使只显示 100dp。必须用 `ImageRequest.size()` 告诉 Coil 按显示尺寸解码。

# 七、易错点

| 误区 | 事实 |
| --- | --- |
| Compose 里可以直接传 `R.drawable.x` | 必须包一层 `painterResource()` |
| 图标目录里放一张就够 | 位图要按密度分目录，否则高密度屏发虚、低密度屏浪费内存 |
| 网络图只写 `contentDescription` 就行 | 还要给容器尺寸，否则会加载原图 |
| `Icons.Default.ArrowBack` 所有语言都对 | 从右到左的语言要用 `Icons.AutoMirrored.*` |
| 图片缓存不用设上限 | 必须设，否则会无限增长 |
| 资源限定符匹配不上会报错 | 不会，会静默回落，容易漏翻译 |
| `FillBounds` 是常用的缩放方式 | 它会拉伸变形，几乎不用 |
| `MaterialTheme.colorScheme` 能在普通函数里取 | 只能在 `@Composable` 中调用 |

# 八、参考资料

- [Compose 图片加载](https://developer.android.com/develop/ui/compose/graphics/images/loading) — `painterResource` 与网络图片
- [Compose 中的资源](https://developer.android.com/develop/ui/compose/resources) — 资源目录与限定符
- [Coil 官方文档](https://coil-kt.github.io/coil/) — Coil 3 的完整用法与配置
- [无障碍与图片](https://developer.android.com/develop/ui/compose/accessibility) — `contentDescription` 的规范
