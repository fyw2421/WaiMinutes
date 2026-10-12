---
title: "ObjectAnimator属性动画的使用"
weight: 14
description: "用 ObjectAnimator 做位移动画，再用 AnimatorSet 组合多个动画同时播放"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

```
ObjectAnimator xAnimator = ObjectAnimator.ofFloat(allChild.get(i), "translationX", 0, x);
ObjectAnimator yAnimator = ObjectAnimator.ofFloat(allChild.get(i), "translationY", 0, y);

AnimatorSet set = new AnimatorSet();
set.setDuration(i * 500);
set.setInterpolator(new BounceInterpolator());

// 可以配合多种动画同时播放，其中还有 after、before 等方法
set.play(xAnimator).with(yAnimator);

OpenAnimatorSets.add(set);
```
