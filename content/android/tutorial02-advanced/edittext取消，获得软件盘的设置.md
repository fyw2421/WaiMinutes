---
title: "edittext取消，获得软件盘的设置"
weight: 8
description: "EditText 默认会自动获焦并弹出软键盘；用 windowSoftInputMode 或 focusable 属性控制"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

通常情况下，含有 EditText 的 Activity 一展现时都会获得焦点，从而触发软键盘。

# 一、让软键盘自动弹出

如果软件盘不出来，可在配置文件对应的 Activity 里加下面这个设置：

```
android:windowSoftInputMode="stateVisible"
```

# 二、不让它一开始就获得焦点

可以在 EditText 的根布局上设置以下属性：

```
android:focusable="true"
android:focusableInTouchMode="true"
```
