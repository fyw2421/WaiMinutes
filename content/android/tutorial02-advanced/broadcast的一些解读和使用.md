---
title: "BroadCast的一些解读和使用"
weight: 5
description: "发送广播后，接收者只要匹配该 action，不论是否同一进程（不同 app）都会触发"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

发送一个广播，只要接收者匹配该广播（接收者和发送广播设置的 action 一致），不管在不在一个进程里面（不同的 app），都将触发广播。
