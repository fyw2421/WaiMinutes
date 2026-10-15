---
title: "AndroidNotes17 : 依赖注入 Hilt"
weight: 18
description: "Hilt 在 Android 中的完整用法：依赖图与作用域、限定符绑定 Dispatcher、@HiltViewModel 与 hiltViewModel、自定义组件与@EntryPoint、模块拆分与 KSP 迁移，以及 Field Injection 与构造注入的取舍和常见编译错误。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、Hilt 解决什么问题

不用 Hilt 时，依赖是这样手写的：

```kotlin
class ArticleViewModel(
    private val repository: ArticleRepository,
    private val savedState: SavedStateHandle
) : ViewModel() {
    companion object {
        val Factory = object : ViewModelProvider.Factory {
            override fun <T : ViewModel> create(modelClass: Class<T>): T {
                val app = getApplication<MyApp>()
                val api = Retrofit.Builder().baseUrl(BASE_URL).build()
                    .create(ArticleApi::class.java)
                val db = Room.databaseBuilder(app, ArticleDb::class.java, "db").build()
                val repo = ArticleRepositoryImpl(api, db.articleDao())
                return ArticleViewModel(repo, createSavedStateHandle()) as T
            }
        }
    }
}
```

问题很明显：**依赖树在 ViewModel 里重建**。换个界面要重来一遍，测试时没法替换，Gradle 升级还会挂。

Hilt 把这棵依赖树收进编译期生成的代码里。

# 二、搭建

```kotlin
// app/build.gradle.kts
plugins {
    alias(libs.plugins.kotlin.ksp)
    alias(libs.plugins.ksp.hilt)      // com.google.devtools.ksp 插件的 Hilt 扩展
    alias(libs.plugins.hilt)          // com.google.dagger.hilt.android
}

dependencies {
    implementation("com.google.dagger:hilt-android:2.60.1")
    ksp("com.google.dagger:hilt-compiler:2.60.1")               // ★ksp，不是 kapt

    implementation("androidx.hilt:hilt-navigation-compose:1.4.0")
    ksp("androidx.hilt:hilt-compiler:1.4.0")                    // ★这个库从 1.4.x 才支持 KSP
}
```

```kotlin
@HiltAndroidApp
class MyApp : Application()
```

⚠ **必须用 `ksp(...)`，不能用 `kapt`**。Dagger 的 KSP 支持从 2.60 起稳定，且速度明显更快（KAPT 要先生成 Java 存根，KSP 直接处理 Kotlin 源码）。

⚠ **`androidx.hilt:hilt-compiler` 只有 1.4.x 才支持 KSP**，更早的版本只能退回 kapt。Hilt 1.4.0 还要求 Kotlin Gradle Plugin ≥ 2.2.0。

# 三、三类绑定

## (一) 构造函数注入

```kotlin
class ArticleViewModel @Inject constructor(
    private val repository: ArticleRepository
) : ViewModel()
```

**能用构造函数注入就用构造函数注入**——依赖显式、可测、无隐藏。

## (二) 字段注入（@AndroidEntryPoint）

```kotlin
@AndroidEntryPoint
class MainActivity : ComponentActivity() {

    @Inject
    lateinit var repository: ArticleRepository      // ★字段注入

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        repository.load()          // 注入在 super.onCreate 之前已完成
    }
}
```

⚠ **`lateinit` 意味着「在注入完成前访问会崩」**。字段注入应该只用在框架回调（`onCreate`、生命周期方法）里，不要把它当普通属性用。

**Compose 里不应该用字段注入**，用参数传递：

```kotlin
@Composable
fun ArticleRoute(
    repository: ArticleRepository,      // ★显式参数
    viewModel: ArticleViewModel = hiltViewModel()
)
```

## (三) 模块

```kotlin
@Module
@InstallIn(SingletonComponent::class)
object NetworkModule {

    @Provides
    @Singleton
    fun provideOkHttp(): OkHttpClient =
        OkHttpClient.Builder()
            .connectTimeout(10, TimeUnit.SECONDS)
            .build()

    @Provides
    @Singleton
    fun provideRetrofit(client: OkHttpClient): Retrofit =
        Retrofit.Builder()
            .baseUrl(BuildConfig.API_BASE_URL)
            .client(client)
            .build()

    @Provides
    @Singleton
    fun provideArticleApi(retrofit: Retrofit): ArticleApi =
        retrofit.create(ArticleApi::class.java)
}
```

**`@Module` 三要素**：

| 注解 | 作用 |
| --- | --- |
| `@Module` | 声明这是个模块 |
| `@InstallIn(XXXComponent)` | 这个模块装到哪个组件的依赖图里 |
| `@Provides` / `@Binds` | 提供依赖的方式 |

# 四、作用域

作用域决定「一个依赖活多久、几份」：

```kotlin
@Singleton                                    // ★整个进程一份
class ArticleRepositoryImpl @Inject constructor(...)

@ActivityScoped                                // 每个 Activity 一份
class ActivityStateHolder @Inject constructor(...)

@ViewModelScoped                               // 每个 ViewModel 一份
class FormStateKeeper @Inject constructor(...)
```

```kotlin
@Module
@InstallIn(SingletonComponent::class)
object RepositoryModule {

    @Provides
    @Singleton                                       // ★与类上的 @Singleton 呼应
    fun provideArticleRepository(
        api: ArticleApi,
        dao: ArticleDao
    ): ArticleRepository = ArticleRepositoryImpl(api, dao)
}
```

⚠ **`@Singleton` 必须两处都有**：`@InstallIn(SingletonComponent::class)` 和 `@Provides`/`@Singleton`。只写一处不生效。

⚠ **作用域要匹配组件**。在 `SingletonComponent` 里提供 `@ActivityScoped` 的依赖会编译报错——组件有层级，作用域不能乱放。

## (一) 组件层级

```
SingletonComponent           进程级
├── ActivityRetainedComponent   Activity 重建时保留
│   └── ViewModelComponent          每个 ViewModel
└── ActivityComponent          每个 Activity
    └── FragmentComponent
```

⚠ **`@ActivityRetainedComponent` 和 `ActivityComponent` 的区别**：配置变更（旋转）时，`ActivityComponent` 会重建、`ActivityRetainedComponent` 会保留。需要跨旋转保留的依赖放前者。

# 五、限定符

同一个类型要提供多个实现时，用限定符区分：

```kotlin
@Qualifier
@Retention(AnnotationRetention.BINARY)
annotation class IoDispatcher

@Qualifier
@Retention(AnnotationRetention.BINARY)
annotation class DefaultDispatcher

@Module
@InstallIn(SingletonComponent::class)
object CoroutineModule {

    @Provides
    @IoDispatcher
    fun provideIoDispatcher(): CoroutineDispatcher = Dispatchers.IO

    @Provides
    @DefaultDispatcher
    fun provideDefaultDispatcher(): CoroutineDispatcher = Dispatchers.Default
}
```

```kotlin
class ArticleRepository @Inject constructor(
    private val api: ArticleApi,
    @IoDispatcher private val io: CoroutineDispatcher      // ★限定符
)
```

⚠ **没有限定符时 Hilt 不知道该给哪个**，会报 "multiple bindings" 或直接绑定到唯一那个。

## (一) 泛型限定符

集合类型需要区分：

```kotlin
@Qualifier
@Retention(AnnotationRetention.BINARY)
@Qualifier
annotation class StringMap

@Provides
@StringMap
fun provideStringMap(): Map<String, String> = mutableMapOf()
```

# 六、Hilt 与 ViewModel

```kotlin
@HiltViewModel
class ArticleViewModel @Inject constructor(
    private val repository: ArticleRepository
) : ViewModel() {
    // ...
}
```

```kotlin
@Composable
fun ArticleRoute(viewModel: ArticleViewModel = hiltViewModel()) {
    // 从当前 ViewModelStoreOwner（Navigation 的 back stack entry）取
}
```

`@HiltViewModel` 让 Hilt 知道怎么构造它，并把 `SavedStateHandle` 一并注入。

⚠ **`@HiltAndroidApp` 和 `@AndroidEntryPoint` 必须都标**，少一个会出现「字段未注入」或整个依赖图为空。

## (一) ViewModel 里注入非默认作用域依赖

`@ViewModelScoped` 的依赖可以注入 `@HiltViewModel`；反过来不行：

```kotlin
@ViewModelScoped
class DraftEditor @Inject constructor() { }

// ❌ 编译错误：SingletonComponent 里找不到 ViewModelScoped 的东西
@Singleton
class BadService @Inject constructor(val editor: DraftEditor)
```

# 七、自定义组件与 EntryPoint

需要在库模块里注入、又不想让整个库都变成 Hilt 模块时，用 `@EntryPoint`：

```kotlin
@EntryPoint
@InstallIn(SingletonComponent::class)
interface AnalyticsEntryPoint {
    fun analytics(): AnalyticsTracker
}

// 在任意地方取
EntryPointAccessors.fromApplication(appContext, AnalyticsEntryPoint::class.java)
    .analytics().track("event")
```

```kotlin
class MyModule {
    companion object {
        @Provides
        @Singleton
        fun provideTracker(): AnalyticsTracker = FirebaseTracker()
    }
}
```

⚠ **`@EntryPoint` 的接口方法要能被 Hilt 满足**——返回的类型必须在同一个依赖图里存在。

# 八、模块拆分

模块按**数据来源**拆，不要按技术类型拆：

```kotlin
// ✅ 按能力拆
network/          依赖 network
database/         依赖 database
repository/       依赖 network + database
feature-article/  依赖 repository
```

```
:core:network  → Retrofit、OkHttp
:core:database → Room
:core:data     → Repository 实现（依赖上面两个）
:feature:article → ViewModel、Composable（依赖 core:data）
```

⚠ **技术类型拆分**（`utils/`、`models/`、`network/`）看起来整齐，实际会让依赖关系一团乱——`utils` 要依赖谁？改一个功能要动几个模块？

# 九、KSP 迁移

从 kapt 迁到 KSP 的检查清单：

- [ ] `kapt { correctErrorTypes = true }` 不再需要（KSP 没有类型错误的中间层）
- [ ] 用到的第三方注解处理器要支持 KSP（尤其生成 `AutoFactory` 的）
- [ ] `dagger.hilt.android.plugin` 的 `enableAggregatingTask` 建议关闭
- [ ] 确认所有 `ksp(...)` 依赖都写对了配置名（不是 `kapt`）

```kotlin
ksp {
    arg("dagger.hilt.android.plugin.enableAggregatingTask", "true")
}
```

⚠ **不是所有注解处理器都支持 KSP**。如果某个库只提供 kapt 版本，且它的生成类需要被 Dagger 检查，会报 "could not be resolved"。查该库的文档确认。

# 十、常见编译错误

| 报错 | 原因 | 处理 |
| --- | --- | --- |
| `@AndroidEntryPoint` 类的字段没注入 | Activity 忘了 `super.onCreate` 前注入，或 Application 缺 `@HiltAndroidApp` | 检查两处注解 |
| `[Dagger/MissingBinding] X cannot be provided` | 没人提供 `X` | 写 `@Provides` 模块 |
| `[Dagger/MissingBinding] @IoDispatcher` | 有多个 `CoroutineDispatcher` 绑定 | 补限定符 |
| `Scope @ActivityScoped may not reference...` | 作用域层级搞反了 | 放到下层组件 |
| `@Inject field cannot be private` | 字段注入不能是 private | 去掉 `private` |
| `@AndroidEntryPoint ... does not have a valid Hilt Android Application` | Application 没加 `@HiltAndroidApp` | 补上 |
| 卸载 Hilt 后进程崩溃 | 混用了 `AndroidInjection.inject()` | 去掉手动调用 |

# 十一、易错点

| 误区 | 事实 |
| --- | --- |
| Hilt 是 Koin 的替代品 | 两者定位不同：Hilt 编译期生成、零反射；Koin 运行期反射 |
| Compose 里要用字段注入 | 应该用参数传递 |
| `@Singleton` 写一个就够了 | 要 `@InstallIn(SingletonComponent)` + `@Provides`/`@Singleton` 两处 |
| 什么都要注入 | 单例工具类直接 object 就行，注入有编译期成本 |
| kapt 和 KSP 随便选 | Dagger KSP 2.60 起稳定、`hilt-compiler` 要 1.4.x，优先 KSP |
| 每个接口都要写 `@Binds` | 只有多实现时才需要，单实现用 `@Provides` |
| `@ActivityRetainedComponent` 和 `@ActivityComponent` 一样 | 前者跨配置变更保留，后者不保留 |

# 十二、参考资料

- [Hilt 官方文档](https://developer.android.com/training/dependency-injection/hilt-android) — 依赖注入基础与 Hilt
- [Dagger Hilt 发布记录](https://developer.android.com/training/dependency-injection/hilt-android) — 版本与变更
- [androidx.hilt 发布记录](https://developer.android.com/jetpack/androidx/releases/hilt) — AndroidX Hilt 扩展库版本
- [Dagger KSP 指南](https://dagger.dev/dev-guide/ksp.html) — 从 kapt 迁移到 KSP 的官方步骤
