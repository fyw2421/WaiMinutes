---
title: "AndroidNotes21 : DataStore 偏好存储"
weight: 21
description: "Android DataStore 现代化数据存储解决方案：SharedPreferences 的替代品、Proto DataStore 与 Preferences DataStore 的选择、异步 API 与 Flow 集成、序列化与迁移策略、错误处理与测试以及与 Hilt 的协同使用。"
featureimage: "covers/android-system.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么选择 DataStore

DataStore 是 Android Jetpack 提供的现代化数据存储库，旨在替代 SharedPreferences。相比 SharedPreferences，DataStore 提供：

- **完全异步**：基于 Kotlin 的 Flow 和协程，避免主线程阻塞
- **强类型支持**：Proto DataStore 提供类型安全的数据访问
- **错误处理**：通过 Result 类型明确处理 IO 和解析错误
- **事务保证**：单次更新操作具有原子性
- **迁移助手**：提供从 SharedPreferences 迁移的机制
- **无墨迹猜测**：明确的 API 设计减少使用错误

DataStore 有两种实现：
- **Preferences DataStore**：类似 SharedPreferences 的键值对存储（无强类型）
- **Proto DataStore**：使用协议缓冲区定义强类型数据结构

# 二、基础设置

## (一) 添加依赖

在模块的 `build.gradle.kts` 中添加 DataStore 及其必要依赖：
```kotlin
dependencies {
    // Preferences DataStore (键值对)
    implementation("androidx.datastore:datastore-preferences:1.2.1")
    
    // 核心 + Proto DataStore（dataStore {} 委托工厂在 datastore 里，不是 core）
    implementation("androidx.datastore:datastore:1.2.1")
    implementation("androidx.datastore:datastore-core:1.2.1")
    
    // Protobuf 依赖（仅 Proto DataStore 需要）
    implementation("com.google.protobuf:protobuf-javalite:4.36.2")
    
    // Kotlin 协程扩展（Flow 支持）
    implementation("org.jetbrains.kotlinx:kotlinx-coroutines-core:1.11.0")
    
    // 测试依赖
    testImplementation("androidx.datastore:datastore-preferences:1.2.1")
    testImplementation("androidx.datastore:datastore-core:1.2.1")
}
```

## (二) 定义 Proto 模式（仅 Proto DataStore）

在 `app/src/main/proto/` 目录下创建 `.proto` 文件：
```proto
// app/src/main/proto/datastore.proto
syntax = "proto3";

option java_package = com.waiminutes.app.datastore;
option java_multiple_files = true;

// 用户偏好设置示例
message UserPreferences {
  // 显示名称
  string display_name = 1;
  
  // 是否启用通知
  bool enable_notifications = 2;
  
  // 主题选择 (0: 系统默认, 1: 深色, 2: 浅色)
  int32 theme_selection = 3;
  
  // 上次同步时间戳
  int64 last_sync_timestamp = 4;
  
  // 已读文章 ID 列表 (使用重复字段)
  repeated string read_article_ids = 5;
}
```

构建时会自动生成 Kotlin/Java 类。

# 三、Preferences DataStore 使用

## (一) 创建 DataStore 实例

使用属性委托获取 DataStore 实例：
```kotlin
// 使用属性委托（推荐）
val Context.preferencesDataStore by preferencesDataStore(name = "settings_preferences")

// 或者手动创建（需要自定义存放位置时）
// ★DataStoreFactory 没有 create(context=, fileName=) 这个重载。
//   Preferences 用 PreferenceDataStoreFactory + preferencesDataFile()；
//   文件名不要手写扩展名，官方惯例是不带后缀。
val context: Context = /* ... */
val dataStore: DataStore<Preferences> = PreferenceDataStoreFactory.create(
    corruptionHandler = ReplaceFileCorruptionHandler { emptyPreferences() },
    produceFile = { context.preferencesDataFile("settings_preferences") }
)
```

## (二) 定义键

在顶层或伴侣对象中定义键：
```kotlin
object UserPrefs {
    // 字符串类型键
    val DISPLAY_NAME = stringPreferencesKey("display_name")
    
    // 布尔类型键
    val ENABLE_NOTIFICATIONS = booleanPreferencesKey("enable_notifications")
    
    // 整数类型键
    val THEME_SELECTION = intPreferencesKey("theme_selection")
    
    // 字符串集合键
    val READ_ARTICLE_IDS = stringSetPreferencesKey("read_article_ids")
}
```

## (三) 读取数据

使用 Flow 读取数据并处理异常：
```kotlin
class UserPreferencesRepository @Inject constructor(
    @ApplicationContext private val context: Context
) {
    
    // 使用属性委托获取 DataStore
    private val dataStore: DataStore<Preferences> = context.preferencesDataStore
    
    // 读取显示名称（返回 Flow）
    val displayNameFlow: Flow<String> = dataStore.data
        .catch { exception ->
            // 如果发生 IO 错误，返回空偏好设置然后继续
            if (exception is IOException) {
                emit(emptyPreferences()) // 返回空偏好设置
            } else {
                throw exception // 否则重新抛出异常
            }
        }
        .map { preferences ->
            // 获取值，如果不存在则返回默认值
            preferences[UserPrefs.DISPLAY_NAME] ?: "匿名用户"
        }
    
    // 读取布尔值
    val enableNotificationsFlow: Flow<Boolean> = dataStore.data
        .catch { exception -> 
            if (exception is IOException) emit(emptyPreferences())
            else throw exception
        }
        .map { prefs -> prefs[UserPrefs.ENABLE_NOTIFICATIONS] ?: true }
    
    // 一次性读取多个值（使用 map）
    val userPrefsFlow: Flow<UserPrefs> = dataStore.data
        .catch { exception -> 
            if (exception is IOException) emit(emptyPreferences())
            else throw exception
        }
        .map { prefs ->
            UserPrefs(
                displayName = prefs[UserPrefs.DISPLAY_NAME] ?: "匿名用户",
                enableNotifications = prefs[UserPrefs.ENABLE_NOTIFICATIONS] ?: true,
                themeSelection = prefs[UserPrefs.THEME_SELECTION] ?: 0,
                readArticleIds = prefs[UserPrefs.READ_ARTICLE_IDS] ?: emptySet()
            )
        }
}

// 数据类用于持有偏好设置
data class UserPrefs(
    val displayName: String,
    val enableNotifications: Boolean,
    val themeSelection: Int, // 0: 系统默认, 1: 深色, 2: 浅色
    val readArticleIds: Set<String>
)
```

## (四) 写入数据

使用 `edit()` 函数原子地更新数据：
```kotlin
class UserPreferencesRepository @Inject constructor(
    @ApplicationContext private val context: Context
) {
    private val dataStore: DataStore<Preferences> = context.preferencesDataStore
    
    // 挂起函数更新单个值
    suspend fun updateDisplayName(newName: String) {
        dataStore.updateData { prefs ->
            prefs.toMutablePreferences().apply {
                set(UserPrefs.DISPLAY_NAME, newName)
            }
        }
    }
    
    // 挂起函数更新多个值
    suspend fun updateUserPrefs(newPrefs: UserPrefs) {
        dataStore.updateData { prefs ->
            prefs.toMutablePreferences().apply {
                set(UserPrefs.DISPLAY_NAME, newPrefs.displayName)
                set(UserPrefs.ENABLE_NOTIFICATIONS, newPrefs.enableNotifications)
                set(UserPrefs.THEME_SELECTION, newPrefs.themeSelection)
                set(UserPrefs.READ_ARTICLE_IDS, newPrefs.readArticleIds)
            }
        }
    }
    
    // 移除特定键
    suspend fun clearDisplayName() {
        dataStore.updateData { prefs ->
            prefs.toMutablePreferences().apply {
                remove(UserPrefs.DISPLAY_NAME)
            }
        }
    }
    
    // 清除所有数据（谨慎使用）
    suspend fun clearAll() {
        dataStore.updateData { emptyPreferences() }
    }
}
```

# 四、Proto DataStore 使用

## (一) 定义 Schema 并生成类

添加 protobuf 插件到 `build.gradle.kts`：
```kotlin
plugins {
    id("com.google.protobuf") version "0.9.2" apply false
}

// 在 android 模块中
android {
    // ...
}

// ★protobuf 插件没有 buildFeatures { protobuf { } } 这个 DSL（AGP 也不允许）。
//   正确写法是顶层 protobuf { } 块：
protobuf {
    protoc {
        artifact = "com.google.protobuf:protoc:4.36.2"
    }
    // lite 版产物更小；用 protobuf-javalite 时配上它
    generateProtoTasks {
        all().configureEach {
            builtins {
                create("java") { option("lite") }
            }
        }
    }
}
```

构建后会生成类如 `UserPreferences` 和 `UserPreferencesOrBuilder`。

⚠ **protobuf 不用 KSP**。`com.google.googlejavaformat:google-java-format-protobuf` 是 **Java 代码格式化器**，与 protobuf 无关，也不存在对应的 KSP 符号处理器——写上去 Gradle 会直接报「找不到 symbol processor」。用上面的 `protoc { artifact = ... }` 就够了。

## (二) 创建 Proto DataStore 实例

定义 Schema 并提供序列化器：
```kotlin
object UserPreferencesSchema {
    // 数据存储文件名
    val FILE_NAME = "user_preferences.pb"
    
    // Protobuf 序列化器
    val SERIALIZER = Serializer<UserPreferences> = 
        object : Serializer<UserPreferences> {
            // 从字节读取数据
            override fun readFrom(input: InputStream): UserPreferences {
                try {
                    return UserPreferences.parseFrom(input)
                } catch (exception: InvalidProtocolBufferException) {
                    throw CorruptionException("Cannot read proto.", exception)
                }
            }
            
            // 将数据写入字节
            override fun writeTo(t: UserPreferences, output: OutputStream) {
                try {
                    t.writeTo(output)
                } catch (exception: IOException) {
                    throw IOException("Cannot write proto.", exception)
                }
            }
        }
}

// 使用属性委托获取 Proto DataStore
private val Context.userDataStore by dataStore(
    fileName = UserPreferencesSchema.FILE_NAME,
    serializer = UserPreferencesSchema.SERIALIZER
)
```

## (三) 读取数据

类似 Preferences DataStore，但返回强类型对象：
```kotlin
class UserPrefsRepository @Inject constructor(
    @ApplicationContext private val context: Context
) {
    private val dataStore: DataStore<UserPreferences> = context.userDataStore
    
    // 读取用户偏好设置（返回 Flow）
    val userPrefsFlow: Flow<UserPreferences> = dataStore.data
        .catch { exception ->
            if (exception is IOException) {
                // 返回默认值而不是抛出异常
                emit(UserPreferences.getDefaultInstance())
            } else {
                throw exception
            }
        }
    
    // 读取特定字段
    val displayNameFlow: Flow<String> = dataStore.data
        .catch { exception -> 
            if (exception is IOException) emit(UserPreferences.getDefaultInstance())
            else throw exception
        }
        .map { prefs -> prefs.displayName }
    
    // 使用 mapNotNull 处理可能为空的字段
    val readArticleIdsFlow: Flow<List<String>> = dataStore.data
        .catch { exception -> 
            if (exception is IOException) emit(UserPreferences.getDefaultInstance())
            else throw exception
        }
        .map { prefs -> prefs.readArticleIdsList }
}
```

## (四) 写入数据

使用 `updateData()` 原子地更新数据：
```kotlin
class UserPrefsRepository @Inject constructor(
    @ApplicationContext private val context: Context
) {
    private val dataStore: DataStore<UserPreferences> = context.userDataStore
    
    // 更新显示名称
    suspend fun updateDisplayName(newName: String) {
        dataStore.updateData { currentPrefs ->
            currentPrefs.toBuilder()
                .setDisplayName(newName)
                .build()
        }
    }
    
    // 更新多个字段
    suspend fun updateUserPrefs(newPrefs: UserPreferences) {
        dataStore.updateData { _ -> newPrefs }
    }
    
    // 添加已读文章 ID
    suspend fun addReadArticleId(articleId: String) {
        dataStore.updateData { currentPrefs ->
            val currentIds = currentPrefs.readArticleIdsList.toMutableList()
            if (!currentIds.contains(articleId)) {
                currentIds.add(articleId)
            }
            currentPrefs.toBuilder()
                .clearReadArticleIds()
                .addAllReadArticleIds(currentIds)
                .build()
        }
    }
    
    // 清除所有已读文章记录
    suspend fun clearReadArticleIds() {
        dataStore.updateData { currentPrefs ->
            currentPrefs.toBuilder()
                .clearReadArticleIds()
                .build()
        }
    }
}
```

# 五、从 SharedPreferences 迁移

DataStore 提供迁移机制帮助从 SharedPreferences 迁移：
```kotlin
// 定义 SharedPreferences 的键
private val PREF_FILE_NAME = "shared_prefs"
private val KEY_DISPLAY_NAME = "display_name"
private val KEY_ENABLE_NOTIFICATIONS = "enable_notifications"

// 创建 DataStore 时指定迁移策略
val Context.preferencesDataStore: DataStore<Preferences> by preferencesDataStore(
    name = "settings_preferences",
    produceMigrations = { context ->
        listOf(SharedPreferencesMigration(context, PREF_FILE_NAME) { sharedPrefs ->
            // 在这里定义如何将 SharedPreferences 值映射到 DataStore 键
            val mutablePrefs = sharedPrefs.edit()
            
            // 迁移显示名称
            if (sharedPrefs.contains(KEY_DISPLAY_NAME)) {
                mutablePrefs.putString(UserPrefs.DISPLAY_NAME, 
                    sharedPrefs.getString(KEY_DISPLAY_NAME, ""))
            }
            
            // 迁移通知开关
            if (sharedPrefs.contains(KEY_ENABLE_NOTIFICATIONS)) {
                mutablePrefs.putBoolean(UserPrefs.ENABLE_NOTIFICATIONS, 
                    sharedPrefs.getBoolean(KEY_ENABLE_NOTIFICATIONS, true))
            }
            
            // 其他迁移规则...
            
            mutablePrefs
        })
    }
)

// 或者使用更简单的替换迁移（如果键名相同）
val Context.preferencesDataStore: DataStore<Preferences> by preferencesDataStore(
    name = "settings_preferences",
    produceMigrations = { context ->
        listOf(SharedPreferencesMigration(context, PREF_FILE_NAME))
    }
)
```

# 六、错误处理与测试

## (一) 错误处理

DataStore 使用 `Result` 类型封装成功或失败：
```kotlin
class UserPrefsRepository @Inject constructor(
    @ApplicationContext private val context: Context
) {
    private val dataStore: DataStore<Preferences> = context.preferencesDataStore
    
    // 读取数据并明确处理错误
    suspend fun readDisplayName(): Result<String> {
        return try {
            val prefs = dataStore.data.first() // 获取最新值
            val displayName = prefs[UserPrefs.DISPLAY_NAME] ?: "匿名用户"
            Result.success(displayName)
        } catch (e: IOException) {
            Result.failure(e) // IO 错误（如磁盘不可用）
        } catch (e: Exception) {
            Result.failure(e) // 其他意外错误
        }
    }
    
    // 写入数据并处理错误
    suspend fun writeDisplayName(name: String): Result<Unit> {
        return try {
            dataStore.updateData { prefs ->
                prefs.toMutablePreferences().apply {
                    set(UserPrefs.DISPLAY_NAME, name)
                }
            }
            Result.success(Unit)
        } catch (e: IOException) {
            Result.failure(e)
        } catch (e: Exception) {
            Result.failure(e)
        }
    }
}
```

## (二) 测试策略

### 1. Preferences DataStore 测试
```kotlin
@ExperimentalCoroutinesApi
class UserPrefsRepositoryTest {
    
    private lateinit var context: Context
    private lateinit var repository: UserPrefsRepository
    private lateinit var testDispatcher: TestDispatcher
    
    @Before
    fun setUp() {
        testDispatcher = TestDispatcher()
        Dispatchers.setMain(testDispatcher)
        
        context = ApplicationProvider.getApplicationContext()
        repository = UserPrefsRepository(context)
    }
    
    @After
    fun tearDown() {
        Dispatchers.resetMain()
        testDispatcher.cleanup()
    }
    
    @Test
    fun `read and write display name`() = runTest {
        // 初始读取应返回默认值
        val initialName = repository.displayNameFlow.first()
        assertEquals("匿名用户", initialName)
        
        // 写入新值
        repository.updateDisplayName("测试用户")
        
        // 读取应返回新值
        val updatedName = repository.displayNameFlow.first()
        assertEquals("测试用户", updatedName)
    }
    
    @Test
    fun `handle io errors gracefully`() = runTest {
        // 模拟 IO 错误（需要特殊设置，这里简化展示概念）
        // 实际测试中可以使用漂移或临时目录不可写来模拟
        // 验证错误被捕获且返回默认值而不是崩溃
        // ... 具体实现取决于测试策略
    }
}
```

### 2. Proto DataStore 测试
```kotlin
@ExperimentalCoroutinesApi
class ProtoUserPrefsRepositoryTest {
    
    private lateinit var context: Context
    private lateinit var repository: UserPrefsRepository
    private lateinit var testDispatcher: TestDispatcher
    
    @Before
    fun setUp() {
        testDispatcher = TestDispatcher()
        Dispatchers.setMain(testDispatcher)
        
        context = ApplicationProvider.getApplicationContext()
        repository = UserPrefsRepository(context)
    }
    
    @After
    fun tearDown() {
        Dispatchers.resetMain()
        testDispatcher.cleanup()
    }
    
    @Test
    fun `read and write proto prefs`() = runTest {
        // 初始读取应返回默认实例
        val initialPrefs = repository.userPrefsFlow.first()
        assertEquals(UserPreferences.getDefaultInstance(), initialPrefs)
        
        // 写入自定义值
        val newPrefs = UserPreferences.newBuilder()
            .setDisplayName("Proto 测试用户")
            .setEnableNotifications(false)
            .setThemeSelection(2) // 浅色主题
            .addReadArticleIds("article_123")
            .build()
        repository.updateUserPrefs(newPrefs)
        
        // 读取应返回更新后的值
        val updatedPrefs = repository.userPrefsFlow.first()
        assertEquals("Proto 测试用户", updatedPrefs.displayName)
        assertFalse(updatedPrefs.enableNotifications)
        assertEquals(2, updatedPrefs.themeSelection)
        assertTrue(updatedPrefs.readArticleIdsList.contains("article_123"))
    }
}
```

# 七、最佳实践与常见问题

## (一) 最佳实践

✅ 使用属性委托获取 DataStore 实例（简化代码）  
✅ 将 DataStore 定义为单例（通过 Hilt 或属性委托）  
✅ 使用 Flow 响应式地监控数据变化  
✅ 在 `catch` 块中处理 IOException 以避免流终止  
✅ 为 Proto DataStore 定义清晰的 protobuf schema  
✅ 使用 `updateData()` 进行原子更新操作  
✅ 避免在频繁更新的场景中使用 DataStore（考虑使用 Room）  
✅ 为 Preferences DataStore 定义类型安全的键容器对象  
✅ 在应用启动时预热 DataStore 以减少首次访问延迟  
✅ 考虑使用 Proto DataStore 而非 Preferences DataStore 获得类型安全  
✅ 将复杂的偏好设置逻辑封装在 Repository 中  

## (二) 常见问题及解决方案

### 1. 问题：DataStore 文件损坏导致启动崩溃
**原因**：意外关机或磁盘错误导致 protobuf 文件损坏  
**解决方案**：
- 在 `catch` 块中处理 `CorruptionException` 并提供默认值
- 考虑实现备份和恢复机制
- 为 Proto DataStore 添加版本字段以便将来进行迁移
- 使用 `produceMigrations` 参数处理已知的 schema 变更

### 2. 问题：读取 Flow 没拿到期望的值
**原因**：`data` Flow **要么发射一个值、要么抛异常**，不存在「静默不发射」。实际原因通常是：文件不存在时 Preferences 发的是 `emptyPreferences()`（一个空偏好集，key 全取默认值），而你期望的是 Proto 的 `getDefaultInstance()` 或某个业务默认值。  
**解决方案**：
- 业务默认值写在下游 `map` 里给，不要指望 DataStore 替你填
- 用 `catch { emit(emptyPreferences()) }` 兜住读盘失败的 `IOException`
- 检查是否有其他进程在写入同一文件导致冲突

### 3. 问题： protobuf 字段号冲突
**原因**：在修改 .proto 文件时分配了重复的字段号  
**解决方案**：
- 永远不要重用字段号，即使字段被删除
- 使用保留字段号范围以便将来添加
- 在版本控制中仔细审查 .proto 文件更改
- 考虑使用语义化版本管理协议

### 4. 问题： SharedPreferences 迁移不完整
**原因**：迁移逻辑未处理所有键或数据类型转换错误  
**解决方案**：
- 编写单元测试验证迁移逻辑
- 记录所有 SharedPreferences 键及其数据类型
- 测试边界情况如空值、默认值和特殊字符
- 考虑在迁移过程中记录日志以便调试

### 5. 问题：频繁更新导致磁盘 I/O 过高
**原因**：DataStore 每次更新都写入磁盘文件  
**解决方案**：
- 批量更新多个值而不是频繁单个更新
- 考虑使用内存缓存层减少磁盘写入频率
- 评估是否真的需要持久化某些经常变化的数据
- 对于临时状态考虑使用 ViewModel 或其他内存方案

# 八、与其他架构组件的协同使用

## (一) 与 ViewModel 结合

```kotlin
@HiltViewModel
class SettingsViewModel @Inject constructor(
    private val prefsRepository: UserPrefsRepository
) : ViewModel() {
    
    // 暴露 Flow 给 UI 层
    val displayName: StateFlow<String> = prefsRepository.displayNameFlow
        .distinctUntilChanged()
        .stateIn(
            scope = viewModelScope,
            started = SharingStarted.WhileSubscribed(5000),
            initialValue = "匿名用户"
        )
    
    val enableNotifications: StateFlow<Boolean> = prefsRepository.enableNotificationsFlow
        .distinctUntilChanged()
        .stateIn(
            scope = viewModelScope,
            started = SharingStarted.WhileSubscribed(5000),
            initialValue = true
        )
    
    // 提供更新函数
    fun updateDisplayName(newName: String) {
        viewModelScope.launch {
            prefsRepository.updateDisplayName(newName)
        }
    }
    
    fun toggleNotifications() {
        viewModelScope.launch {
            val current = prefsRepository.enableNotificationsFlow.first()
            prefsRepository.updateEnableNotifications(!current)
        }
    }
}
```

## (二) 与 Compose UI 结合

```kotlin
@Composable
fun SettingsScreen(
    viewModel: SettingsViewModel = hiltViewModel()
) {
    val displayName by viewModel.displayName.collectAsState()
    val enableNotifications by viewModel.enableNotifications.collectAsState()
    
    Column(modifier = Modifier
        .fillMaxWidth()
        .padding(16.dp)
    ) {
        Text(
            text = "显示名称: $displayName",
            style = MaterialTheme.typography.bodyLarge
        )
        
        OutlinedTextField(
            value = displayName,
            onValueChange = { viewModel.updateDisplayName(it) },
            label = { Text("显示名称") }
        )
        
        Switch(
            checked = enableNotifications,
            onChange = { viewModel.toggleNotifications() },
            label = { Text("启用通知") }
        )
        
        // 其他设置项...
    }
}
```

## (三) 与 WorkManager 结合

```kotlin
// 备份用户偏好设置到服务器的 Worker
class BackupPrefsWorker(
    appContext: Context,
    workerParams: WorkerParameters,
    private val prefsRepository: UserPrefsRepository
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        return try {
            // 读取当前偏好设置
            val prefs = prefsRepository.userPrefsFlow.first()
            
            // 转换为可传输的格式（这里简化为 JSON）
            val jsonPrefs = Gson().toJson(prefs)
            
            // 发送到服务器（实际中应使用 Retrofit 等网络库）
            val backupResult = prefsRepository.backupToServer(jsonPrefs)
            
            if (backupResult.isSuccess) {
                Result.success()
            } else {
                Result.retry()  // 失败时重试
            }
        } catch (e: Exception) {
            Result.retry()
        }
    }
}

// 每日调度备份工作
fun schedulePrefsBackup(context: Context) {
    val backupWork = PeriodicWorkRequestBuilder<BackupPrefsWorker>(24, TimeUnit.HOURS)
        .setBackoffCriteria(
            BackoffPolicy.EXPONENTIAL,
            Duration.ofMinutes(15),
            TimeUnit.MINUTES
        )
        .build()
    
    WorkManager.getInstance(context)
        .enqueueUniquePeriodicWork(
            "prefs_backup",
            ExistingPeriodicWorkPolicy.KEEP,
            backupWork
        )
}
```

## (四) 易错点对照

| 误区 | 事实 |
| --- | --- |
| DataStore 适合存大对象 | 不适合。它每次写都是一次文件 I/O，大数据请用 Room |
| Preferences DataStore 没有 schema | 对。它是**无类型的键值 Map**，编译期查不出 key 拼写错误；需要强类型就改用 Proto |
| `updateData` 的块里可以随便挂起或再调 `updateData` | 不能。块内**禁止再次访问同一个 DataStore**（会死锁），也只保证串行、并发写仍可能互相覆盖 |
| `SharedPreferences` 可以 `commit()` 阻塞 | `commit()` 会阻塞调用线程，`apply()` 才是异步的 |
| DataStore 能替代 MMKV | 两者定位不同：DataStore 是异步、类型安全、协程原生；MMKV 是同步、内存映射、毫秒级读 |
| Proto 字段号可以随便改 | 字段号一旦分配就**永久绑定**。改号等于换字段，旧数据读不出来 |
| `updateData` 里修改局部再返回会覆盖别人 | `updateData` 是「读-改-写」，并发下要用它返回基于最新值的完整新对象 |
| `data` 首次拿到的是「旧值」 | 它总在**当前**值上发射（首次是空偏好 / 默认 proto），不会拿缓存的历史值。需要「首次读」用 `updateData` 或在下游比对 |

# 九、参考资料

- [DataStore 官方文档](https://developer.android.com/topic/libraries/architecture/datastore)
- [DataStore 与 SharedPreferences 对比](https://developer.android.com/topic/libraries/architecture/datastore#preferences)
- [Protobuf 语言指南](https://developers.google.com/protocol-buffers/docs/proto3)
- [Kotlin Flow 详解](https://developer.android.com/kotlin/flow)
- [Android 架构组件指南](https://developer.android.com/topic/architecture)
- [Hilt 依赖注入指南](https://developer.android.com/training/dependency-injection/hilt-android)
- [WorkManager 后台处理指南](https://developer.android.com/develop/background-work/background-tasks/persistent/getting-started)
- [Jetpack Compose 状态与 UI 指南](https://developer.android.com/jetpack/compose/state)
- [Android 数据存储最佳实践](https://developer.android.com/training/data-storage)
- [Protobuf 在 Android 中的使用](https://developers.google.com/protocol-buffers/docs/javatutorial)
- [DataStore 测试指南](https://developer.android.com/topic/libraries/architecture/datastore)
- 使用 DataStore 进行偏好设置迁移
- [SharedPreferences 迁移工具包说明](https://developer.android.com/topic/libraries/architecture/datastore#preferences)
- [协程异常处理最佳实践](https://kotlinlang.org/docs/exceptions.html)
