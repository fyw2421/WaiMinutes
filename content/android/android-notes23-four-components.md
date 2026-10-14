---
title: "AndroidNotes23 : 四大组件"
weight: 23
description: "Android 四大组件（Activity、Service、BroadcastReceiver、ContentProvider）的完整用法：生命周期管理、启动模式、任务返回栈、后台服务类型、广播注册与过滤、数据共享与URI权限以及与现代架构的协同使用。"
featureimage: "covers/android-system.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么需要理解四大组件

四大组件是 Android 应用的基本构建块，它们提供：

- **Activity**：用户交互的入口和界面承载者
- **Service**：后台长运行操作，无界面
- **BroadcastReceiver**：系统和应用事件的监听者
- **ContentProvider**：应用间数据共享的标准方式

理解它们的工作机制是构建健壮 Android 应用的基础，即使在采用现代架构（如单活动架构、Jetpack Compose）时，也需要了解它们的行为以处理特殊场景和系统交互。

# 二、Activity 详解

## (一) 生命周期

Activity 有七个核心生命周期方法：
```kotlin
class MainActivity : AppCompatActivity() {
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // 创建 Activity，初始化界面和数据
    }
    
    override fun onStart() {
        super.onStart()
        // Activity 变得可见，但可能不在前台
    }
    
    override fun onResume() {
        super.onResume()
        // Activity 在前台并可交互
    }
    
    override fun onPause() {
        super.onPause()
        // Activity 即将失去焦口，保存临时状态
    }
    
    override fun onStop() {
        super.onStop()
        // Activity 完全不可见，释放资源
    }
    
    override fun onDestroy() {
        super.onDestroy()
        // Activity 被销毁，进行最终清理
    }
    
    override fun onRestart() {
        super.onRestart()
        // Activity 从停止状态重新启动
    }
}
```

## (二) 启动模式

在 `AndroidManifest.xml` 中通过 `launchMode` 属性控制：
```xml
<activity
    android:name=".MainActivity"
    android:launchMode="standard" /> <!-- 默认模式 -->
```

四种启动模式：
- **standard**（默认）：每次启动创建新实例
- **singleTop**：如果已在栈顶，则复用实例（通过 `onNewIntent`）
- **singleTask**：栈内只有一个实例，若存在则将其置于栈顶并清除其上的所有活动
- **singleInstance**：唯一实例，占用单独的任务栈

## (三) 任务返回栈与 Intent

使用 Intent 启动 Activity：
```kotlin
// 显式 Intent
val intent = Intent(this, DetailActivity::class.java)
intent.putExtra("article_id", 123)
startActivity(intent)

// 隐式 Intent（需要匹配 Intent Filter）
val intent = Intent(Intent.ACTION_VIEW).apply {
    data = Uri.parse("https://example.com/articles/123")
}
startActivity(intent)

// 获取结果（使用 Activity Result API）
private val getContent = registerForActivityResult(
    ActivityResultContracts.GetContent()
) { uri: Uri? ->
    // 处理返回的 URI
}

// 启动获取内容
getContent.launch("image/*")
```

## (四) 状态保存与恢复

处理配置变更（如屏幕旋转）：
```kotlin
class MainActivity : AppCompatActivity() {
    
    private var draftText: String? = null
    
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        // 恢复之前保存的状态
        draftText = savedInstanceState?.getString("DRAFT_TEXT")
    }
    
    override fun onSaveInstanceState(outState: Bundle) {
        super.onSaveInstanceState(outState)
        // 保存临时状态
        outState.putString("DRAFT_TEXT", draftText)
    }
    
    // 或者使用 ViewModel 和 SavedStateHandle（推荐）
    class MainViewModel(
        savedStateHandle: SavedStateHandle
    ) : ViewModel() {
        val draftText: MutableStateFlow<String?> = 
            savedStateHandle.getLiveData<String>("DRAFT_TEXT")
                .asStateFlow()
    }
}
```

# 三、Service 详解

## (一) 服务类型

1. **前台服务（Foreground Service）**
   - 必须显示通知
   - 适合用户能感知的后台操作（如音乐播放、文件下载）
   ```kotlin
   @AndroidEntryPoint
   class MusicService : Service() {
       
       private val notificationManager: NotificationManager by inject()
       
       override fun onCreate() {
           super.onCreate()
           startForeground(
               NOTIFICATION_ID,
               createNotification()
           )
       }
       
       private fun createNotification(): Notification {
           return NotificationCompat.Builder(this, CHANNEL_ID)
               .setContentTitle("音乐播放")
               .setContentText("正在播放...")
               .setSmallIcon(R.drawable.ic_music_note)
               .setContentIntent(pendingIntent)
               .build()
       }
   }
   ```

2. **后台服务（Background Service）**
   - Android 8.0+ 受限制
   - 需要满足特定条件或使用 JobIntentService/WorkManager
   ```kotlin
   // 使用 JobIntentService（已过时，推荐 WorkManager）
   class UploadService : JobIntentService() {
       
       companion object {
           private const val JOB_ID = 1000
           
           fun enqueueWork(context: Context, work: Intent) {
               enqueueWork(context, UploadService::class.java, JOB_ID, work)
           }
       }
       
       override fun onHandleWork(intent: Intent) {
           // 执行后台工作
           uploadFile(intent.getStringExtra("FILE_PATH"))
       }
   }
   ```

3. **绑定服务（Bound Service）**
   - 允许组件绑定并交互
   ```kotlin
   class LocalService : Service() {
       
       private val binder = LocalBinder()
       
       inner class LocalBinder : Binder() {
           fun getService(): LocalService = this@LocalService
       }
       
       override fun onBind(intent: Intent?): IBinder {
           return binder
       }
       
       fun getRandomNumber(): Int {
           return Random.nextInt(100)
       }
   }
   
   // 在 Activity 中绑定
   class MainActivity : AppCompatActivity() {
       
       private var service: LocalService? = null
       private val mConnection = object : ServiceConnection {
           
           override fun onServiceConnected(className: ComponentName, service: IBinder) {
               val binder = service as LocalService.LocalBinder
               this.service = binder.getService()
           }
           
           override fun onServiceDisconnected(className: ComponentName) {
               this.service = null
           }
       }
       
       override fun onStart() {
           super.onStart()
           Intent(this, LocalService::class.java).also { intent ->
               bindService(intent, mConnection, Context.BIND_AUTO_CREATE)
           }
       }
       
       override fun onStop() {
           super.onStop()
           unbindService(mConnection)
       }
   }
   ```

# 四、BroadcastReceiver 详解

## (一) 注册方式

1. **静态注册（在 AndroidManifest.xml 中）**
   ```xml
   <receiver android:name=".BootCompletedReceiver">
       <intent-filter>
           <action android:name="android.intent.action.BOOT_COMPLETED" />
       </intent-filter>
   </receiver>
   ```
   - 应用安装后即可接收广播
   - 即使应用未运行也能被触发（但需注意系统限制）

2. **动态注册（在代码中）**
   ```kotlin
   class NetworkChangeReceiver : BroadcastReceiver() {
       
       override fun onReceive(context: Context, intent: Intent?) {
           val isConnected = isNetworkAvailable(context)
           // 处理网络状态变化
       }
   }
   
   // 在 Activity 中注册
   class MainActivity : AppCompatActivity() {
       
       private var receiver: NetworkChangeReceiver? = null
       
       override fun onStart() {
           super.onStart()
           receiver = NetworkChangeReceiver()
           val filter = IntentFilter(ConnectivityManager.CONNECTIVITY_ACTION)
           registerReceiver(receiver, filter)
       }
       
       override fun onStop() {
           super.onStop()
           receiver?.let { unregisterReceiver(it) }
       }
   }
   ```

## (二) 常见系统广播

- `ACTION_BOOT_COMPLETED`：设备启动完成
- `ACTION_POWER_CONNECTED` / `ACTION_POWER_DISCONNECTED`：电源连接/断开
- `ACTION_BATTERY_LOW`：电量低
- `ACTION_BATTERY_OKAY`：电量恢复
- `ACTION_TIME_TICK`：时间每分钟变化
- `ACTION_TIME_CHANGED`：时间被设置
- `ACTION_TIMEZONE_CHANGED`：时区变化
- `ACTION_LOCALE_CHANGED`：语言环境变化
- `CONNECTIVITY_ACTION`：网络状态变化

## (三) 本地广播（LocalBroadcastManager 已废弃）

推荐使用：
- **LiveData** 或 **Flow** 进行组件内通信
- **事件总线** 库（如 EventBus，但需注意生命周期管理）
- **ViewModel** 共享数据
- **通知栏** 与用户交互

# 五、ContentProvider 详解

## (一) 基本结构

ContentProvider 提供 URI 方式的数据访问：
```kotlin
class ArticleProvider : ContentProvider() {
    
    companion object {
        private const val AUTHORITY = "com.waiminutes.app.provider"
        private const val BASE_PATH = "articles"
        val CONTENT_URI = Uri.parse("content://$AUTHORITY/$BASE_PATH")
        
        private const val ARTICLES = 1
        private const val ARTICLE_ID = 2
        
        private val sUriMatcher = UriMatcher(UriMatcher.NO_MATCH).apply {
            addURI(AUTHORITY, BASE_PATH, ARTICLES)
            addURI(AUTHORITY, "$BASE_PATH/#", ARTICLE_ID)
        }
    }
    
    private lateinit var dbHelper: ArticleDbHelper
    
    override fun onCreate(): Boolean {
        dbHelper = ArticleDbHelper(context)
        return true
    }
    
    override fun query(
        uri: Uri,
        projection: Array<String>?,
        selection: String?,
        selectionArgs: Array<String>?,
        sortOrder: String?
    ): Cursor? {
        return when (sUriMatcher.match(uri)) {
            ARTICLES -> {
                dbHelper.readableDatabase.query(
                    "articles",
                    projection,
                    selection,
                    selectionArgs,
                    null,
                    null,
                    sortOrder
                )
            }
            ARTICLE_ID -> {
                val articleId = uri.getPathSegment(1).toLong()
                dbHelper.readableDatabase.query(
                    "articles",
                    projection,
                    "_id = ?",
                    arrayOf(articleId.toString()),
                    null,
                    null,
                    sortOrder
                )
            }
            else -> throw IllegalArgumentException("Unknown URI: $uri")
        }
    }
    
    // 实现其他必要方法：insert、update、delete、getType
    override fun insert(uri: Uri, values: ContentValues?): Uri? { /* ... */ }
    override fun update(uri: Uri, values: ContentValues?, selection: String?, selectionArgs: Array<String>?): Int { /* ... */ }
    override fun delete(uri: Uri, selection: String?, selectionArgs: Array<String>?): Int { /* ... */ }
    override fun getType(uri: Uri): String? { /* ... */ }
}
```

## (二) 访问 ContentProvider

通过 ContentResolver 访问：
```kotlin
// 查询数据
val cursor = contentResolver.query(
    ArticleProvider.CONTENT_URI,
    null, // 投影
    null, // 选择条件
    null, // 选择参数
    null  // 排序
)

// 插入数据
val values = ContentValues().apply {
    put("title", "标题")
    put("content", "内容")
}
val newUri = contentResolver.insert(ArticleProvider.CONTENT_URI, values)

// 更新数据
val updateValues = ContentValues().apply {
    put("is_read", 1)
}
val updatedCount = contentResolver.update(
    Uri.withAppendedPath(ArticleProvider.CONTENT_URI, "5"),
    updateValues,
    null,
    null
)

// 删除数据
val deletedCount = contentResolver.delete(
    Uri.withAppendedPath(ArticleProvider.CONTENT_URI, "3"),
    null,
    null
)
```

## (三) URI 权限与授予

临时授予 URI 权限：
```kotlin
// 在提供数据的一方（如 Service）
class FileService : Service() {
    
    fun shareFile(fileUri: Uri) {
        val shareIntent = Intent(Intent.ACTION_SEND).apply {
            type = "application/pdf"
            putExtra(Intent.EXTRA_STREAM, fileUri)
            // 临时授予读取权限
            addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
        }
        startActivity(Intent.createChooser(shareIntent, "分享文件"))
    }
}

// 在接收数据的一方自动获得临时权限
// 无需额外代码，系统会处理
```

## (四) 与现代架构的协同使用

在 Repository 中封装 ContentProvider 访问：
```kotlin
class ArticleRepository @Inject constructor(
    @ApplicationContext private val context: Context
) {
    
    private val contentResolver: ContentResolver = context.contentResolver
    
    fun getArticles(): Flow<List<Article>> = callbackFlow {
        try {
            val cursor = contentResolver.query(
                ArticleProvider.CONTENT_URI,
                null,
                null,
                null,
                "date DESC"
            ) ?: return@callbackFlow
            
            val articles = mutableListOf<Article>()
            while (cursor.moveToNext()) {
                articles.add(
                    Article(
                        id = cursor.getLong(cursor.getColumnIndexOrThrow("_id")),
                        title = cursor.getString(cursor.getColumnIndexOrThrow("title")),
                        content = cursor.getString(cursor.getColumnIndexOrThrow("content")),
                        author = cursor.getString(cursor.getColumnIndexOrThrow("author")),
                        createTime = cursor.getLong(cursor.getColumnIndexOrThrow("create_time")),
                        isRead = cursor.getInt(cursor.getColumnIndexOrThrow("is_read")) == 1
                    )
                )
            }
            close()
            trySend(articles)
        } catch (e: Exception) {
            close()
        }
    }
    
    private fun close() {
        // 关闭资源的辅助函数
    }
}
```

# 六、四大组件与现代架构的关系

## (一) 在单活动架构中的作用

- **Activity**：通常只有一个 MainActivity，承载导航宿主
- **Service**：通过 WorkManager 或前台服务处理后台任务
- **BroadcastReceiver**：较少使用，多被 LiveData/Flow 替代，但系统事件仍需使用
- **ContentProvider**：在需要跨应用数据共享时仍然有用，内部数据共享可用 Room

## (二) Jetpack Compose 中的考虑

- Compose 不直接处理组件生命周期，但可以在 Activity/Fragment 中设置
- 使用 `rememberSaveable` 处理 UI 状态
- 通过 ViewModel 获取数据，ViewModel 可以从 Repository 获取来自各种数据源的数据
- 为特定组件交互使用 Accompanist 或系统 API

# 七、最佳实践与常见问题

## (一) 最佳实践

✅ 理解每种组件的适用场景  
✅ 正确处理生命周期以避免内存泄漏  
✅ 使用显式 Intent 而非隐式 Intent（除非必要）  
✅ 为后台任务优先考虑 WorkManager 而非 Service  
✅ 动态注册的 BroadcastReceiver 及时注销  
✅ ContentProvider 仅在需要跨应用共享时使用  
✅ 使用 ContentObserver 监听数据变化  
✅ 尊重用户选择和系统设置（如后台限制）  
✅ 测试不同的启动模式和任务栈行为  

## (二) 常见问题及解决方案

### 1. 问题：Activity 泄漏
**原因**：长生命周期对象持有 Activity 的引用  
**解决方案**：
- 使用 ViewModel 或 Application 上下文
- 使用弱引用（WeakReference）
- 在 onDestroy 中清理引用
- 使用 LeakCanary 检测内存泄漏

### 2. 问题：Service 被系统杀死
**原因**：系统资源紧张或后台执行限制  
**解决方案**：
- 使用前台服务进行关键后台任务
- 使用 WorkManager 保证任务最终执行
- 检查是否设置了正确的优先级
- 处理 onDestroy 中的重启逻辑

### 3. 问题：BroadcastReceiver 未收到广播
**原因**：未正确注册、被系统优化或权限不足  
**解决方案**：
- 验证 Intent Filter 匹配
- 检查是否需要特殊权限（如 RECEIVE_BOOT_COMPLETED）
- 考虑使用调度算法而非实时广播
- 使用高优先级广播（需谨慎）

### 4. 问题：ContentProvider 查询慢
**原因**：缺少索引或查询不当  
**解决方案**：
- 为常用查询字段添加数据库索引
- 使用限定的投影而非 SELECT *
- 考虑使用分页查询
- 使用 SQLite Profiler 分析查询性能

## (三) 易错点对照

| 误区 | 事实 |
| --- | --- |
| `BroadcastReceiver` 里可以直接做耗时操作 | 不能，`onReceive` 约 10 秒就被杀。耗时工作转给 `WorkManager` |
| 注册广播就必须声明权限 | 注册时用 `registerReceiver(receiver, filter, permission, handler)` 的权限参数才生效 |
| `ContentProvider` 一定要 `exported = true` 才能用 | 恰好相反。不导出时只有本应用（含同 UID）能用 |
| `Service` 停了 `startService` 就没在跑 | `startService` 的服务不会自带后台保活，`targetSdk 26+` 还会受限。要长期运行必须前台服务 |
| `ContentProvider` 的 `query` 可以同步调 | 它跑在 Binder 线程上，耗时查询会拖慢调用方，必须转异步 |
| 四个组件都可以 `new` 出来用 | 只有 `ContentProvider` 可手动实例化，`Activity`/`Service`/`Receiver` 都由系统创建 |
| `android:exported` 默认值是安全的 | `targetSdk 31+` 必须显式声明。含 `intent-filter` 而没声明会在安装期直接报错 |

# 八、参考资料

- [Activity 官方文档](https://developer.android.com/guide/components/activities)
- [Service 官方文档](https://developer.android.com/guide/components/services)
- [BroadcastReceiver 官方文档](https://developer.android.com/guide/components/broadcasts)
- [ContentProvider 官方文档](https://developer.android.com/guide/topics/providers/content-providers)
- [Android 应用架构指南](https://developer.android.com/topic/architecture)
- [Jetpack Compose 与生命周期](https://developer.android.com/jetpack/compose/lifecycle)
- [后台执行优化指南](https://developer.android.com/topic/performance)
- [后台限制适用场景](https://developer.android.com/about/versions/oreo/background)
- [后台执行最佳实践](https://developer.android.com/topic/performance/vitals)
- [ContentProvider 性能优化](https://developer.android.com/training/data-storage)
- [Intent 和 Intent Filter 详解](https://developer.android.com/guide/components/intents-filters)
- [任务和返回栈](https://developer.android.com/guide/components/tasks-and-back-stack)
- [Activity Result API](https://developer.android.com/training/basics/intents/result)
- [前台服务开发指南](https://developer.android.com/guide/components/foreground-services)
- [使用 WorkManager 处理后台任务](https://developer.android.com/develop/background-work/background-tasks/persistent/getting-started)
