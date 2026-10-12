---
title: "adb"
weight: 1
description: "adb 常用命令：查看已安装的包名、release 模式安装 apk、启动 Android 设置"
featureimage: "covers/android-notes.svg"
date: 2016-11-02
tags: ["Android"]
---

# 一、查看已安装的包名

```
adb shell pm list package
```

# 二、release 模式安装 apk

1. Download 最新的版本到你的 DGGM 机器中。

2. 在 PC 上通过 mtp 在设备上创建 vtech 目录，并写入几个标志文件和 eid.txt。

3. 打开机器的 debug。

4. 利用 windows 系统的 cmd.exe，`adb shell` 进入 adb 客户端。

5. 在 adb shell 中输入命令 `mount -o remount, rw /system` 让 system 目录可写。

6. 在 adb shell 中输入命令 `cd /system/app` 进入 app 目录。

7. 在 adb shell 中输入命令 `rm myapp.apk` 删除你的 apk。

8. Ctrl + D 退出 adb shell。

9. 在 cmd 模式下，输入命令 `adb push myapp.apk /system/app`

10. 在 cmd 模式下，输入命令 `adb reboot` 重启机器。

11. 通过 DGGM 的桌面，点击 icon 来运行你的 app，如果运行正常，就可以提交你的 apk 到 \10.126.72.97 的对应目录了。

# 三、启动 Android 设置

```
adb shell am start com.android.settings/com.android.settings.Settings
```
