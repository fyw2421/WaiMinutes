---
title: "notifcaton的使用"
weight: 13
description: "Notification 的基本用法与自定义视图：设置事件、声音、进度，再由 RemoteViews 刷新内容"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

# 一、简单 Notification

创建好一个 notification 对象：

```
notification.setLatestEventInfo(context, contentTitle, contentText, contentIntent);
```

最后一个参数不能设置为 null。如果点击 notification 后不需要进入某个 Activity，需要在创建 contentIntent 时设置：

```
PendingIntent contentIntent = PendingIntent.getActivity(context, 0, null, 0);
```

# 二、自定义 Notification

更新 view 里显示的方法：实例化一个 notification 后先做一些普通设置，如声音提醒：

```
notification.sound = Notification.DEFAULT_SOUND | Notification.DEFAULT_LIGHTS;
```

触发时间为 `notification.when`。

最后配上一个视图，每次更新进度的时候都要重新 new 一个 RemoteViews 出来：

```
notification.contentView = new RemoteViews(getPackageName(), R.layout.downloadfinishlayout);

manager.notify(1, notification);
```

更新 Notification 视图的一些方法：

```
notification.contentView.setProgressBar(R.id.notiPb, 100, values[0], false);
notification.contentView.setTextViewText(viewId, text);
```

远程视图中的控件是有一定限制的。截至 android 2.2 版本，以下控件才能用于远程视图：

- FrameLayout
- LinearLayout
- RelativeLayout
- AnalogClock
- Button
- Chronometer
- ImageButton
- ImageView
- ProgressBar
- TextView
