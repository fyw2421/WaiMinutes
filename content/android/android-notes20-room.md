---
title: "AndroidNotes20 : Room 数据库"
weight: 20
description: "Android Room 持久化库的完整用法：实体定义、DAO 接口、数据库构建、类型转换器、关系映射、事务处理、迁移策略、协程与 Flow 集成、测试策略以及与 LiveData 的协同使用。"
featureimage: "covers/android-system.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么选择 Room

Room 是 Android Jetpack 推出的 SQLite 对象映射库，提供：

- **编译时检查**：SQL 查询在编译时验证，减少运行时崩溃
- **便捷的 API**：注解减少样板代码，@Entity、@Dao、@Database
- **响应式支持**：原生支持 LiveData、Flow 和协程
- **迁移助手**：内置迁移框架处理数据库版本升级
- **安全保护**：主线程防护防止意外的长running 操作

# 二、基础设置

## (一) 添加依赖

在模块的 `build.gradle.kts` 中添加 Room 及其必要依赖：
```kotlin
plugins {
    alias(libs.plugins.ksp)          // com.google.devtools.ksp
}

dependencies {
    // Room 核心库
    implementation("androidx.room:room-runtime:2.8.5")
    // ★ 用 KSP，不再用 kapt（第 17 篇讲过原因：KAPT 要生成 Java 存根，慢得多）
    ksp("androidx.room:room-compiler:2.8.5")

    // Kotlin 协程支持
    implementation("androidx.room:room-ktx:2.8.5")

    // 测试依赖
    testImplementation("androidx.room:room-testing:2.8.5")
}
```

⚠ **`ksp(...)` 和 `kapt(...)` 不能混用同一个处理器**。Room 全系（`room-compiler`）与 Hilt 全系都应走 KSP；kapt 已在 Android 生态里基本退役，只在个别只提供 kapt 的第三方库上还有必要。

```kotlin
// 仍然依赖 kapt 的老工程，迁移时改这两处：
// 1) build.gradle.kts：kapt("...") → ksp("...")
// 2) gradle.properties：删掉 kapt.incremental.apt / kapt.use.worker.api
```

## (二) 定义实体

使用 `@Entity` 注解定义数据表：
```kotlin
@Entity(tableName = "articles")
data class Article(
    @PrimaryKey(autoGenerate = true)
    val id: Long = 0,
    
    @ColumnInfo(name = "title")
    val title: String,
    
    @ColumnInfo(name = "content")
    val content: String,
    
    @ColumnInfo(name = "author")
    val author: String?,
    
    @ColumnInfo(name = "create_time")
    val createTime: Long = System.currentTimeMillis(),
    
    @ColumnInfo(name = "is_read")
    val isRead: Boolean = false
)

// 复合主键示例
@Entity(
    tableName = "article_tags",
    primaryKeys = ["articleId", "tagId"]
)
data class ArticleTag(
    @ColumnInfo(name = "articleId")
    val articleId: Long,
    
    @ColumnInfo(name = "tagId")
    val tagId: Long
)
```

## (三) 定义 DAO

使用 `@Dao` 接口定义数据访问方法：
```kotlin
@Dao
interface ArticleDao {
    
    // 查询单篇文章
    @Query("SELECT * FROM articles WHERE id = :articleId")
    suspend fun getArticleById(articleId: Long): Article?
    
    // 查询多篇文章（支持分页和过滤）
    @Query("""
        SELECT * FROM articles 
        WHERE (:author IS NULL OR author = :author)
        ORDER BY create_time DESC
        LIMIT :limit OFFSET :offset
    """)
    suspend fun getArticles(
        author: String?,
        limit: Int,
        offset: Int
    ): List<Article>
    
    // 插入文章
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertArticle(article: Article): Long
    
    // 批量插入
    @Insert(onConflict = OnConflictStrategy.REPLACE)
    suspend fun insertArticles(varargs articles: Article): LongArray
    
    // 更新文章
    @Update
    suspend fun updateArticle(article: Article): Int
    
    // 删除文章
    @Delete
    suspend fun deleteArticle(article: Article): Int
    
    // 删除所有文章
    @Query("DELETE FROM articles")
    suspend fun deleteAllArticles(): Int
    
    // 使用自定义返回类型
    @Query("SELECT COUNT(*) FROM articles WHERE isRead = 1")
    suspend fun countReadArticles(): Int
    
    // 使用 Flow 实时监控数据变化
    @Query("SELECT * FROM articles ORDER BY create_time DESC")
    fun getAllArticlesAsFlow(): Flow<List<Article>>
}
```

# 三、构建数据库

## (一) 定义数据库抽象类

使用 `@Database` 注解定义数据库：
```kotlin
@Database(
    entities = [Article::class, ArticleTag::class],
    version = 1,
    exportSchema = false  // 生产环境建议设置为 true 并导出 schema
)
abstract class AppDatabase : RoomDatabase() {
    
    abstract fun articleDao(): ArticleDao
    
    // 单例模式获取数据库实例
    companion object {
        @Volatile
        private var INSTANCE: AppDatabase? = null
        
        fun getInstance(context: Context): AppDatabase {
            return INSTANCE ?: synchronized(this) {
                INSTANCE ?: buildDatabase(context).also { INSTANCE = it }
            }
        }
        
        private fun buildDatabase(context: Context): AppDatabase {
            return Room.databaseBuilder(
                context.applicationContext,
                AppDatabase::class.java,
                "waiminutes.db"
            )
                // 添加回调处理创建、打开、迁移等事件
                .addCallback(object : RoomDatabase.Callback() {
                    override fun onCreate(db: SupportSQLiteDatabase) {
                        super.onCreate(db)
                        // 首次创建时可以插入初始数据
                    }
                    
                    override fun onOpen(db: SupportSQLiteDatabase) {
                        super.onOpen(db)
                        // 每次打开数据库时调用
                    }
                })
                // 设置查询执行器（可选）
                .setQueryExecutor(Executors.newSingleThreadExecutor())
                // 启用防止主线程误用的保护（调试时很有用）
                .setAllowMainThreadQueries(false)
                .build()
        }
    }
}
```

## (二) 通过 Hilt 提供数据库实例

在依赖注入中提供数据库单例：
```kotlin
@Module
@InstallIn(SingletonComponent::class)
object DatabaseModule {
    
    @Provides
    @Singleton
    fun provideAppDatabase(@ApplicationContext appContext: Context): AppDatabase {
        return AppDatabase.getInstance(appContext)
    }
    
    @Provides
    @Singleton
    fun provideArticleDao(database: AppDatabase): ArticleDao {
        return database.articleDao()
    }
}
```

# 四、类型转换器

处理 Room 不原生支持的数据类型：
```kotlin
// 日期时间转换器
class DateConverter {
    @TypeConverter
    fun fromTimestamp(value: Long?): Date? {
        return value?.let { Date(it) }
    }
    
    @TypeConverter
    fun toTimestamp(date: Date?): Long? {
        return date?.time
    }
}

// 列表转换器（存储为 JSON）
class Converters {
    @TypeConverter
    fun stringToStringList(value: String?): List<String>? {
        return value?.let { 
            Gson().fromJson<List<String>>(it, object : TypeToken<List<String>>() {}.type)
        }
    }
    
    @TypeConverter
    fun stringListToString(list: List<String>?): String? {
        return list?.let { 
            Gson().toJson(it) 
        }
    }
}

// 在数据库类中应用转换器
@Database(
    entities = [Article::class],
    version = 1,
    converters = [DateConverter::class, Converters::class]
)
abstract class AppDatabase : RoomDatabase() { ... }
```

# 五、关系映射

定义实体之间的关系并使用关系返回类型：
```kotlin
// 定义关系数据类
data class ArticleWithTags(
    @Embedded val article: Article,
    @Relation(
        parentColumn = "id",
        entityColumn = "articleId",
        associateBy = Junction(
            value = ArticleTag::class,
            parentColumn = "tagId",
            entityColumn = "articleId"
        )
    )
    val tags: List<Tag> = emptyList()
)

// 在 DAO 中使用关系查询
@Transaction
@Query("SELECT * FROM articles WHERE id = :articleId")
suspend fun getArticleWithTagsById(articleId: Long): ArticleWithTags?
```

# 六、事务处理

确保多个操作要么全部成功要么全部失败：
```kotlin
@Dao
interface ArticleDao {
    @Transaction
    suspend fun batchProcessArticles(newArticles: List<Article>) {
        // 删除旧文章
        deleteAllArticles()
        // 插入新文章
        insertArticles(*newArticles.toTypedArray())
        // 如果中途出现异常，整个事务会回滚
    }
}
```

# 七、迁移策略

处理数据库版本升级：
```kotlin
// 定义迁移路径
val MIGRATION_1_2 = object : Migration(1, 2) {
    override fun migrate(database: SupportSQLiteDatabase) {
        // 添加新列
        database.execSQL("ALTER TABLE articles ADD COLUMN subtitle TEXT DEFAULT ''")
        
        // 创建新表
        database.execSQL("""
            CREATE TABLE article_views (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                article_id INTEGER NOT NULL,
                view_time INTEGER NOT NULL,
                FOREIGN KEY (article_id) REFERENCES articles(id) ON DELETE CASCADE
            )
        """)
    }
}

val MIGRATION_2_3 = object : Migration(2, 3) {
    override fun migrate(database: SupportSQLiteDatabase) {
        // 更复杂的 schema 变更可能需要临时表
        database.execSQL("""
            CREATE TABLE temp_articles (
                id INTEGER PRIMARY KEY,
                title TEXT NOT NULL,
                content TEXT,
                author TEXT,
                create_time INTEGER,
                is_read INTEGER DEFAULT 0,
                subtitle TEXT DEFAULT ''
            )
        """)
        
        database.execSQL("INSERT INTO temp_articles (id, title, content, author, create_time, is_read, subtitle) " +
                         "SELECT id, title, content, author, create_time, is_read, subtitle FROM articles")
        
        database.execSQL("DROP TABLE articles")
        database.execSQL("ALTER TABLE temp_articles RENAME TO articles")
    }
}

// 在数据库构建器中添加迁移
Room.databaseBuilder(context, AppDatabase::class.java, "waiminutes.db")
    .addMigrations(MIGRATION_1_2, MIGRATION_2_3)
    .build()
```

# 八、协程与 Flow 集成

Room 原生支持 Kotlin 的协程和 Flow：
```kotlin
// Repository 使用协程
class ArticleRepository @Inject constructor(
    private val articleDao: ArticleDao
) {
    
    suspend fun getArticleById(id: Long): Result<Article> {
        return try {
            val article = articleDao.getArticleById(id)
            article?.let { Result.success(it) }
                ?: Result.failure(Exception("Article not found"))
        } catch (e: Exception) {
            Result.failure(e)
        }
    }
    
    // 使用 Flow 实时监控数据变化
    fun observeAllArticles(): Flow<List<Article>> = articleDao.getAllArticlesAsFlow()
    
    // 结合其他异步操作
    suspend fun refreshAndGetArticles(): List<Article> {
        // 先从网络获取数据
        val networkArticles = fetchFromNetwork()
        // 清空本地数据并插入新数据
        articleDao.deleteAllArticles()
        articleDao.insertArticles(*networkArticles.toTypedArray())
        // 返回最新数据
        // ★上面定义的 DAO 只有 getAllArticlesAsFlow()，没有 getAllArticles()
        return articleDao.getAllArticlesAsFlow().first()
    }
}

// ViewModel 收集 Flow 数据
@HiltViewModel
class ArticleViewModel @Inject constructor(
    private val articleRepository: ArticleRepository
) : ViewModel() {
    
    private val _articles = MutableStateFlow<List<Article>>(emptyList())
    val articles: StateFlow<List<Article>> = _articles.asStateFlow()
    
    init {
        viewModelScope.launch {
            articleRepository.observeAllArticles()
                .collect { articles ->
                    _articles.value = articles
                }
        }
    }
    
    fun refreshArticles() {
        viewModelScope.launch {
            articleRepository.refreshAndGetArticles()
        }
    }
}
```

# 九、测试策略

## (一) 本地 JVM 测试

使用 AndroidX Room 测试库进行快速单元测试：
```kotlin
@ExperimentalCoroutinesApi
class ArticleDaoTest {
    
    private lateinit var database: AppDatabase
    private lateinit var articleDao: ArticleDao
    
    @Before
    fun createDb() {
        val context = ApplicationProvider.getApplicationContext()
        database = Room.inMemoryDatabaseBuilder(
            context,
            AppDatabase::class.java
        ).allowMainThreadQueries().build()
        articleDao = database.articleDao()
    }
    
    @After
    fun closeDb() {
        database.close()
    }
    
    @Test
    fun `insert and get article`() = runTest {
        val article = Article(
            title = "Test Article",
            content = "Test Content",
            author = "Test Author"
        )
        
        val id = articleDao.insertArticle(article)
        assertThat(id).isGreaterThan(0L)
        
        val retrieved = articleDao.getArticleById(id)
        assertThat(retrieved).isNotNull()
        assertThat(retrieved!!.title).isEqualTo(article.title)
    }
    
    @Test
    fun `flow emits updates`() = runTest {
        // 初始状态应该是空列表
        val articlesFlow = articleDao.getAllArticlesAsFlow()
        
        // 收集第一次发射
        val first = articlesFlow.first()
        assertThat(first).isEmpty()
        
        // 插入数据后应发射新值
        launch {
            delay(10)  // 确保事务完成
            articleDao.insertArticle(Article(
                title = "Flow Test",
                content = "Testing Flow",
                author = "Tester"
            ))
        }
        
        // 第二次收集应该包含新插入的数据
        val second = articlesFlow.first()
        assertThat(second).isNotEmpty()
        assertThat(second.first().title).isEqualTo("Flow Test")
    }
}
```

## (二) 应用测试

在 Android 设备或模拟器上进行仪器化测试：
```kotlin
@RunWith(AndroidJUnit4::class)
class ArticleDaoInstrumentedTest {
    
    private lateinit var articleDao: ArticleDao
    private lateinit var database: AppDatabase
    
    @Before
    fun createDb() {
        val context = ApplicationProvider.getApplicationContext()
        database = Room.databaseBuilder(
            context,
            AppDatabase::class.java,
            "test-db"
        ).build()
        articleDao = database.articleDao()
    }
    
    @After
    fun closeDb() {
        database.close()
    }
    
    @Test
    fun `insert and retrieve article`() {
        val article = Article(
            title = "Instrumented Test",
            content = "Test Content",
            author = "Tester"
        )
        
        val id = articleDao.insertArticle(article)
        assertThat(id).isGreaterThan(0L)
        
        val retrieved = articleDao.getArticleById(id)
        assertThat(retrieved).isNotNull()
        assertThat(retrieved!!.title).isEqualTo(article.title)
    }
}
```

# 十、最佳实践与常见问题

## (一) 最佳实践

✅ 使用 `@Embedded` 处理拍平的对象  
✅ 使用 `@Relation` 处理一对多和多对多关系  
✅ 在 DAO 中使用 `suspend` 函数以利用协程  
✅ 使用 `Flow` 或 `LiveData` 实时监控数据变化  
✅ 为所有表添加适当的索引以提高查询性能  
✅ 使用事务确保多个操作的原子性  
✅ 使用 Room 的内置迁移框架处理 schema 变更  
✅ 在生产环境中导出 schema 以便审查  
✅ 使用 `@TypeConverter` 处理不支持的数据类型  
✅ 考虑使用 `Pagination 3` 配合 Room 处理大数据集  

## (二) 常见问题及解决方案

### 1. 问题：主线程异常
**原因**：在主线程执行数据库操作  
**解决方案**：
- 使用协程的 `suspend` 函数
- 使用 `withContext(Dispatchers.IO)` 
- 调试时暂时允许：`.allowMainThreadQueries()`（仅调试）
- 生产代码中绝不要在主线程执行耗时操作

### 2. 问题：无法找到实体类
**原因**：实体类未被正确注册或编译器未生成元数据  
**解决方案**：
- 确保实体类有 `@Entity` 注解
- 确保没有混用 `kapt` 和 `ksp` 
- 清理重建项目：`./gradlew cleanBuildCache`
- 检查 `build.gradle.kts` 中的注解处理器配置

### 3. 问题：迁移失败
**原因**：迁移 SQL 语句错误或未处理所有 schema 变更  
**解决方案**：
- 在测试中验证迁移：使用 `MigrationTestHelper`
- 导出 schema 并检查差异
- 考虑使用降级策略：`.fallbackToDestructiveMigration()`（仅在可以丢失数据时）
- 编写增量迁移而尝试一次性完成所有变更

### 4. 问题：关系查询返回空
**原因**：关系定义错误或外键约束未满足  
**解决方案**：
- 检查 `@Relation` 中的 `parentColumn`、`entityColumn` 和 `associateBy` 
- 确保关联表中存在对应的外键关系
- 使用 `@Transaction` 确保查询在同一事务中执行
- 验证实际数据中是否存在匹配的记录

## (三) 易错点对照

| 误区 | 事实 |
| --- | --- |
| `@Query` 只能查、不能改 | 能改。`@Query("DELETE FROM x")` / `@Query("UPDATE ...")` 都是合法的，返回值可以是受影响行数 |
| 主线程查 Room 不成问题 | 必须在后台。`suspend fun` 查询默认跑在 Room 的事务执行器（后台）；**非 suspend 的 `@Query` 才会报主线程异常** |
| `@Database(version = 1)` 永远够用 | 改了实体就要 `version + 1` 并提供 `Migration`，否则运行时会崩 |
| `fallbackToDestructiveMigration()` 是方便的兜底 | 它会**直接删库重建**。有用户数据时用它等于数据丢失，只能在「数据可丢」的缓存表上用 |
| `Flow<List<T>>` 查询会自动刷新 | 前提是表在 Room 的追踪范围内，且写操作走 Room。用 `execSQL` 直接改不会触发 |
| `@Database` 可以写成普通 class | 必须 `abstract class`。Room 生成的是它的子类，写成 `open class` 或具体类编译不过 |
| `MigrationTestHelper` 不需要导出 schema | 需要 `exportSchema = true` 才有 schema JSON 可比，否则它无法验证 |
| Room 不能跨进程共享 | 可以，但要显式开 `enableMultiInstanceInvalidation()`，并接受多进程间写入通知的延迟 |

# 十一、参考资料

- [Room 官方文档](https://developer.android.com/training/data-storage/room)
- [Room 架构指南](https://developer.android.com/topic/libraries/architecture/room)
- [Room 迁移指南](https://developer.android.com/training/data-storage/room/migrating-db-versions)
- [Room 与 Kotlin 协程](https://developer.android.com/reference/kotlin/androidx/room/package-summary)
- [Room 与 Flow](https://developer.android.com/reference/kotlin/androidx/room/package-summary)
- [Android 架构组件样例](https://github.com/android/architecture-samples)
- [Room 类型转换器指南](https://developer.android.com/reference/androidx/room/TypeConverter)
- [Room 关系映射文档](https://developer.android.com/reference/androidx/room/Relation)
- [Room 官方文档与测试](https://developer.android.com/topic/libraries/architecture/room)
- [Room 性能优化技巧](https://developer.android.com/topic/performance)
- [Room 与 Pagination 3 集成](https://developer.android.com/topic/libraries/architecture/paging/v3-overview)

> **提示**：Room 不是万能的，对于极高性能要求或复杂查询的场景，仍然可以考虑直接使用 SQLite API 或使用第三方库如 SQLDelight。但对于大多数应用场景，Room 提供了良好的抽象层显著减少了样板代码并提高了开发效率。
