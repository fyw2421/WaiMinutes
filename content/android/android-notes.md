---
title: "AndroidNotes"
weight: 1
description: "Android 开发笔记的总目录：28 篇按起步、UI、架构、数据与系统、工程五个分组编排，每篇附一句话说明，可顺读也可跳读。"
date: 2026-10-15
tags: ["Android"]
featureimage: covers/android-index.svg
---

# 一、这份笔记

面向 **Kotlin + Jetpack Compose** 技术栈，共 28 篇。技术基线为 Kotlin 2.2+、AGP 9.x、`compileSdk` / `targetSdk` 36、`minSdk` 24。

编排按**依赖顺序**而非主题分类：后面的内容默认你已经读过前面的。可以从 01 顺读到 28，也可以直接跳到需要的分组。

每篇都带可运行示例、易错点对照表和参考资料。全部为原创整理，不是翻译。

# 二、起步（01–05）

从零搭起工程，到能写出一个带列表的界面。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 01 | [起步]({{< ref "android-notes01-getting-started" >}}) | 开发环境、工程结构与技术选型 |
| 02 | [Kotlin 速览]({{< ref "android-notes02-kotlin" >}}) | 空安全、密封类与作用域函数 |
| 03 | [Compose 基础]({{< ref "android-notes03-compose-basics" >}}) | `@Composable`、重组与 remember |
| 04 | [布局与状态提升]({{< ref "android-notes04-layout-state" >}}) | Column/Row/Box 与状态边界 |
| 05 | [列表与滚动]({{< ref "android-notes05-lists" >}}) | LazyColumn、稳定的 key 与粘性头 |

# 三、UI（06–12）

视觉呈现与交互的全部组件。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 06 | [Material 3 与主题]({{< ref "android-notes06-material3" >}}) | ColorScheme、动态取色与主题叠加 |
| 07 | [组件与交互]({{< ref "android-notes07-components" >}}) | 按钮、表单、对话框与 Snackbar |
| 08 | [图片与资源]({{< ref "android-notes08-images" >}}) | painterResource、Coil 3 与资源目录 |
| 09 | [动画与转场]({{< ref "android-notes09-animation" >}}) | 状态动画、可见性与转场 |
| 10 | [手势]({{< ref "android-notes10-gestures" >}}) | pointerInput、拖拽与嵌套滚动 |
| 11 | [边缘到边缘与 insets]({{< ref "android-notes11-insets" >}}) | enableEdgeToEdge 与 inset 消费 |
| 12 | [自适应与大屏]({{< ref "android-notes12-adaptive" >}}) | 宽度断点、分栏与折叠屏 |

# 四、架构（13–18）

应用代码怎么组织：状态放哪、依赖怎么连、模块怎么拆。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 13 | [分层与 UDF]({{< ref "android-notes13-architecture" >}}) | 分层边界、单向数据流与状态建模 |
| 14 | [ViewModel]({{< ref "android-notes14-viewmodel" >}}) | viewModelScope、SavedStateHandle 与参数 |
| 15 | [协程与 Flow]({{< ref "android-notes15-coroutines-flow" >}}) | 作用域选择、Flow 冷流与异常处理 |
| 16 | [Navigation 3]({{< ref "android-notes16-navigation3" >}}) | 应用自己拥有 back stack |
| 17 | [依赖注入 Hilt]({{< ref "android-notes17-hilt" >}}) | 依赖图、作用域与限定符 |
| 18 | [多模块与约定插件]({{< ref "android-notes18-multimodule-convention-plugins" >}}) | 模块拆分、版本目录与约定插件 |

# 五、数据与系统（19–24）

网络、存储、后台任务，以及系统组件与权限。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 19 | [Retrofit 网络层]({{< ref "android-notes19-retrofit" >}}) | 接口定义、协程调用与拦截器 |
| 20 | [Room 数据库]({{< ref "android-notes20-room" >}}) | 实体、DAO、迁移与关系映射 |
| 21 | [DataStore 偏好存储]({{< ref "android-notes21-datastore" >}}) | Preferences、Proto 与迁移 |
| 22 | [WorkManager 后台任务]({{< ref "android-notes22-workmanager" >}}) | 约束、退避重试与唯一工作 |
| 23 | [四大组件]({{< ref "android-notes23-four-components" >}}) | Activity、Service、Receiver 与 Provider |
| 24 | [权限与安全]({{< ref "android-notes24-permissions-security" >}}) | 运行时权限、Keystore 与网络安全 |

# 六、工程（25–28）

把代码变成能发布、能维护的东西。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 25 | [测试]({{< ref "android-notes25-testing" >}}) | 单元测试、仪器化测试与 TDD |
| 26 | [Gradle 构建与 R8]({{< ref "android-notes26-gradle-r8" >}}) | 版本目录、构建变种、签名与混淆 |
| 27 | [性能优化]({{< ref "android-notes27-performance" >}}) | 重组、布局、内存与启动时间 |
| 28 | [发布与上架]({{< ref "android-notes28-release-publishing" >}}) | 版本管理、多渠道打包与上架流程 |

# 七、参考资料

- [Android 官方文档](https://developer.android.com/)
- [Jetpack Compose 官方文档](https://developer.android.com/jetpack)
- [Kotlin 官方文档](https://kotlinlang.org/docs/home.html)
- [Android Architecture Samples](https://github.com/android/architecture-samples)
