---
title: "Android"
description: "Android 开发学习笔记：Kotlin 与 Jetpack Compose 声明式 UI、分层架构与单向数据流、数据层与四大组件，以及时下主流工程实践。"
---

Android 开发笔记，面向 **Kotlin + Jetpack Compose** 技术栈，按学习顺序编排为 28 篇，可从头顺读，也可按需检索：

- **01–05 起步** — 开发环境与工程结构、Android 视角的 Kotlin 速览、Compose 声明式基础、布局与状态提升、列表与滚动
- **06–12 UI** — Material 3 与主题、组件与交互、图片与资源、动画与转场、手势、边缘到边缘与 insets、自适应与大屏
- **13–18 架构** — 分层与 UDF、ViewModel、协程与 Flow、Navigation 3、依赖注入 Hilt、多模块与约定插件
- **19–24 数据与系统** — Retrofit 网络层、Room、DataStore、WorkManager、四大组件、权限与安全
- **25–28 工程** — 测试、Gradle 构建与 R8、性能优化、发布与上架

写作进度：01–28 已完成。

每一篇都配有可运行示例、易错点说明与参考来源。内容为原创整理，依据 [Android 官方文档](https://developer.android.com/)、[Jetpack 官方文档](https://developer.android.com/jetpack) 与 [Kotlin 官方文档](https://kotlinlang.org/docs/home.html) 等公开资料写成。

> **为什么是 Compose**：Google 已宣布 Android 进入 [Compose-first](https://developer.android.com/develop/ui/compose/first) 阶段，View 工具链转入**维护模式**——不再有新特性，但也没有被移除，学习存量代码时仍会遇到。
