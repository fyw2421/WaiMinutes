---
title: "AndroidNotes16 : Navigation 3"
weight: 16
description: "Navigation 3 的核心模型：应用自己拥有 back stack、类型安全的 key、NavDisplay 与 entryProvider、SceneStrategy 实现多窗格、entry scoped 与 ViewModel 的作用域、深链接与状态保存，以及与 Navigation 组件的差异和迁移取舍。"
featureimage: "covers/android-engineering.svg"
date: 2026-10-13
tags: ["Android"]
---

# 一、核心理念：你自己拥有 back stack

Navigation 3 与老 `NavController` 最大的区别是：**它不持有导航状态，你持有**。

老写法，状态藏在组件里：

```kotlin
navController.navigate("detail/$id")
```

Navigation 3 的写法，状态是一个普通列表：

```kotlin
// ★全文统一用 sealed 声明所有 key，否则下面出现的是两套互不兼容的类型
@Serializable
sealed interface ArticleKey {
    @Serializable data object List : ArticleKey
    @Serializable data class Detail(val id: Long) : ArticleKey
}

// back stack 就是个 SnapshotStateList
val backStack = remember { mutableStateListOf<ArticleKey>(ArticleKey.List) }

// 前进 = 往末尾加
backStack.add(ArticleDetail(id))
// 返回 = 从末尾删
backStack.removeLastOrNull()
```

**这是设计哲学的转变**：导航状态从「框架的黑盒」变成「你的一段普通状态」。它能 `rememberSaveable`、能被观察、能手动保存恢复。

## (一) 依赖

```kotlin
dependencies {
    implementation("androidx.navigation3:navigation3-runtime:1.2.0")
    implementation("androidx.navigation3:navigation3-ui:1.2.0")
}
```

Navigation 3 已于 **1.2.0 稳定**（2026-09），当前最新 alpha 为 1.3.0-alpha01。

⚠ **它是独立于 `androidx.navigation` 的新库**，不是升级版。两者可以共存（迁移期），但不要混着写同一套导航。

# 二、完整示例

```kotlin
// 1) 定义所有可导航的目标
sealed interface ArticleKey {
    data object List : ArticleKey
    data class Detail(val id: Long) : ArticleKey
}

@Composable
fun ArticleApp(repository: ArticleRepository) {
    val backStack = rememberNavBackStack(ArticleKey.List)   // 辅助函数
    val navigator = remember { ArticleNavigator(backStack) }

    NavDisplay(
        backStack = backStack,
        onBack = { backStack.removeLastOrNull() },
        entryProvider = { key ->
            when (key) {
                is ArticleKey.List -> NavEntry("list") {
                    ArticleListRoute(
                        onOpen = { id -> backStack.add(ArticleKey.Detail(id)) }
                    )
                }
                is ArticleKey.Detail -> NavEntry("detail-${key.id}") {
                    ArticleDetailRoute(
                        articleId = key.id,
                        onBack = { backStack.removeLastOrNull() }
                    )
                }
            }
        }
    )
}
```

**三个参数**：

| 参数 | 作用 |
| --- | --- |
| `backStack` | 当前导航栈，`SnapshotStateList<Key>` |
| `entryProvider` | 把每个 key 变成一个 `NavEntry`（即界面） |
| `onBack` | 返回时调什么，通常是 `removeLastOrNull()` |

⚠ **`entryProvider` 必须能处理任意 key**。它是穷举的 `when`，加了新 key 却忘了加分支——编译报错，这是好事。

## (一) key 要能精确区分

```kotlin
// ❌ 不可行
NavEntry("detail") { ... }              // ★所有详情页共用一个 key

// ✅ key 里带 id
NavEntry("detail-${key.id}") { ... }
```

⚠ **entry 的 key 必须唯一**。两篇文章详情如果用同一个 key，ViewModel 会被复用，第二篇显示第一篇的内容——这是迁移到 Navigation 3 时最常见的 bug。

# 三、back stack 的持久化

因为 back stack 是普通状态，保存恢复极其简单：

```kotlin
val backStack = rememberSaveable(
    saver = Saver(
        save = { it.toTypedArray() },                 // 转成可存的形式
        restore = { restored -> restored.toMutableStateList() }
    )
) { mutableStateListOf<ArticleKey>(ArticleKey.List) }
```

⚠ **key 里不要放不可序列化的东西**（`Context`、`View`）。`data class Detail(val id: Long)` 很好；`data class Detail(val callback: () -> Unit)` 存不下来。

# 四、类型安全的导航

不需要字符串常量，也不需要 `NavType`：

```kotlin
// ❌ 老写法：字符串 + 手工解析
navController.navigate("detail/$id")
// 目标：navController.route("detail/{id}", navArgument("id") { type = NavType.LongType })

// ✅ Navigation 3：编译期就有类型
backStack.add(ArticleKey.Detail(id = 42L))
```

⚠ **参数名写错从运行时崩溃变成了编译错误**——这是 Navigation 3 相对老方案最大的改进之一。

# 五、SceneStrategy：多窗格

默认的 `SinglePaneSceneStrategy` 一次只显示栈顶。1.2.0 起可以自己实现 `SceneStrategy`，表达「同一个 back stack 在不同窗口宽度下显示成什么样」：

```kotlin
NavDisplay(
    backStack = backStack,
    sceneStrategies = listOf(ArticleSceneStrategy),
    entryProvider = { key -> /* ... */ }
)
```

⚠ **`SceneStrategy` 是接口，没有构造参数**——不能写 `SceneStrategy<ArticleKey>(ArticleKey::class)`。正确的形态是 `object`（或 `class`）+ `override`：

```kotlin
object ArticleSceneStrategy : SceneStrategy<ArticleKey> {

    // 接收者是 SceneStrategyScope<T>（不是类成员），返回可空：
    // 返回 null 表示「本策略不处理，交给下一个」。
    override fun SceneStrategyScope<ArticleKey>.calculateScene(
        entries: List<NavEntry<ArticleKey>>
    ): Scene<ArticleKey>? {
        val widthClass = currentWindowAdaptiveInfo().windowSizeClass.widthSizeClass

        return if (widthClass == WindowWidthSizeClass.Compact) {
            // 窄屏：只显示栈顶，单栏
            SinglePane(entries.last())
        } else {
            // 宽屏：列表与详情并排
            TwoPaneScene(entries.first(), entries.getOrNull(1))
        }
    }
}
```

`Scene<T>` 也是接口，`ListScene` / `DetailScene` / `ListDetailScene` 这些类**并不存在**，要自己实现：

```kotlin
/** 宽屏用：左列表 + 右详情 */
class TwoPaneScene<T : Any>(
    private val listPane: NavEntry<T>,
    private val detailPane: NavEntry<T>?
) : Scene<T> {
    override val key: Any get() = "TwoPane:$listPane"
    override val entries: List<NavEntry<T>> get() = listOf(listPane)
    override val previousEntries: List<NavEntry<T>> get() = emptyList()
    override val content: @Composable (T) -> Unit get() = { TwoPaneContent(it) }
    override val metadata: Map<String, Any> get() = emptyMap()
}
```

**同一个 back stack，在手机上是两页，在平板上是两栏**——不需要写两份导航逻辑。

⚠ **这一整块是实验 API，签名仍可能变**。生产项目更稳的做法是用 `ListDetailPaneScaffold`（第 12 篇）或 `material3-adaptive` 的 `adaptive-navigation3`，它们把 Scene 的细节封装掉了。

# 六、entry 作用域与 ViewModel

⚠ **「每个 NavEntry 有独立 ViewModelStore」不是默认行为**，这一点最容易踩坑。`NavDisplay` 的默认 `entryDecorators` **只有** `rememberSaveableStateHolderNavEntryDecorator()`（只管 `rememberSaveable`）。要用 ViewModel，必须显式加：

```kotlin
dependencies {
    implementation("androidx.lifecycle:lifecycle-viewmodel-navigation3:2.11.0")
}

NavDisplay(
    backStack = backStack,
    entryDecorators = listOf(
        rememberSaveableStateHolderNavEntryDecorator(),   // 默认就有，写出来是为了对照
        rememberViewModelStoreNavEntryDecorator()         // ★这行才是关键
    ),
    entryProvider = { key -> /* ... */ }
)
```

**加上之后**，每个 `NavEntry` 才有自己的 ViewModelStore：

```kotlin
NavEntry("detail-${key.id}") {          // ★key 唯一 = ViewModel 独立
    val vm: ArticleDetailViewModel = hiltViewModel()
    // 这个 ViewModel 活到该 entry 出栈为止
}
```

⚠ **不加那一行的后果**：所有 entry 共用 Activity 的 ViewModelStore。导航到第二篇文章时，拿到的还是第一篇的 ViewModel——而且这与 `key` 传不传、`hiltViewModel()` 的 key 参数都无关，唯一的原因是 store 是共享的。

加上之后，返回列表再进详情，ViewModel 是新建的（出栈即销毁）。需要跨页面共享状态时，**不要靠 back stack**，靠上层共享：

```kotlin
// ✅ 从 Activity 取，所有界面共用一个实例
val vm: AppStateViewModel = hiltViewModel(activity = LocalActivity.current)
```

⚠ **别指望 back stack 帮你保存状态**。它只管「现在显示哪一页」，不负责持久化。

# 七、常见导航模式

## (一) 底部导航

底部 Tab 是**平级关系**，不该进 back stack：

```kotlin
var selectedTab by rememberSaveable { mutableStateOf(Tab.Home) }
val backStack = rememberNavBackStack(ArticleKey.List)

// 切 Tab 时把栈重置，而不是往里加
fun switchTab(tab: Tab) {
    selectedTab = tab
    backStack.clear()
    backStack.add(tab.startKey)
}

NavigationBar {
    Tab.entries.forEach { tab ->
        NavigationBarItem(
            selected = selectedTab == tab,
            onClick = { switchTab(tab) },
            icon = { Icon(tab.icon, contentDescription = tab.label) },
            label = { Text(tab.label) }
        )
    }
}
```

⚠ **每次切 Tab 都 `backStack.add()` 会让返回栈无限增长**，用户按返回会依次回到上一个 Tab 的页面——体验很差。正确做法是切 Tab 时 `clear()` 再 `add()`。

## (二) 弹窗

```kotlin
data class ConfirmDelete(val articleId: Long) : ArticleKey

// 打开
backStack.add(ConfirmDelete(id))
// 关闭
backStack.removeLastOrNull()
```

⚠ **用 sealed 的不同子类表达不同形态**（列表页 / 详情页 / 弹窗），SceneStrategy 里就能按类型分别处理——比在一个界面里用 `if (showDialog)` 更清晰。

# 八、深链接

Navigation 3 从 1.2.0 起支持深链接。**一个 matcher 对应一个 URI 模板**，不是「KClass 到 lambda 的映射表」：

```kotlin
// 1) 定义「一条深链接 ↔ 一个 key」的匹配器
val detailMatcher = UriDeepLinkMatcher<ArticleKey.Detail>(
    uriPattern = "https://example.com/article/{articleId}",
    serializer = serializer<ArticleKey.Detail>()        // 用 kotlinx.serialization
)

// 2) match 的入参是 DeepLinkRequest（不是 Uri），结果是 MatchResult，要取 .key
LaunchedEffect(deepLinkRequest) {
    deepLinkRequest?.let { request ->
        val key = detailMatcher.match(request)?.key ?: return@LaunchedEffect
        backStack.clear()          // ★深链接进来应重置栈，见下
        backStack.add(key)
    }
}
```

⚠ **深链接进来时应该 `clear()` 再 `add()`，而不是直接 `add()`**。否则用户从深链接点进详情后按返回，会回到一个本不该出现的中间页。

⚠ key 必须能被序列化（`@Serializable` 或实现 `Parcelable`），`UriDeepLinkMatcher` 要靠它把 URI 参数还原成 key。

⚠ **深链接进来的内容如果需要登录，要先重置 back stack 而不是 `add`**——否则用户返回时可能回到一个不该出现的中间页。

# 九、与老 Navigation 组件的取舍

| | `NavController` | Navigation 3 |
| --- | --- | --- |
| back stack | 框架持有 | **你持有** |
| 目标声明 | 字符串路由 | 类型 key |
| 参数校验 | 运行时 | **编译期** |
| 动态改栈 | `popUpTo` 等一堆参数 | 直接改 List |
| 多窗格 | 需自己判断宽度 | `SceneStrategy` |
| 生态成熟度 | 高（大量库依赖） | 新 |
| Fragment 支持 | 支持 | 不支持 |

**新项目**：直接上 Navigation 3。
**存量项目**：可以继续用 `NavController`——它不会消失。但如果是纯 Compose 单 Activity，迁到 Navigation 3 的收益（动态栈、多窗格、编译期类型）值得。

⚠ **需要 Fragment 的场景暂时还用不了 Navigation 3**。这是它目前最实际的一个限制。

# 十、易错点

| 误区 | 事实 |
| --- | --- |
| `NavEntry` 的 key 可以随便取 | 必须唯一，否则 ViewModel 被错误复用 |
| back stack 可以无脑 `add` | 切 Tab 要 `clear()` 再 `add`，否则栈无限增长 |
| back stack 能自动保存状态 | 不会，key 必须可序列化，要自己 `rememberSaveable` |
| 返回上一级时 ViewModel 还活着 | entry 出栈即销毁，需要共享状态就挂上层 |
| key 里可以放 `Context` | 存不下来，只放可序列化的数据 |
| Navigation 3 是 `navigation` 的升级版 | 是**独立新库**，两者可共存但别混写 |
| SceneStrategy 已经稳定 | 目前是实验 API，接口可能变 |
| 所有导航都该进 back stack | 底部 Tab 是平级的，不该进栈 |

# 十一、参考资料

- [Navigation 3 基础](https://developer.android.com/guide/navigation/navigation-3/basics) — 官方入门：back stack 与 NavDisplay
- [Navigation 3 发布记录](https://developer.android.com/jetpack/androidx/releases/navigation3) — 版本、稳定状态与变更说明
- [NavDisplay API 参考](https://developer.android.com/reference/kotlin/androidx/navigation3/ui/NavDisplay) — 参数与 `NavEntry` 完整签名
