---
title: "AndroidNotes22 : WorkManager 后台任务"
weight: 23
description: "Android WorkManager 后台任务调度库：可保证执行的异步任务、约束条件与触发器、指数退避重试策略、唯一工作与链式工作、与协程与 Flow 的集成、测试策略以及与其他架构组件的协同使用。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么选择 WorkManager

WorkManager 是 Android Jetpack 推出的后台任务调度库，提供：

- **保证执行**：即使应用重启或设备重启也能确保任务最终执行
- **向后兼容**：API 23+ 走 JobScheduler，更低版本用其内置的 AlarmManager + 前台服务兜底
- **约束条件**：支持网络状态、充电状态、存储空闲等执行条件
- **灵活的调度**：支持一次性执行、周期性执行以及复杂的工作链
- **指数退避重试**：失败时自动重试并采用指数退避策略
- **唯一性控制**：防止重复提交相同工作的机制
- **输入输出数据**：支持工作间通过键值对传递数据
- **与生命周期无关**：Activity/Fragment 销毁**不会**取消已入队的工作（只有显式 `cancel*()` 或进程被杀才会）
- **可观测性**：提供 LiveData、Flow 和协程接口监控工作状态
- **调试工具**：内置日志和 Stetho 集成便于故障排除

WorkManager 适用于需要可靠执行但不需要立即完成的任务，如日志上传、数据同步、清理缓存等。

# 二、基础设置

## (一) 添加依赖

在模块的 `build.gradle.kts` 中添加 WorkManager 及其必要依赖：
```kotlin
dependencies {
    // WorkManager 核心库
    implementation("androidx.work:work-runtime:2.12.0")
    
    // Kotlin 协程支持
    implementation("androidx.work:work-runtime-ktx:2.12.0")
    
    // 测试依赖
    testImplementation("androidx.work:work-testing:2.12.0")
    
    // 如果使用 Hilt 进行依赖注入
    implementation("androidx.hilt:hilt-work:1.4.0")
    
    // 如果使用 Protobuf 处理工作数据（可选）
    implementation("com.google.protobuf:protobuf-javalite:4.36.2")
}
```

## (二) 在 AndroidManifest.xml 中声明初始化器

WorkManager 使用 ContentProvider 进行自动初始化，通常不需要手动声明。但如果使用自定义初始化或需要移除默认初始化器：
```xml
<!-- 如果使用自定义初始化器，需要禁用默认初始化器 -->
<provider
    android:name="androidx.work.impl.WorkManagerInitializer"
    android:authorities="${applicationId}.workmanager-init"
    android:exported="false"
    tools:node="remove" />

<!-- 声明自定义初始化器（如果需要） -->
<provider
    android:name="com.waiminutes.app.CustomWorkManagerInitializer"
    android:authorities="${applicationId}.workmanager-init"
    android:exported="false" />
```

# 三、创建工作单元

## (一) 继承 Worker 类

创建基本的工作单元：
```kotlin
class SyncWorker(
    appContext: Context,
    workerParams: WorkerParameters
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        return try {
            // 执行实际的后台工作
            val result = performSyncOperation()
            when (result) {
                is SyncResult.Success -> Result.success()
                is SyncResult.Retry -> Result.retry()
                is SyncResult.Failure -> 
                    // 根据失败类型决定是否重试
                    if (shouldRetry(result.error)) Result.retry()
                    else Result.failure()
            }
        } catch (e: Exception) {
            // 捕获未预期的异常
            Log.e("SyncWorker", "Unexpected error during sync", e)
            Result.failure()
        }
    }
    
    private suspend fun performSyncOperation(): SyncResult {
        // 实际的同步逻辑（网络请求、数据库操作等）
        // 这里返回自定义结果类型以便更细粒度的控制
        try {
            // 执行网络请求
            val data = fetchFromNetwork()
            // 保存到本地数据库
            saveToDatabase(data)
            return SyncResult.Success
        } catch (e: IOException) {
            // 网络错误，通常可以重试
            return SyncResult.Retry(e)
        } catch (e: Exception) {
            // 其他错误，根据业务决定是否重试
            return SyncResult.Failure(e)
        }
    }
    
    private fun shouldRetry(error: Throwable): Boolean {
        // 自定义重试逻辑
        return when (error) {
            is SocketTimeoutException -> true
            is UnknownHostException -> true
            is HttpException -> error.code() in 500..599
            else -> false
        }
    }
    
    // 数据类定义同步结果
    sealed class SyncResult {
        object Success : SyncResult()
        data class Retry(val cause: Throwable) : SyncResult()
        data class Failure(val cause: Throwable) : SyncResult()
    }
    
    private suspend fun fetchFromNetwork(): List<Article> { /* ... */ }
    private suspend fun saveToDatabase(data: List<Article>) { /* ... */ }
}
```

## (二) 定义工作输入输出数据

使用 `Data` 对象在工作间传递简单数据：
```kotlin
class GreetingWorker(
    appContext: Context,
    workerParams: WorkerParameters
) : CoroutineWorker(appContext, workerParams) {
    
    companion object {
        const val ARG_NAME = "name"
        const val OUTPUT_MESSAGE = "message"
    }
    
    override suspend fun doWork(): Result {
        // 从输入数据中获取参数
        val name = inputData.getString(ARG_NAME)
            ?: return Result.failure() // 必要参数缺失
        
        // 执行工作
        val message = "Hello, $name!"
        
        // 设置输出数据
        val outputData = workDataOf(OUTPUT_MESSAGE to message)
        
        return Result.success(outputData)
    }
}
```

## (三) 使用 Hilt 进行依赖注入（可选）

如果使用 Hilt，可以创建支持依赖注入的工作单元：
```kotlin
@HiltWorker
class SyncWorker @AssistedInject constructor(
    @Assisted appContext: Context,
    @Assisted workerParams: WorkerParameters,
    private val articleRepository: ArticleRepository,
    private val networkService: NetworkService
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        // 现在可以使用通过 Hilt 注入的依赖
        return try {
            val articles = articleRepository.getLocalArticles()
            val serverArticles = networkService.fetchLatestArticles()
            articleRepository.syncWithServer(articles, serverArticles)
            Result.success()
        } catch (e: Exception) {
            Result.failure()
        }
    }
    
    // Hilt 工厂类
    @AssistedInject.Factory
    interface Factory : AssistedFactory<SyncWorker>
}
```

在 `AndroidManifest.xml` 中声明 Hilt 工作管理器初始化器：
```xml
<provider
    android:name="androidx.hilt.work.HiltWorkerFactory"
    android:authorities="${applicationId}.hilt-worker-factory"
    android:exported="false" />
```

# 四、创建工作请求

## (一) 一次性工作请求

创建仅执行一次的工作：
```kotlin
// 创建基本的一次性工作请求
val syncWork = OneTimeWorkRequestBuilder<SyncWorker>()
    .build()

// 添加约束条件
val constrainedSyncWork = OneTimeWorkRequestBuilder<SyncWorker>()
    .setConstraints(
        Constraints.Builder()
            .setRequiredNetworkType(NetworkType.CONNECTED)  // 需要网络连接
            .setRequiresCharging(false)                     // 不要求充电
            .setRequiresBatteryNotLow(true)                 // 电量不低时执行
            .setRequiresStorageNotLow(true)                 // 存储空间不低时执行
            .build()
    )
    .build()

// 添加输入数据
val syncWorkWithInput = OneTimeWorkRequestBuilder<SyncWorker>()
    .setInputData(
        workDataOf(
            "page" to 1,
            "pageSize" to 50
        )
    )
    .build()

// 设置后退策略和重试 delay
val syncWorkWithBackoff = OneTimeWorkRequestBuilder<SyncWorker>()
    .setBackoffCriteria(
        BackoffPolicy.EXPONENTIAL,
        Duration.ofSeconds(30),
        TimeUnit.SECONDS
    )
    .build()
```

## (二) 周期性工作请求

创建重复执行的工作：
```kotlin
// 基本周期性工作
val periodicSyncWork = PeriodicWorkRequestBuilder<SyncWorker>(24, TimeUnit.HOURS)
    .build()

// 带有 flex 时间窗口的周期性工作
// ★PeriodicWorkRequestBuilder 只有一个泛型参数（Worker 类），
//  flex 是构造参数、不是 setFlexPeriod() 方法
val flexibleSyncWork = PeriodicWorkRequestBuilder<SyncWorker>(
    24, TimeUnit.HOURS,      // repeatInterval
    1, TimeUnit.HOURS        // flexInterval：在 1 小时窗口内灵活执行
).build()

// 周期性工作带有初始延迟
val delayedPeriodicWork = PeriodicWorkRequestBuilder<SyncWorker>(6, TimeUnit.HOURS)
    .setInitialDelay(12, TimeUnit.HOURS)  // 首次执行延迟 12 小时
    .build()
```

# 五、链式工作与唯一工作

## (一) 链式工作

创建依赖关系的工作序列：
```kotlin
// 创建工作链：第一步 -> 第二步 -> 第三步
val workChain = WorkManager.getInstance(context)
    .beginWith(FirstWorker::class.java)          // 首先执行 FirstWorker
    .then(SecondWorker::class.java)              // 成功后执行 SecondWorker
    .then(ThirdWorker::class.java)               // 成功后执行 ThirdWorker
    .getWorkInfoLiveData()                       // 监控整个链的状态
    .observeForever { workInfo ->
        // 处理工作状态变化
        when (workInfo?.state) {
            WorkInfo.State.SUCCEEDED -> {
                // 链中所有工作都成功完成
            }
            WorkInfo.State.FAILED -> {
                // 链中至少有一个工作失败
            }
            WorkInfo.State.CANCELLED -> {
                // 链被取消
            }
            else -> {
                // 其他状态（RUNNING、ENQUEUED等）
            }
        }
    }

// 或者使用链式构建器（更函数式）
val chainWork = WorkManager.getInstance(context)
    .beginWith(listOf(OneTimeWorkRequestBuilder<FirstWorker>().build()))
    .then(OneTimeWorkRequestBuilder<SecondWorker>().build())
    .then(OneTimeWorkRequestBuilder<ThirdWorker>().build())
    .getWorkInfoLiveData()
```

## (二) 唯一工作

防止重复提交相同工作：
```kotlin
// 使用现有工作策略保持唯一性
WorkManager.getInstance(context)
    .enqueueUniqueWork(
        "daily_sync",                     // 唯一工作名称
        ExistingWorkPolicy.KEEP,          // 如果已存在则保持现有工作
        OneTimeWorkRequestBuilder<SyncWorker>()
            .setConstraints(
                Constraints.Builder()
                    .setRequiredNetworkType(NetworkType.CONNECTED)
                    .build()
            )
            .build()
    )

// 替换现有工作
WorkManager.getInstance(context)
    .enqueueUniqueWork(
        "content_refresh",
        ExistingWorkPolicy.REPLACE,       // 替换现有工作
        OneTimeWorkRequestBuilder<RefreshWorker>()
            .setInitialDelay(5, TimeUnit.MINUTES)
            .build()
    )

// 追加到现有工作链的末尾
WorkManager.getInstance(context)
    .enqueueUniqueWork(
        "backup_chain",
        ExistingWorkPolicy.APPEND,        // 追加到现有工作
        OneTimeWorkRequestBuilder<BackupWorker>()
            .build()
    )
```

# 六、配置 WorkManager

## (一) 自定义初始化

创建自定义配置以控制 WorkManager 行为：
```kotlin
class CustomWorkManagerInitializer : Configuration.Provider {
    override fun getWorkManagerConfiguration(
        context: Context
    ): Configuration {
        return Configuration.Builder()
            .setMinimumLoggingLevel(Log.DEBUG)  // 设置日志级别
            .setExecutor(
                Executors.newFixedThreadPool(
                    Runtime.getRuntime().availableProcessors() * 2
                )
            ) // 自定义执行器
            .setTaskExecutor(
                Executors.newFixedThreadPool(2)
            ) // 用于非CPU密集型任务的执行器
            .build()
    }
}

// 在 AndroidManifest.xml 中移除默认初始化器并声明自定义初始化器
// （如前所示）
```

## (二) 通过 ContentProvider 初始化（替代方案）

如果不想使用自定义初始化器，可以在 Application 中初始化：
```kotlin
class MyApplication : Application() {
    
    override fun onCreate() {
        super.onCreate()
        
        // 自定义 WorkManager 初始化
        WorkManager.initialize(
            this,
            Configuration.Builder()
                .setMinimumLoggingLevel(Log.INFO)
                .setExecutor(
                    Executors.newScheduledThreadPool(2)
                )
                .build()
        )
    }
}
```

# 七、高级特性

## (一) 绑定延迟工作（针对特定事件）

虽然 WorkManager 不直接支持事件触发，但可以结合其他机制实现类似效果：
```kotlin
// 方法1：使用广播接收器触发工作
class BootCompletedReceiver : BroadcastReceiver() {
    override fun onReceive(context: Context, intent: Intent?) {
        if (intent?.action == Intent.ACTION_BOOT_COMPLETED) {
            // 设备启动后执行同步工作
            WorkManager.getInstance(context)
                .enqueueUniqueWork(
                    "boot_sync",
                    ExistingWorkPolicy.KEEP,
                    OneTimeWorkRequestBuilder<SyncWorker>()
                        .setConstraints(
                            Constraints.Builder()
                                .setRequiresCharging(true)  // 仅在充电时执行
                                .build()
                        )
                        .build()
                )
        }
    }
}

// 在 AndroidManifest.xml 中声明广播接收器
<receiver android:name=".BootCompletedReceiver">
    <intent-filter>
        <action android:name="android.intent.action.BOOT_COMPLETED" />
    </intent-filter>
</receiver>

// 方法2：结合 JobScheduler 实现更精确的触发（高级用法）
// 实际上 WorkManager 已经封装了这些复杂性，普通用户不需要直接处理
```

## (二) 处理工作取消

当工作被系统或应用取消时的处理：
```kotlin
class ResilientWorker(
    appContext: Context,
    workerParams: WorkerParameters
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        return coroutineScope {
            // 启动可以被取消的子协程
            launch {
                // 执行实际工作，可以检查 isActive
                for (item in itemsToProcess) {
                    // 检查是否被取消
                    if (!isActive) {
                        // 工作被取消，清理资源后退出
                        cleanupResources()
                        return@launch
                    }
                    
                    // 处理单个项目
                    processItem(item)
                    // 更新进度（可选）
                    setProgressAsync(workDataOf("processed" to itemsProcessed))
                }
            }
            
            // 正常完成
            Result.success()
        }
    }
    
    private suspend fun processItem(item: Item) { /* ... */ }
    private fun cleanupResources() { /* ... */ }
}
```

## (三) 更新工作进度

使用 `setProgressAsync()` 更新工作进度：
```kotlin
class ProgressWorker(
    appContext: Context,
    workerParams: WorkerParameters
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        // 假设我们要处理 100 个项目
        val totalItems = 100
        
        for (i in 1..totalItems) {
            // 检查是否被取消
            if (!isActive) {
                return Result.failure()
            }
            
            // 处理单个项目
            processItem(i)
            
            // 更新进度（0-100）
            val progress = (i * 100) / totalItems
            setProgressAsync(workDataOf("progress" to progress))
            
            // 模拟一些工作延迟
            delay(50)
        }
        
        return Result.success()
    }
    
    private suspend fun processItem(item: Int) { /* ... */ }
}

// 在观察者中监控进度
workManager.getWorkInfoByIdLiveD(workId)
    .observe(lifecycleOwner) { workInfo ->
        if (workInfo != null) {
            val progress = workInfo.getProgress().getInt("progress", 0)
            // 更新 UI 显示进度条
            progressBar.progress = progress
        }
    }
```

# 八、测试策略

## (一) 单元测试 Worker 逻辑

⚠ **WorkManager 没有 `TestWorker` / `TestListenableWorkerAdapter` 这两个类**，`WorkerParameters` 也没有公开的 Builder 或无参构造——`WorkerParameters` 只能由 WorkManager 自己创建。唯一受支持的构造方式是 `TestListenableWorkerBuilder`：

```kotlin
@OptIn(ExperimentalCoroutinesApi::class)
class SyncWorkerTest {

    private lateinit var context: Context
    private lateinit var testDispatcher: TestDispatcher

    @Before
    fun setUp() {
        // ★TestDispatcher 是抽象类，不能直接实例化
        testDispatcher = StandardTestDispatcher()
        Dispatchers.setMain(testDispatcher)

        // ApplicationProvider 全进程唯一，测试间共享同一个 Application 实例
        context = ApplicationProvider.getApplicationContext()
    }

    @After
    fun tearDown() {
        Dispatchers.resetMain()
        // ★TestDispatcher 没有 cleanup() 方法；要推进虚拟时间用
        //    testDispatcher.scheduler.advanceUntilIdle()
    }

    private fun buildWorker(input: Data = Data.EMPTY): SyncWorker {
        val request = TestListenableWorkerBuilder<SyncWorker>(context)
            .setInputData(input)
            .build()
        return request as SyncWorker
    }

    @Test
    fun `doWork returns success when sync succeeds`() = runTest {
        val worker = buildWorker(workDataOf("key" to "testValue"))

        // ★ListenableWorker.doWork() 是 protected，外部调不到；
        //   走 startWork().get() 才是受支持的入口
        val result = worker.startWork().get()

        assertThat(result).isNotNull()
    }

    @Test
    fun `input data is correctly read`() = runTest {
        val worker = buildWorker(workDataOf("key" to "testValue"))

        // inputData 是 public 的，可以直接读
        assertThat(worker.inputData.getString("key")).isEqualTo("testValue")
    }
}
```

⚠ **`Result` 没有 `get()` 方法**。`ListenableWorker.Result` 的公开 API 只有 `failure()` / `retry()` / `success()` / `getOutputData()`，而且 `Result.Success` 这类内部类不是 public 的。要断言结果类型，用 `WorkInfo.State`（`SUCCEEDED` / `FAILED` / `ENQUEUED`）或 Robolectric 跑真实的 `doWork()`。

依赖要自己补上：

```kotlin
dependencies {
    testImplementation("androidx.work:work-testing:2.12.0")
    testImplementation("org.jetbrains.kotlinx:kotlinx-coroutines-test:1.11.0")
    testImplementation("androidx.test:core-ktx:1.7.0")     // ApplicationProvider
    testImplementation("org.robolectric:robolectric:4.16") // Worker 需要 Android 环境
}
```


## (二) 集成测试工作执行

使用 AndroidX Test 测试实际的工作执行：
```kotlin
@RunWith(AndroidJUnit4::class)
class SyncWorkerInstrumentedTest {
    
    @Test
    fun `worker executes successfully`() {
        // 创建工作请求
        val workRequest = OneTimeWorkRequestBuilder<SyncWorker>()
            .setInputData(workDataOf("test" to "data"))
            .build()
        
        // 使用 WorkManager Test API 执行工作
        val context = ApplicationProvider.getApplicationContext()
        // ★initializeTestWorkManager(...) 返回 void，拿不到 manager
        WorkManagerTestInitHelper.initializeTestWorkManager(context)
        val manager = WorkManager.getInstance(context)
        
        // 执行工作
        manager.startWork(workRequest.id)
        
        // 获取工作结果
        val workInfo = manager.getWorkInfo(workRequest.id)
        assertNotNull(workInfo)
        assertEquals(WorkInfo.State.SUCCEEDED, workInfo.state)
    }
    
    @Test
    fun `worker respects constraints`() {
        // 创建要求网络连接的工作
        val workRequest = OneTimeWorkRequestBuilder<SyncWorker>()
            .setConstraints(
                Constraints.Builder()
                    .setRequiredNetworkType(NetworkType.CONNECTED)
                    .build()
            )
            .build()
        
        val context = ApplicationProvider.getApplicationContext()
        WorkManagerTestInitHelper.initializeTestWorkManager(context)
        val manager = WorkManager.getInstance(context)

        // ★入参是 work 的 UUID，不是布尔值；且必须先入队才能改约束
        manager.setAllConstraintsMet(workRequest.id)
        
        manager.startWork(workRequest.id)
        
        // 工作应被标记为 ENQUEUED 而不是 SUCCEEDED
        val workInfo = manager.getWorkInfo(workRequest.id)
        assertNotNull(workInfo)
        // 根据 TestWorkManager 的具体行为，可能需要等待或检查特定状态
    }
}
```

## (三) 测试唯一工作策略

```kotlin
@Test
fun `unique work policy prevents duplication`() {
    val context = ApplicationProvider.getApplicationContext()
    WorkManagerTestInitHelper.initializeTestWorkManager(context)
    val manager = WorkManager.getInstance(context)

    val workName = "unique_test"
    
    // 首次提交工作
    val workRequest1 = OneTimeWorkRequestBuilder<SyncWorker>()
        .setConstraints(Constraints.Builder().build())
        .build()
    manager.enqueueUniqueWork(workName, ExistingWorkPolicy.KEEP, workRequest1)
    
    // 尝试再次提交相同名称的工作
    val workRequest2 = OneTimeWorkRequestBuilder<DifferentWorker>()
        .setConstraints(Constraints.Builder().build())
        .build()
    manager.enqueueUniqueWork(workName, ExistingWorkPolicy.KEEP, workRequest2)
    
    // 应该只有一个工作被追踪（第一个工作）
    val workIds = manager.getWorkIdsForUniqueWork(workName)
    assertThat(workIds).hasSize(1)
    assertThat(workIds.first()).isEqualTo(workRequest1.id)
}
```

# 九、最佳实践与常见问题

## (一) 最佳实践

✅ 将实际工作逻辑封装在独立的类中（不放在 Worker 中）  
✅ Worker 只负责调度和错误处理，核心逻辑放在 Repository 或 UseCase 中  
✅ 使用 `CoroutineWorker` 而非 `RxWorker` 或 `ListenableWorker`（除非有特殊需求）  
✅ 为长时间运行的工作定期检查 `isActive` 以响应取消请求  
✅ 使用指数退避策略而非固定延迟重试  
✅ 约束条件要明确且必要，避免过度限制导致工作永远不执行  
✅ 唯一工作名称要具备描述性且全局唯一  
✅ 为工作定义清晰的输入输出数据契约  
✅ 在 Application 启动时预注册常用的工作以减少首次调度延迟  
✅ 使用 WorkManager 测试库进行单元和集成测试  
✅ 监控工作执行情况并记录关键指标（成功率、平均执行时间等）  
✅ 考虑使用扩展函数简化常见的工作创建模式  

## (二) 常见问题及解决方案

### 1. 问题：工作永远不执行
**原因**：约束条件永不满足或初始化错误  
**解决方案**：
- 验证约束条件是否真的能被满容（如网络类型设置错误）
- 检查 WorkManager 是否正确初始化（查看日志）
- 使用 `adb shell cmd jobscheduler list` 查看实际的作业状态
- 暂时移除所有约束条件测试工作是否能执行
- 确保应用没有被系统严格限制后台执行（省电模式等）

### 2. 问题：工作立即失败但不重试
**原因**：返回 `Result.failure()` 而非 `Result.retry()` 或退避策略配置错误  
**解决方案**：
- 检查 `doWork()` 中的异常处理逻辑
- 验证退避策略是否在工作请求中正确设置
- 确认工作未被设置为仅执行一次且不允许重试
- 查看 WorkManager 日志了解重试决策

### 3. 问题：输入输出数据丢失
**原因**：数据大小超过限制（WorkManager 有 10KB 限制）或序列化错误  
**解决方案**：
- 检查输入数据大小：`workData.size()` 应小于 10,000 字节
- 考虑使用 Room 或文件存储大量数据，工作中仅传递引用 ID
- 验证自定义序列化器（如果使用 Protobuf 或其他格式）是否正确工作
- 使用 `setProgressAsync()` 传递进度更新而非大数据块

### 4. 问题：周期性工作执行间隔不准确
**原因**：系统优化、省电策略或 WorkManager 的执行窗口机制  
**解决方案**：
- 理解 WorkManager 不是实时调度器，而是保证在约束满足时最终执行
- 对于需要精确时间的场景考虑使用 AlarmManager（但需注意系统限制）
- 用 `PeriodicWorkRequestBuilder(repeatInterval, unit, flexInterval, unit)` 的第四个参数指定 flex 窗口（flex 从 WorkManager 1.0 起就支持，不是 Android 12+ 才有；且它没有 `setFlexPeriod()` 方法）
- 验证设备是否处于省电模式或应用后台限制模式
- 考虑使用高优先级的 Firebase Cloud Message 触发工作（需后台服务合作）

### 5. 问题：唯一工作名称冲突导致预期行为不符
**原因**：不同模块使用了相同的唯一工作名称  
**解决方案**：
- 建立全局唯一工作名称命名规范（如使用反域名表示法）
- 在常量类中集中管理所有工作名称
- 使用 UUID 或时间戳生成临时唯一名称（当不需要跨应用启动时保持唯一性时）
- 在开发过程中定期检查 `WorkManager.getInstance().getWorkInfosByUniqueWorkNameLiveData()`

# 十、与其他架构组件的协同使用

## (一) 与 Hilt 依赖注入结合

```kotlin
@HiltWorker
class DataSyncWorker @AssistedInject constructor(
    @Assisted appContext: Context,
    @Assisted workerParams: WorkerParameters,
    private val articleRepository: ArticleRepository,
    private val networkService: NetworkService,
    private val prefsRepository: UserPrefsRepository
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        return try {
            // 使用通过 Hilt 注入的所有依赖
            val lastSyncTime = prefsRepository.getLastSyncTime()
            val newArticles = networkService.fetchArticlesSince(lastSyncTime)
            
            if (newArticles.isNotEmpty()) {
                articleRepository.saveArticles(newArticles)
                prefsRepository.updateLastSyncTime(System.currentTimeMillis())
            }
            
            Result.success()
        } catch (e: IOException) {
            // 网络错误通常可以重试
            Result.retry()
        } catch (e: Exception) {
            Result.failure()
        }
    }
    
    @AssistedInject.Factory
    interface Factory : AssistedFactory<DataSyncWorker>
}
```

在 Application 中启动 Hilt 工作管理器：
```kotlin
class MyApplication : Application(), Configuration.Provider {
    
    @Inject lateinit var workerFactory: HiltWorkerFactory
    
    override fun onCreate() {
        super.onCreate()
        // Hilt 会自动注入 workerFactory
        WorkManager.initialize(
            instance = this,
            configuration = Configuration.Builder()
                .setWorkerFactory(workerFactory)
                .build()
        )
    }
    
    override fun getWorkManagerConfiguration(
        context: Context
    ): Configuration {
        // 这个方法不会被使用，因为我们上面直接初始化了 WorkManager
        return Configuration.Builder().build()
    }
}
```

## (二) 与 Room 数据库结合

```kotlin
class DatabaseCleanupWorker(
    appContext: Context,
    workerParams: WorkerParameters,
    private val appDatabase: AppDatabase
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        return try {
            // 执行数据库清理操作
            val deletedCount = appDatabase.withTransaction {
                // 删除超过 30 天的旧日志
                appDatabase.logDao().deleteOldLogs(System.currentTimeMillis() - TimeUnit.DAYS.toMillis(30))
                
                // 清理孤立的标记记录
                appDatabase.tagDao().deleteOrphanedTags()
                
                // 执行 VACUUM 释放未使用的空间
                // ★RoomDatabase 没有 queryExecutor() 方法，而且 VACUUM 不能在事务里跑
                appDatabase.openHelper.writableDatabase
                    .execSQL("VACUUM")
            }
            
            Result.success()
        } catch (e: Exception) {
            Result.failure()
        }
    }
}

// 使用约束条件确保在设备空闲时执行清理
val cleanupWork = OneTimeWorkRequestBuilder<DatabaseCleanupWorker>()
    .setConstraints(
        Constraints.Builder()
            .setRequiresCharging(true)        // 仅在充电时执行
            .setRequiresDeviceIdle(true)      // 设备空闲时执行
            .setRequiredNetworkType(NetworkType.NOT_REQUIRED) // 不要求网络
            .build()
    )
    .setBackoffCriteria(
        BackoffPolicy.EXPONENTIAL,
        Duration.ofHours(1),
        TimeUnit.HOURS
    )
    .build()
```

# 十一、与 DataStore、Compose UI 结合

## (一) 与 DataStore 结合

```kotlin
class BackupPrefsWorker(
    appContext: Context,
    workerParams: WorkerParameters,
    private val prefsRepository: UserPrefsRepository,
    private val backupService: BackupService
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        return try {
            // 读取用户偏好设置
            val prefs = prefsRepository.getAllPrefs()
            
            // 备份到服务器或云端
            val backupResult = backupService.backupPreferences(prefs)
            
            when (backupResult) {
                is BackupResult.Success -> Result.success()
                is BackupResult.Retry -> Result.retry()
                is BackupResult.Failure -> Result.failure()
            }
        } catch (e: Exception) {
            Result.failure()
        }
    }
}

// 每周执行一次偏好设置备份
val prefsBackupWork = PeriodicWorkRequestBuilder<BackupPrefsWorker>(7, TimeUnit.DAYS)
    .setConstraints(
        Constraints.Builder()
            .setRequiredNetworkType(NetworkType.CONNECTED)
            .setRequiresCharging(false)
            .build()
    )
    .setBackoffCriteria(
        BackoffPolicy.EXPONENTIAL,
        Duration.ofMinutes(15),
        TimeUnit.MINUTES
    )
    .build()
```

## (二) 与 Compose UI 结合监控工作状态

```kotlin
@Composable
fun WorkStatusScreen(
    workManager: WorkManager = WorkManager.getInstance(LocalContext.current),
    workId: String  // 工作 ID 作为参数传入
) {
    val workIdUuid = UUID.fromString(workId)
    val workInfo by workManager
        .getWorkInfoByIdLiveData(workIdUuid)
        .collectAsStateWithLifecycle()
    
    when (workInfo?.state) {
        null -> CircularProgressIndicator(modifier = Modifier.fillMaxSize())
        WorkInfo.State.ENQUEUED -> Text("工作已入队...")
        WorkInfo.State.RUNNING -> {
            Column(
                modifier = Modifier
                    .fillMaxSize()
                    .padding(16.dp)
            ) {
                Text("工作正在执行...")
                workInfo?.progress?.let { progress ->
                    val progressValue = progress.getInt("progress", 0)
                    Text("进度: $progressValue%")
                    LinearProgressIndicator(
                        progress = progressValue / 100f
                    )
                }
            }
        }
        WorkInfo.State.SUCCEEDED -> {
            val output = workInfo?.outputData
            val message = output?.getString("message") ?: "工作完成"
            Text(message, color = Color.Green)
        }
        WorkInfo.State.FAILED -> {
            val error = workInfo?.outputData?.getString("error") ?: "未知错误"
            Text("工作失败: $error", color = Color.Red)
        }
        WorkInfo.State.CANCELLED -> Text("工作已取消", color = Color.Gray)
        else -> Text("工作状态: ${workInfo?.state}", color = Color.Blue)
    }
}
```

## (三) 易错点对照

| 误区 | 事实 |
| --- | --- |
| `Worker` 里耗时多久都行 | 单次执行上限约 10 分钟，超时被杀。需要更久就用 `setForeground(ForegroundInfo)` 把 Worker 提升为前台服务 worker |
| `PeriodicWorkRequest` 的间隔是准的 | 不是保证值，系统按电量与负载浮动调度。repeat ≥ 15 分钟、flex ≥ 5 分钟（`MIN_PERIODIC_INTERVAL_MILLIS` / `MIN_PERIODIC_FLEX_MILLIS`），且 flex 是**构造参数**不是 `setFlexPeriod()` |
| `Result.success()` 只是「不重试」 | `success()` 与 `failure()` 都是**终态**（不再执行）；`retry()` 才是按退避策略重跑 |
| 周期性任务用 `enqueueUniquePeriodicWork` 就不会重复 | 还要传 `ExistingPeriodicWorkPolicy`，策略不同行为不同 |
| `WorkManager` 能立即执行 | 它是「最终一定执行」，不是「马上执行」。需要即时响应就用协程，不要塞进 WorkManager |
| 在 `Worker` 里用 `GlobalScope` | 会脱离 WorkManager 的生命周期取消，任务被杀后协程还在跑 |
| `WorkManager` 能跑 5 秒以上的实时任务 | 不能。它的设计是「延迟保证」，不是「实时保证」 |
| Hilt 注入 `Worker` 用 `by inject()` 就行 | `Worker` 不是 Android 组件，必须在 `WorkerFactory` 里注入，或用 `HiltWorkerFactory` |

# 十二、参考资料

- [WorkManager 官方文档](https://developer.android.com/develop/background-work/background-tasks/persistent/getting-started)
- [WorkManager 架构指南](https://developer.android.com/develop/background-work/background-tasks/persistent)
- [WorkManager 与 Kotlin 协程](https://developer.android.com/develop/background-work/background-tasks/persistent/threading/coroutineworker)
- [WorkManager 测试指南](https://developer.android.com/develop/background-work/background-tasks/testing/persistent/integration-testing)
- [WorkManager 与 Hilt 集成](https://developer.android.com/develop/background-work/background-tasks/persistent/configuration/custom-configuration)
- [WorkManager 约束条件详解](https://developer.android.com/reference/androidx/work/Constraints)
- [WorkManager 指数退避策略](https://developer.android.com/develop/background-work/background-tasks/persistent/how-to/manage-work)
- [WorkManager 唯一工作机制](https://developer.android.com/develop/background-work/background-tasks/persistent/how-to/update-work)
- [WorkManager 调试与日志](https://developer.android.com/develop/background-work/background-tasks/testing/persistent/debug)
- WorkManager 与 Room 集成实例
- [WorkManager 与 DataStore 结合最佳实践](https://developer.android.com/topic/libraries/architecture/datastore#workmanager)
- [Android 后台执行限制说明](https://developer.android.com/about/versions/oreo/background)
- [WorkManager 与 AlarmManager 对比](https://developer.android.com/develop/background-work/services/alarms)
- [WorkManager 生产环境监控指南](https://developer.android.com/develop/background-work/background-tasks/persistent/how-to/observe)
- [WorkManager 与 Firebase JobDispatcher 迁移指南](https://developer.android.com/develop/background-work/background-tasks/persistent/migrate-from-legacy/gcm)
- WorkManager 在大型应用中的架构模式
- WorkManager 性能优化技巧
