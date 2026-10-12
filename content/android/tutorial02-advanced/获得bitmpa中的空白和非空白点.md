---
title: "获得bitmpa中的空白和非空白点"
weight: 33
description: "用 Bitmap.getPixels() 取出全部像素，统计值为 0 的像素个数得到空白区域占比"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

```
int w = getWidth();
int h = getHeight();
int wipeArea = 0;
int totalArea = w * h;
int[] piexs = new int[w * h];
coverBitmap.getPixels(piexs, 0, w, 0, 0, w, h);
```

图片可以看作是由二维像素组成的，`bitmap.getPixels(pixels, offset, stride, x, y, width, height)` 各参数含义：

- `pixels` 是接收图片像素点的数组
- `offset` 表示从第几个像素点开始从图片中读取
- `stride` 表示每行读取几个像素
- `x`、`y` 表示读取的第一个点的坐标
- `width`、`height` 表示要读取的长宽

然后遍历像素统计空白点：

```
for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
        if (piexs[i * w + j] == 0) {
            wipeArea += 1;
        }
    }
}

wipePercent = wipeArea * 1.0f / totalArea;
```
