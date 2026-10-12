---
title: "handler机制"
weight: 9
description: "Handler 用于跨线程更新 UI 与发送消息；Handler、Looper、MessageQueue 三者组成消息机制"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

# 一、Handler 的作用

1. 更新 UI
2. 发送处理消息

后台工作线程不能更新 UI，需要 Handler 传递消息到主线程来更新 UI。

创建一个消息有两种方式：

1. `new Message()`
2. `handler.obtain()`——在这个方法中，它会把消息的发送目标 target 设置为 handler 本身

发送消息也有两种方式：

1. `handler.sendMessage()`
2. `message.sendToTarget()`

# 二、为什么要用 Handler

使用 Handler 的根本原因是：为了解决多线程并发导致错乱的问题。如果每个线程都去更新 UI，很有可能界面混乱；如果给更新界面的方法加上锁，对于需要大量绘制 UI 的操作效率极其低下。

所以 Android 提供了 Handler 机制，让所有的 UI 更新操作都放到主线程中，利用 Handler 发送消息到主线程，形成一个消息队列，顺序执行 UI 更新。

# 三、Handler、Looper、MessageQueue

Handler 发送消息出去，Looper 负责接收消息，并把消息发送给目标（一般是 Handler 本身）；MessageQueue 则是负责存储消息，由 Looper 来管理，包含在 Looper 这个类里面。

创建线程自己的 Handler：首先调用 `Looper.prepare()`，通过 `Looper.myLooper()` 可以获得本线程的 Looper，然后调用 `Looper.loop()` 开始轮询消息。有消息则会进行处理，否则阻塞。

在主线程中它自己已经调用了这两个方法、创建了主线程的 Looper 对象，所以主线程中只要重写 Handler 即可。

# 四、指定 Looper

Handler 在创建的时候可以指定 Looper 对象：

```
handler = new Handler(thread.getLooper()) {
    public void handMessage() {}
};
```

然后通过 Handler 发送消息，消息传到这个线程去执行，也就相当于开启了一个子线程，可以执行网络操作、IO 操作。
