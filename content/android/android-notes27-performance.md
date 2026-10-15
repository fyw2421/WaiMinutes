---
title: "AndroidNotes27 : 性能优化"
weight: 28
description: "Android 性能优化全面指南：启动时间优化、布局和渲染优化、内存管理、电池续航、网络性能、GPU 渲染、存储 I/O以及性能监控与分析工具。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么需要性能优化

性能直接影响用户体验和应用成功，良好的性能可以：

- **提升用户满意度**：快速响应和流畅交互增加用户留存
- **减少资源消耗**：降低电池耗尽和数据使用量
- **提高应用商店排名**：性能是应用商店评分的重要因素
- **支持更多设备**：在低端设备上也能良好运行
- **减少崩溃和ANR**：通过优化降低异常终止率

Android 性能优化需要从多个维度进行考虑和测量。

# 二、性能测量基础

## (一) 关键性能指标

1. **启动时间（Startup Time）**
   - 冷启动：从未启动状态启动应用
   - 热启动：从后台恢复应用
   - 温启动：应用被系统杀死但未完全冷启动

2. **帧率（Frame Rate）**
   - 目标：60fps（每帧16.67ms）或90fps/120fps（高刷屏）
   - 掉帧（Jank）：帧渲染超过16.67ms

3. **内存使用（Memory Usage）**
   - 堆内存（Heap）
   - 原生内存（Native）
   - 图片缓存

4. **电池消耗（Battery Drain）**
   - 后台活动
   - 唤醒锁持续时间
   - 网络频率

5. **网络性能（Network Performance）**
   - 延迟（Latency）
   - 吞吐量（Throughput）
   - 失败率

## (二) 性能测量工具

### 1. Android Studio Profiler
- CPU Profiler：监控方法执行时间和频率
- Memory Profiler：追踪内存分配和泄漏
- Energy Profiler：估算电池消耗
- Network Profiler：监控网络请求

### 2. 命令行工具
```bash
# 启动时间测量
adb shell am start -S -W com.waiminutes.app/.MainActivity

# GPU 渲染分析
adb shell dumpsys gfxinfo com.waiminutes.app

# 内存信息
adb shell dumpsys meminfo com.waiminutes.app

# 电池历史
adb shell dumpsys batterystats --charged com.waiminutes.app
```

### 3. 第三方库
- **LeakCanary**：内存泄漏检测
- **BlockCanary**：UI 线程卡顿检测
- **MethodTrace**：方法执行时间追踪
- **Firebase Performance Monitoring**：生产环境性能监控

# 三、启动时间优化

## (一) 启动阶段分析

启动过程可以分为：
1. **进程启动**：Zygote  fork 新进程
2. **应用启动**：Application.onCreate()
3. **Activity 启动**：Activity.onCreate()
4. **界面绘制**：第一帧渲染完成

## (二) 常见优化策略

### 1. 延迟初始化
```kotlin
// 不好的做法：在 Application.onCreate() 中做重耗时操作
class MyApplication : Application() {
    override fun onCreate() {
        super.onCreate()
        // 避免在这里做网络请求、大量计算等
        initAnalytics()  // 应该延迟到需要时
        loadConfigs()    // 可以延迟或异步加载
        registerComponents() // 根据需要注册
    }
}

// 好的做法：使用延迟初始化
class MyApplication : Application() {
    
    private val analyticsManager by lazy {
        AnalyticsManager.init(this)
    }
    
    override fun onCreate() {
        super.onCreate()
        // 只做必需的初始化
        initializeCoreComponents()
    }
    
    fun getAnalyticsManager(): AnalyticsManager {
        return analyticsManager  // 第一次访问时初始化
    }
}
```

### 2. 使用 ContentProvider 进行静态初始化
```kotlin
// 初始化提供器，在 Application 之前运行
class InitProvider : ContentProvider() {
    
    override fun onCreate(): Boolean {
        // 这里可以做一些不需要 Application 上下文的初始化
        // 例如：配置 Crashlytics、初始化某些 SDK 等
        Crashlytics.init()
        return true
    }
    
    // 实现其他必要方法返回 null 或默认值
    override fun query(uri: Uri, projection: Array<String>?, selection: String?,
                      selectionArgs: Array<String>?, sortOrder: String?): Cursor? {
        return null
    }
    // ...
}
```

在 AndroidManifest.xml 中声明：
```xml
<provider
    android:name=".InitProvider"
    android:authorities="${applicationId}.init-provider"
    android:exported="false" />
```

### 3. 优化依赖注入初始化
```kotlin
// 使用 Hilt 时，考虑哪些组件真的需要立即初始化
@Module
@InstallIn(SingletonComponent::class)
object AppModule {
    
    // 只有在需要时才提供的依赖
    @Provides
    fun provideAnalyticsRepository(): AnalyticsRepository {
        return AnalyticsRepositoryImpl()  // 仅在需要分析时创建
    }
}
```

### 4. 使用延迟加载的界面
```kotlin
@Composable
fun HomeScreen(viewModel: HomeViewModel = hiltViewModel()) {
    // 只在需要时加载重内容
    val showDetails by viewModel.showDetails.collectAsStateWithLifecycle()
    
    Column {
        Text(title = viewModel.title)
        
        if (showDetails) {
            // 这个重组件只有在需要时才会被加载
            HeavyDetailContent()
        }
    }
}
```

# 四、布局和渲染优化

## (一) 常见布局性能问题

1. **布局层级过深**
   - 每增加一层布局，都需要更多的测量和布局计算
   - 深层嵌套会显著增加布局时间

2. **过度绘制（Overdraw）**
   - 同一个像素被绘制多次
   - 浪费 GPU 带宽和电池

3. **频繁布局抖动（Layout Thrashing）**
   - 反复触发布局重新计算

## (二) Compose 下的优化手段

> 本节针对 Compose。View 时代的等价手段（`ConstraintLayout` 扁平化、`ViewStub` 延迟 inflate、`RecyclerView` + `DiffUtil`）在本笔记里不再展开——View 工具链已进入维护模式，新项目用不到；维护存量代码时可在 [官方 View 性能文档](https://developer.android.com/topic/performance/vitals) 里查对应写法。

### 1. 用扁平组合替代嵌套

Compose 没有「嵌套层级」的开销问题——它是单趟测量的扁平树，所以不需要 `ConstraintLayout` 来扁平化。真正的开销在**参数化重组**：

```kotlin
// ❌ 把整个 User 传下去：User 里任何一个字段变了，
//    所有接收到它的子组件都重组，哪怕它只用到 name
Column {
    Header(user)
    Text(user.name)
    FollowButton(user.following) { onFollow() }
}

// ✅ 只传用到的字段：改 email 不影响 Text，改 following 不影响 Header
Column {
    Header(user.name, user.avatarUrl)
    Text(user.name)
    FollowButton(
        following = user.following,
        onFollow = { onFollow() }        // lambda 字面量，编译器会 remember
    )
}
```

⚠ **「传得越少，重组越少」是有代价的**：字段拆多了，组件签名变长、调用处变啰嗦。经验做法是**按使用边界拆**——`Header` 只关心展示字段就给展示字段，实在需要整个对象（比如要调它的方法）再传对象。

⚠ **真正会拖累重组的是「新建的对象」，不是 lambda 本身**。`Modifier.clickable { onClick() }` 里的 lambda 字面量会被 Compose 编译器 `remember` 住，跨重组是同一个实例，所以它是**稳定**的、可以用来跳过重组。危险的是这些：

```kotlin
// ❌ 每次重组都新建 lambda 变量 → 引用身份变了 → 子组件被迫重组
val handler = { doSomething() }
Child(onClick = handler)

// ❌ 在重组中 new 对象
Child(style = TextStyle(fontSize = 16.sp))    // 每帧新实例

// ✅ 直接写 lambda 字面量：编译器会 remember 它
Child(onClick = { doSomething() })

// ✅ 确实需要非稳定对象时，用 remember 固定住
val style = remember { TextStyle(fontSize = 16.sp) }
Child(style = style)
```

### 2. 用组合条件延迟构建

Compose 版的 `ViewStub`——不进入组合就不执行，更不需要 inflate：

```kotlin
// 面板默认不参与组合，展开时才构建
AnimatedVisibility(visible = showImportPanel) {
    ImportPanel(onConfirm = { /* ... */ })
}
```

比 `ViewStub` 更彻底：没有任何布局文件，也没有 `findViewById`。

### 3. 减少过度绘制

```kotlin
// ✅ Surface 自身提供背景色，父容器不必再画一层
Surface(color = MaterialTheme.colorScheme.surface) {
    Column {
        // 这里不要再给每一层都设 background
    }
}
```

开发者选项里的 **「调试 GPU 过绘」** 同样适用于 Compose（它工作在渲染管线层，与 UI 框架无关）。看到大片红色区域就说明有容器在重复刷背景。

### 4. 长列表一律用 Lazy 系列

```kotlin
// ❌ 一千个 item 全部 measure
Column { items.forEach { Item(it) } }

// ✅ 只 measure 可见项
LazyColumn { items(items, key = { it.id }) { Item(it) } }
```

Compose 里没有 `ListView` / `RecyclerView` 的选择题——`LazyColumn` 就是唯一答案。要做的是给它稳定的 `key`（第 5 篇）。

### 5. 把状态读取推迟到该读的地方

滚动位置每帧都在变。如果让整棵子树都读它，就等于每帧重组一次。

```kotlin
// ❌ Header 读了 scrollOffset —— 它每帧都变，于是整层每帧重组
val scrollState = rememberScrollState()
val scrollOffset = scrollState.value          // ← 读一次就把整个函数标成依赖

Column(Modifier.verticalScroll(scrollState)) {
    Header(scrollOffset)                    // 每帧重组
    ArticleList()                           // 被连坐，虽然它自己没读
}

// ✅ 只让真正要用偏移的那一个元素读它
Column(Modifier.verticalScroll(scrollState)) {
    Header()                                 // 不读偏移，不重组
    ArticleList()

    // 读取发生在绘制阶段，绘制不触发重组
    TopFade(
        alpha = { 1f - scrollState.value / 400f },
        modifier = Modifier.align(Alignment.TopCenter)
    )
}

@Composable
fun TopFade(alpha: () -> Float, modifier: Modifier = Modifier) {
    Box(modifier.drawWithContent {
        drawContent()                        // 先画正常内容
        drawRect(
            color = Color.Black.copy(alpha = alpha()),   // 再叠一层渐隐
            topLeft = Offset.Zero,
            size = Size(size.width, 60.dp.toPx())
        )
    })
}
```

⚠ **`derivedStateOf` 解决的是另一个问题**——它用于「依赖比输出密」（如滚动位置每帧变，但只有跨过每 10 条才需要更新埋点），此时用 `derivedStateOf` 折叠中间值：

```kotlin
// 滚动位置每帧变，但只按「每 10 条」分档通知外部
val reportIndex by remember {
    derivedStateOf { listState.firstVisibleItemIndex / 10 }
}
LaunchedEffect(reportIndex) { analytics.scrollDepth(reportIndex) }
```

两者不冲突：**上面的 `alpha = { … }` 是把读取推迟到绘制阶段**，这里的 `derivedStateOf` 是把高频依赖折叠成低频输出。


# 五、内存管理优化

## (一) 内存泄漏常见原因

1. **静态变量持有视图引用**
   ```kotlin
   // 错误示例
   class MyActivity : Activity() {
       companion object {
           // 错误：静态变量持有 Activity 引用
           var sInstance: MyActivity? = null
       }
       
       override fun onCreate(savedInstanceState: Bundle?) {
           super.onCreate(savedInstanceState)
           sInstance = this  // 泄漏！
       }
   }
   ```

2. **未注销的回调或监听器**
   ```kotlin
   // 错误示例
   class MyActivity : Activity() {
       private val sensorListener = SensorEventListener { /* ... */ }
       
       override fun onStart() {
           super.onStart()
           SensorManager.getDefault().registerListener(sensorListener, /* ... */)
           // 忘记在 onStop() 中注销
       }
   }
   ```

3. **内部类持有外部类引用**
   ```kotlin
   // 错误示例
   class MyActivity : Activity() {
       private inner class MyTask : AsyncTask<Void, Void, Void>() {
           // 隐式持有 MyActivity 的引用
           override fun doInBackground(vararg params: Void): Void? {
               // 长时间运行的任务
               Thread.sleep(5000)
               return null
           }
       }
       
       override fun onCreate(savedInstanceState: Bundle?) {
           super.onCreate(savedInstanceState)
           MyTask().execute()  // 即使 Activity 销毁，任务仍持有引用
       }
   }
   ```

## (二) 内存优化策略

### 1. 不要用 WeakReference「防泄漏」

```kotlin
// ❌ 网上流传的「防泄漏」写法，实为反模式
class MyHandler(handler: Handler) : Handler(handler.looper) {
    private val externalRef: WeakReference<MyActivity> = WeakReference(external)

    override fun handleMessage(msg: Message) {
        val activity = externalRef.get()      // 随时可能为 null
        activity?.let { /* ... */ }
    }
}
```

⚠ **弱引用会让对象「本该回收却没回收」时被提前回收，而不是延后回收**。它解决不了真正的泄漏，只会制造「昨天还能用今天变 null」的随机崩溃。GC 时机不确定，弱引用的存活期同样不确定——这正是问题所在。

**内存泄漏的正解是切断引用链，不是把引用包一层**：

```kotlin
// ✅ Compose：用 remember + DisposableEffect 让资源的生命周期跟着组合走
@Composable
fun SensorObserver(onChanged: () -> Unit) {
    val context = LocalContext.current

    DisposableEffect(context) {
        val sm = context.getSystemService(Context.SENSOR_SERVICE) as SensorManager
        val listener = SensorEventListener { _, _, _ -> onChanged() }
        sm.registerListener(listener, SensorManager.SENSOR_STEP_COUNTER)

        onDispose {                              // ★离开组合必定注销
            sm.unregisterListener(listener)
        }
    }
}
```

**判断标准**：如果你需要 `WeakReference` 才能让对象回收，那说明**有人本来就该释放它而没释放**。找出那个地方。

### 2. 正确注销资源
```kotlin
class MyActivity : AppCompatActivity() {
    
    private var sensorManager: SensorManager? = null
    private var sensorListener: SensorEventListener? = null
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        sensorManager = getSystemService(Context.SENSOR_SERVICE) as SensorManager
        sensorListener = SensorEventListener { /* ... */ }
    }
    
    override fun onStart() {
        super.onStart()
        sensorListener?.let { 
            sensorManager?.registerListener(it, /* ... */)
        }
    }
    
    override fun onStop() {
        super.onStop()
        sensorListener?.let {
            sensorManager?.unregisterListener(it)
        }
    }
}
```

### 3. 使用静态内部类或匿名对象
```kotlin
// 改进：使用静态内部类
class MyActivity : Activity() {
    private static class MyTask : AsyncTask<Void, Void, Void>() {
        // 不再持有外部类引用
        override fun doInBackground(vararg params: Void): Void? {
            // 需要通过构造函数或其他方式传递所需引用
            return null
        }
    }
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // 如果需要 Activity 引用，通过构造函数传递
        MyTask(this).execute()
    }
}
```

### 4. 优化 Bitmap 使用
```kotlin
// 采样大图片以减少内存使用
fun decodeSampledBitmapFromResource(
    res: Resources,
    resId: Int,
    reqWidth: Int,
    reqHeight: Int
): Bitmap {
    // 第一次解析获取尺寸但不分配内存
    val options = BitmapFactory.Options().apply {
        inJustDecodeBounds = true
    }
    BitmapFactory.decodeResource(res, resId, options)
    
    // 计算采样率
    options.inSampleSize = calculateInSampleSize(options, reqWidth, reqHeight)
    
    // 真正解析 bitmap
    options.inJustDecodeBounds = false
    return BitmapFactory.decodeResource(res, resId, options)
}

private fun calculateInSampleSize(options: BitmapFactory.Options, reqWidth: Int, reqHeight: Int): Int {
    val height = options.outHeight
    val width = options.outWidth
    
    var inSampleSize = 1
    if (height > reqHeight || width > reqWidth) {
        val halfHeight = height / 2
        val halfWidth = width / 2
        
        // 计算最大的采样率，作为2的幂
        while ((halfHeight / inSampleSize) >= reqHeight &&
               (halfWidth / inSampleSize) >= reqWidth) {
            inSampleSize *= 2
        }
    }
    return inSampleSize
}

// 使用示例
val bitmap = decodeSampledBitmapFromResource(resources, R.id.my_image, 100, 100)
imageView.setImageBitmap(bitmap)
```

### 5. 使用 LruCache 缓存位图
```kotlin
class BitmapCache(
    private val maxSize: Int = 10 * 1024 * 1024  // 10MB
) {
    
    private val cache: LruCache<String, Bitmap> = LruCache(maxSize) { _, bitmap ->
        // 以字节为单位衡量缓存大小
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.KITKAT) {
            bitmap.allocationByteCount()
        } else {
            bitmap.rowBytes() * bitmap.height()
        }
    }
    
    fun put(key: String, bitmap: Bitmap) {
        cache.put(key, bitmap)
    }
    
    fun get(key: String): Bitmap? {
        return cache.get(key)
    }
    
    fun remove(key: String) {
        cache.remove(key)
    }
    
    fun evictAll() {
        cache.evictAll()
    }
}
```

# 六、电池续航优化

## (一) 耗电的常见来源

1. **唤醒锁（Wake Locks）**
   - 防止设备进入睡眠状态
   - 持续时间过长会显著耗电

2. **后台服务**
   - 不必要的后台运行
   - 频繁启动/停止

3. **定位服务**
   - 高精度定位（GPS）耗电大
   - 频繁请求位置更新

4. **网络操作**
   - 频繁的网络请求
   - 保持长连接

5. **传感器使用**
   - 持续注册传感器监听器
   - 高频率传感器数据

## (二) 优化策略

### 1. 正确使用唤醒锁
```kotlin
// 好的做法：只在必要时获取，并且尽快释放
class MyService : Service() {
    
    private var wakeLock: PowerManager.WakeLock? = null
    
    fun doWork() {
        val powerManager = getSystemService(POWER_SERVICE) as PowerManager
        wakeLock = powerManager.newWakeLock(
            PowerManager.PARTIAL_WAKE_LOCK,
            "MyService::WakecockTag"
        ).apply {
            acquire(10*60*1000L) // 10分钟超时，防止忘记释放
        }
        
        try {
            // 执行不允许睡眠的工作
            doCriticalWork()
        } finally {
            wakeLock?.release()
            wakeLock = null
        }
    }
}

// 更好的做法：使用 WorkManager 而非自行管理唤醒锁
```

### 2. 优化后台执行
```kotlin
// 使用 WorkManager 处理后台任务
class MyWorker(
    appContext: Context,
    workerParams: WorkerParameters
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        // 执行后台工作
        return try {
            doBackgroundWork()
            Result.success()
        } catch (e: Exception) {
            Result.failure()
        }
    }
}

// 设置约束条件以节省电量
val constraints = Constraints.Builder()
    .setRequiredNetworkType(NetworkType.UNMETERED)  // 仅在非计费网络
    .setRequiresCharging(true)                      // 仅在充电时
    .setRequiresDeviceIdle(true)                    // 仅在设备空闲时
    .build()

val workRequest = OneTimeWorkRequestBuilder<MyWorker>()
    .setConstraints(constraints)
    .build()

WorkManager.getInstance(context).enqueue(workRequest)
```

### 3. 优化定位使用
```kotlin
// 使用合适的定位精度
val locationRequest = LocationRequest.create().apply {
    interval = 10_000L  // 10秒更新一次
    fastestInterval = 5_000L  // 最快5秒
    priority = when {
        isInForeground() -> LocationRequest.PRIORITY_HIGH_ACCURACY
        isBatteryLow() -> LocationRequest.PRIORITY_LOW_POWER
        else -> LocationRequest.PRIORITY_BALANCED_POWER_ACCURACY
    }
}

// 使用 fused location provider 而非直接 GPS
val fusedLocationClient = LocationServices.getFusedLocationProviderClient(this)

// 只在需要时请求更新
fun startLocationUpdates() {
    fusedLocationClient.requestLocationUpdates(
        locationRequest,
        locationCallback,
        Looper.getMainLooper()
    )
}

fun stopLocationUpdates() {
    fusedLocationClient.removeLocationUpdates(locationCallback)
}
```

### 4. 批量网络操作
```kotlin
// 好的做法：批量请求而非频繁小请求
class ApiRepository {
    
    // 一次获取多条数据
    suspend fun fetchBatchItems(start: Int, limit: Int): List<Item> {
        return api.getItems(start, limit)
    }
    
    // 避免这种做法：
    // suspend fun fetchItemsOneByOne(count: Int): List<Item> {
    //     val items = mutableListOf<Item>()
    //     for (i in 0 until count) {
    //         items.add(api.getItem(i))  // 网络请求！
    //     }
    //     return items
    // }
}

// 使用增量更新或推送而非轮询
class ItemRepository {
    
    // 使用 Firebase Cloud Messaging 或类似服务推送更新
    // 而非客户端轮询服务器
    
    private val fcmlListener: FirebaseMessagingService.() -> Unit = {
        // 处理推送消息
        handleRemoteMessage(it)
    }
}
```

# 七、网络性能优化

## (一) 减少网络开销

1. **启用 HTTP/2**
   ```kotlin
   // OkHttp 默认支持 HTTP/2 当服务器支持时
   val client = OkHttpClient.Builder()
       .protocols(listOf(Protocol.HTTP_2, Protocol.HTTP_1_1))
       .build()
   ```

2. **启用压缩**
   ```kotlin
   // OkHttp 自动处理 GZIP 压缩响应
   // 但可以主动请求压缩
   val request = Request.Builder()
       .url("https://api.example.com/data")
       .header("Accept-Encoding", "gzip")
       .build()
   ```

3. **减少请求大小**
   ```kotlin
   // 使用 ProtoBuf 而非 JSON 减小 payload
   // 或者使用响应过滤只返回必要字段
   
   // 好的做法：指定只需要的字段
   @GET("users/{id}")
   suspend fun getUser(
       @Path("id") id: String,
       @Query("fields") fields: String = "id,name,email"  // 只请求需要的字段
   ): UserResponse
   ```

4. **缓存策略**
   ```kotlin
   // 使用合适的缓存头
   // 服务器应返回：
   // Cache-Control: max-age=3600
   // 或者：
   // Cache-Control: no-cache, must-revalidate
   // ETag: "some-etag-value"
   
   // 客户端可以使用 OkHttp 的缓存
   val cache = Cache(directory, 10 * 1024 * 1024)  // 10MB 缓存
   
   val client = OkHttpClient.Builder()
       .cache(cache)
       .build()
   ```

## (二) 优化图片加载

1. **使用合适的图片格式**
   - WebP：通常比 JPEG/PNG 小 25-35%
   - SVG：对于图标和简单图形
   - AVIF：新格式，压缩率更高（但支持度有限）

2. **适当压缩质量**
   ```kotlin
   // 使用 Glide 或 Coil 时可以设置质量
   Glide.with(context)
       .load(imageUrl)
       .override(800, 600)  // 调整到合适尺寸
       .format(DecodeFormat.PREFER_RGB_565)  // 减少每像素字节数
       .diskCacheStrategy(DiskCacheStrategy.ALL)
       .into(imageView)
   ```

3. **使用模糊占位符**
   ```kotlin
   // 先加载低质量模糊图片，然后渐变到高质量
   Coil.with(context)
       .load(imageUrl)
       .crossfade(true)
       .placeholder(R.drawable.blur_placeholder)
       .error(R.drawable.error_placeholder)
       .into(imageView)
   ```

4. **实现图片缓存策略**
   ```kotlin
   // 三级缓存策略：
   // 1. 内存缓存（快速访问）
   // 2. 磁盘缓存（持久存储）
   // 3. 网络（最慢）
   
   // 使用 Glide/Coil 已经实现了这种策略
   ```

# 八、GPU 渲染优化

## (一) 渲染流程优化

1. **减少布局 pass 次数**
   - 避免在 onDraw() 中触发布局请求
   - 使用 post() 延迟执行布局操作

2. **优化自定义 View 的 onDraw()**
   ```kotlin
   // 不好的做法：在 onDraw() 中创建对象
   class MyView : View {
       @Override
       protected void onDraw(Canvas canvas) {
           // 错误：每帧都创建新对象导致 GC 抖动
           Paint paint = new Paint(); 
           paint.setColor(Color.RED);
           canvas.drawRect(0, 0, 100, 100, paint);
       }
   }
   
   // 好的做法：提前创建对象
   class MyView : View {
       private val paint = Paint().apply {
           color = Color.RED
           isAntiAlias = true
       }
       
       @Override
       protected void onDraw(Canvas canvas) {
           super.onDraw(canvas)
           canvas.drawRect(0, 0, 100, 100, paint)
       }
   }
   ```

3. **使用硬件加速**
   ```xml
   <!-- 在 AndroidManifest.xml 中 -->
   <application
       android:hardwareAccelerated="true"
       ... >
       ...
   </application>
   
   <!-- 或者为特定 Activity 禁用（罕见需要） -->
   <activity
       android:name=".MyActivity"
       android:hardwareAccelerated="false" />
   ```

## (二) 减少不必要的重绘

1. **使用 invalidate() 的精确区域**
   ```kotlin
   // 不好：重绘整个视图
   invalidate()
   
   // 好：只重绘需要更新的区域
   invalidate(left, top, right, bottom)
   ```

2. **使用 ViewPropertyAnimator 动画**
   ```kotlin
   // 好：使用属性动画由 GPU 加速处理
   view.animate()
       .translationX(100f)
       .setDuration(300)
       .start()
   
   // 不好：使用帧动画或手动更新位置导致频繁重绘
   ```

# 九、存储 I/O 优化

## (一) 减少磁盘操作

1. **批量写入**
   ```kotlin
   // 不好：频繁小写入
   fun saveItemsBad(items: List<Item>) {
       for (item in items) {
           // 每次打开文件、写入、关闭
           FileOutputStream(file).use { out ->
               out.write(item.toByteArray())
           }
       }
   }
   
   // 好：批量写入
   fun saveItemsGood(items: List<Item>) {
       FileOutputStream(file).use { out ->
           for (item in items) {
               out.write(item.toByteArray())
           }
       }
   }
   ```

2. **使用缓冲流**
   ```kotlin
   // 好：使用缓冲流减少系统调用
   FileOutputStream(file).buffered().use { out ->
       out.write(data)
   }
   ```

3. **合适的刷新策略**
   ```kotlin
   // 不必要时避免刷新
   FileOutputStream(file).use { out ->
       out.write(data)
       // 只在必要时刷新
       if (needsImmediatePersistence) {
           out.flush()
       }
   }
   ```

## (二) 选择合适的存储方案

1. **SharedPreferences / DataStore**
   - 适用于小型键值对数据
   - 频繁写入会导致性能问题

2. **内部存储文件**
   - 适用于中等大小的数据
   - 支持顺序读写和随机访问

3. **SQLite / Room**
   - 适用于结构化数据和复杂查询
   - 事务操作要使用批处理

4. **Room 事务优化**
   ```kotlin
   // 好的做法：批量操作放在同一个事务中
   @Dao
   interface ItemDao {
       @Transaction
       fun insertItems(items: List<Item>) {
           // 所有插入操作在同一个事务中
           for (item in items) {
               insertItem(item)
           }
       }
       
       @Insert
       suspend fun insertItem(item: Item): Long
   }
   ```

# 十、性能监控与分析

## (一) 建立性能基线

1. **定义关键指标**
   - 启动时间（冷启动、热启动）
   - 关键路径帧率（如列表滚动、动画播放）
   - 内存峰值使用
   - 电池消耗率（每小时百分比）

2. **设置性能预算**
   - 启动时间 < 2秒（冷启动）
   - 帧率稳定在 60fps（掉帧 < 1%）
   - 内存使用 < 150MB
   - 后台电池消耗 < 2%/小时

## (二) 生产环境监控

1. **Firebase Performance Monitoring**
   ```kotlin
   // 自动追踪：
   // - 应用启动时间
   // - 网络请求
   // - 屏幕渲染
   
   // 自定义追踪
   val trace = FirebasePerformance.getInstance().newTrace("image_loading_trace")
   trace.start()
   // 加载图片...
   trace.stop()
   ```

2. **自定义性能埋点**
   ```kotlin
   object PerformanceTracker {
        
        private const val PREF_FILE = "perf_metrics"
        
        fun recordStartupTime(context: Context, timeMs: Long) {
            val prefs = context.getSharedPreferences(PREF_FILE, Context.MODE_PRIVATE)
            prefs.edit()
                .putLong("startup_cold_${Build.VERSION.SDK_INT}", timeMs)
                .apply()
        }
        
        fun recordFrameDrop(context: Context, frameTimeMs: Long) {
            val prefs = context.getSharedPreferences(PREF_FILE, Context.MODE_PRIVATE)
            val key = "frame_drops_${frameTimeMs}ms"
            val current = prefs.getInt(key, 0)
            prefs.edit()
                .putInt(key, current + 1)
                .apply()
        }
        
        fun getMetrics(context: Context): Map<String, Long> {
            val prefs = context.getSharedPreferences(PREF_FILE, Context.MODE_PRIVATE)
            return prefs.all.map { (k, v) -> k to (v as Long) }.toMap()
        }
   }
   ```

## (三) 性能回归测试

1. **在 CI 中添加性能检查**
   ```yaml
   # GitHub Actions 示例
   - name: Run Performance Tests
     run: ./gradlew connectedAndroidTest -Pandroid.testInstrumentationRunnerArguments=perf
   ```

2. **建立性能基线版本**
   - 为每个版本记录性能指标
   - 比较与基线版本的性能变化
   - 设置性能退出阈值（如启动时间增加20%则失败）

# 十一、最佳实践与常见问题

## (一) 最佳实践

✅ 从测量开始：不要猜测，先测量后优化  
✅ 关注用户可感知的性能：启动时间、帧率、响应时间  
✅ 在真实设备上测试：模拟器性能特征与真实设备不同  
✅ 考虑冷启动和热启动场景  
✅ 使用宏基准测试（Macrobenchmark）进行重复测量  
✅ 优化主要瓶颈而非次要问题  
✅ 考虑电量和性能的权衡  
✅ 定期审查性能指标而非仅仅在出问题时  
✅ 为性能优化设置可测量的目标  
✅ 使用 A/B 测试验证优化效果  
✅ 文档优化决策和测量结果  

## (二) 常见问题及解决方案

### 1. 问题：优化后没有明显改善
**原因**：优化了非瓶颈位置或测量方法不当  
**解决方案**：
- 使用性能分析工具确认真正的瓶颈在哪里
- 确保在真实用户场景下测量而非理想条件
- 考虑使用对照组进行 A/B 测试验证效果
- 检查是否存在平台限制或硬件瓶颈

### 2. 问题：内存优化导致崩溃
**原因**：过度释放资源或错误的生命周期管理  
**解决方案**：
- 使用 LeakCanary 检测是否真的解决了泄漏
- 确保资源释放时机正确（如在 onDestroy 中）
- 检查是否有其他地方仍在引用该资源
- 考虑使用弱引用而非强制置空

### 3. 问题：启动时间优化复杂化了代码
**原因**：过度使用延迟初始化导致代码难以理解  
**解决方案**：
- 只对真正影响启动时间的代码进行优化
- 保持优化后的代码可读性和可维护性
- 考虑使用依赖注入框架的特性（如懒加载）
- 添加清晰的注释解释为什么需要这样做

### 4. 问题：过度优化导致可维护性下降
**原因**：为微小的性能提升牺牲了代码清晰度  
**解决方案**：
- 遵守90/10法则：90%的时间花在10%的代码上
- 设定性能目标：达到目标后停止优化
- 考虑使用性能预算而非极致优化
- 记住：可维护性往往比微小的性能提升更重要

## (三) 易错点对照

| 误区 | 事实 |
| --- | --- |
| Compose 页面一定比 XML 页面快 | 不一定。Compose 的开销在组合与重组，盲目嵌套与频繁重组照样掉帧 |
| Compose 里嵌套 `Column` 会像 `LinearLayout` 那样重复测量 | 不会。Compose 是单趟测量的扁平树，嵌套不带来额外遍历。真正的开销是**参数化重组**，用 lambda 修饰符隔离 |
| `AsyncTask` 是处理后台任务的好选择 | 已废弃。用 `viewModelScope` + 协程，或 WorkManager |
| 只要没 ANR 就说明性能没问题 | ANR 是**已经卡死**的信号。可接受的标准是 60fps 下掉帧率 < 5% |
| 内存占用高就一定有泄漏 | 不一定。泄漏是「本该回收却仍被引用」，占用高可能只是缓存策略激进 |
| 优化前不需要先测量 | 必须先测量。没有基线的优化只是换一种写法，无法证明有效 |
| 缓存越大命中率越高 | 缓存有内存上限，超限会触发 LRU 逐出，反而降低命中率。需按实测调整 |
| `BuildConfig.DEBUG` 判断足以跳过 release 逻辑 | 不够。它在 release 构建下为 false，但 ProGuard 仍会解析分支；敏感代码要靠 R8 规则真正剥掉 |

# 十二、参考资料

- [Android 性能官方文档](https://developer.android.com/topic/performance)
- [Android 性能模式](https://developer.android.com/topic/performance/vitals)
- [启动时间优化](https://developer.android.com/topic/performance/vitals/launch-time)
- [渲染性能](https://developer.android.com/topic/performance/rendering)
- [电池续航优化](https://developer.android.com/topic/performance/background-optimization)
- [内存管理官方指南](https://developer.android.com/topic/performance/memory-overview)
- 宏基准测试指南
- [Firebase Performance Monitoring](https://firebase.google.com/docs/perf-mon)
- [Profile GPU Rendering 工具](https://developer.android.com/topic/performance/rendering)
- [Systrace 性能分析工具](https://developer.android.com/tools/trace)
- [Memory Profiler 使用指南](https://developer.android.com/studio/profile/memory-profiler)
- [Network Profiler 使用指南](https://developer.android.com/studio/profile/network-profiler)
- [Jetpack Macrobenchmark 库](https://developer.android.com/jetpack/androidx/releases/macrobenchmark)
- [Android Vitals 术语表](https://support.google.com/googleplay/android-developer/answer/6112435)
- 性能优化案例研究
- [布局性能优化指南](https://developer.android.com/training/improving-layouts)
- [过度绘制检测与优化](https://developer.android.com/topic/performance/rendering)
- [使用 StrictMode 检测性能问题](https://developer.android.com/reference/android/os/StrictMode)
- [TextureView 与 SurfaceView 性能比较](https://developer.android.com/reference/android/view/TextureView)
- ViewStub 与 ViewPager 性能影响
- [使用 Profile Guard 检测性能回归](https://developer.android.com/studio/profile)
