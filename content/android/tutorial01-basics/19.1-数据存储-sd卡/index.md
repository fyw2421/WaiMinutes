---
title: "19.1 数据存储--SD卡"
weight: 17
description: "往 SDCard 写入数据需要的权限与判断方法，Environment 类的常用常量与方法，SD 卡存储流程与实际例子"
featureimage: "covers/android-basics.svg"
date: 2016-11-02
tags: ["Android"]
---

# 一、权限和判断！

1. `<!-- 在SDCard中创建与删除文件权限 -->`
2. `<uses-permission android:name="android.permission.MOUNT_UNMOUNT_FILESYSTEMS" />`
3.
4. `<!-- 往SDCard中写入数据权限 -->`

```
<!-- 往SDCard中写入数据权限 -->

<uses-permission
    android:name
    =
    "android.permission.WRITE_EXTERNAL_STORAGE"
    />

//要判断一下是否插入SD卡和是否具有读写的能力：

Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)
```

# 二、Environment类

1. 常用常量
2. 常用方法
3. 使用IO流操作SD卡上的文件
4. 关闭数据流

# 三、SD卡存储流程

判断是否有SD卡→获取SD卡目录`"/mnt/sdcard/"`→使用IO流操作SD卡上的文件→关闭数据流

Android系统中数据基本都是私有的，一般存放在"data/data/程序包名"目录下

- `Context.MODE_PRIVATE`：默认操作模式,覆盖原文件的内容.
- `Context.MODE_APPEND`：追加文件内容

# 四、实际例子

http://blog.csdn.net/worker90/article/details/6975523

# 五、Environment类

![图片](figure01.webp)

## (一) 常用常量

String MEDIA_MOUNTED|当前Android的外部存储器可读可写
---|---
String MEDIA_MOUNTED_READ_ONLY|当前Android的外部存储器只读

## (二) 常用方法

方法名称|描述
---|---
public static File getDataDirectory ()|获得Android下的data文件夹的目录
public static File getDownloadCacheDirectory ()|获得Android Download/Cache 内容的目录
public static File getExternalStorageDirectory ()|获得Android外部存储器也就是SDCard的目录
public static String getExternalStorageState ()|获得Android外部存储器的当前状态
public static File getRootDirectory ()|获得Android下的root文件夹的目录

```
//获得存储路径

String Root = Environment.getExternalStorageDirectory().getPath();
```
