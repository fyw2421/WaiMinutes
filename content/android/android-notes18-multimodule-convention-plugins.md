---
title: "AndroidNotes18 : 多模块与约定插件"
weight: 19
description: "Android 项目多模块化最佳实践：按功能拆分、依赖方向管理、版本集中管理、约定插件编写、发布插件应用以及 Gradle 配置技巧，提升构建速度和团队协作效率。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么需要多模块化

单模块项目在团队扩大和功能迭代时会遇到以下问题：

- 编译时间过长：修改一点代码需要重新编译整个项目
- 依赖关系混乱：模块间依赖难以追踪和控制
- 代码复用困难：共享代码没有明确边界
- 团队协作冲突：多人同时修改同一文件导致合并冲突

# 二、模块拆分原则

## (一) 按功能拆分而非按技术类型

不要这样拆分（技术类型拆分）：
```text
app/
network/
database/
utils/
models/
```
而是这样拆分（功能拆分）：
```text
app/
core/
   network/
   database/
   util/
feature/
   article/
   user/
   setting/
```
功能拆分的好处：
- 每个功能模块都能独立编译和测试
- 依赖关系清晰：feature 模块依赖 core 模块，但 core 不依赖 feature
- 团队可以按功能划分责任，减少冲突

## (二) 明确依赖方向

依赖应该是单向的，避免循环依赖：
- feature 模块可以依赖 core 模块
- core 模块不应该依赖 feature 模块
- 同一层级的模块之间尽量不要相互依赖

在 Gradle 中通过 `implementation` 和 `api` 控制依赖可见性：
```kotlin
// feature/article/build.gradle.kts
dependencies {
    implementation(project(":core:network"))   // 只在内部使用，不对外暴露
    api(project(":core:util"))               // 需要对外暴露的依赖
}
```

# 三、版本集中管理

在根项目的 `build.gradle.kts` 或 `settings.gradle.kts` 中定义版本：
```kotlin
// root/build.gradle.kts
plugins {
    `kotlin-dsl`
    `gradle-version-plugin`
}

// 版本统一放在 gradle/libs.versions.toml（见第三节），不在这里硬编码

allprojects {
    configurations.all {
        resolutionStrategy.eachDependency { details ->
            if (details.requested.group == 'org.jetbrains.kotlin') {
                details.useVersion(kotlinVersion)
            }
            // ... 其他版本匹配规则
        }
    }
}
```

或者使用**版本目录**（推荐，Gradle 7+ 内置）：

```kotlin
// gradle/libs.versions.toml
[versions]
kotlin = "2.2.21"
agp = "9.2.1"
composeBom = "2026.06.01"
coreKtx = "1.19.1"
room = "2.8.5"

[libraries]
kotlin-stdlib = { group = "org.jetbrains.kotlin", name = "kotlin-stdlib", version.ref = "kotlin" }
androidx-core-ktx = { group = "androidx.core", name = "core-ktx", version.ref = "coreKtx" }
compose-bom = { group = "androidx.compose", name = "compose-bom", version.ref = "composeBom" }
compose-material3 = { group = "androidx.compose.material3", name = "material3" }
room-runtime = { group = "androidx.room", name = "room-runtime", version.ref = "room" }

[plugins]
android-application = { id = "com.android.application", version.ref = "agp" }
android-library = { id = "com.android.library", version.ref = "agp" }
kotlin-android = { id = "org.jetbrains.kotlin.android", version.ref = "kotlin" }
```

```kotlin
// 在模块的 build.gradle.kts 里用别名，IDE 有补全
dependencies {
    implementation(libs.androidx.core.ktx)
    implementation(platform(libs.compose.bom))     // BOM 用 platform 引入
    implementation(libs.compose.material3)
    implementation(libs.room.runtime)
}
```

⚠ **版本目录是 TOML 文件，不是 Kotlin 脚本**。Gradle 里也不存在 `versions` 这个对象——把版本写成 `"1.9.0" to "1.9.0"` 只是构造了一个 `Pair`，`versions.kotlin.get()` 根本解析不了。

⚠ **BOM 必须用 `platform(...)` 引入**，且只给一个 BOM 声明版本，其余 Compose 库不写版本号。

# 四、约定插件编写

约定插件是指将通用的 Gradle 配置封装成插件，在多个模块中复用。

## (一) 基本结构

创建 `build-logic` 目录存放约定插件：
```text
root/
  build-logic/
    conventions/
      src/
        main/
          kotlin/
            android-library-convention.kt
            android-application-convention.kt
            common-convention.kt
    plugins/
      src/
        main/
          kotlin/
            android-library-plugin.kt
            android-application-plugin.kt
```

## (二) 编写约定插件

```kotlin
// build-logic/conventions/src/main/kotlin/android-library-convention.kt
package com.waiminutes.android.conventions

import org.gradle.kotlin.dsl.*
import org.gradle.api.Plugin
import org.gradle.api.Project

class AndroidLibraryConvention : Plugin<Project> {
    override fun apply(project: Project) {
        // 应用通用插件
        project.plugins.apply("com.android.library")
        project.plugins.apply("org.jetbrains.kotlin.android")
        
        // 配置通用属性
        project.extensions.configure<com.android.build.api.dsl.LibraryExtension> {
            compileSdk = 36
            
            defaultConfig {
                minSdk = 24
                // ★library 模块不设 targetSdk / versionCode / versionName，
                //   这三项只属于 application 模块，AGP 8+ 会直接报错
                testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
            }
            
            buildTypes {
                release {
                    isMinifyEnabled = false
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
            
            // ★Kotlin 2.x 起用 compilerOptions，kotlinOptions 已废弃
            kotlin {
                compilerOptions {
                    jvmTarget.set(JvmTarget.JVM_17)
                }
            }
        }
        
        // 应用依赖约定
        project.dependencies {
            implementation("androidx.core:core-ktx:1.19.1")
            implementation("androidx.appcompat:appcompat:1.8.0")
            implementation(composeBom) {
                library("androidx.compose.ui:ui")
                library("androidx.compose.material3:material3")
                library("androidx.compose.ui:ui-tooling-preview")
            }
            debugImplementation("androidx.compose.ui:ui-tooling")
            debugImplementation("androidx.compose.ui:ui-test-manifest")
        }
    }
}
```

# 五、发布插件应用

在根项目的 `settings.gradle.kts` 中应用约定插件：
```kotlin
// root/settings.gradle.kts
pluginManagement {
    repositories {
        gradlePluginPortal()
        google()
        mavenCentral()
    }
    // 包含本地构建逻辑
    includeBuild("build-logic")
}

dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPO)
    repositories {
        google()
        mavenCentral()
    }
}

rootProject.name = "WaiMinutesAndroid"
include(":app")
include(":core:network")
include(":core:database")
include(":feature:article")
```

在各个模块中应用插件：
```kotlin
// feature/article/build.gradle.kts
plugins {
    alias(libs.plugins.android.library)   // 引用版本目录中的插件别名
}

// 或者直接应用
plugins {
    id("com.waiminutes.android.library")
}
```

# 六、版本目录与类型安全访问

使用版本目录实现类型安全的依赖声明（沿用第三节的 TOML）：
```kotlin
// gradle/libs.versions.toml
[libraries]
kotlin-stdlib = { group = "org.jetbrains.kotlin", name = "kotlin-stdlib", version.ref = "kotlin" }
androidx-core-ktx = { group = "androidx.core", name = "core-ktx", version.ref = "coreKtx" }
compose-bom = { group = "androidx.compose", name = "compose-bom", version.ref = "composeBom" }
compose-ui = { group = "androidx.compose.ui", name = "ui" }          // ★BOM 内的库不写版本
```

```kotlin
// 模块的 build.gradle.kts —— 别名由 Gradle 生成，IDE 可补全、可跳转
dependencies {
    implementation(libs.kotlin.stdlib)
    implementation(libs.androidx.core.ktx)

    // ★ BOM 用 platform(...) 引入，其余 Compose 库不写版本号
    implementation(platform(libs.compose.bom))
    implementation(libs.compose.ui)
}
```

**别名是怎么生成的**：TOML 里的 `androidx-core-ktx` 会变成 `libs.androidx.core.ktx`，`-` 变 `.`、嵌套用 `.` 连接。这样打错坐标是**编译错误**而不是运行时的解析失败。

⚠ **不要用 `composeBom.map { it.library(...) }` 这类写法**。`library(...)` 不是依赖对象上的方法，BOM 也不是一个能当 `Map` 遍历的东西——它只能用 `platform()` 引入一次。

# 七、构建性能优化

## (一) 启用缓存

```kotlin
// root/gradle.properties
org.gradle.parallel=true
org.gradle.caching=true
org.gradle.jvmargs=-Xmx4g -XX:MaxMetaspaceSize=1g
kotlin.daemon.jvmargs=-Xmx2g
```

⚠ **不要开 `org.gradle.configureondemand`**。它与 included build（本节的 `build-logic`）和 AGP 常冲突，实际收益接近零。

⚠ **Kotlin daemon 的堆用独立键 `kotlin.daemon.jvmargs`**，不要塞进 `org.gradle.jvmargs` 的 `-D` 里——两套进程会争抢内存，反而更容易 OOM。

## (二) 配置变更避免

在模块间共享资源时，避免触发不必要的重构建：
```kotlin
// 在共享模块中
android {
    // 避免资源合并导致的重构建
    resourcePrefix = "shared_"
    
    // 使用严格的资源限制
    // ★AaptOptions 在 AGP 8/9 已废弃，ignoreAssetsPattern 迁到了这里。
    //   而且 noCompress 是只读集合属性，不能用 = 赋值，要用 +=
    androidResources {
        noCompress += listOf("png", "jpg", "jpeg", "gif", "webp")
        ignoreAssetsPatterns += "!.svn:!.git:!.ds_store"
    }
}
```

# 八、常见问题及解决方案

## (一) 循环依赖检测

项目级的循环依赖在**配置或解析阶段就直接失败**，构建根本走不到警告那一步，所以没有任何开关能「检测出循环依赖」。真要排查依赖关系，用这两个命令看实际结构：

```bash
./gradlew projects                # 模块树
./gradlew :app:dependencies       # 单个模块的依赖解析结果
```

`org.gradle.warning.mode` 只有一个实际用途：控制**构建警告**的显示级别（`none` / `warn` / `fail` / `all`），跟循环依赖无关。

## (二) 版本冲突解决

当不同模块依赖同一库的不同版本时：
```kotlin
// 在根项目中统一处理
allprojects {
    configurations.all {
        resolutionStrategy.eachDependency { details ->
            if (details.requested.name == "guava") {
                details.useVersion("32.1.2-jre")
            }
        }
    }
}
```

## (三) IDE 同步问题

多模块项目在 IDE 中可能出现同步延迟：
- 增加 Gradle 守护进程内存
- 禁用不必要的插件
- 使用离线模式减少网络请求
- 定义清晰的模块边界减少同步范围

# 九、最佳实践清单

✅ 按功能而非技术类型拆分模块  
✅ 保持依赖方向单向，避免循环依赖  
✅ 使用版本目录或 ext 块集中管理版本  
✅ 编写约定插件封装通用 Gradle 配置  
✅ 在根项目应用插件，保持模块配置简洁  
✅ 启用 Gradle 缓存和并行编译  
✅ 定期清理未使用的依赖和插件  
✅ 设置清晰的模块所有权和维护责任  

# 十、参考资料

- [Android 架构指南 - 多模块化](https://developer.android.com/topic/modularization)
- [Gradle 文档 - 组织构建逻辑](https://docs.gradle.org/current/userguide/organizing_build_logic.html)
- [Gradle 版本目录](https://docs.gradle.org/current/userguide/platforms.html#sub:version-catalogs)
- [Android Gradle 插件 DSL 参考](https://developer.android.com/reference/tools/gradle-api)
- [Keep 约定插件示例](https://github.com/nowinandroid/nowinandroid)
