---
title: "android中的一些文件操作"
weight: 3
description: "创建目录前先判断是否存在；RandomAccessFile 可在文件任意位置写入数据"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

在创建一个目录时，先要判断它存不存在：

```
if (file.exits()) {
    file.makeDir();
}
```

`RandomAccessFile` 是一种可以在文件任意位置写入数据的流：

```
RandomAccessFile accessFile = new RandomAccessFile(file, "rwd");  // rwd 表示可以 read write delete
```
