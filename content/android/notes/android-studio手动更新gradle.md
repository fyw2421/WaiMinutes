---
title: "Android Studio手动更新Gradle"
weight: 2
description: "在打开Android Studio项目的时候，会下载项目对应版本的gradle，由于是国外网站导致更新较慢，该版本是在项目根目录下gradle/gradle-w"
featureimage: "covers/android-notes.svg"
date: 2016-11-02
tags: ["Android"]
---

  在打开Android Studio项目的时候，会下载项目对应版本的gradle，由于是国外网站导致更新较慢，该版本是在项目根目录下gradle/gradle-wrapper.properties文件中指定的：

解决办法：
　　
1. 离线下载gradle*.zip: <http://services.gradle.org/distributions>
2. 将下载的文件复制到C:\Users\username\.gradle\wrapper\dists\gradle-XXXX\d3xh0kipe7wr2bvnx5sk0hao8
3. 重启Android Studio

   备注: C:\Users\Administrator\.gradle\wrapper\dists\gradle-2.5-all\d3xh0kipe7wr2bvnx5sk0hao8
