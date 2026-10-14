---
title: "AndroidNotes01 : 起步"
weight: 1
description: "Android 开发环境与工程结构：Compose-first 时代的技术选型、Android Studio 与 SDK 安装、Gradle 工程与模块划分、Manifest 与资源体系、第一个可运行的应用，以及构建产物与常见构建错误排查。"
featureimage: "covers/android-basics.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、为什么是 Compose

Android 的 UI 技术栈在 2026 年发生了方向性变化。Google 已正式宣布进入 [Compose-first](https://developer.android.com/develop/ui/compose/first) 阶段：

> "Compose has matured into the standard for Android UI development... From today, we'll provide all APIs, libraries, tools and guidance in Compose."

同时，**View 工具链转入维护模式**。官方原文是："We have no plans to deprecate or remove View components and will continue to support them with critical bug fixes, but they will **receive no new features**."

被点名的维护模式库包括 `CustomView`、`Fragment`、`RecyclerView`、`ViewPager`、`ConstraintLayout`、`CoordinatorLayout`、`DataBinding`、`DynamicAnimation` 等一长串。Android Studio 的 Layout Editor 与 Navigation Editor 同样进入维护模式，新 UI 工具只为 Compose 开发。

这不等于 View 不能用了——存量代码里全是 View，`findViewById` 依然能跑。但**新项目不该再从 View 学起**，这是本笔记选择 Compose 的唯一理由。

| 维度 | View + XML | Jetpack Compose |
| --- | --- | --- |
| UI 定义 | 独立 XML 文件，运行时 inflate | Kotlin 函数，直接执行 |
| 更新方式 | `findViewById` + 命令式赋值 | 改状态 → 自动重组 |
| 控件依赖传递 | 靠 `findViewById` 手动查找 | **参数传递，编译期可查** |
| 预览 | Layout Editor（维护模式） | `@Preview` + Live Edit |
| 调试 | 层级视图 | Compose Layout Inspector |

# 二、版本要求

| 组件 | 版本 | 说明 |
| --- | --- | --- |
| Android Studio | Meerkat 或更新 | 自带 SDK 管理器与 JBR |
| Gradle | 9.x | 由 wrapper 下载，与本机无关 |
| Android Gradle Plugin | 9.x | 简称 AGP，决定构建行为 |
| Kotlin | 2.2+ | 2.0 起 K1/K2 编译器默认切换 |
| Compose BOM | 2026.06+ | 统一对齐所有 Compose 库版本 |
| JDK | 17+ | 构建用的 JDK，**与 minSdk 无关** |
| compileSdk / targetSdk | 36（Android 16） | |
| minSdk | 24（Android 7.0） | 按业务实际需求定 |

## (一) JDK 与 minSdk 是两回事

这是新手最常混淆的一点：

- **`minSdk 24`** 指的是「这个 App 能装到 Android 7.0 的手机上」，是一个**兼容性声明**
- **JDK 17** 指的是「构建工具链跑在哪个 Java 上」，是**构建环境要求**

两者没有推导关系。你的 `minSdk` 可以是 21，但构建机必须装 JDK 17+ 才能跑 AGP 9.x。

**`targetSdk 35` 与 `36` 各有一个必须处理的变化**：

1. **targetSdk 35（Android 15）起 edge-to-edge 强制**：应用默认铺满全屏，状态栏/导航栏不再由系统预留空间，必须自己处理 insets
2. **targetSdk 36（Android 16）起彻底无法退出**：opt-out 属性 `windowOptOutEdgeToEdgeEnforcement` 被弃用并禁用

# 三、搭建环境

## (一) 安装

1. 装 [Android Studio](https://developer.android.com/studio)
2. 首次启动选 **Standard** 安装，它会自动拉取最新 SDK 平台与构建工具
3. 单独装一个 JDK 17+（Temurin / Oracle / Microsoft Build of OpenJDK 都行）

## (二) 验证

```bash
java -version     # 应为 17 或更高
```

Android Studio 自带 JBR（JetBrains Runtime），在 `Settings → Build, Execution, Deployment → Build Tools → Gradle → Gradle JDK` 里可以切换用哪个 JDK。

⚠ **这一项和命令行用的 JDK 可以不同**——Gradle 默认继承 IDE 进程的环境变量。排查「IDE 能编译、命令行失败」时，先比对两处的 JDK 版本，这是最常见的原因。

## (三) 常用 adb 命令

```bash
adb devices                          # 列出连接的设备
adb install app-debug.apk            # 安装
adb shell am start -n com.example.myapp/.MainActivity   # 启动指定 Activity
adb logcat                           # 看日志
adb logcat --pid=$(adb shell pidof com.example.myapp)   # ★只看本 App 的日志
adb shell dumpsys package com.example.myapp | grep -A5 "Activity Resolver"  # 查导出组件
adb uninstall com.example.myapp
```

`adb logcat` 不加过滤会刷屏。`--pid` 是最实用的过滤方式，排查崩溃时几乎必用。

# 四、工程结构

## (一) 标准布局

```text
MyApp/
├── app/                          主模块
│   ├── src/
│   │   ├── main/
│   │   │   ├── kotlin/           源码目录（不是 java/）
│   │   │   │   └── com/example/myapp/
│   │   │   │       ├── MainActivity.kt
│   │   │   │       └── ui/theme/Theme.kt
│   │   │   ├── res/
│   │   │   │   ├── drawable/     图片、XML drawable
│   │   │   │   ├── mipmap/       启动图标（各密度）
│   │   │   │   ├── values/       strings / colors / themes
│   │   │   │   └── xml/          备份规则等
│   │   │   └── AndroidManifest.xml
│   │   ├── debug/                debug 专属（可调试的 release 构建）
│   │   └── release/              release 专属（签名配置）
│   ├── build.gradle.kts
│   └── proguard-rules.pro
├── gradle/
│   ├── libs.versions.toml        版本目录
│   └── wrapper/
│       ├── gradle-wrapper.jar
│       └── gradle-wrapper.properties
├── build.gradle.kts              根构建脚本
├── settings.gradle.kts           模块声明
└── gradle.properties             JVM 内存等
```

**源码目录是 `kotlin/` 而非 `java/`**——Kotlin 项目用前者更清晰。若有 Java 与 Kotlin 混编，两个目录都要有。

## (二) settings.gradle.kts

```kotlin
pluginManagement {
    repositories {
        google()
        mavenCentral()
        gradlePluginPortal()
    }
}

dependencyResolutionManagement {
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "MyApp"
include(":app")
```

`pluginManagement` 的仓库顺序会影响插件从哪解析。`google()` 必须在前，因为 AGP 只发布在那里。

## (三) 版本目录

`gradle/libs.versions.toml` 把版本号从构建脚本里抽出来集中管理，这是 2026 年的标准做法：

```toml
[versions]
agp = "9.2.1"
kotlin = "2.2.21"
composeBom = "2026.06.01"
coreKtx = "1.19.1"

[libraries]
androidx-core-ktx = { group = "androidx.core", name = "core-ktx", version.ref = "coreKtx" }
compose-bom = { group = "androidx.compose", name = "compose-bom", version.ref = "composeBom" }
compose-material3 = { group = "androidx.compose.material3", name = "material3" }

[plugins]
android-application = { id = "com.android.application", version.ref = "agp" }
kotlin-android = { id = "org.jetbrains.kotlin.android", version.ref = "kotlin" }
kotlin-compose = { id = "org.jetbrains.kotlin.plugin.compose", version.ref = "kotlin" }
```

## (四) 根 build.gradle.kts

```kotlin
plugins {
    alias(libs.plugins.android.application) apply false   // ★apply false
    alias(libs.plugins.kotlin.android) apply false
}
```

`apply false` 的意思是「在这里声明版本，但不在根项目应用」。少了它，插件会在根项目被应用并报错——这是新手最常见的构建错误之一。

## (五) app/build.gradle.kts

```kotlin
plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
    alias(libs.plugins.kotlin.compose)      // Compose 需要单独的插件
}

android {
    namespace = "com.example.myapp"
    compileSdk = 36

    defaultConfig {
        applicationId = "com.example.myapp"
        minSdk = 24
        targetSdk = 36
        versionCode = 1                       // 整数，每次发布递增
        versionName = "1.0.0"
    }

    buildFeatures { compose = true }         // 开启 Compose

    buildTypes {
        release {
            isMinifyEnabled = true           // 开启 R8
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
}

// ★kotlin {} 是顶层块，不能写在 android {} 内部
kotlin {
    compilerOptions {
        jvmTarget.set(org.jetbrains.kotlin.gradle.dsl.JvmTarget.JVM_17)
    }
}

dependencies {
    implementation(libs.androidx.core.ktx)
    implementation(platform(libs.compose.bom))   // BOM 统一管理 Compose 版本
    implementation(libs.compose.material3)
}
```

三个易错点：

- **`compose = true` 必须开**，否则 `@Composable` 注解不生效
- **`sourceCompatibility`、`targetCompatibility`、`jvmTarget` 三处要一致**，不一致会报 "Inconsistent JVM-target compatibility"
- **BOM 用 `platform(...)` 引入**，不要给每个 Compose 库单独写版本号——BOM 会统一对齐，避免版本冲突

## (六) 版本一致性检查

```bash
./gradlew :app:dependencies --configuration debugRuntimeClasspath
```

依赖冲突时用 `./gradlew buildEnvironment` 看实际生效的 AGP/Kotlin 版本。AGP 与 Kotlin 版本不匹配会在配置阶段就报错，报错信息里会写明两者要求。

# 五、Manifest 与资源

## (一) AndroidManifest.xml

```xml
<?xml version="1.0" encoding="utf-8"?>
<manifest xmlns:android="http://schemas.android.com/apk/res/android">

    <application
        android:label="@string/app_name"
        android:theme="@style/Theme.MyApp"
        android:allowBackup="true">
        <activity
            android:name=".MainActivity"
            android:exported="true">      <!-- targetSdk 31+ 必须显式声明 -->
            <intent-filter>
                <action android:name="android.intent.action.MAIN" />
                <category android:name="android.intent.category.LAUNCHER" />
            </intent-filter>
        </activity>
    </application>

    <uses-permission android:name="android.permission.INTERNET" />
</manifest>
```

**`android:exported` 是 `targetSdk 31` 起强制的**：含 `intent-filter` 的组件必须显式写 `true` 或 `false`，否则安装时直接报错。这是从旧项目迁移时最常见的构建障碍。

## (二) 主题资源

Manifest 里的 `@style/Theme.MyApp` 必须真实存在，否则 AAPT 报 "resource style/Theme.MyApp not found"：

```xml
<!-- res/values/themes.xml -->
<resources>
    <style name="Theme.MyApp" parent="android:Theme.Material.Light.NoActionBar" />
</resources>
```

纯 Compose 项目**不需要 AppCompat 主题**。这里用平台自带的 `android:Theme.Material.Light.NoActionBar`，比 `Theme.AppCompat.*` 更轻——后者会带入一整套兼容层，纯 Compose 用不上。

## (三) 字符串与资源

```xml
<!-- res/values/strings.xml -->
<resources>
    <string name="app_name">我的应用</string>
    <string name="greeting">你好，%1$s！</string>   <!-- 带占位符 -->
</resources>
```

```kotlin
// ✅
Text(text = stringResource(R.string.greeting, "Alice"))
Text(text = getString(R.string.greeting, "Alice"))

// ❌ 硬编码
Text(text = "你好，Alice！")
```

**所有面向用户的文案都放 `strings.xml`**，不要硬编码在代码里——这是本地化的前提，也是代码审查最容易查出来的问题。带参数的用 `%1$s` 占位，位置参数要显式写序号。

```kotlin
// 复数与带选项的字符串要用资源目录变体，不能拼接
val options = resources.getStringArray(R.array.city_options)
Text(text = options.firstOrNull() ?: getString(R.string.no_city))
```

命名规范：`drawable/` 放图标与位图，`mipmap/` 专放启动图标（各密度一套，`-hdpi`/`-xhdpi`/`-xxhdpi`/`-xxxhdpi`），`values/` 放字符串/颜色/尺寸，`values-night/` 放夜间模式的对应值。

# 六、第一个应用

下面三个文件合起来是一个能直接跑起来的最小工程。

```kotlin
// MainActivity.kt
package com.example.myapp

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.activity.enableEdgeToEdge
import androidx.compose.foundation.layout.Arrangement
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Button
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp

class MainActivity : ComponentActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        enableEdgeToEdge()                    // targetSdk 35+ 已是默认行为，这行让低版本也一致
        super.onCreate(savedInstanceState)

        setContent {                         // 替代 setContentView
            MyAppTheme {
                Surface(modifier = Modifier.fillMaxSize()) {
                    CounterScreen()
                }
            }
        }
    }
}

@Composable
fun CounterScreen(modifier: Modifier = Modifier) {
    var count by remember { mutableIntStateOf(0) }

    Column(
        modifier = modifier
            .fillMaxSize()
            .padding(24.dp),
        verticalArrangement = Arrangement.spacedBy(16.dp, Alignment.CenterVertically),
        horizontalAlignment = Alignment.CenterHorizontally
    ) {
        Text(text = stringResource(R.string.greeting, "Alice"),
             style = MaterialTheme.typography.headlineMedium)
        Text(text = "点击了 $count 次", style = MaterialTheme.typography.bodyLarge)
        Button(onClick = { count++ }) {
            Text(text = "加一")
        }
    }
}

@Preview(showBackground = true)
@Composable
private fun CounterScreenPreview() {
    MyAppTheme {           // ★必须用主题包起来，否则颜色不对
        CounterScreen()
    }
}
```

```kotlin
// ui/theme/Theme.kt
package com.example.myapp.ui.theme

import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.lightColorScheme
import androidx.compose.runtime.Composable

private val LightColors = lightColorScheme()

@Composable
fun MyAppTheme(content: @Composable () -> Unit) {
    MaterialTheme(
        colorScheme = LightColors,
        content = content
    )
}
```

四个关键点：

1. **`setContent { }` 取代 `setContentView()`**——Compose 没有"布局文件"
2. **`enableEdgeToEdge()` 要在 `super.onCreate` 之前调用**，这是 `targetSdk 36` 的硬性要求
3. **`Surface` 是 Material 3 的根容器**，提供背景色与内容颜色配对
4. **`@Preview` 必须用主题包起来**，否则渲染出来的颜色和真实 App 不一致，预览就失去意义

`ComponentActivity` 来自 `androidx.activity`，比老的 `AppCompatActivity` 更轻——它不带 AppCompat 的主题兼容层，纯 Compose 项目不需要。

# 七、构建与产物

```bash
./gradlew assembleDebug      # 产物在 app/build/outputs/apk/debug/
./gradlew assembleRelease    # 需要签名配置
./gradlew installDebug       # 构建并装到连接的设备
./gradlew clean              # 清 app/build/
```

产物路径：

| 任务 | 输出 |
| --- | --- |
| `assembleDebug` | `app/build/outputs/apk/debug/app-debug.apk` |
| `bundleRelease` | `app/build/outputs/bundle/release/app-release.aab` |
| `assembleRelease` | `app/build/outputs/apk/release/` |

**上架 Google Play 必须传 `.aab`**（App Bundle），`.apk` 只是调试与侧载用的。AGP 8+ 可以让一个模块不出 `.apk`，只出 `.aab`。

签名配置不要硬编码在 `build.gradle.kts`（会泄密进 git），从 `local.properties` 或环境变量读：

```kotlin
android {
    signingConfigs {
        create("release") {
            storeFile = file(System.getenv("KEYSTORE_PATH") ?: "$rootDir/keystore.jks")
            storePassword = System.getenv("KEYSTORE_PASSWORD")
            keyAlias = System.getenv("KEY_ALIAS")
            keyPassword = System.getenv("KEY_PASSWORD")
        }
    }
    buildTypes {
        release { signingConfig = signingConfigs.getByName("release") }
    }
}
```

# 八、常见构建错误

| 报错 | 原因 | 处理 |
| --- | --- | --- |
| `resource style/Theme.MyApp not found` | Manifest 引用的主题没定义 | 补 `values/themes.xml` |
| `Manifest merger failed` | 库与 App 的 Manifest 冲突 | 看冲突报告，多数是 `android:exported` 打架 |
| `Installed SDK version 36 is missing` | 没装对应平台 | SDK Manager 里装 |
| `Inconsistent JVM-target compatibility` | 三处 target 不一致 | 见上文「(五)」 |
| `Plugin already on the classpath` | 根脚本漏了 `apply false` | 补上 |
| `Could not resolve com.android.tools.build:gradle` | 仓库顺序或网络问题 | 检查 `pluginManagement`，配置代理 |
| 安装时报 `INSTALL_FAILED_UPDATE_INCOMPATIBLE` | 签名与已装版本不一致 | 先卸载 |

# 九、易错点

| 误区 | 事实 |
| --- | --- |
| minSdk 24 就该用 JDK 8 | 无关。JDK 是构建环境要求，要 17+ |
| Manifest 里不写 `exported` 更简洁 | targetSdk 31+ 强制要求，否则安装失败 |
| IDE 能编译就说明环境没问题 | Gradle 用的是 IDE 进程继承的 JDK，可能与命令行不同 |
| 版本号写在各自模块里就行 | 用版本目录统一管理，改一处即可 |
| 硬编码字符串最快 | 硬编码会挡掉本地化，也让 lint 检查失效 |
| release 包可以用 debug 签名 | 不能，上架必须正式签名 |

# 十、参考资料

- [Android 官方文档](https://developer.android.com/) — 开发者文档首页
- [Compose-first 阶段说明](https://developer.android.com/develop/ui/compose/first) — Compose-first 的完整说明与维护模式清单
- [Jetpack Compose 指南](https://developer.android.com/develop/ui/compose) — Compose 官方文档入口
- [Kotlin 官方文档](https://kotlinlang.org/docs/home.html) — 语言参考与标准库
- [Gradle 版本目录](https://docs.gradle.org/current/userguide/platforms.html) — 版本目录与约定插件的官方说明
- [应用架构建议](https://developer.android.com/topic/architecture/recommendations) — 官方推荐的分层架构与 UDF 原则
- [Android 开发者速查表](https://developer.android.com/studio/intro) — IDE 与构建流程速查
