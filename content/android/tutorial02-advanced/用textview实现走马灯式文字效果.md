---
title: "用TextView实现走马灯式文字效果"
weight: 28
description: "给 TextView 加 ellipsize=\"marquee\"，再重写 isFocused() 让它一直获得焦点即可滚动显示"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

有时文字过长，又只想单行显示来节省空间，走马灯式显示是个不错的选择（饿了么顶部地址显示就是这种情况）。

其实只要在布局文件的 TextView 中添加属性 `android:ellipsize="marquee"`，但要在获得焦点的情况下该属性才会起作用。那么怎么让控件一直获得焦点呢？

系统判断 TextView 是否获得焦点是通过 `isFocused()` 这个方法的返回值来判断的，这样的话就可以新建一个类继承 TextView，重写 `isFocused()` 方法，让它返回 `true` 就 ko 了。
