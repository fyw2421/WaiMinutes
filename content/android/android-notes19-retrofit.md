---
title: "AndroidNotes19 : Retrofit 网络层"
weight: 20
description: "Android 网络请求库 Retrofit 的完整用法：REST API 接口定义、异步处理与协程集成、数据转换器配置、拦截器与日志、错误处理与重试机制、动态 URL 与查询参数、文件上传下载以及与其他库的协同使用。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么选择 Retrofit

Retrofit 是 Square 公司出品的类型安全的 HTTP 客户端，专为 Android 和 Java 设计。相比传统的 HttpURLConnection 或 Volley，Retrofit 提供：

- **类型安全**：通过接口注解定义 API，编译时检查参数和返回类型
- **轻松扩展**：支持自定义转换器（JSON、XML、Protocol Buffers等）
- **协程友好**：原生支持 Kotlin 的 suspend 函数和 Flow
- **强大的拦截器机制**：支持日志、缓存、认证、重试等功能
- **良好的生命周期管理**：与 ViewModel、LiveData 等架构组件无缝集成

# 二、基础设置

## (一) 添加依赖

在模块的 `build.gradle.kts` 中添加 Retrofit 及其必要依赖：
```kotlin
dependencies {
    // Retrofit 核心库
    implementation("com.squareup.retrofit2:retrofit:3.0.0")
    
    // JSON 转换器（这里使用 Moshi，也可以选择 Gson 或其他）
    implementation("com.squareup.retrofit2:converter-moshi:3.0.0")
    
    // 日志拦截器（调试时必备）
    implementation("com.squareup.okhttp3:logging-interceptor:5.5.0")
}
```

⚠ **协程不需要额外依赖**。Retrofit 从 2.9 起原生支持在接口方法上声明 `suspend`，底层自动用 OkHttp 的协程适配器完成请求，`await()` 是内置的。市面上不存在 `adapter-kotlin-coroutines` 这个坐标——看到它就说明文档抄错了。

## (二) 定义 API 接口

使用接口和注解来定义网络请求：
```kotlin
// ArticleApi.kt
interface ArticleApi {
    
    // GET 请求，路径参数和查询参数
    @GET("articles/{id}")
    suspend fun getArticle(
        @Path("id") articleId: String,
        @Query("format") format: String = "json"
    ): Response<Article>
    
    // POST 请求，发送 JSON 数据
    @POST("articles")
    suspend fun createArticle(
        @Body article: ArticleCreateRequest
    ): Response<Article>
    
    // 使用表单提交
    @Multipart
    @POST("articles/upload")
    // ★不要加 @Headers("Content-Type: multipart/form-data")：
    //   Retrofit 会用你写的 Content-Type 覆盖掉 body 自带的 boundary 串，
    //   服务端解析不出表单。boundary 必须由 @Multipart 自动生成
    suspend fun uploadArticle(
        @Part article: RequestBody,
        @Part image\: MultipartBody.Part
    ): Response<UploadResponse>
    
    // 流式下载大文件
    @GET("downloads/large-file")
    suspend fun downloadFile(
        @Url url: String
    ): Response<ResponseBody>
}
```

# 三、创建 Retrofit 实例

## (一) 基础配置

最基本的 Retrofit 实例创建方式：
```kotlin
// NetworkModule.kt
@Module
@InstallIn(SingletonComponent::class)
object NetworkModule {
    
    @Provides
    @Singleton
    fun provideRetrofit(
        okHttpClient: OkHttpClient,
        moshiConverterFactory: MoshiConverterFactory
    ): Retrofit {
        return Retrofit.Builder()
            .baseUrl("https://api.example.com/")   // 基础 URL，必须以斜杠结尾
            .client(okHttpClient)
            .addConverterFactory(moshiConverterFactory)
            .addCallAdapterFactory(CoroutineCallAdapterFactory()) // 协程适配器
            .build()
    }
    
    @Provides
    @Singleton
    fun provideOkHttpClient(
        loggingInterceptor: HttpLoggingInterceptor
    ): OkHttpClient {
        return OkHttpClient.Builder()
            .addInterceptor(loggingInterceptor)
            .connectTimeout(30, TimeUnit.SECONDS)
            .readTimeout(30, TimeUnit.SECONDS)
            .writeTimeout(30, TimeUnit.SECONDS)
            .build()
    }
    
    @Provides
    @Singleton
    fun provideLoggingInterceptor(): HttpLoggingInterceptor {
        return HttpLoggingInterceptor().apply {
            level = if (BuildConfig.DEBUG) 
                HttpLoggingInterceptor.Level.BODY 
            else 
                HttpLoggingInterceptor.Level.NONE
        }
    }
    
    @Provides
    @Singleton
    fun provideMoshiConverterFactory(): MoshiConverterFactory {
        return MoshiConverterFactory.create(
            Moshi.Builder().build()
        )
    }
    
    @Provides
    @Singleton
    fun provideArticleApi(retrofit: Retrofit): ArticleApi {
        return retrofit.create(ArticleApi::class.java)
    }
}
```

## (二) 动态基础 URL

有时需要根据不同环境或用户输入切换基础 URL：
```kotlin
// 使用 @Url 注解完全替换 URL
@GET
suspend fun getUserProfile(
    @Url url: String   // 完整的 URL，如 "https://api.github.com/users/octocat"
): Response<UserProfile>

// 或者使用查询参数动态拼接（不推荐，易错）
@GET("user/{username}")
suspend fun getUser(
    @Path("username") username: String,
    @Query("version") version: String? = null
): Response<User>
```

# 四、数据转换与响应处理

## (一) 转换器选择

Retrofit 支持多种数据转换器，常用选项包括：

- **Moshi**（推荐）：现代化的 JSON 库，良好的 Kotlin 支持
- **Gson**：成熟流行的 JSON 库
- **Scalar**：处理纯文本响应
- **Protobuf**：Protocol Buffers 二进制格式
- **XML**：XML 解析（Simple XML）

示例：配置 Moshi 以更好地支持 Kotlin：
```kotlin
@Provides
@Singleton
fun provideMoshiConverterFactory(): MoshiConverterFactory {
    val moshi = Moshi.Builder()
        // 添加 Kotlin 反射适配器，处理 data class 和默认值
        .add(KotlinJsonAdapterFactory())
        .build()
    return MoshiConverterFactory.create(moshi)
}
```

## (二) 响应包装

使用 `Response<T>` 获取完整的 HTTP 响应信息：
```kotlin
// 在 ViewModel 或 Repository 中
suspend fun fetchArticle(id: String): Result<Article> {
    return try {
        val response = articleApi.getArticle(id)
        if (response.isSuccessful) {
            response.body()?.let { Result.success(it) }
                ?: Result.failure(Exception("Empty response body"))
        } else {
            // 处理 HTTP 错误状态码
            val errorMessage = response.errorBody()?.string()
                ?: "Unknown error with code ${response.code()}"
            Result.failure(Exception("HTTP ${response.code()}: $errorMessage"))
        }
    } catch (e: IOException) {
        // 网络异常
        Result.failure(Exception("Network error: ${e.message}"))
    } catch (e: HttpException) {
        // 非 2xx 响应（已被上面的 isSuccessful 捕获，但以防万一）
        Result.failure(Exception("HTTP error: ${e.code()} ${e.message()}"))
    }
}
```

或者使用自定义包装类：
```kotlin
data class ApiResponse<T>(
    val isSuccess: Boolean,
    val code: Int,
    val data: T?,
    val message: String?,
    val errors: Map<String, List<String>>? = null
)

// 在拦截器中统一处理
class ApiResponseInterceptor : Interceptor {
    override fun intercept(chain: Interceptor.Chain): Response {
        val originalResponse = chain.proceed(chain.request())
        // 这里可以根据业务需求包装响应
        return originalResponse
    }
}
```

# 五、拦截器与进阶功能

## (一) 日志拦截器

调试网络请求的必备工具：
```kotlin
fun provideLoggingInterceptor(): HttpLoggingInterceptor {
    return HttpLoggingInterceptor { message ->
        Log.d("Retrofit", message)
    }.apply {
        level = HttpLoggingInterceptor.Level.BASIC  // 或 HEADERS, BODY
    }
}
```

# 六、认证、缓存与重试

## (一) 认证拦截器

自动添加认证头部：
```kotlin
class AuthInterceptor(
    private val authRepository: AuthRepository
) : Interceptor {
    
    override fun intercept(chain: Interceptor.Chain): Response {
        val original = chain.request()
        
        val token = authRepository.getAuthToken()
        val authorized = original.newBuilder()
            .header("Authorization", "Bearer $token")
            .method(original.method(), original.body())
            .build()
        
        return chain.proceed(authorized)
    }
}

// 在 OkHttpClient 构建时添加
.okHttpClientBuilder.addInterceptor(AuthInterceptor(authRepository))
```

## (二) 缓存拦截器

实现智能缓存策略：
```kotlin
@Provides
@Singleton
fun provideCache(cacheDir: File): Cache {
    // 10 MB 缓存空间
    return Cache(cacheDir, 10 * 1024 * 1024)
}

@Provides
@Singleton
fun provideCacheControlInterceptor(cache: Cache): Interceptor {
    return Chain { chain ->
        val response = chain.proceed(chain.request())
        // 根据业务需求设置缓存头
        val cacheControl = if (isNetworkAvailable()) {
            // 有网时：先尝试缓存，失败则请求网络
            CacheControl.Builder()
                .maxAge(0, TimeUnit.SECONDS)  // 强制重新验证
                .maxStale(1, TimeUnit.DAYS)   // 最多容忍1天过期
                .build()
        } else {
            // 无网时：强制使用缓存
            CacheControl.Builder()
                .onlyIfCached()
                .maxStale(7, TimeUnit.DAYS)   // 一周内的缓存都可用
                .build()
        }
        
        val newResponse = response.newBuilder()
            .removeHeader("Pragma")
            .removeHeader("Cache-Control")
            .header("Cache-Control", cacheControl.toString())
            .build()
        
        return newResponse.proceed()
    }
}
```

## (三) 重试机制

实现请求失败时的自动重试：
```kotlin
class RetryInterceptor(
    private val maxRetries: Int = 3,
    private val retryDelayMillis: Long = 1000
) : Interceptor {
    
    override fun intercept(chain: Interceptor.Chain): Response {
        var attempt = 0
        while (attempt <= maxRetries) {
            try {
                val response = chain.proceed(chain.request())
                // 只在网络异常或特定状态码时重试
                if (!shouldRetry(response)) {
                    return response
                }
                
                // 等待后重试
                Thread.sleep(retryDelayMillis * (attempt + 1)) // 指数退避可以考虑
                attempt++
            } catch (e: IOException) {
                if (attempt >= maxRetries) throw e
                Thread.sleep(retryDelayMillis * (attempt + 1))
                attempt++
            }
        }
        // 这里理论上不会到达，因为上面会抛出异常或返回响应
        return chain.proceed(chain.request())
    }
    
    private fun shouldRetry(response: Response): Boolean {
        // 重试条件：网络异常、5xx 错误、特定的 4xx 错误等
        return !(response.isSuccessful) && 
               (response.code() == 500 || 
                response.code() == 502 || 
                response.code() == 503 || 
                response.code() == 504)
    }
}
```

# 七、实际应用场景

## (一) 在 Repository 中使用

典型的 Repository 实现模式：
```kotlin
class ArticleRepository @Inject constructor(
    private val articleApi: ArticleApi,
    private val dispatcher: Dispatcher
) {
    
    suspend fun getArticleDetail(id: String): Result<Article> {
        return withContext(dispatcher.io) {
            articleApi.getArticle(id).let { response ->
                if (response.isSuccessful) {
                    response.body()?.let { Result.success(it) }
                        ?: Result.failure(Exception("Empty response"))
                } else {
                    Result.failure(Exception("HTTP ${response.code()}"))
                }
            }
        }
    }
    
    suspend fun createArticle(article: ArticleCreateRequest): Result<Article> {
        // ★withContext 的 block 是非 inline 的 suspend lambda，
        //   块里裸写 return 是非局部返回，编译不过。用 return@withContext，
        //   或者直接把 withContext 作为整个函数的返回值
        return withContext(dispatcher.io) {
            try {
                val response = articleApi.createArticle(article)
                if (response.isSuccessful) {
                    response.body()?.let { Result.success(it) }
                        ?: Result.failure(Exception("Empty response"))
                } else {
                    val errorMsg = response.errorBody()?.string()
                        ?: "Unknown error"
                    Result.failure(Exception("HTTP ${response.code()}: $errorMsg"))
                }
            } catch (e: IOException) {
                Result.failure(Exception("Network error: ${e.message}"))
            }
        }
    }
}
```

## (二) 在 ViewModel 中使用

结合 Kotlin Flow 和 StateFlow 进行响应式更新：
```kotlin
@HiltViewModel
class ArticleViewModel @Inject constructor(
    private val articleRepository: ArticleRepository,
    private val savedStateHandle: SavedStateHandle
) : ViewModel() {
    
    private val _articleUiState = MutableStateFlow<UiState<Article>>(UiState.Loading)
    val articleUiState: StateFlow<UiState<Article>> = _articleUiState.asStateFlow()
    
    fun loadArticle(articleId: String) {
        viewModelScope.launch {
            _articleUiState.value = UiState.Loading
            val result = articleRepository.getArticleDetail(articleId)
            _articleUiState.value = when (result) {
                is Result.Success -> UiState.Success(result.getOrNull())
                is Result.Failure -> UiState.Error(result.exceptionOrNull() ?: Exception("Unknown error"))
            }
        }
    }
    
    // 使用 Flow 进行实时更新
    fun observeArticleUpdates(articleId: String): Flow<Article> = callbackFlow {
        // 这里可以结合 WebSocket 或其他实时推送机制
        // 为示例，我们简单轮询（实际项目中不推荐频繁轮询）
        // ★getOrNull() 返回 Article?，而 Flow<Article> 的 trySend 要非空值
        articleRepository.getArticleDetail(articleId).getOrNull()?.let { trySend(it) }
        awaitClose()
    }
}

// UI 层收集数据
@Composable
fun ArticleDetailScreen(
    articleId: String,                                   // ★作为参数传进来
    viewModel: ArticleViewModel = hiltViewModel(),
    navController: NavHostController
) {
    // ★原来这里用 savedStateHandle，但 Composable 拿不到它——
    //   它属于 ViewModel，或由导航层显式传参
    val uiState by viewModel.articleUiState.collectAsStateWithLifecycle()
    
    when (uiState) {
        is UiState.Loading -> CircularProgressIndicator(modifier = Modifier.fillMaxSize())
        is UiState.Success -> {
            val article = uiState.data ?: return
            ArticleContent(article = article)
        }
        is UiState.Error -> ErrorMessage(message = uiState.error?.localizedMessage ?: "未知错误")
    }
}
```

# 八、错误处理与调试

## (一) 统一异常处理

创建统一的异常处理机制：
```kotlin
sealed class NetworkException : Exception() {
    data class TimeoutException(val timeoutDuration: Long) : NetworkException()
    data class NoInternetException : NetworkException()
    data class ServerErrorException(val statusCode: Int, val message: String?) : NetworkException()
    data class ParseException(val cause: Throwable) : NetworkException()
    data class UnknownException(val cause: Throwable) : NetworkException()
}

// 在拦截器中转换异常
class ExceptionCapturingInterceptor : Interceptor {
    override fun intercept(chain: Interceptor.Chain): Response {
        try {
            return chain.proceed(chain.request())
        } catch (e: UnknownHostException) {
            throw NetworkException.NoInternetException()
        } catch (e: SocketTimeoutException) {
            throw NetworkException.TimeoutException(30_000) // 从配置中读取更好
        } catch (e: IOException) {
            throw NetworkException.UnknownException(e)
        }
    }
}
```

## (二) 调试技巧

1. **使用 Stetho 或 Chuck**：查看网络请求详情
2. **启用详细日志**：在开发版本中使用 BODY 级别日志
3. **检查握手过程**：SSL/TLS 问题的调试
4. **模拟网络条件**：使用 Android Studio 的网络延缓功能
5. **检查协议版本**：HTTP/1.1 vs HTTP/2 的性能差异

# 九、与其他库的协同使用

## (一) 与 Coil 结合图片加载

Retrofit 下载图片 URL，Coil 负责实际加载：
```kotlin
// API 接口
@GET("users/{userId}/avatar")
suspend fun getUserAvatarUrl(
    @Path("userId") userId: String
): Response<AvatarResponse>

// 数据类
data class AvatarResponse(val url: String)

// 在 ViewModel 中
val avatarUrl: StateFlow<String?> = callbackFlow {
    try {
        val response = userApi.getUserAvatarUrl(userId)
        if (response.isSuccessful) {
            response.body()?.url?.let { trySend(it) }
        }
    } catch (e: Exception) {
        // 处理错误
    }
    awaitClose()
}

// 在 Compose 中
AsyncImage(
    model = ImageRequest.Builder(LocalContext.current)
        .data(avatarUrl.collectAsState().value)
        .crossfade(true)
        .placeholder(placeholder = painterResource(R.drawable.placeholder_avatar))
        .error(painterResource(R.drawable.error_avatar))
        .build(),
    contentDescription = null,
    modifier = Modifier.size(80.dp)
)
```

## (二) 与 WorkManager 结合后台同步

使用 Retrofit 执行网络请求，WorkManager 管理执行时机：
```kotlin
// Worker 实现
class SyncWorker(
    appContext: Context,
    workerParams: WorkerParameters,
    private val articleRepository: ArticleRepository
) : CoroutineWorker(appContext, workerParams) {
    
    override suspend fun doWork(): Result {
        return try {
            val result = articleRepository.syncLatestArticles()
            when (result) {
                is Result.Success -> Result.success()
                is Result.Failure -> Result.retry()   // 或 Result.failure() 取决于业务
            }
        } catch (e: Exception) {
            Result.retry()
        }
    }
}

// 在需要触发同步时
val syncWork = OneTimeWorkRequestBuilder<SyncWorker>()
    .setBackoffCriteria(
        BackoffPolicy.EXPONENTIAL,
        Duration.ofSeconds(30),
        TimeUnit.SECONDS
    )
    .build()

WorkManager.getInstance(context)
    .enqueueUniqueWork(
        "article_sync",
        ExistingWorkPolicy.KEEP,
        syncWork
    )
```

# 十、性能优化与最佳实践

## (一) 单例原则

确保整个应用只有一个 Retrofit 实例：
```kotlin
// 正确做法：通过 Hilt 单例提供
@Module
@InstallIn(SingletonComponent::class)
object NetworkModule {
    @Provides
    @Singleton
    fun provideRetrofit(...): Retrofit { /* ... */ }
}

// 错误做法：在每个地方都创建新实例
// class SomeRepository {
//     private val retrofit = Retrofit.Builder() // ❌ 创建多个实例
//         .baseUrl(BASE_URL)
//         .build()
//     // ...
// }
```

## (二) 连接池复用

OkHttp 自带连接池，确保正确配置：
```kotlin
// 共享连接池（可选，通常默认就好）
val sharedConnectionPool = ConnectionPool(
    maxIdleConnections = 5,
    keepAliveDuration = 5L, TimeUnit.MINUTES
)

OkHttpClient.Builder()
    .connectionPool(sharedConnectionPool)
    // ... 其他配置
    .build()
```

## (三) 请求压缩

⚠ **一般不需要自己做请求压缩**。OkHttp 默认就会发 `Accept-Encoding: gzip` 并在响应侧自动解压。只有在服务端要求压缩**请求体**时才需要自己实现，而且要注意：

- **请求侧压缩用的请求头是 `Content-Encoding`**，不是 `Accept-Encoding`（后者用于「我接受什么压缩格式」的响应协商）
- 真正的实现必须把流压缩掉，不能原样返回

```kotlin
private fun RequestBody.gzip(): RequestBody = object : RequestBody() {
    override fun contentType(): MediaType? = this@gzip.contentType()
    override fun contentLength(): Long = -1L                       // 长度未知
    override fun writeTo(sink: BufferedSink) {
        val gzipSink = GzipSink(sink).buffer()
        // ★原样 return this 等于没压缩，只是换了个壳
        this@gzip.writeTo(gzipSink)
        gzipSink.flush()
    }
}
```

⚠ **加了 `Content-Encoding: gzip` 后，服务端必须支持解压**——不支持的话请求体会变成乱码，通常表现为 400/415。

## (四) 减少不必要的请求

实现智能缓存和条件请求：
```kotlin
// 使用 ETag 或 Last-Modified 进行条件请求
@GET("articles/{id}")
suspend fun getArticleWithETag(
    @Path("id") id: String,
    @Header("If-None-Match") etag: String? = null
): Response<Article>

// 在拦截器中自动处理 ETag
class ETagCacheInterceptor(
    private val etagCache: EtagCache   // 自定义缓存实现
) : Interceptor {
    override fun intercept(chain: Interceptor.Chain): Response {
        val request = chain.request()
        val url = request.url.toString()
        
        // 从缓存获取 ETag
        val savedEtag = etagCache.get(url)
        
        var taggedRequest = request
        if (savedEtag != null) {
            taggedRequest = request.newBuilder()
                .header("If-None-Match", savedEtag)
                .build()
        }
        
        val originalResponse = chain.proceed(taggedRequest)
        
        // 处理 304 未修改响应
        if (originalResponse.code() == 304) {
            val cachedResponse = etagCache.getCachedResponse(url)
            return cachedResponse ?: originalResponse
        }
        
        // 保存新的 ETag 和响应
        val newEtag = originalResponse.header("ETag")
        if (newEtag != null) {
            etagCache.put(url, newEtag, originalResponse)
        }
        
        return originalResponse
    }
}
```

# 十一、常见问题及解决方案

## (一) SSL 握手失败

原因：服务器证书不受信任、域名不匹配或协议版本不兼容
解决方案：
- 检查服务器证书是否有效且受信任
- 对于开发环境，可以使用调试证书（但生产环境绝不能这样做）
- 确保支持的 TLS 版本匹配（通常需要 TLS 1.2+）
```kotlin
// 调试时仅用于信任开发证书（危险！生产环境禁止）
OkHttpClient.Builder()
    .certificatePinner(CertificatePinner.Builder()
        .add("api.example.com", "sha256/AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA=")
        .build())
    .build()
```

## (二) JSON 解析异常

原因：实际返回的 JSON 与数据类不匹配
解决方案：
- 检查实际 API 返回（使用日志拦截器或网络调试工具）
- 使用宽松的数据类定义（可空类型、默认值）
- 自定义 JsonAdapter 处理特殊情况
```kotlin
@JsonClass(generateAdapter = true)
data class Article(
    val id: String,
    val title: String,
    @Json(name = "publish_time") val publishTime: Long?,  // 处理字段名映射
    val author: Author? = null,                           // 提供默认值
    @Json(name = "read_count") val readCount: Int = 0     // 默认值
)

// 自定义适配器处理特殊类型
@JsonQualifier
annotation class TimestampAsDate

@JsonAdapter(TimestampAsDate::class)
class TimestampAsDateAdapter : JsonAdapter<Date>() {
    override fun fromJson(reader: JsonReader): Date {
        val timestamp = reader.nextLong()
        return Date(timestamp * 1000)  // 假设后端返回秒级时间戳
    }
    
    override fun toJson(writer: JsonWriter, value: Date) {
        writer.value(value.time / 1000)  // 转回秒级
    }
}

// 使用自定义注解
data class Article(
    // ...
    @TimestampAsDate
    val publishTime: Date
)
```

## (三) 请求频率限制（429 错误）

原因：触发了服务器的速率限制
解决方案：
- 实现指数退避重试策略
- 添加请求队列或令牌桶限流
- 向后端申请更高的配额或使用批量接口
```kotlin
class RateLimitInterceptor(
    private val maxRequestsPerMinute: Int = 60,
    private val retryBaseDelay: Long = 1000
) : Interceptor {
    
    // ★Kotlin 没有 MutableList() 零参工厂（只有 mutableListOf<T>()）
    private val requestTimestamps = Collections.synchronizedList(mutableListOf<Long>())
    
    override fun intercept(chain: Interceptor.Chain): Response {
        val now = System.currentTimeMillis()
        
        // 清理过期记录（超过1分钟的）
        requestTimestamps.removeIf { now - it > 60_000 }
        
        // 检查是否超过限制
        if (requestTimestamps.size >= maxRequestsPerMinute) {
            val earliest = requestTimestamps.minOrNull() ?: 0L
            val waitTime = Math.max(0, 60_000 - (now - earliest))
            
            // 等待后重试（简化实现，实际应该更优雅）
            Thread.sleep(waitTime + retryBaseDelay)
        }
        
        // 记录此次请求时间
        requestTimestamps.add(now)
        
        return chain.proceed(chain.request())
    }
}
```

## (四) 易错点对照

| 误区 | 事实 |
| --- | --- |
| Retrofit 需要 `adapter-kotlin-coroutines` 才能用协程 | 不存在这个坐标。2.9 起接口方法直接写 `suspend` 即可 |
| 每个 `Retrofit.Builder()` 都新建一个实例 | 应当全局单例，每次新建都会白建一个 OkHttp 连接池 |
| `response.body()` 就是响应数据 | 它可能为 `null`（解析失败或 204），要判空 |
| HTTP 非 2xx 会抛异常 | Retrofit 不抛。`response.isSuccessful` 才是判断依据，异常只有网络层故障 |
| `Call.execute()` 可以在主线程调 | 会抛 `NetworkOnMainThreadException`，必须包在 `suspend` 或 `Dispatchers.IO` 里 |
| 加了 `@Query` 就一定出现在请求里 | 参数为 `null` 时整个键值对会被省略 |
| 用 `@Body` 传 `application/json` 之外的内容 | 要同时加 `@Headers("Content-Type: ...")`，否则序列化成 JSON |
| `@Path` 可以传含 `/` 的值 | 会被当成路径分隔符，需先 `Uri.encode` |
| 每个接口都单独写一个 `Api` 接口 | 按后端资源划分接口即可，过度拆分只是徒增编译产物 |

# 十二、参考资料

- [Retrofit 官方文档](https://github.com/square/retrofit)
- [OkHttp 官方文档](https://github.com/square/okhttp)
- [Moshi 官方文档](https://github.com/square/moshi)
- [Kotlin 协程指南](https://kotlinlang.org/docs/coroutines-guide.html)
- [Android 架构组件](https://developer.android.com/topic/architecture)
- [Restful API 设计指南](https://restfulapi.net/)
- [HTTP/2 解释](https://http2.github.io/faq/)
- [Android 性能优化指南](https://developer.android.com/topic/performance)
- [网络安全最佳实践](https://developer.android.com/training/articles/security-ssl)
- [移动网络优化技巧](https://developer.android.com/training/efficient-downloads)

> **提示**：随着 Android 生态的发展，新的网络库和模式不断涌现。虽然 Retrofit 仍然是主流选择，但也值得关注 [Ktor](https://ktor.io/) 和 [Fuel](https://github.com/kittinunf/Fuel) 等现代替代方案，特别是在纯 Kotlin 多平台项目中。
