---
title: "如何得到String的宽高"
weight: 25
description: "用 Paint 的 getTextBounds() 量出文本的绘制范围，再从 Rect 中取宽度与高度"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

`Paint` 的 `getTextBounds()` 会把文本的绘制范围写进一个 `Rect`，从中即可取到宽高。

```
Rect rect = new Rect();
paint.getTextBounds(string, 0, string.length(), rect);

int strWidth = rect.width();
int strHeight = rect.height();
```
