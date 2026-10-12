---
title: "ValueAnimator的使用"
weight: 17
description: "ValueAnimator 能在动画过程中逐帧给出设定好的值，用它实现自定义效果"
featureimage: "covers/android-advanced.svg"
date: 2016-11-02
tags: ["Android"]
---

`ValueAnimator` 的作用是可以逐一获得你设置好的值，获得这些值后再利用它实现你想要的效果。我用这个动画改进了那个扇形百分比控件，效果十分炫酷。

```
valueAnimator = ValueAnimator.ofInt(0, sweepAngle);
valueAnimator.setDuration(1000);
valueAnimator.setInterpolator(new BounceInterpolator());

valueAnimator.addUpdateListener(new AnimatorUpdateListener() {

    @Override
    public void onAnimationUpdate(ValueAnimator animation) {
        // TODO Auto-generated method stub
        Integer f = (Integer) animation.getAnimatedValue();
        setSweepAngle(f);
    }
});

valueAnimator.start();
```
