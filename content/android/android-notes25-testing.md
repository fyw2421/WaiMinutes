---
title: "AndroidNotes25 : 测试"
weight: 25
description: "Android 应用测试完整指南：单元测试、仪器化测试、UI 测试、测试驱动开发（TDD）、Mock 框架、协程测试、测试覆盖率、持续集成以及与现代架构的协同使用。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-14
tags: ["Android"]
---

# 一、为什么需要测试

测试是确保应用质量的关键实践，提供：

- **早期发现缺陷**：在开发过程中尽早发现和修复问题
- **回归防护**：确保新功能不会破坏现有功能
- **设计指导**：测试驱动开发促进更好的代码设计
- **文档作用**：测试用例展示了代码的预期行为
- **信心和速度**：有良好测试覆盖的代码库能够更快速地迭代
- **用户满意度**：减少崩溃和错误，提升用户体验

Android 提供了完整的测试生态系统，支持从单元测试到端到端 UI 测试的各种层次。

# 二、测试类型与金字塔

## (一) 测试金字塔

理想的测试分布应遵循测试金字塔原则：
```
                    UI 测试 (E2E)
                       ▲
              ┌───────────────┐
              │  集成测试     │
              └───────────────┘
                       ▲
              ┌───────────────┐
              │  单元测试     │
              └───────────────┘
```
- **单元测试**（底层，数量最多）：测试单个函数或类，快速、可靠
- **集成测试**（中间层）：测试多个组件之间的交互
- **UI 测试**（顶层，数量最少）：测试完整的用户界面流程，较慢但最真实

## (二) Android 测试分类

Android 测试主要分为两类：

1. **本地 JVM 测试（Instrumented Tests）**
   - 在本地 JVM 上运行，不需要 Android 设备或模拟器
   - 速度快，适合单元测试和纯 Java/Kotlin 逻辑测试
   - 存放在 `src/test/java/` 目录
   - 使用 JUnit、Mockito、Truth 等框架

2. **仪器化测试（Instrumented Tests）**
   - 在 Android 设备或模拟器上运行
   - 能够访问 Android 框架 API
   - 适合需要 Android 组件的测试
   - 存放在 `src/androidTest/java/` 目录
   - 使用 AndroidJUnit4、Espresso、UI Automator 等框架

# 三、测试依赖设置

在模块的 `build.gradle.kts` 中添加测试依赖：
```kotlin
dependencies {
    // 本地 JVM 测试
    testImplementation("junit:junit:4.13.2")
    testImplementation("org.mockito:mockito-core:5.0.0")
    testImplementation("org.mockito.kotlin:mockito-kotlin:6.4.0")
    testImplementation("com.google.truth:truth:1.4.5")
    testImplementation("org.jetbrains.kotlinx:kotlinx-coroutines-test:1.11.0")
    testImplementation("org.junit.jupiter:junit-jupiter:5.10.0")
    
    // 仪器化测试
    androidTestImplementation("androidx.test:core:1.7.0")
    androidTestImplementation("androidx.test:runner:1.7.0")
    androidTestImplementation("androidx.test:rules:1.7.0")
    androidTestImplementation("androidx.test.ext:junit:1.3.0")
    androidTestImplementation("androidx.test.ext:junit-ktx:1.3.0")
    androidTestImplementation("androidx.test.espresso:espresso-core:3.7.0")
    androidTestImplementation("androidx.test.espresso:espresso-contrib:3.7.0")
    androidTestImplementation("androidx.test.espresso:espresso-intents:3.7.0")
    androidTestImplementation("androidx.test.espresso:idling-resource:3.7.0")
    androidTestImplementation("com.google.truth:truth:1.4.5")
    androidTestImplementation("org.mockito:mockito-android:5.0.0")
    
    // 架构组件测试
    testImplementation("androidx.arch.core:core-testing:2.2.0")
    
    // LifeCycle 测试
    testImplementation("androidx.lifecycle:lifecycle-runtime-testing:2.11.0")
    androidTestImplementation("androidx.lifecycle:lifecycle-runtime-testing:2.11.0")
    
    // ViewModel 测试
    testImplementation("androidx.lifecycle:lifecycle-viewmodel-ktx:2.11.0")
    androidTestImplementation("androidx.lifecycle:lifecycle-viewmodel-ktx:2.11.0")
    
    // Room 测试
    testImplementation("androidx.room:room-testing:2.8.5")
    androidTestImplementation("androidx.room:room-testing:2.8.5")
}
```

# 四、单元测试（Local JVM Tests）

## (一) 基本结构

```kotlin
// src/test/java/com/waiminutes/app/unit/ExampleUnitTest.kt
class ExampleUnitTest {

    @Test
    fun addition_isCorrect() {
        assertEquals(4, 2 + 2)
    }

    @Test
    fun stringReversal() {
        assertEquals("dlroW olleH", "Hello World".reversed())
    }
}
```

## (二) 使用 Mockito 进行依赖模拟

```kotlin
// src/test/java/com/waiminutes/app/unit/UserRepositoryTest.kt
class UserRepositoryTest {

    private lateinit var mockApi: UserApi
    private lateinit var repository: UserRepository

    @Before
    fun setUp() {
        mockApi = mock(UserApi::class.java)
        repository = UserRepository(mockApi)
    }

    @Test
    fun `getUser returns user when api succeeds`() = runTest {
        // Given
        val testUser = User("123", "John Doe")
        // ★getUser 是 suspend 函数，普通 whenever() 拿到的只是带 Continuation 的
        //   桥接方法，桩打不进去。要用 wheneverBlocking()（mockito-kotlin）
        wheneverBlocking(mockApi.getUser("123")).thenReturn(testUser)

        // When
        val result = repository.getUser("123")

        // Then
        assertEquals(testUser, result)
        verify(mockApi).getUser("123")
    }

    @Test
    fun `getUser throws exception when api fails`() = runTest {
        // Given
        wheneverBlocking(mockApi.getUser("123")).thenThrow(IOException("Network error"))

        // When/Then
        assertThrows<IOException::class.java> {
            runBlocking { repository.getUser("123") }
        }
    }
}
```

## (三) 测试协程代码

```kotlin
// src/test/java/com/waiminutes/app/unit/ViewModelTest.kt
@ExperimentalCoroutinesApi
class ArticleViewModelTest {

    private lateinit var viewModel: ArticleViewModel
    private lateinit var testDispatcher: TestDispatcher

    @Before
    fun setUp() {
        // ★TestDispatcher 是抽象类，不能实例化
        testDispatcher = StandardTestDispatcher()
        Dispatchers.setMain(testDispatcher)
        
        val mockRepository = mock(ArticleRepository::class.java)
        viewModel = ArticleViewModel(mockRepository)
    }

    @After
    fun tearDown() {
        Dispatchers.resetMain()
        // ★TestDispatcher 没有 cleanup() 方法。
        //   要推进虚拟时间用 testDispatcher.scheduler.advanceUntilIdle()
    }

    @Test
    fun `loadArticle updates state correctly`() = runTest {
        // Given
        val testArticle = Article("123", "Test Title", "Test Content")
        wheneverBlocking(mockRepository.getArticle("123"))
            .thenReturn(testArticle)

        // When
        viewModel.loadArticle("123")

        // Then
        // 假设 ViewModel 有 StateFlow 暴露 UI 状态
        // 实际测试中需要根据具体实现调整
    }
}
```

# 五、仪器化测试（Instrumented Tests）

## (一) 基本结构

```kotlin
// src/androidTest/java/com/waiminutes/app/integration/ExampleInstrumentedTest.kt
@RunWith(AndroidJUnit4::class)
class ExampleInstrumentedTest {

    @Test
    fun useAppContext() {
        // Context of the app under test.
        val appContext = InstrumentationRegistry.getInstrumentation().targetContext
        assertEquals("com.waiminutes.app", appContext.packageName)
    }
}
```

## (二) 测试 Activity

```kotlin
// src/androidTest/java/com/waiminutes/app/integration/MainActivityTest.kt
@RunWith(AndroidJUnit4::class)
class MainActivityTest {

    @get:Rule
    val activityRule = ActivityScenarioRule(MainActivity::class.java)

    @Test
    fun activityStartsSuccessfully() {
        val scenario = activityRule.scenario
        scenario.onActivity { activity ->
            assertNotNull(activity)
            // 可以添加更多断言来检查 Activity 状态
        }
    }
}
```

## (三) 使用 Compose 测试 UI

```kotlin
@RunWith(AndroidJUnit4::class)
class LoginScreenTest {

    @get:Rule
    val composeRule = createAndroidComposeRule<MainActivity>()

    @Test
    fun 凭据正确时登录成功() {
        composeRule.onNodeWithText("用户名")
            .performTextInput("testuser")
        composeRule.onNodeWithText("密码")
            .performTextInput("password123")
        composeRule.onNodeWithText("登录").performClick()

        composeRule.onNodeWithText("欢迎，testuser")
            .assertIsDisplayed()
    }

    @Test
    fun 凭据错误时显示错误提示() {
        composeRule.onNodeWithText("用户名")
            .performTextInput("wronguser")
        composeRule.onNodeWithText("密码")
            .performTextInput("wrongpass")
        composeRule.onNodeWithText("登录").performClick()

        composeRule.onNodeWithText("用户名或密码错误")
            .assertIsDisplayed()
    }
}
```

**为什么不用 Espresso**：Espresso 面向 View 层级，通过 `R.id` 定位控件。Compose 界面里根本没有这些 View 节点，`onView(withId(...))` 必然找不到。Compose 用**语义树**定位——控件的 `text`、`contentDescription`、是否可点击都会进入语义树，测试按「用户能看到/能操作的东西」来断言，而不是按内部实现。

同一个测试同时覆盖 UI 与逻辑时，用 `createAndroidComposeRule`（能拿到 `Activity` 与 `Intent`）；纯 UI 且不需要 Activity 时，`createComposeRule` 更轻量。


## (四) 测试列表交互

Compose 列表用 `createAndroidComposeRule` + 语义匹配，不用 Espresso 的 `RecyclerViewActions`：

```kotlin
@RunWith(AndroidJUnit4::class)
class ArticleListTest {

    @get:Rule
    val composeRule = createAndroidComposeRule<MainActivity>()

    @Test
    fun 点击列表项进入详情() {
        composeRule.onNodeWithText("Compose 声明式基础")
            .performScrollTo()          // 目标不在可见区时先滚动
            .performClick()

        composeRule.onNodeWithText("正文内容")
            .assertIsDisplayed()
    }

    @Test
    fun 空态时显示提示而非列表() {
        composeRule.onNodeWithText("还没有内容").assertIsDisplayed()
        composeRule.onNodeWithTag("article_list").assertDoesNotExist()
    }
}
```

⚠ **不要给列表加 `Modifier.testTag` 后再用 Espresso 去找**。Espresso 面向 View 层级，在纯 Compose 界面里看不到任何节点，会直接找不到——这是从 View 迁过来时最常见的测试失败原因。

几个常用语义匹配器：

```kotlin
composeRule.onNodeWithText("提交")                 .performClick()
composeRule.onNode(hasClickAction())                .performClick()   // 第 N 个可点的
composeRule.onAllNodesWithText("标签").onFirst()    .performClick()
composeRule.onNodeWithContentDescription("返回")     .performClick()
composeRule.onNode(hasScrollAction()).performScrollToIndex(20)
```

⚠ **同一个文案出现多次时 `onNodeWithText` 会抛异常**（它要求唯一匹配）。改用 `onAllNodesWithText(...)[0]` 或补 `useUnmergedTree = true`。


# 六、测试驱动开发（TDD）

## (一) TDD 流程

1. **编写失败的测试**（Red）：先写一个会失败的测试
2. **使测试通过**（Green）：编写最小量的代码使测试通过
3. **重构代码**（Refactor）：在不改变行为的情况下改进代码结构
4. **重复循环**：对于下一个功能，重复以上步骤

## (二) TDD 示例：实现密码验证器

```kotlin
// 第一步：编写失败的测试
@Test
fun `passwordValidator_rejectsShortPassword`() {
    val validator = PasswordValidator()
    assertFalse(validator.isValid("ab")) // 期望失败，因为还没实现
}

// 第二步：实验最小实现使测试通过
class PasswordValidator {
    fun isValid(password: String): Boolean {
        return password.length >= 8 // 临时实现
    }
}

// 第三步：编写更多测试并重构
@Test
fun `passwordValidator_acceptsLongEnoughPassword`() {
    val validator = PasswordValidator()
    assertTrue(validator.isValid("12345678")) // 应该通过
}

@Test
fun `passwordValidator_rejectsPasswordWithSpaces`() {
    val validator = PasswordValidator()
    assertFalse(validator.isValid("abc defg")) // 根据业务需求可能需要
}

// 重构后的实现
class PasswordValidator {
    fun isValid(password: String): Boolean {
        return password.length >= 8 && !password.contains(" ")
    }
}
```

# 七、测试覆盖率

## (一) 配置 JaCoCo 生成覆盖率报告

```kotlin
// 在 build.gradle.kts 中
android {
    buildTypes {
        release {
            // ...
        }
        debug {
            // ...
        }
    }
    
    testOptions {
        unitTests.includeAndroidResources = true
    }
    
    // 配置测试覆盖率
    buildTypes.all {
        isTestCoverageEnabled = true
    }
}

// 生成报告的任务
// ./gradlew createDebugCoverageReport
```

## (二) 解释覆盖率指标

- **行覆盖率（Line Coverage）**：有多少代码行被测试执行了
- **分支覆盖率（Branch Coverage）**：有多少分支（如 if/else）被测试执行了
- **方法覆盖率（Method Coverage）**：有多少方法被调用了
- **类覆盖率（Class Coverage）**：有多少类被实例化了

## (三) 提高覆盖率的策略

✅ 编写边界条件测试（如空列表、单元素、最大值）  
✅ 测试异常路径和错误处理  
✅ 使用参数化测试覆盖多种输入组合  
✅ 不要为了追求覆盖率而编写无意义的测试  
✅ 专注于测试业务逻辑而非简单的 getter/setter  
✅ 考虑使用变异测试（Mutation Testing）评估测试质量  

# 八、持续集成与测试

## (一) 在 CI 中运行测试

```yaml
# 例如 GitHub Actions 配置
name: Android CI

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
    
    - name: set up JDK
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
      # 需要模拟器或设备，这里使用 Firebase Test Lab 作为示例
      # 或者使用 github actions 的 android-emulator 插件
      run: ./gradlew connectedAndroidTest
    
    - name: Upload Test Results
      if: always()
      uses: actions/upload-artifact@v4
      with:
        name: test-results
        path: |
          **/build/reports/
          **/build/test-*/
```

## (二) 测试分片（Sharding）加速执行

```kotlin
// 在 gradle.properties 中
android.experimental.androidTestNumShards=4
android.experimental.androidTestShardDelaySeconds=10

// 或者在命令行中
./gradlew connectedAndroidTest -Pandroid.testInstrumentationRunnerArguments.numShards=4
```

# 九、与现代架构的协同使用

## (一) 测试 ViewModel

```kotlin
// src/test/java/com/waiminutes/app/unit/MainViewModelTest.kt
@ExperimentalCoroutinesApi
class MainViewModelTest {

    private lateinit var viewModel: MainViewModel
    private lateinit var testDispatcher: TestDispatcher
    private lateinit var mockRepository: MainRepository

    @Before
    fun setUp() {
        // ★TestDispatcher 是抽象类，不能实例化
        testDispatcher = StandardTestDispatcher()
        Dispatchers.setMain(testDispatcher)
        
        mockRepository = mock(MainRepository::class.java)
        viewModel = MainViewModel(mockRepository)
    }

    @After
    fun tearDown() {
        Dispatchers.resetMain()
        // ★TestDispatcher 没有 cleanup() 方法。
        //   要推进虚拟时间用 testDispatcher.scheduler.advanceUntilIdle()
    }

    @Test
    fun `loadData updates state correctly`() = runTest {
        // Given
        val testData = listOf(Item("1", "First"), Item("2", "Second"))
        wheneverBlocking(mockRepository.fetchData())
            .thenReturn(testData)

        // When
        viewModel.loadData()

        // Then
        // 假设 ViewModel 暴露了 StateFlow<List<Item>> 作为 uiState
        // 实际测试中需要根据具体实现调整
        // 例如：viewModel.uiState.test {
        //     assertEquals(testData, items)
        // }
    }
}
```

## (二) 测试 Repository

```kotlin
// src/test/java/com/waiminutes/app/unit/ArticleRepositoryTest.kt
class ArticleRepositoryTest {

    private lateinit var repository: ArticleRepository
    private lateinit var mockApi: ArticleApi
    private lateinit var mockDao: ArticleDao

    @Before
    fun setUp() {
        mockApi = mock(ArticleApi::class.java)
        mockDao = mock(ArticleDao::class.java)
        repository = ArticleRepository(mockApi, mockDao)
    }

    @Test
    fun `getArticleFromCache_returnsCachedData`() = runTest {
        // Given
        val cachedArticle = Article("123", "Cached Title", "Cached Content")
        wheneverBlocking(mockDao.getArticleById("123")).thenReturn(cachedArticle)

        // When
        val result = repository.getArticle("123")

        // Then
        assertEquals(cachedArticle, result)
        verify(mockDao).getArticleById("123")
        verify(mockApi, never()).getArticle(any()) // 不应该调用网络
    }

    @Test
    fun `getArticleFromNetwork_whenCacheMiss`() = runTest {
        // Given
        val networkArticle = Article("123", "Network Title", "Network Content")
        wheneverBlocking(mockDao.getArticleById("123")).thenReturn(null)
        wheneverBlocking(mockApi.getArticle("123")).thenReturn(networkArticle)

        // When
        val result = repository.getArticle("123")

        // Then
        assertEquals(networkArticle, result)
        verify(mockDao).getArticleById("123")
        verify(mockApi).getArticle("123")
        // 验证是否缓存了结果
        verify(mockDao).insertArticle(networkArticle)
    }
}
```

## (三) 测试 UseCase/Interactor

```kotlin
// src/test/java/com/waiminutes/app/unit/GetArticleUseCaseTest.kt
class GetArticleUseCaseTest {

    private lateinit var useCase: GetArticleUseCase
    private lateinit var mockRepository: ArticleRepository

    @Before
    fun setUp() {
        mockRepository = mock(ArticleRepository::class.java)
        useCase = GetArticleUseCase(mockRepository)
    }

    @Test
    fun `execute_returnsArticleFromRepository`() = runTest {
        // Given
        val testArticle = Article("123", "Test Title", "Test Content")
        wheneverBlocking(mockRepository.getArticle("123"))
            .thenReturn(testArticle)

        // When
        val result = useCase.execute("123")

        // Then
        assertEquals(testArticle, result)
        verify(mockRepository).getArticle("123")
    }
}
```

# 十、UI 测试与 Jetpack Compose

## (一) 测试 Compose UI

```kotlin
// src/androidTest/java/com/waiminutes/app/compose/GreetingTest.kt
@RunWith(AndroidJUnit4::class)
class GreetingTest {

    @get:Rule
    // ★纯 UI 测试用 createComposeRule()：它不启动 Activity，setContent 由你自己调
    val composeTestRule = createComposeRule()

    @Test
    fun greetingsDisplaysCorrectText() {
        // 设置组合内容
        composeTestRule.setContent {
            Greeting("Android")
        }

        // 验证文本显示
        composeTestRule.onNodeWithText("Hello Android!")
            .assertIsDisplayed()
    }

    @Test
    fun greetingsHandlesEmptyName() {
        composeTestRule.setContent {
            Greeting("")
        }

        composeTestRule.onNodeWithText("Hello !")
            .assertIsDisplayed()
    }
}

// 可组合函数示例
@Composable
fun Greeting(name: String) {
    Text(text = "Hello $name!")
}
```

## (二) 使用 Compose 测试库的断言

```kotlin
// 检查节点存在且可见
composeTestRule.onNodeWithText("Hello")
    .assertIsDisplayed()

// 检查节点不存在
composeTestRule.onNodeWithText("Goodbye")
    .assertDoesNotExist()

// 检查节点属性
composeTestRule.onNodeWithTag("button")
    .assert(hasClickAction())        // ★hasClickAction() 不接受参数
    .assertIsEnabled()

// 检查状态变化
composeTestRule.onNodeWithText("Count: 0")
    .performClick()
composeTestRule.onNodeWithText("Count: 1")
    .assertIsDisplayed()
```

## (三) 测试状态变化

```kotlin
@ExperimentalCoroutinesApi
@Test
fun counterIncrementsOnClick() = runTest {
    composeTestRule.setContent {
        var count by remember { mutableStateOf(0) }
        Column {
            Text(text = "Count: $count")
            Button(onClick = { count++ }) {
                Text("Increment")
            }
        }
    }

    // 初始状态
    composeTestRule.onNodeWithText("Count: 0")
        .assertIsDisplayed()

    // 点击按钮
    composeTestRule.onNodeWithText("Increment")
        .performClick()

    // 验证状态更新
    composeTestRule.onNodeWithText("Count: 1")
        .assertIsDisplayed()
}
```

# 十一、最佳实践与常见问题

## (一) 最佳实践

✅ 遵循Arrange-Act-Assert（AAA）模式编写测试  
✅ 使用描述性测试名称，使用反引号包裹（Kotlin 支持）  
✅ 保持测试独立且可重复执行  
✅ 为每个单元测试类只测试一个类  
✅ 测试边界条件和错误路径  
✅ 使用断言库如 Truth 或 AssertJ 提高可读性  
✅ 模拟外部依赖（网络、数据库、文件系统）  
✅ 测试驱动开发（TDD）对于复杂逻辑非常有用  
✅ 持续集成中自动运行测试  
✅ 监控测试覆盖率但不要成为唯一目标  
✅ 定期审查和更新测试以匹配代码变化  

## (二) 常见问题及解决方案

### 1. 问题：测试太慢影响开发效率
**原因**：过多的仪器化测试或不必要的延迟  
**解决方案**：
- 尽量使用本地 JVM 测试而不是仪器化测试
- 使用 `TestDispatcher` 测试协程代码而非延迟
- 优化测试设置，避免重复初始化
- 考虑使用测试分片并行执行
- 对于 UI 测试，专注于关键路径而非覆盖每个像素

### 2. 问题：模拟 Android 框架类困难
**原因**：Android 框架类往往 final 或有复杂依赖  
**解决方案**：
- 使用依赖注入将框架类抽象为接口
- 使用如 Robolectric 框架（虽然有局限性）
- 将 Android 特定代码隔离在薄层中，主要逻辑保持纯 Kotlin/Java
- 使用 `android-test-kit` 或类似库提供的假实现

### 3. 问题：测试覆盖率误导
**原因**：高覆盖率但测试质量低（如无断言的测试）  
**解决方案**：
- 关注测试有效性而非仅仅覆盖率
- 使用变异测试工具评估测试质量
- 审查测试以确保每个测试都有明确的断言
- 鼓励编写能够发现真实错误的测试而非形式化的测试

### 4. 问题：难以测试私有方法
**原因**：私有方法无法直接访问  
**解决方案**：
- 通过公共方法间接测试私有方法的行为
- 考虑将复杂的私有逻辑提取为可测试的公共方法或内部类
- 仅在必要时使用反射测试（但要认识到这是设计气味）
- 记住：如果无法轻松测试，可能表示需要重构

### 5. 问题：测试依赖冲突导致构建失败
**原因**：不同库依赖同一库的不同版本  
**解决方案**：
- 使用 Gradle 的依赖冲突解决策略
- 在 `build.gradle.kts` 中使用 `resolutionStrategy`
- 考虑使用強制版本或排除特定传递依赖
- 定期运行 `./gradlew dependencies` 检查依赖树

## (三) 易错点对照

| 误区 | 事实 |
| --- | --- |
| 追求 100% 覆盖率 | 覆盖率是**发现盲区的手段**，不是目标。100% 覆盖的代码照样可能全错 |
| 应该 mock 掉所有依赖 | 被测单元以外的依赖才 mock。mock 掉被测逻辑等于什么都没测 |
| Compose 测试要 `createComposeRule` | 纯 UI 用 `createComposeRule`；需要注入/导航时用 `createAndroidComposeRule` |
| 仪器测试必须用真机 | 模拟器也行，但必须用 API 级别与真机接近的镜像，否则权限与后台行为测不准 |
| `runTest` 里能 `Thread.sleep` | 不能，它跑在虚拟时钟上。用 `advanceTimeBy` / `advanceUntilIdle` |
| 测试间共享 `Context` 会互相污染 | `ApplicationProvider` 返回的是**整个 instrumentation 进程唯一的 Application 实例**，所有测试共享——它恰恰是污染的来源。要独立实例得自己 `new`，或靠 Robolectric 每个测试重建环境 |
| 先写实现再补测试 | 那测的是「实现的样子」而不是「需求的样子」。测试应在写实现前先写 |
| 用 Espresso 测 Compose | Espresso 面向 View 层级，测 Compose 用 `createAndroidComposeRule` + 语义匹配 |

# 十二、参考资料

- [Android 测试基础知识](https://developer.android.com/training/testing)
- [本地单元测试](https://developer.android.com/develop/ui/compose/testing)
- [仪器化测试](https://developer.android.com/studio/test)
- [Espresso 测试框架](https://developer.android.com/training/testing/espresso)
- [UI Automator 测试框架](https://developer.android.com/training/testing/ui-automator)
- [Jetpack Compose 测试指南](https://developer.android.com/jetpack/compose/testing)
- [架构组件测试指南](https://developer.android.com/topic/architecture/recommendations)
- [JUnit 5 用户指南](https://junit.org/junit5/docs/current/user-guide/)
- [Mockito 官方文档](https://site.mockito.org/)
- [Truth 断言库](https://truth.dev/)
- [Kotlin 协程测试指南](https://developer.android.com/kotlin/coroutines/test)
- [测试驱动开发（TDD）](https://www.agilealliance.org/glossary/tdd/)
- [测试金字塔概念](https://martinfowler.com/articles/practical-test-pyramid.html)
- [变异测试入门](https://pitest.org/)
- [Android 测试保险箱（内部测试共享UID）](https://developer.android.com/studio/test)
- [Gradle 测试任务](https://docs.gradle.org/current/userguide/java_testing.html)
- [JaCoCo 代码覆盖率工具](https://www.jacoco.org/jacoco/)
- [Spoon 测试分发运行器](https://square.github.io/spoon/)
- [Firebase Test Lab 进行云端测试](https://firebase.google.com/docs/test-lab)
- GitHub Actions for Android
- Bitrise 持续集成平台
- CircleCI Android 示例
- Testcontainers for Android 集成测试
