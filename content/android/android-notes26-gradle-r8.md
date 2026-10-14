---
title: "AndroidNotes26 : Gradle 构建与 R8"
weight: 26
description: "Android Gradle 构建系统与 R8 优化器的深入使用：构建流程分析、依赖管理、版本目录、构建变种、签名配置、R8 压缩与混淆、构建性能优化、缓存策略以及自定义插件开发。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么需要理解 Gradle 和 R8

Gradle 是 Android 构建系统的核心，R8 是代码 shrinking、优化和混淆工具，它们共同决定了：

- **构建速度**：增量编译、缓存和并行执行影响开发效率
- **应用大小**：R8 的压缩和移除未用代码直接影响下载和安装大小
- **运行时性能**：代码优化可以提升应用执行效率
- **安全性**：代码混淆增加了逆向工程的难度
- **发布流程**：签名、多渠道打包和发布准备工作
- **自定义需求**：通过插件和构建变种满足特定业务需求

理解这些工具能够显著提升开发效率和应用质量。

# 二、Gradle 构建基础

## (一) Gradle 构建生命周期

Gradle 构建分为三个阶段：

1. **初始化阶段（Initialization）**
   - 确定参与构建的项目
   - 创建 `Settings` 实例
   - 解析 `settings.gradle.kts` 文件

2. **配置阶段（Configuration）**
   - 创建并配置项目对象模型
   - 解析所有 `build.gradle.kts` 文件
   - 配置任务及其依赖关系
   - 此阶段会执行所有构建脚本代码

3. **执行阶段（Execution）**
   - 根据任务依赖图确定要执行的任务
   - 执行任务中的动作（actions）
   - 只有在需要时才执行任务（UP-TO-DATE 检查）

## (二) 基本项目结构

典型的 Android Gradle 项目结构：
```text
root/
  ├── gradle/
  │   └── wrapper/                 # Gradle Wrapper
  │       ├── gradle-wrapper.jar
  │       └── gradle-wrapper.properties
  ├── gradle.properties            # 全局属性
  ├── settings.gradle.kts          # 项目设置
  ├── build.gradle.kts             # 构建配置（项目级）
  ├── README.md
  └── app/                         # 应用模块
      ├── build.gradle.kts         # 模块构建配置
      ├── proguard-rules.pro       # R8 规则
      └── src/
          ├── main/                # 主源码集
          │   ├── java/
          │   ├── res/
          │   └── AndroidManifest.xml
          ├── test/                # 本地单元测试
          └── androidTest/         # 仪器化测试
```

## (三) Gradle Wrapper

使用 Gradle Wrapper 确保构建环境一致：
```properties
# gradle-wrapper.properties
distributionBase=GRADLE_USER_HOME
distributionPath=wrapper/dists
zipStoreBase=GRADLE_USER_HOME
zipStorePath=wrapper/dists
distributionUrl=https\://services.gradle.org/distributions/gradle-9.1-all.zip
```

# 三、依赖管理

## (一) 依赖配置

在模块的 `build.gradle.kts` 中声明依赖：
```kotlin
dependencies {
    // 实现依赖（编译时需要，但不对外传递）
    implementation("androidx.core:core-ktx:1.19.1")
    implementation("androidx.appcompat:appcompat:1.8.0")
    implementation("com.google.android.material:material:1.12.0")
    
    // API 依赖（编译时需要且对外传递）
    api("com.google.code.gson:gson:2.10.1")
    
    // 编译期依赖（仅编译时需要，不打包）
    compileOnly("javax.annotation:javax.annotation-api:1.3.2")
    
    // 运行时依赖（仅运行时需要）
    runtimeOnly("com.google.android.gms:play-services-maps:18.2.0")
    
    // 注解处理器（走 KSP，不用 kapt）
    ksp("com.google.dagger:hilt-android-compiler:2.60.1")
    ksp("androidx.room:room-compiler:2.8.5")
    
    // 测试依赖
    testImplementation("junit:junit:4.13.2")
    androidTestImplementation("androidx.test.ext:junit:1.3.0")
}
```

## (二) 版本目录（推荐）

使用版本目录实现类型安全的依赖声明：
```kotlin
// settings.gradle.kts
dependencyResolutionManagement {
    versionCatalogs {
        libs {
            library("androidx-core", "androidx.core:core-ktx", "1.19.1")
            library("appcompat", "androidx.appcompat:appcompat", "1.8.0")
            library("material", "com.google.android.material:material", "1.13.0")
            library("gson", "com.google.code.gson:gson", "2.13.2")
            library("hilt-compiler", "com.google.dagger:hilt-android-compiler", "2.60.1")
            library("room-compiler", "androidx.room:room-compiler", "2.8.5")
            
            // 插件
            plugin("android-gradle-plugin", "com.android.tools.build:gradle", "9.2.1")
            plugin("kotlin-gradle-plugin", "org.jetbrains.kotlin:kotlin-gradle-plugin", "2.2.21")
        }
    }
}

// 在 build.gradle.kts 中使用
dependencies {
    implementation(libs.androidx.core)
    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.gson)
    ksp(libs.hilt.compiler)
    ksp(libs.room.compiler)
}

// 使用插件
plugins {
    alias(libs.plugins.android.gradle.plugin) apply false
    alias(libs.plugins.kotlin.gradle.plugin) apply false
}
```

## (三) 依赖冲突解决

处理依赖冲突的策略：
```kotlin
android {
    // 在 android 块中配置
    packagingOptions {
        // 解决资源冲突
        resources {
            excludes += "/META-INF/{AL2.0,LGPL2.1}"
        }
        // 解决库文件冲突
        doNotStrip "*/armeabi-v7a/lib*.so"
    }
}

allprojects {
    configurations.all {
        resolutionStrategy {
            // 强制使用特定版本
            force("com.google.code.gson:gson:2.10.1")
            
            // 禁止某些模块
            // ★ResolutionStrategy 只有 exclude(Map) 和 exclude(Object...)，
            //   没有 group/module 这两个具名参数；排除模块要写在依赖声明处
            //   implementation("com.squareup.okhttp3:okhttp") {
            //       exclude(group = "org.slf4j")
            //   }
            exclude(mapOf("group" to "com.squareup.okhttp3", "module" to "okhttp"))
            
            // 解决冲突时的策略
            cacheChangingModulesFor(0, "seconds")
            cacheModulesFor(0, "seconds")
        }
    }
}
```

# 四、构建变种和产品风味

## (一) 构建类型（Build Types）

定义不同的构建配置：
```kotlin
android {
    buildTypes {
        getByName("debug") {
            isDebuggable = true
            apply {
                // 开启调试属性
                isMinifyEnabled = false
                debuggable = true
                // 开启调试日志
                buildConfigField("boolean", "LOG_DEBUG", "true")
            }
        }
        
        getByName("release") {
            isDebuggable = false
            apply {
                // 启用压缩和混淆
                isMinifyEnabled = true
                isShrinkResources = true
                proguardFiles(
                    getDefaultProguardFile("proguard-android-optimize.txt"),
                    "proguard-rules.pro"
                )
                // 移除调试信息
                buildConfigField("boolean", "LOG_DEBUG", "false")
            }
        }
        
        // 自定义构建类型
        create("staging") {
            initWith(getByName("debug"))
            applicationIdSuffix = ".staging"
            versionNameSuffix = "-STAGING"
            buildConfigField("String", "API_BASE_URL", '"https://staging.api.example.com/"')
        }
    }
}
```

## (二) 产品风味（Product Flavors）

为不同的应用变种配置不同的资源和功能：
```kotlin
android {
    flavorDimensions += "mode"
    productFlavors {
        create("free") {
            dimension = "mode"
            applicationIdSuffix = ".free"
            versionNameSuffix = "-FREE"
            
            // 特定于免费版的资源
            resValue("string", "app_name", "Waiminutes Free")
            
            // 特定于免费版的依赖
            // implementation("com.google.android.gms:play-services-ads:22.5.0")
        }
        
        create("paid") {
            dimension = "mode"
            applicationIdSuffix = ".paid"
            versionNameSuffix = "-PAID"
            
            // 特定于付费版的资源
            resValue("string", "app_name", "Waiminutes Pro")
        }
    }
}

// 组合维度示例
android {
    flavorDimensions += "mode"
    flavorDimensions += "api"
    
    productFlavors {
        create("free") {
            dimension = "mode"
            // ...
        }
        create("pro") {
            dimension = "mode"
            // ...
        }
        
        // ★命名与实际值要一致，否则这个 flavor 毫无意义
        create("minApi24") {
            dimension = "api"
            minSdk = 24
            // ...
        }
        create("minApi26") {
            dimension = "api"
            minSdk = 26
            // ...
        }
    }
}
```

# 五、构建变种（类型与风味的组合）

## (一) 构建变种（Build Variants）

构建变种 = 构建类型 + 产品风味：
```
构建变种:
- freeDebug
- freeRelease
- freeStaging
- paidDebug
- paidRelease
- paidStaging
```

每个变种可以有不同的：
- 应用 ID（applicationId）
- 版本名称和版本码
- 资源值
- 依赖项
- 构建配置

# 六、签名配置

## (一) 基本签名配置

```kotlin
android {
    signingConfigs {
        create("debug") {
            storeFile(file("debug-keystore.keystore"))
            storePassword("android")
            keyAlias = "androiddebugkey"
            keyPassword = "android"
        }
        
        create("release") {
            // 为了安全，不要将密码明文写入构建文件
            // 使用环境变量或 Gradle 属性
            if (project.hasProperty("MYAPP_RELEASE_STORE_FILE")) {
                storeFile(file(MYAPP_RELEASE_STORE_FILE))
                storePassword(MYAPP_RELEASE_STORE_PASSWORD)
                keyAlias = MYAPP_RELEASE_KEY_ALIAS
                keyPassword = MYAPP_RELEASE_KEY_PASSWORD
            }
        }
    }
    
    buildTypes {
        getByName("release") {
            signingConfig = signingConfigs.getByName("release")
        }
    }
}
```

## (二) 通过 Gradle 属性管理敏感信息

在 `gradle.properties` 中（不要提交到版本控制）：
```properties
MYAPP_RELEASE_STORE_FILE=/path/to/release-keystore.keystore
MYAPP_RELEASE_STORE_PASSWORD=your_store_password
MYAPP_RELEASE_KEY_ALIAS=your_key_alias
MYAPP_RELEASE_KEY_PASSWORD=your_key_password
```

或者在 CI 系统中设置环境变量：
```bash
# 在 CI 配置中
export MYAPP_RELEASE_STORE_FILE="/path/to/release-keystore.keystore"
export MYAPP_RELEASE_STORE_PASSWORD="your_store_password"
export MYAPP_RELEASE_KEY_ALIAS="your_key_alias"
export MYAPP_RELEASE_KEY_PASSWORD="your_key_password"
```

## (三) 多渠道打包

使用产品风味或自定义任务进行多渠道打包：
```kotlin
// 方法1：使用产品风味
android {
    flavorDimensions += "channel"
    productFlavors {
        create("wandoujia") {
            dimension = "channel"
        }
        create("xiaomi") {
            dimension = "channel"
        }
        create("huwei") {
            dimension = "channel"
        }
        create("qq") {
            dimension = "channel"
        }
        create("official") {
            dimension = "channel"
        }
    }
}

// 方法2：新 Variant API（AGP 8.0+ 引入，AGP 9 已移除旧 API）
androidComponents {
    onVariants { variant ->
        // 在配置期注册变体回调；不要写进 doLast —— 那时变体已全部创建完毕，
        // 回调一次都不会触发，任务等于空跑
    }
}
```

# 七、R8 优化器

## (一) R8 的作用

R8 是 Android 构建过程中替代 ProGuard 的工具，负责：

1. **Tree Shaking（摇树优化）**：移除未使用的代码和资源
2. **代码压缩（Minification）**：缩短类名、方法名和字段名
3. **代码优化（Optimization）**：应用字节码级别的优化
4. **资源压缩**：移除未使用的资源并压缩已用资源

## (二) 启用 R8

在 `gradle.properties` 中：
```properties
# R8 在 AGP 3.4+ 就是默认且唯一的选择了。
# android.enableR8 / android.enableR8.libraries / android.enableR8.fullMode
# 这三个开关在 AGP 8+ 已废弃或无效——full mode 自 AGP 8.0 起默认开启。
# 只有确实需要退出全模式时才写下面这一行：
# android.enableR8.fullMode=false
```

在构建类型中配置：
```kotlin
android {
    buildTypes {
        getByName("release") {
            isMinifyEnabled = true  // 启用 R8
            isShrinkResources = true // 启用资源压缩
            
            // 自定义保留规则
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
}
```

# 八、R8 自定义保留规则

## (一) 自定义保留规则

在 `proguard-rules.pro` 中保留必要的代码：
```proguard
# 保留 Application 子类
-keep public class * extends android.app.Application

# 保留 Activity 子类
-keep public class * extends android.app.Activity

# 保留 Service 子类
-keep public class * extends android.app.Service

# 保留 BroadcastReceiver 子类
-keep public class * extends android.content.BroadcastReceiver

# 保留 ContentProvider 子类
-keep public class * extends android.content.ContentProvider

# 保留 ViewModel 子类（ViewModel 自身有 @Keep，根本不会被裁）
-keep class * extends androidx.lifecycle.ViewModel { *; }

# 保留枚举类
-keepclassmembers enum * {
    public static **[] values();
    public static ** valueOf(java.lang.String);
}

# 保留 native 方法
-keepclasseswithmemberclass,includedescriptorclasses class * {
    native <methods>;
}

# 保留在 XML 中声明的自定义 View
-keep public class * extends android.view.View {
    public <init>(android.content.Context);
    public <init>(android.content.Context, android.util.AttributeSet);
    public <init>(android.content.Context, android.util.AttributeSet, int);
}

# 保留处理回调方法（如 onClick）
-keepclassmembers class * extends android.app.Activity {
    public void *(android.view.View);
}

# 保留 Parcelable 实现
-keep class * implements android.os.Parcelable {
  public static final android.os.Parcelable$Creator *;
}

# 保留 Serializable 实现
-keepclassmembers class * implements java.io.Serializable {
    static final long serialVersionUID;
    private static final java.io.ObjectStreamField[] serialPersistentFields;
    private void writeObject(java.io.ObjectOutputStream);
    private void readObject(java.io.ObjectInputStream);
    java.lang.Object writeReplace();
    java.lang.Object readResolve();
}

# 保留泛型签名（如果使用了反射泛型）
-keepattributes Signature

# 保留注解（如果使用了运行时注解如 Gson）
-keepattributes *Annotation*

# 保留特定库的类
-keep class com.google.gson.** { *; }
-keep class org.json.** { *; }
```

## (二) 针对特定库的规则

常见库的保留规则示例：

**Gson**：
```proguard
# Gson 使用反射，需要保留被序列化的类
-keep class com.waiminutes.app.model.** { *; }
-keepclassmembers class com.waiminutes.app.model.** {
    <fields>;
}
```

**Glide**（本系列用 Coil，不引 Glide；维护 Glide 存量项目时才需要这段）：

```proguard
# Glide 需要保留其接口和实现
-keep public class * implements com.bumptech.glide.module.GlideModule
-keep public class * extends com.bumptech.glide.module.AppGlideModule
-keep public enum com.bumptech.glide.load.ImageHeaderParser$** {
  **[] $VALUES;
  public *;
}
```

**Firebase**：
```proguard
# Firebase 常用规则
-keepclassmembers class com.google.firebase.*.Firebase* {
  public *** getInstance*;
}
```

**Retrofit**：
```proguard
# Retrofit 使用反射，需要保留接口及其注解
-keepattributes Signature
-keepattributes RuntimeVisibleAnnotations
-keep class retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory
-keep class retrofit2.converter.gson.GsonConverterFactory
-keepclassmembers class * {
    @retrofit2.http.* <methods>;
}
```

# 九、构建性能优化

## (一) 配置 Gradle 属性

在 `gradle.properties` 中启用性能优化：
```properties
# 启用并行项目执行
org.gradle.parallel=true

# 启用构建缓存
org.gradle.caching=true

# 启用配置缓存
org.gradle.configuration-cache=true
org.gradle.configuration-cache.problems=warn

# 设置守护进程 JVM 参数
org.gradle.jvmargs=-Xmx4g -XX:MaxMetaspaceSize=1g

# Kotlin daemon 有独立键，不要用 -D 塞进上面那行（两套进程会争抢堆、易 OOM）
kotlin.daemon.jvmargs=-Xmx2g

# Jetifier 把第三方库重写成 AndroidX（与增量编译无关）
# ★android.enableJetifier 在 AGP 8.1+ 已废弃；全 AndroidX 项目不需要它
android.useAndroidX=true

# R8 相关优化
android.enableR8=true
android.enableR8.libraries=true
```

## (二) 优化依赖解析

减少依赖解析时间：
```kotlin
// ★dependencyResolutionManagement 是 Settings 级 API，只能写在
//   settings.gradle.kts 里，写在项目的 allprojects {} 里解析不到这个扩展
// （版本目录与仓库声明见前面的 settings.gradle.kts 示例）

// 在根 build.gradle.kts 中
allprojects {
    // 解决依赖冲突
    configurations.all {
        resolutionStrategy {
            // 缓存动态版本10分钟
            cacheDynamicVersionsFor(10, "minutes")
            // 缓存会变化的模块
            cacheChangingModulesFor(0, "seconds")
        }
    }
}

// 在模块 build.gradle.kts 中
dependencies {
    // 使用实现策略减少传递依赖暴露
    implementation("com.squareup.retrofit2:retrofit:3.0.0") {
        // 指定不需要的传递依赖
        exclude(group = "org.jetbrains.kotlin", module = "kotlin-reflect")
    }
    
    // 使用 API 仅暴露必要的依赖
    api("org.jetbrains.kotlin:kotlin-stdlib:2.2.21") {
        // 明确导出的依赖
    }
    
    // 实现依赖不对外传递
    implementation("com.google.code.gson:gson:2.10.1")
}
```

# 十、资源处理与增量注解处理

## (一) 优化资源处理

减少资源处理时间：
```kotlin
android {
    // 启用资源压缩
    buildTypes.release {
        isShrinkResources = true
    }
    
    // 设置资源前缀避免不必要的重构建
    defaultConfig {
        resourcePrefix = "waiminutes_"  // 仅保留以此前缀开头的资源
    }
    
    // 合并资源时的策略
    packagingOptions {
        resources {
            // 排除不需要的资源
            excludes += [
                "/drawable-hdpi/**",
                "/drawable-xhdpi/**",
                "/drawable-xxhdpi/**",
                "/drawable-xxxhdpi/**"
            ]
        }
        
        // 或仅保留特定密度
        // 只保留 mdpi 资源，其他密度通过缩放获得
        // 但这种方式通常不推荐，除非有特殊需求
    }
    
    // 使用向量图形减少资源数量
    // 在 build.gradle.kts 中
    android {
        defaultConfig {
            vectorDrawables.useSupportLibrary = true
        }
    }
}
```

## (二) 增量注解处理

KSP 默认就是增量的——它按文件粒度重处理，不像 KAPT 那样整模块重来，所以**不需要任何开关**：

```properties
# gradle.properties —— 这两行属于 kapt 时代，迁移到 KSP 后应当删掉
# kapt.incremental.apt=true
# kapt.use.worker.api=true
```

```kotlin
// 需要额外提速时再开（KSP2 的解析器优化）
ksp {
    arg("ksp.experimental.psi.resolution", "true")
}
```

# 十一、缓存策略

## (一) 构建缓存

利用 Gradle 构建缓存：
```kotlin
// 在 gradle.properties 中
org.gradle.caching=true
# 用于调试缓存命中率
org.gradle.caching.debug=true

// 在 CI 中可以使用远程缓存
// 需要配置构建缓存服务器（如 Gradle Enterprise 或自建）
// org.gradle.caching=(local|remote)
// org.gradle.caching.keyring=
// org.gradle.caching.username=
// org.gradle.caching.password=
```

## (二) 配置缓存

实验性功能，可显著提升配置速度：
```properties
org.gradle.configuration-cache=true
org.gradle.configuration-cache.problems=warn
```

# 十二、依赖缓存与失效分析

## (一) 依赖缓存

Gradle 自动缓存下载的依赖，但可以优化：
```properties
# 不要频繁清理缓存
# 缓存位置：~/.gradle/caches/
# 可以定期清理旧版本但保持最近的版本
```

## (二) 缓存失效的常见原因

✅ 构建脚本更改  
✅ 依赖版本更改  
✅ JVM 参数更改  
✅ Gradle 版本更改  
✅ 环境变量更改（如果构建脚本依赖它们）  
✅ 自定义插件更改  

# 十三、自定义插件开发

## (一) 构建逻辑（build-logic）方法

推荐的方式：创建 `build-logic` 模块
```text
root/
  ├── build-logic/
  │   ├── conventions/
  │   │   └── src/
  │   │       └── main/
  │   │           └── kotlin/
  │   │               ├── android-library-convention.kt
  │   │               └── android-application-convention.kt
  │   └── plugins/
  │       └── src/
  │           └── main/
  │               └── kotlin/
  │                   ├── android-library-plugin.kt
  │                   └── android-application-plugin.kt
  ├── settings.gradle.kts
  └── build.gradle.kts
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
        // 应用基础插件
        project.plugins.apply("com.android.library")
        project.plugins.apply("org.jetbrains.kotlin.android")
        
        // 配置 Android 插件
        // ★getByName(...) 返回 Any，里面的成员一个都解析不到。
        //   要按类型取扩展：
        project.extensions.configure<LibraryExtension> {
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
                    isShrinkResources = false
                    proguardFiles(
                        getDefaultProguardFile("proguard-android-optimize.txt"),
                        "proguard-rules.pro"
                    )
                }
                
                debug {
                    isDebuggable = true
                }
            }
            
            compileOptions {
                sourceCompatibility = JavaVersion.VERSION_17
                targetCompatibility = JavaVersion.VERSION_17
            }
            
            kotlinOptions {
                jvmTarget = "17"
            }
            
            // 启用视图绑定
            buildFeatures {
                viewBinding = true
            }
        }
        
        // 应用依赖约定
        project.dependencies {
            implementation("androidx.core:core-ktx:1.19.1")
            implementation("androidx.appcompat:appcompat:1.8.0")
            implementation("com.google.android.material:material:1.12.0")
            
            // 测试依赖
            testImplementation("junit:junit:4.13.2")
            androidTestImplementation("androidx.test.ext:junit:1.3.0")
        }
    }
}
```

# 十四、实现插件与发布

## (一) 编写实现插件

```kotlin
// build-logic/plugins/src/main/kotlin/android-library-plugin.kt
package com.waiminutes.android.plugins

import org.gradle.api.Plugin
import org.gradle.api.Project

class AndroidLibraryPlugin : Plugin<Project> {
    override fun apply(project: Project) {
        // 应用约定
        project.plugins.apply<AndroidLibraryConvention>()
        
        // 可以在这里添加额外的自定义逻辑
        project.afterEvaluate {
            // 构建完成后的处理
        }
    }
}
```

## (二) 发布和使用自定义插件

在根 `settings.gradle.kts` 中包含构建逻辑：
```kotlin
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

rootProject.name = "WaiminutesAndroid"
include(":app")
```

在模块中使用插件：
```kotlin
// app/build.gradle.kts
// ★一个脚本文件只能有一个 plugins {} 块
// ★libs.plugins.android.library 映射的是 AGP 自己的 com.android.library，
//   不是本项目的约定插件；两者不是「二选一」的关系——
//   约定插件自己会 apply AGP，要用它的 id：
plugins {
    id("com.waiminutes.android.library")
}
```

# 十五、高级主题

## (一) 构建缓存条目（Build Cache Entry）

理解 Gradle 如何确定任务是否是最新的：
- 输入（inputs）：源码、资源、依赖、配置
- 输出（outputs）：生成的 class 文件、资源、APK
- 当且仅当输入和输出都未改变时，任务是最新的

## (二) 任务自定义

自定义任务示例：
```kotlin
// 在 build.gradle.kts 中
val verifyApkSignatures = tasks.register("verifyApkSignatures") {
    group = "verification"
    description = "验证所有 APK 的签名"

    doLast {
        val apks = fileTree(layout.buildDirectory.dir("outputs/apk")) { include("**/*.apk") }
        apks.forEach { println("Verifying ${it.name}") }
    }
}

// ★applicationVariants 在 AGP 9 已默认禁用，要用新 Variant API；
//   而且回调必须在【配置期】注册，不能塞进 doLast
androidComponents {
    onVariants { variant ->
        // 这里可以给该变体注册任务依赖
    }
}
```

## (三) 构建扫描（Build Scans）

使用 Gradle Build Scans 获得深入洞察：
```kotlin
// 根 build.gradle.kts —— 插件 id 已从 com.gradle.build-scan
// 改名为 com.gradle.develocity
plugins {
    id("com.gradle.develocity") version "4.0.2"
}

// 在 settings.gradle.kts 里
buildScan {
    // 条款要在 Develocity 网页上接受，termsOfService* 这两个属性已移除
    // 仅在 CI 中启用以避免泄露敏感信息
    if (providers.gradleProperty("ci").isPresent) {
        publishAlways()
    }
}
```

# 十六、最佳实践与常见问题

## (一) 最佳实践

✅ 使用版本目录管理依赖版本  
✅ 将构建逻辑抽象到 build-logic 模块  
✅ 使用构建变种管理不同的部署目标  
✅ 保持构建脚本声明式而非过程式  
✅ 使用 Gradle Wrapper 确保环境一致性  
✅ 启用构建缓存和配置缓存  
✅ 配置合适的 JVM 内存参数  
✅ 理解并正确使用 implementation vs api  
✅ 为发布构建启用 R8 和资源压缩  
✅ 编写自定义保留规则而非依赖默认规则  
✅ 定期更新 Gradle 和插件版本  
✅ 使用并行和配置按需特性  
✅ 监控构建性能并识别瓶颈  

## (二) 常见问题及解决方案

### 1. 问题：依赖冲突导致构建失败
**原因**：不同库依赖同一库的不同版本  
**解决方案**：
- 使用 `./gradlew app:dependencies` 查看依赖树
- 在 resolutionStrategy 中强制使用特定版本
- 使用版本目录集中管理版本
- 考虑使用 `constraints` 声明版本偏好

### 2. 问题：构建时间太长
**原因**：缺少缓存、过度的资源处理、低效的依赖解析  
**解决方案**：
- 启用并行执行和配置按需
- 启用构建缓存和配置缓存
- 优化依赖声明（使用 implementation vs api）
- 减少资源数量并使用向量图形
- 考虑使用增量注解处理

### 3. 问题：R8 移除了必要的代码导致运行时崩溃
**原因**：保留规则不完整或错误  
**解决方案**：
- 使用 `-printconfiguration` 查看实际应用的规则
- 测试时使用 `-dontobfuscate` 先定位问题
- 使用 `-whyareyoukeeping` 理解为什么保留某些类
- 使用 `-whyareyoukeeping class {method}` 理解方法保留原因
- 逐步添加保留规则直到问题解决

### 4. 问题：签名配置泄露敏感信息
**原因**：将密码明文写入构建文件  
**解决方案**：
- 使用 Gradle 属性（gradle.properties）并添加到 .gitignore
- 在 CI 系统中使用环境变量或密钥管理
- 考虑使用 Google Play 应用签名由 Google 管理密钥

### 5. 问题：多渠道打包生成大量重复文件
**原因**：每个渠道都完整解压和重新打包 APK  
**解决方案**：
- 使用申请包名（Package Name）而非完整重新打包
- 使用美团的和面包签名方案（Walle）
- 考虑使用 Google Play 多渠道分发而非自行打包

### 6. 问题：增量编译不起作用导致全量重建
**原因**：构建脚本有副作用或依赖易变的输入  
**解决方案**：
- 查找使用了 `file()` 或依赖当前时间的代码
- 避免在构建脚本中执行不可重复的操作
- 使用 `inputs.files` 和 `outputs.files` 正确声明任务依赖
- 考虑使用配置缓存减少重复配置

## (三) 易错点对照

| 误区 | 事实 |
| --- | --- |
| 有了 `libs.versions.toml` 就不用版本目录的插件了 | 版本目录的 `[plugins]` 和约定插件（`build-logic`）是两件事，后者管的是构建逻辑复用 |
| 依赖越多越安全 | 多一个依赖就多一份攻击面与冲突可能。能用平台 API 解决就别引库 |
| `kapt` 和 `ksp` 可以混用 | 同一个处理器不能混用。Room / Hilt 全系统一走 `ksp` |
| R8 默认就会保留反射用到的类 | 不会。反射、序列化、注解都会在混淆后失效，必须写 `-keep` 规则 |
| `-keep` 越多越保险 | 过度 keep 会让 R8 基本失效，包体积反而变大。按需精确指定 |
| 签名配置写进 `build.gradle.kts` 更方便 | 会把密钥和口令提交进仓库。必须从环境变量或 `local.properties` 读 |
| 版本目录能管 `kotlin-stdlib` | 能，但 Kotlin 插件会强制对齐 stdlib 版本，手动指定极易与编译器版本冲突 |
| `minSdk` 调高能简化代码 | 会直接砍掉一部分用户。行家应该用 `Build.VERSION.SDK_INT` 判断分支，而不是抬 minSdk |

# 十七、参考资料

- [Gradle 用户指南](https://docs.gradle.org/current/userguide/userguide.html)
- [Android Gradle 插件用户指南](https://developer.android.com/studio/build)
- [应用ID 申请包名](https://developer.android.com/studio/build/application-id)
- [构建变种概览](https://developer.android.com/studio/build/build-variants)
- [产品风味和构建类型](https://developer.android.com/studio/build/build-variants#product-flavors)
- [签名您的应用](https://developer.android.com/studio/publish/app-signing)
- [R8 说明文档](https://developer.android.com/studio/build/shrink-code)
- [R8 配置文件格式](https://developer.android.com/studio/build/shrink-code#keep-rules)
- [依赖项管理基础知识](https://developer.android.com/studio/build/dependencies)
- [Gradle 版本目录](https://docs.gradle.org/current/userguide/platforms.html#sub:version-catalogs)
- [Gradle 构建环境](https://docs.gradle.org/current/userguide/build_environment.html)
- [Gradle 配置避免](https://docs.gradle.org/current/userguide/more_about_tasks.html#sec:skipping_task_execution)
- [Gradle 增量构建](https://docs.gradle.org/current/userguide/more_about_tasks.html#sec:incremental_build)
- Gradle 并行执行
- [Gradle 配置缓存](https://docs.gradle.org/current/userguide/configuration_cache.html)
- [Gradle 构建缓存](https://docs.gradle.org/current/userguide/build_cache.html)
- [Gradle 依赖项锁定](https://docs.gradle.org/current/userguide/dependency_locking.html)
- [Android 构建性能调试](https://developer.android.com/studio/build/optimize-your-build)
- Gradle 性能调试指南
- [构建扫描（Build Scans）](https://scans.gradle.com/)
- [Gradle 插件开发指南](https://docs.gradle.org/current/userguide/custom_plugins.html)
- [Gradle Kotlin DSL 参考](https://docs.gradle.org/current/userguide/kotlin_dsl.html)
- Gradle Groovy DSL 参考
- [安全的密钥管理](https://developer.android.com/studio/publish/app-signing#manage-keys)
- 多渠道打包技术比较
- R8 完整模式 vs 默认模式
- [应用体积优化指南](https://developer.android.com/guide/app-bundle)
- [构建重现性（Reproducible Builds）](https://reproducible-builds.org/)
