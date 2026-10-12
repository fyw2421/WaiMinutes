---
title: "Activity和Service之间通信的方式"
weight: 2
description: "两种方式：绑定服务后通过返回的 Binder 取值，或在 Activity 里注册广播接收器"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

# 一、绑定服务

Activity 可以采用绑定的方式 `bindService`，然后通过返回的 Binder 获得返回值。

# 二、广播

在 Activity 里添加一个内部类继承 `BroadcastReceiver`，然后 Service 有数据要 Activity 更新时，就发送一个广播。
