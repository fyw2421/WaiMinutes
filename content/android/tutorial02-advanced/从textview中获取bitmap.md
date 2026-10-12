---
title: "从textview中获取bitmap"
weight: 22
description: "给 TextView 设上 drawable 并开启 drawing cache，再由缓存位图创建 Bitmap"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

先给 `TextView` 设上 drawable 并开启 drawing cache，控件绘制完成后即可从缓存里取出位图。

```
TextView tv = new TextView(context);
tv.setText("zhongge");
tv.setCompoundDrawablesIntrinsicBounds(drawable, null, null, null);
tv.setDrawingCacheEnabled(true);

Bitmap b = Bitmap.createBitmap(tv.getDrawingCache());

tv.destroyDrawingCache();
```
