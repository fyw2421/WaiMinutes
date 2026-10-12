---
title: "Intent 的匹配模式"
weight: 10
description: "隐式 Intent 由框架按 action、category 等信息与 Intent-filter 逐项匹配，显式调用则无需匹配"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

Intent 实际上有 android 框架层的一个组件服务管理（一个线程）负责，每次发起意图（Intent），组件服务管理器都会获取发起者的信息（action、componnet、cateory），然后在配置文件中找出注册的组件（activity、service、Reciver……）的过滤信息，然后进行匹配。

Intent 需要匹配的前提是采用的是隐式调用，显式调用（指定了要调用某个组件）则无须匹配。

Intent 匹配有两个角色：一个是 Intent 的发起者，一个是 Intent 的接受者——后者就是配置文件中每个 Activity 的 Intent-filter 信息注册。

# 一、action 的匹配

发起者指定一个 action：

```
Intent intent = new Intent(action的名字);
```

接受者的 action 列表中至少要包含一个相同名字的 action，或者没有 action 列表。

另外如果 category 没有特殊的要求，要设置为 `category="android.intent.category.DEFAULT"`。
