---
title: "AndroidNotes28 : 发布与上架"
weight: 29
description: "Android 应用发布和上架完整流程：版本管理、签名配置、多渠道打包、测试发布、生产发布、监控与反馈、崩溃分析、性能监控以及应用商店优化（ASO）。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么需要规范的发布流程

规范的发布流程可以：

- **减少发布风险**：通过分阶段发布降低问题影响范围
- **提高发布效率**：自动化流程减少人为错误
- **确保质量标准**：通过测试和审核保持应用质量
- **加快问题响应**：快速回滚和热更新能力
- **满足商店要求**：符合各应用商店的技术和内容要求
- **优化用户体验**：通过渐进式功能发布和A/B测试改善体验
- **保障安全合规**：满足安全标准和法律法规要求

# 二、版本管理策略

## (一) 版本号方案

使用语义化版本控制（Semantic Versioning）：
```
MAJOR.MINOR.PATCH
```
- **MAJOR**：不兼容的 API 变更
- **MINOR**：向后兼容的功能增添
- **PATCH**：向后兼容的问题修复

在 Android 中对应：
- `versionCode`：递增的整数，用于内部比较
- `versionName`：字符串，向用户显示的版本号

```kotlin
android {
    defaultConfig {
        // 建议的 versionCode 管理方式
        versionCode = getVersionCode()
        versionName = "1.2.3"
    }
}

fun getVersionCode(): Int {
    // 可以基于日期、Git 提交号或其他策略生成
    // 例如：YYYYMMDDXX 格式
    val now = Calendar.getInstance()
    val datePart = now.get(Calendar.YEAR) * 10000 +
            (now.get(Calendar.MONTH) + 1) * 100 +
            now.get(Calendar.DAY_OF_MONTH)
    
    // 获取当天的补丁号（需要持久化存储）
    val patch = getTodayPatchNumber()
    
    return datePart * 100 + patch
}

// 或者使用递增方式（简单但需注意分支）
val versionCodes = mapOf(
    "main" to 1000,
    "develop" to 2000,
    "feature/" to 3000
)
```

## (二) 分支策略

常见的 Git 分支模型：
```text
main          ← 生产分支（仅发布代码）
develop       ← 开发分支（集成所有功能）
feature/*     ← 特性分支
release/*     ← 发布准备分支
hotfix/*      ← 生产紧急修复
```

发布流程：
1. 从 `develop` 创建 `release/v1.2.0` 分支
2. 在 release 分支上进行测试、修复和准备
3. 发布时将 release 分支合并到 `main` 和 `develop`
4. 为 main 分支创建 tag：`v1.2.0`
5. 如果发现生产问题，从 main 创建 hotfix 分支修复后重复流程

## (三) 渠道策略

不同渠道的发布策略：
```kotlin
// 在产品风味中定义渠道
android {
    flavorDimensions += "channel"
    productFlavors {
        create("googleplay") {
            dimension = "channel"
        }
        create("huabei") {
            dimension = "channel"
        }
        create("oppo") {
            dimension = "channel"
        }
        create("xiaomi") {
            dimension = "channel"
        }
        create("wandoujia") {
            dimension = "channel"
        }
        create("internal") {
            dimension = "channel"  // 内部测试渠道
        }
    }
}

// 或者使用多渠道打包技术（如美团 Walle）
// 通过修改 APK 中的渠道信息而不重新打包
```

# 三、签名和安全配置

## (一) 生产签名要点

1. **使用强密码保护密钥库**
   ```properties
   # gradle.properties（不提交到版本控制）
   MYAPP_RELEASE_STORE_FILE=/path/to/release-keystore.keystore
   MYAPP_RELEASE_STORE_PASSWORD=strong_password_here
   MYAPP_RELEASE_KEY_ALIAS=myapp_release
   MYAPP_RELEASE_KEY_PASSWORD=strong_password_here
   ```

2. **分离调试和生产密钥**
   - 永远不要在生产中使用调试密钥
   - 调试密钥库应包含在版本控制中（但仅用于调试）
   - 生产密钥库应妥善保管，访问受限

3. **考虑使用 Google Play 应用签名**
   - 让 Google 保存和保护您的签名密钥
   - 您只需上传加密后的上传密钥
   - 即使丢失上传密钥也可以通过 Google 恢复

## (二) 签名配置示例
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
            // 从环境变量或属性文件读取敏感信息
            // ★Kotlin DSL 里裸写的 MYAPP_... 只是普通标识符，解析不到
            //   gradle.properties 的值（那是 Groovy 动态属性才有的）
            val storePath = providers.gradleProperty("MYAPP_RELEASE_STORE_FILE")
            if (storePath.isPresent) {
                storeFile(file(storePath.get()))
                storePassword(providers.gradleProperty("MYAPP_RELEASE_STORE_PASSWORD").get())
                keyAlias = providers.gradleProperty("MYAPP_RELEASE_KEY_ALIAS").get()
                keyPassword = providers.gradleProperty("MYAPP_RELEASE_KEY_PASSWORD").get()
            }
        }
    }
    
    buildTypes {
        getByName("release") {
            signingConfig = signingConfigs.getByName("release")
            isMinifyEnabled = true
            isShrinkResources = true
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
}
```

# 四、构建和打包

## (一) 生产构建流程
```bash
# 清理构建缓存（可选但推荐）
./gradlew clean

# 构建生产版本
./gradlew assembleRelease

# 或者构建特定变种
./gradlew assembleGoogleplayRelease
./gradlew assembleHuabeiRelease

# 生成App Bundle（推荐用于Google Play）
./gradlew bundleRelease
./gradlew bundleGoogleplayRelease
```

## (二) App Bundle 优势

相比传统 APK，App Bundle 提供：
- **动态分发**：根据设备特性仅发送必要代码和资源
- **减少下载大小**：平均减少 35% 的下载大小
- **自动分割APK**：Google Play 根据设备生成优化的 APK
- **支持免安装应用**：同一项目可生成免安装版本

```kotlin
// 在模块 build.gradle.kts 中启用
android {
    // ...
    bundle {
        language {
            // 启用语言资源分割
            enableSplit = true
        }
        density {
            // 启用屏幕密度分割
            enableSplit = true
        }
        abi {
            // 启用 CPU 架构分割
            enableSplit = true
        }
    }
}
```

## (三) 多渠道打包

### 1. 使用产品风味（适用于渠道数较少）
```kotlin
// 已在产品风味中定义的渠道会自动生成对应的变种
// 构建命令：./gradlew assembleRelease
// 会生成：googleplayRelease, huabeiRelease, oppoRelease 等
```

### 2. 使用 Walle 工具（适用于渠道数较多）

渠道数一多，手写几十个 `productFlavors` 就不可维护了。Walle 是凹凸实验室开源的渠道打包工具——读一份 `channel.txt`，批量生成 APK/AAB，不用在构建脚本里列出每个渠道。

```kotlin
// 在 settings.gradle.kts 中
pluginManagement {
    repositories {
        gradlePluginPortal()
        maven("https://jitpack.io")
    }
}

// 在根 build.gradle.kts 中
plugins {
    // Walle 的插件 id 是 meituan.walle，坐标是 com.meituan.android.walle:plugin
    id("meituan.walle") version "1.1.6" apply false
}

// ★'apply plugin: xxx' 是 Groovy 语法，在 .kts 里要写成 plugins { id("xxx") }

// 在模块 build.gradle.kts 中
android {
    // ...
    walle {
        // 渠道文件，每行一个渠道名称
        channelFile = file("channel.txt")
    }
}

// channel.txt 内容：
googleplay
huabei
oppo
xiaomi
wandoujia
// ...

// 构建命令生成所有渠道包
./gradlew assembleAllRelease
// 或者指定渠道
./gradlew assembleAllRelease -Pwalle.channel=googleplay
```

# 五、测试发布（内部测试）

## (一) 内部测试分发渠道

1. **Google Play 内部测试轨道**
   - 最多 100 名内部测试者
   - 快速反馈循环（通常几分钟内可用）
   - 适合早期验证和日常构建

2. **Firebase App Distribution**
   - 支持 iOS 和 Android
   - 集成 Firebase 生态系统
   - 支持群组管理和自动更新通知

3. **企业内部分发**
   - 使用 MDM 移动设备管理方案
   - 适用于企业内部应用
   - 可以绕过公共应用商店限制

## (二) 内部测试流程
```text
1. 构建内测版本
   ./gradlew assembleGoogleplayRelease
   
2. 上传到内测平台
   # 使用 Firebase CLI
   firebase appdistribution:release build/app/outputs/apk/googleplay/release/app-googleplay-release.apk \
       --app 1:1234567890:android:abcdef123456 \
       --groups internal-testers,qa-team \
       --release-notes "修复了登录崩溃问题，优化了启动速度"
   
3. 通知测试者
   # 平台会自动发送邮件或通知
   
4. 收集反馈和崩溃报告
   # 通过 Firebase Crashlytics 或类似服务
   
5. 根据反馈进行迭代
   # 修复问题，重复流程
```

# 六、生产发布策略

## (一) 分阶段发布

分阶段发布可以降低风险：
```text
阶段 1：5% 用户（内部员工 + 关键外部用户）
阶段 2：15% 用户（早期采纳者）
阶段 3：30% 用户（一般用户）
阶段 4：50% 用户（更广泛的用户群）
阶段 5：100% 用户（全量发布）
```

在 Google Play 中：
1. 创建新的发布版本
2. 选择“分阶段发布”而非“立即发布”
3. 设置每阶段的用户百分比和持续时间
4. 监控关键指标（崩溃率、ANR率、评分等）
5. 如果指标正常，逐步增加发布比例
6. 如果发现问题，可以暂停或回滚发布

## (二) 生产发布前检查清单

### 1. 技术检查
- [ ] 使用生产签名而非调试签名
- [ ] 禁用调试属性（debuggable=false）
- [ ] 启用代码压缩和资源压缩（isMinifyEnabled=true）
- [ ] 移除测试依赖和调试代码
- [ ] 验证 ProGuard/R8 规则不误删必要代码
- [ ] 确保所有第三方库许可证合规
- [ ] 检查是否使用了受限制的 API 或权限
- [ ] 验证 HTTPS 使用和网络安全配置

### 2. 功能检查
- [ ] 所有核心功能在目标设备上正常工作
- [ ] 关键用户路径可完成（如登录、搜索、购买）
- [ ] 适配不同屏幕尺寸和方向
- [ ] 兼容目标最低和最高 Android 版本
- [ ] 后台任务正确调度和执行
- [ ] 通知正确显示和处理
- [ ] 权限申请合理且有解释

### 3. 性能检查
- [ ] 启动时间符合预期（冷启动 < 2s）
- [ ] 关键场景帧率稳定（掉帧 < 2%）
- [ ] 内存使用在合理范围内（< 150MB）
- [ ] 电池消耗率在可接受范围内
- [ ] 网络请求合理且有缓存策略

### 4. 法律和合规检查
- [ ] 隐私政策已更新并可见
- [ ] 服务条款符合当地法律
- [ ] 儿童隐私保护（如适用）符合 COPPA/GDPR
- [ ] 广告内容符合商店政策
- [ ] 数据跨境传输符合当地法规

# 七、监控与反馈

## (一) 崩溃和异常监控

### 1. Firebase Crashlytics 集成
```kotlin
// 添加依赖
dependencies {
    implementation("com.google.firebase:firebase-crashlytics:18.3.5")
    // 如果使用 Kotlin
    implementation("com.google.firebase:firebase-crashlytics:20.0.0")
}

// 在 Application 中初始化（通常自动完成）
class MyApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        // Firebase 自动初始化 Crashlytics
    }
}

// 自定义记录异常
fun recordException(e: Exception) {
    FirebaseCrashlytics.getInstance().recordException(e)
}

// 记录自定义错误
fun logError(message: String, fatal: Boolean = false) {
    if (fatal) {
        FirebaseCrashlytics.getInstance().setCustomKey("error_type", "fatal")
        FirebaseCrashlytics.getInstance().log(message)
        FirebaseCrashlytics.getInstance().crash() // 或者使用 recordException
    } else {
        FirebaseCrashlytics.getInstance().log(message)
        FirebaseCrashlytics.getInstance().setCustomKey("error_type", "non_fatal")
    }
}
```

### 2. 自定义崩溃上报
```kotlin
// 未捕获异常处理器
class CrashHandler : Thread.UncaughtExceptionHandler {
    private val handler: Thread.UncaughtExceptionHandler
    
    constructor(defaultHandler: Thread.UncaughtExceptionHandler) {
        this.handler = defaultHandler
    }
    
    override fun uncaughtException(thread: Thread, ex: Throwable) {
        // 上报到自己的服务器
        reportToServer(thread, ex)
        
        // 然后调用默认处理器（通常会杀死进程）
        handler.uncaughtException(thread, ex)
    }
    
    private fun reportToServer(thread: Thread, ex: Throwable) {
        // 发送崩溃报告到后端服务器
        // 包含设备信息、应用版本、堆栈跟踪等
    }
}

// 在 Application 中设置
class MyApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        Thread.setDefaultUncaughtExceptionHandler(
            CrashHandler(Thread.getDefaultUncaughtExceptionHandler())
        )
    }
}
```

## (二) 性能监控

### 1. Firebase Performance Monitoring
```kotlin
// 添加依赖
dependencies {
    implementation("com.google.firebase:firebase-perf:20.4.1")
    implementation("com.google.firebase:firebase-perf:21.0.1")
}

// 自动收集：
// - 应用启动时间
// - 网络请求性能
// - 屏幕渲染性能

// 自定义性能追踪
fun traceNetworkRequest() {
    val trace = FirebasePerformance.getInstance().newTrace("network_request_trace")
    trace.start()
    
    // 执行网络请求
    val response = makeNetworkRequest()
    
    // ★putMetric 的第二个参数是 long，Int 过不去；
    //   而且必须放在 stop() 之前，stop 之后写没有意义
    trace.putMetric("response_size", (response.body?.length ?: 0).toLong())
    trace.stop()
}
```

### 2. 自定义性能埋点
```kotlin
object AppMetrics {
    private const val PREF_FILE = "app_metrics"
    
    fun recordLaunchTime(context: Context, timeMs: Long) {
        val prefs = context.getSharedPreferences(PREF_FILE, Context.MODE_PRIVATE)
        prefs.edit()
            .putLong("launch_cold_${Build.VERSION.SDK_INT}", timeMs)
            .apply()
    }
    
    fun recordFrameDrop(context: Context, frameTimeMs: Long) {
        val prefs = context.getSharedPreferences(PREF_FILE, Context.MODE_PRIVATE)
        val key = "frame_drops_over_${frameTimeMs}ms"
        val current = prefs.getInt(key, 0)
        // ★统一用 Long 存取，否则 getMetrics 里 (v as Long) 会 ClassCastException
        prefs.edit()
            .putLong(key, (prefs.getLong(key, 0L) + 1L))
            .apply()
    }
    
    fun getMetrics(context: Context): Map<String, Long> {
        val prefs = context.getSharedPreferences(PREF_FILE, Context.MODE_PRIVATE)
        return prefs.all.mapNotNull { (k, v) ->
            // SharedPreferences 存的是 Any?，取值时按实际类型判断
            (v as? Long)?.let { k to it }
        }.toMap()
    }
}
```

## (三) 用户反馈收集

### 1. 应用内反馈机制
```kotlin
@Composable
fun FeedbackDialog(
    onDismiss: () -> Unit,
    onSendFeedback: (String) -> Unit
) {
    var feedbackText by remember { mutableStateOf("") }
    var isSending by remember { mutableStateOf(false) }
    
    AlertDialog(
        onDismissRequest = onDismiss,
        title = { Text("发送反馈") },
        text = { Text("请描述您遇到的问题或提供建议") },
        confirmButton = {
            TextButton(
                onClick = {
                    if (feedbackText.isNotBlank()) {
                        isSending = true
                        onSendFeedback(feedbackText)
                    }
                },
                enabled = !isSending
            ) {
                if (isSending) CircularProgressIndicator(modifier = Modifier.size(24.dp))
                else Text("发送")
            }
        },
        dismissButton = {
            TextButton(onClick = onDismiss) {
                Text("取消")
            }
        }
    ) {
        OutlinedTextField(
            value = feedbackText,
            onValueChange = { feedbackText = it },
            label = { Text("反馈内容") },
            maxLines = 5
        )
    }
}
```

### 2. 第三方反馈平台
- **UserVoice**：专业的用户反馈和支持平台
- **Helpshift**：应用内客服和反馈解决方案
- **Instabug**：错误报告、用户反馈和应用内测试
- **Shake**：通过摇晃设备快速反馈

# 八、应用商店优化（ASO）

## (一) 应用商店排名因素

### 1. Google Play 排名因素
1. **标题和描述中的关键词**
2. **下载量和下载速度**
3. **用户评分和评价数量**
4. **崩溃率和ANR率**
5. **用户保留率和参与度**
6. **更新频率和质量**
7. **反向链接和社会信号**
8. **本地化质量和完整度**

### 2. Google Play 的两个「隐形」因素

除了上面这些显性指标，Google Play 还看两个容易被忽略的：

| 因素 | 说明 |
| --- | --- |
| **崩溃率 / ANR 率** | 超过阈值会被自动降权，甚至停止推荐。装量大的应用尤其敏感 |
| **本地化完整度** | 只发中文商店与只发英文商店，覆盖面完全不同。多语言全量上架能显著扩大可触达人群 |

⚠ **「更新频率」不是指越频繁越好**。Google Play 会识别内容雷同的批量更新并降权。合理的节奏是「有实际改进时更新」，而不是为了排名而刷版本。

## (二) 优化策略

### 1. 标题和图标优化
```text
标题优化原则：
- 前置核心关键词（前 3-5 个字符最重要）
- 保持简洁易记（建议 < 30 个字符）
- 避免使用特殊字符和ALL CAPS
- 考虑品牌化和差异化

图标优化原则：
- 简单易识别（即使在小尺寸下也能辨认）
- 使用对比鲜明的颜色
- 避免文字信息（在小尺寸下不可读）
- 考虑不同背景下的适应性
- 提供不同尺寸的版本（考虑平台要求）
```

### 2. 描述优化
```text
描述结构建议：
1. 开篇黄金段落（前 3 行）：核心价值卖点 + 关键词
2. 功能特点列表：使用项目符号突出主要功能
3. 使用场景描述：帮助用户想象使用情境
4. 社会证明：获奖信息、用户数量、好评引用
5. 技术细节：支持的 Android 版本、设备要求
6. 更新说明：最近更新了什么
7. 叫到行动（CTA）：鼓励下载和使用

关键词布局：
- 标题：1-2 个最重要的关键词
- 描述前 200 字符：3-5 个重要关键词
- 全文：自然分布 8-12 个相关关键词
- 避免关键词堆砌：保持语言自然流畅
```

### 3. 截图和预览视频
```text
截图优化：
- 第一张截图必须展示核心价值 proposition
- 使用真实数据而非占位符内容
- 添加简短文字说明突出特色功能
- 保持视觉流程：引导用户眼神运动
- 展示不同场景和使用时间点
- 考虑使用帧动画 GIF 展示交互过程

预览视频：
- 长度：30 秒左右（前 15 秒最重要）
- 先展示问题，再展示您的应用如何解决
- 使用字幕说明关键功能（很多用户静音观看）
- 展示真实使用场景而非演示版本
- 结尾包含明确的叫到行动
```

### 4. 评价和评分管理
```text
获取好评策略：
- 在用户完成关键任务后适时请求评分
- 提供应用内反馈渠道减少差评
- 对负面评价及时响应和解决问题
- 考虑使用应用内评价 API 而跳转到商店
- 避免在用户不满时请求评分

评分恢复策略：
1. 监控评分趋势和新评价内容
2. 对负面评价进行公开和私下回应
3. 解决提到的具体问题并更新应用
4. 请求用户在问题解决后修改评价
5. 分享更新说明和修复的具体内容
6. 利用正面评价进行社会证明
```

# 九、发布后活动

## (一) 发布后 24 小时关键指标

监控这些指标可以快速发现问题：
```text
1. 崩溃率（Crash Rate）：
   - 目标：< 1% （新发布通常会有轻微波动）
   - 警戒线：> 2% 需要立即调查

2. ANR率（Application Not Responding Rate）：
   - 目标：< 0.5%
   - 警戒线：> 1% 需要检查主线程阻塞

3. 启动时间异常：
   - 比基线增加 > 20% 需要调查

4. 评分趋势：
   - 新版本评分显著低于前版本需要关注

5. 功能使用率：
   - 关键功能使用率异常下降可能表示问题
```

## (二) 发布后一周活动

1. **深度数据分析**
   - 按设备型号、Android 版本、国家/地区分析表现
   - 识别特定人群的问题
   - 对比不同渠道用户的行为差异

2. **A/B 测试迭代**
   - 基于早期反馈进行小范围功能测试
   - 使用 Firebase Remote Config 或类似服务
   - 测试 UI 变化、文案修改、功能调整等

3. **社区和媒体关系**
   - 监控社交媒体提及和讨论
   - 回应用户问题和感谢正面反馈
   - 向相关媒体和博客发送更新通知
   - 参与行业讨论和技术分享

4. **为下一版本做准备**
   - 收集和整理用户功能请求
   - 分析竞争对手的最新动态
   - 更新技术栈和依赖库（注意兼容性）
   - 规划下一版本的主要功能和改进

# 十、特殊发布场景

## (一) 强制更新机制

当发现严重问题时实施强制更新：
```kotlin
// 从服务器获取最低要求版本
suspend fun getMinimumRequiredVersion(): String {
    return versionService.getMinimumRequiredVersion()
}

// 在 Application 中检查
class MyApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        
        // ★Application 不是 LifecycleOwner，没有 lifecycleScope（该扩展定义在
        //   LifecycleOwner 上）。自建一个应用级 scope：
        val appScope = CoroutineScope(SupervisorJob() + Dispatchers.Main.immediate)

        appScope.launch {
            // getPackageInfo 是 Context.packageManager 上的实例方法，
            // 不能 PackageManager.getPackageInfo(...) 这样静态调用；
            // targetSdk 33+ 还得传 PackageInfoFlags
            val currentVersion = packageManager.getPackageInfo(
                packageName,
                PackageManager.PackageInfoFlags.of(0)
            ).versionName ?: "0"

            val minimumVersion = getMinimumRequiredVersion()

            if (VersionUtils.compare(currentVersion, minimumVersion) < 0) {
                showForceUpdateDialog()
            }
        }
    }
    
    private fun showForceUpdateDialog() {
        // ★android.app.AlertDialog 必须用 Builder；直接 new 出来没有
        //   setTitle/setMessage 的链式 API
        android.app.AlertDialog.Builder(this@MyApplication)
            .setTitle("需要更新")
            .setMessage("发现严重问题，请立即更新应用以继续使用。")
            .setPositiveButton("更新") { _, _ ->
                // 跳转到应用商店更新页面
                val intent = Intent(Intent.ACTION_VIEW).apply {
                    data = Uri.parse("market://details?id=$packageName")
                }
                startActivity(intent)
                // 防止用户继续使用旧版本
                // ★finishAffinityAndStopService() 不存在
                finishAffinity()
            }
            .setCancelable(false)
            .show()
    }
}
```

## (二) 动态配置：能做什么，不能做什么

| 手段 | 能否改代码 | 能否改文案/开关 | 商店政策 |
| --- | --- | --- | --- |
| 远程配置（Remote Config） | 否 | **能** | 允许 |
| 服务端开关 / 特性开关 | 否 | **能** | 允许 |
| Google Play Feature Delivery | 否 | **能**（按条件启停模块） | 允许 |
| AAB 动态功能模块 | 否 | 否（体积按需下发） | 允许 |
| WebView 容器 | 否 | 否（内容算远程内容） | 允许但受「不得伪装成原生」约束 |
| 下载并执行 DEX / 热修复 | **能** | 能 | **明令禁止** |

⚠ **Google Play 的政策非常明确：不许下发可执行代码。** 插件化框架把功能模块做成可下载的 DEX、React Native / Flutter 的热修复包，本质都是在应用运行时下载并加载代码——这类应用会被拒审，甚至下架。

所以「上线后发现文案错了要紧急修」这类需求，**不要想热修复，要用远程配置**：把文案、开关、阈值、URL 全部抽到配置里，改配置不需要发版。

```kotlin
class AppConfig @Inject constructor(
    private val remote: RemoteConfigService
) {
    // 本地兜底值：网络不通时仍能启动
    val bannerText: String
        get() = remote.getString("banner_text") ?: "默认文案"

    val enableNewCheckout: Boolean
        get() = remote.getBoolean("enable_new_checkout", defaultValue = false)
}
```

**能远程改的**：文案、功能开关、阈值参数、服务器地址、活动页 URL、引导流程分支。
**改不了的**：任何编译进 APK 的行为（新增界面、改算法、改布局）。这些只能靠发版。

真正的「紧急修复」只有一条路：**Google Play 的分阶段发布 + 暂停推广**。发现问题时先暂停推广、修复、重新提交，比任何热更新都稳妥。


# 十一、发布流程自动化

## (一) 持续交付流水线

典型的 CI/CD 流水线：
```text
1. 代码提交 → 触发构建
2. 单元测试运行 → 测试通过继续
3. 静态代码检查 → 通过继续
4. 构建测试版本 → 分发到内测渠道
5. 自动化测试运行 → 通过继续
6. 人工评审和批准 → 获得批准继续
7. 构建生产版本 → 上传到发布平台
8. 分阶段发布 → 监控关键指标
9. 全量发布 → 完成发布流程
```

### 1. 使用 GitHub Actions 示例
```yaml
name: Android CI/CD

on:
  push:
    branches: [ main ]
  pull_request:
    branches: [ main ]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v4
    
    - name: Set up JDK
      uses: actions/setup-java@v4
      with:
        java-version: '17'
        distribution: 'temurin'
    
    - name: Grant execute permission for gradlew
      run: chmod +x gradlew
    
    - name: Build with Gradle
      run: ./gradlew build
    
    - name: Run Unit Tests
      run: ./gradlew testDebugUnitTest
    
    - name: Run Instrumented Tests
      # 需要模拟器或设备
      run: ./gradlew connectedAndroidTest
    
    - name: Build Release
      run: ./gradlew assembleRelease
    
    - name: Build Signed Release APK
      # 签名交给 Gradle 的 signingConfig（assembleRelease 出来的已经是签好名的）
      # ★不要再用 jarsigner 二次签名：会产生双重签名/损坏的 APK，
      #   且 SHA1withRSA 已被 Google Play 拒收（要求 v2/v3 签名、SHA-256 起）
      env:
        KEYSTORE_PATH: ${{ secrets.KEYSTORE_PATH }}
        KEYSTORE_PASSWORD: ${{ secrets.KEYSTORE_PASSWORD }}
        KEY_ALIAS: ${{ secrets.KEY_ALIAS }}
        KEY_PASSWORD: ${{ secrets.KEY_PASSWORD }}
      run: |
        # 仅在确实需要外部签名时用 apksigner（它会替换而非叠加签名）
        $ANDROID_HOME/build-tools/35.0.0/apksigner sign \
          --ks "${KEYSTORE_PATH}" \
          --ks-key-alias "${KEY_ALIAS}" \
          --ks-pass "pass:${KEYSTORE_PASSWORD}" \
          --key-pass "pass:${KEY_PASSWORD}" \
          build/app/outputs/apk/release/app-release.apk
    
    - name: Upload Artifact
      uses: actions/upload-artifact@v4
      with:
        name: release-apk
        path: build/app/outputs/apk/release/app-release.apk
```

## (二) 自动化版本号递增
```kotlin
// 在 gradle.properties 中
VERSION_MAJOR=1
VERSION_MINOR=2
VERSION_PATCH=3
VERSION_BUILD=1

// 在 build.gradle.kts 中
// ★Kotlin DSL 里裸写的 VERSION_MAJOR 取不到属性值，要用 providers
val major = providers.gradleProperty("VERSION_MAJOR").get().toInt()
val minor = providers.gradleProperty("VERSION_MINOR").get().toInt()
val patch = providers.gradleProperty("VERSION_PATCH").get().toInt()
val build = providers.gradleProperty("VERSION_BUILD").get().toInt()

android {
    defaultConfig {
        versionCode = major * 1000000 + minor * 10000 + patch * 100 + build
        versionName = "$major.$minor.$patch.$build"
    }
}

// 创建更新版本号的任务（上面那段是 Groovy 写法，.kts 里要这么写）
tasks.register("incrementVersionCode") {
    group = "versioning"
    doLast {
        val propsFile = file("gradle.properties")
        val props = java.util.Properties().apply { propsFile.inputStream().use { load(it) } }

        val next = props.getProperty("VERSION_BUILD").toInt() + 1
        props.setProperty("VERSION_BUILD", next.toString())

        propsFile.outputStream().use { props.store(it, null) }
        logger.lifecycle("Incremented VERSION_BUILD to $next")
    }
}

// ★不要用 gradle.taskGraph.whenReady + incrementVersionCode.execute() 自动递增：
//   - Task.execute() 是已废弃的执行期触发方式
//   - taskGraph.whenReady 与配置缓存互斥
//   - 每次 assembleRelease 都改版本号 → 构建不可复现
//   正确做法：由发布流程显式调用该任务，或由 CI 注入版本号：
//   ./gradlew incrementVersionCode && ./gradlew assembleRelease
```

# 十二、常见问题与解决方案

## (一) 商店审核被拒绝

### 1. 常见原因及解决方案
```text
1. 功能不符合描述：
   - 确保应用描述准确反映实际功能
   - 移除夸张或误导性声明
   - 截图和视频必须展示实际可用功能

2. 隐私政策问题：
   - 提供完整且易于访问的隐私政策
   - 确保政策说明了数据收集、使用和共享
   - 如果涉及儿童，确保符合 COPPA 要求

3. 受限制权限使用：
   - 移除不必要的短信、通话、后台定位等权限
   - 如果必须使用，提供充分理由并考虑替代方案
   - 对于默认处理器权限，申请相应资格

4. 内容不符合分级标准：
   - 确保内容不包含暴力、色彩或其他禁止内容
   - 使用适当的分级标签
   - 提供家长控制选项（如适用）

5. 广告或变现问题：
   - 确保广告内容符合商店政策
   - 不使用欺骗性广告或强制观看
   - 尊重用户选择并提供退出选项
```

## (二) 发布后崩溃率升高

### 1. 调试步骤
```text
1. 查看崩溃堆栈：
   - 确定是哪个版本引入的问题
   - 检查是否是特定设备或 Android 版本的问题

2. 检查最近更改：
   - 新增的权限或功能
   - 更新的第三方库
   - 修改的启动序列或初始化代码

3. 使用 bisect 定位问题提交：
   - git bisect start
   - git bisect bad  # 当前版本有问题
   - git bisect good v1.2.0  # 已知好的版本
   - 按照指令测试中间版本

4. 检查原生代码和第三方 SDK：
   - 更新的 NDK 或原生库可能导致兼容性问题
   - 第三方 SDK 可能与设备或系统版本不兼容
```

## (三) 下载量或用户留存下降

### 1. 调查方向
```text
1. 用户反馈分析：
   - 查看应用商店评价和评论
   - 检查社交媒体和论坛讨论
   - 使用应用内反馈机制收集具体问题

2. 功能使用率分析：
   - 识别使用率下降的具体功能
   - 检查是否是引入的回归或设计问题
   - 考虑是否与用户期望不匹配

3. 性能问题检查：
   - 启动时间是否显著增加
   - 关键功能响应时间是否变慢
   - 电池消耗是否异常增加

4. 竞争对手分析：
   - 检查竞争对手是否有新功能或改进
   - 分析市场趋势和用户偏好变化
   - 考虑是否需要功能更新或方向调整
```

## (四) 易错点对照

| 误区 | 事实 |
| --- | --- |
| 上架要提交 APK | 必须提交 **App Bundle**（`.aab`）。APK 只用于调试与侧载 |
| `versionCode` 可以随便填 | 它是整数且**必须严格递增**。回退会导致无法覆盖安装，Google Play 直接拒收 |
| 用 `BuildConfig.VERSION_CODE` 自增 | 自增必须写回文件或 CI 变量。App 内自增每次重装都会回到 1 |
| 签名丢了换新签名还能覆盖安装 | 不能。签名变了系统视为另一个应用，用户必须重装且数据丢失 |
| 多渠道只需要多几个 `productFlavor` | `productFlavor` 是**编译期变体**，每个渠道都要单独构建。仅渠道号不同时，可让同一产物携带 `channel` 字段，由服务端按渠道分发 |
| 上架前不测 release 构建 | release 开启 R8 与资源收缩，未处理的反射会**运行时**崩溃，debug 完全测不到 |
| 内测渠道不需要填隐私政策 | 需要。Google Play 对所有渠道都要求隐私政策与数据安全表单 |
| 「构建成功」等于「能跑」 | 成功只说明编译通过。必须装真机跑一遍 release 包 |

# 十三、参考资料

- [Google Play 发布概览](https://developer.android.com/studio/publish)
- [应用内更新 API](https://developer.android.com/guide/app-bundle/in-app-updates)
- [Firebase App Distribution](https://firebase.google.com/docs/app-distribution)
- [Firebase Crashlytics](https://firebase.google.com/docs/crashlytics)
- [Firebase Performance Monitoring](https://firebase.google.com/docs/perf-mon)
- [Android App Bundle 详解](https://developer.android.com/guide/app-bundle)
- 多渠道打包方案比较
- 应用商店优化（ASO）指南
- [Google Play 政策中心](https://support.google.com/googleplay/android-developer/answer/9893335)
- 发布流程自动化最佳实践
- 版本号策略实践
- 内部测试平台比较
- 应用内反馈机制实现
- 热更新技术方案对比
- Android Vitals 指标解释
- 应用启动时间优化实战
- [电池优化最佳实践](https://developer.android.com/topic/performance/background-optimization)
- [内存泄漏检测与修复指南](https://developer.android.com/topic/performance/memory-overview)
- 使用 Macrobenchmark 进行性能测试
- [Firebase Remote Config 使用指南](https://firebase.google.com/docs/remote-config)
- 应用商店评价管理策略
- 隐私政策生成工具
- [应用合规检查清单](https://support.google.com/googleplay/android-developer/answer/6112435)
- 发布后监控仪表盘搭建
- 自动化版本号管理方案
- 企业内部应用分发方案
- 开源许可证合规检查工具
- 应用体积分析工具
- [使用 Play Console 进行发布管理](https://support.google.com/googleplay/android-developer/answer/9842756)
- 灰度发布策略与技术实现
- [功能开关最佳实践框架](https://martinfowler.com/articles/feature-toggles.html)
- [应用安全基线检查清单](https://developer.android.com/topic/security/best-practices)
- [第三方SDK安全评估指南](https://developer.android.com/privacy-and-security/cryptography)
- [数据隐私保护技术指南](https://developer.android.com/training/data-storage)
- [网络安全配置最佳实践](https://developer.android.com/training/articles/security-config)
- [WebView安全使用指南](https://developer.android.com/reference/android/webkit/WebView)
- [生物识别认证最佳实践](https://developer.android.com/training/sign-in/biometric-auth)
- [凭据存储最佳实践](https://developer.android.com/training/articles/keystore)
- [安全网络通信最佳实践](https://developer.android.com/training/articles/security-ssl)
- [组件导出安全控制指南](https://developer.android.com/guide/topics/manifest/manifest-element)
- [Intent 安全使用最佳实践](https://developer.android.com/guide/components/intents-filters#security)
- [剪贴板访问安全最佳实践](https://developer.android.com/reference/android/content/ClipboardManager)
- [储存访问框架（SAF）安全使用](https://developer.android.com/guide/topics/providers/document-provider)
- 媒体存储访问最佳实践
- [后台执行限制说明（Android 8+）](https://developer.android.com/about/versions/oreo/background)
- [前台服务开发指南](https://developer.android.com/guide/components/foreground-services)
- [WorkManager 与后台任务](https://developer.android.com/develop/background-work/background-tasks/persistent/getting-started)
- [AlarmManager 与精确调度](https://developer.android.com/training/scheduling/alarms)
- [SyncAdapter 与数据同步（已弃用）](https://developer.android.com/training/sync-adapters)
- [JobScheduler 与工作调度（Android 5-8）](https://developer.android.com/reference/android/app/job/JobScheduler)
- [广播接收器最佳实践](https://developer.android.com/guide/components/broadcasts)
- [通知栏使用最佳实践](https://developer.android.com/guide/topics/ui/notifiers/notifications)
- [服务使用最佳实践](https://developer.android.com/guide/components/services)
- [内容提供者最佳实践](https://developer.android.com/guide/topics/providers/content-providers)
- [共享偏好设置最佳实践](https://developer.android.com/guide/topics/data/data-storage)
- [文件存储最佳实践](https://developer.android.com/guide/topics/data/data-storage)
- [SQLite 数据库最佳实践](https://developer.android.com/training/data-storage/sqlite)
- [Room 持久化库最佳实践](https://developer.android.com/training/data-storage/room)
- [DataStore 偏好存储最佳实践](https://developer.android.com/topic/libraries/architecture/datastore)
- [SharedPreferences 迁移到 DataStore](https://developer.android.com/topic/libraries/architecture/datastore#migrating)
- [加密文件存储最佳实践](https://developer.android.com/reference/androidx/security/crypto/EncryptedFile)
- [主密钥生成说明](https://developer.android.com/reference/androidx/security/crypto/MasterKeys)
- [生物识别加密最佳实践](https://developer.android.com/reference/androidx/biometric/BiometricPrompt)
- [凭据存储最佳实践](https://developer.android.com/training/articles/keystore)
- [安全随机数生成指南](https://developer.android.com/reference/java/security/SecureRandom)
- [哈希函数使用指南](https://developer.android.com/reference/java/security/MessageDigest)
- [数字签名最佳实践](https://developer.android.com/reference/java/security/Signature)
- [公钥基础设施（PKI）概述](https://developer.android.com/reference/java/security/cert/CertificateFactory)
- [安全套接字层（SSL）/传输层安全（TLS）](https://developer.android.com/reference/javax/net/ssl/SSLSocket)
- [HTTP 安全最佳实践](https://developer.android.com/reference/javax/net/ssl/HttpsURLConnection)
- [安全 Web 开发最佳实践](https://developer.android.com/reference/androidx/webkit/package-summary)
- 安全数据库访问最佳实践
- [安全网络库选型指南](https://developer.android.com/privacy-and-security/cryptography)
- [安全音频处理最佳实践](https://developer.android.com/reference/android/media/AudioFormat)
- [安全图像处理最佳实践](https://developer.android.com/reference/android/widget/ImageView)
- [安全视频处理最佳实践](https.2100000000000001)
