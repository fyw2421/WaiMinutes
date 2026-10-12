---
title: "重写adapter时注意的问题"
weight: 35
description: "重写 BaseAdapter 的 getView 时用 ViewHolder 复用 convertView，避免每次都新建 itemView"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

`BaseAdapter` 中 `getView` 写法的注意：

```
public void getView() {
    if (convertView == null) {
        // item 中各个变量的初始化
        // 用一个 viewHolder 包含各个子控件
    } else {
        viewhloder = (ViewHold) convertView.getTag();
    }

    // item 中各个控件要加载的内容，要实现的事件
}
```

`convertView` 是一个 item 的池，已有的 item 消失后会存入 `convertView`。用上面的方法充分利用这个特性，就不会每次都创建一个新的 itemView。
