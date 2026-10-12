---
title: "dialog设计stytle配置详解"
weight: 6
description: "用 style 里的 window* 属性控制 Dialog 有无标题栏、背景、边框以及是否悬浮"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

属性|说明
---|---
`<item name="android:windowNoTitle">true</item>`|设置 dialog 有无标题栏
`<item name="android:backgroundDimEnabled">false</item>`|设置 dialog 出现时是否屏幕变黑
`<item name="android:windowBackground">@color/transparent</item>`|设置 dialog 的北京透明度
`<item name="android:windowFrame">@null</item>`|Dialog 的 windowFrame 框为无
`<item name="android:windowIsFloating">true</item>`|是否浮现在 activity 之上
`<item name="android:windowIsTranslucent">false</item>`|是否半透明
`<item name="android:windowBackground">@drawable/dia_bg</item>`|设置 dialog 的背景
