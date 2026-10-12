---
title: "onMeasure()方法及控件的绘制"
weight: 15
description: "super.onMeasure() 调用后即可拿到控件宽高；测量模式由布局文件里的宽高属性决定"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

在这个方法中，`super.onMeasure()` 调用后将可以得到控件的宽高。

控件的 `onMeasure()`、`onLayout()` 方法会被执行多次，注意这里面执行的逻辑，不要被多次执行了。

# 一、绘制控件分两步

1. 在 `onMeasure()` 方法中绘制好控件的大小
2. 在 `onLayout` 方法里布局好控件的位置，用 `view.layout(left, top, right, bottm)`

# 二、测量模式

在 `onMeasure()` 中：

```
int parentWidth = MeasureSpec.getSize(widthMeasureSpec);
int parentHeight = MeasureSpec.getSize(heightMeasureSpec);
int heightMode = MeasureSpec.getMode(heightMeasureSpec);
```

`onMeasure()` 的两个参数 `widthMeasureSpec`、`heightMeasureSpec` 里包含了控件的长度和测量模式。测量模式有 `MeasureSpec.EXACTLY`、`MeasureSpec.ATMOST`、`MeasureSpec.UNSPECIFIED`，得出的测量模式和布局文件里 `layout_width`、`layout_height` 设置的属性有关：

模式|属性值
---|---
`MeasureSpec.EXACTLY`|fill_parent，或具体数值
`MeasureSpec.ATMOST`|wrap_content
`MeasureSpec.UNSPECIFIED`|很少用，意思为想设置多大就多大

# 三、指定控件大小

最后可以为控件指定大小，用 `setMeasuredDimension()`，这个方法里的参数是数值，不包含测量模式。

还有另外一种方法去控制控件大小，这里的参数是带有测量模式的：

```
// 制作带有测量模式的长度值
int sideWidth = MeasureSpec.makeMeasureSpec((int) (MaxWidth * 0.5), MeasureSpec.EXACTLY);

leftlayout.measure(sideWidth, heightMeasureSpec);
```
