---
title: "44.RxJava 基础"
weight: 39
description: "RxJava 入门：Observable、Observer 与 Subscriber 的创建与订阅关系，以及 subscribe 支持的不完整回调写法"
featureimage: "covers/android-basics.svg"
date: 2016-11-02
tags: ["Android"]
---

The Reactive Extensions

Java

Rx最显著的特性是使用可观察集合 来达到集成异步 和基于事件的编程的效果。

http://gank.io/post/560e15be2dca930e00da1083#toc_1

本篇要点：

1. 创建 Observer（观察者，订阅者）
2. 创建 Observable（被观察者）
3. Subscribe（订阅）
4. Rxjava的Operator和Operator
5. 自定义回掉

参考资料：

- 理解RxJava线程模型 http://blog.saymagic.cn/2016/08/20/understand-rxjava-threading-model.html
- RxJava https://github.com/sunflower-zyb/Blog/blob/master/R/Rxjava.md
- RxJava + Retrofit http://www.jianshu.com/p/2b0aeb6b6b61

![图片](figure01.webp)

Rxjava进阶一

Rxjava进阶二

Rxjava进阶三

Rxjava进阶四

教你写响应式框架（一）

http://blog.csdn.net/dd864140130/article/details/50877063

```
Observable.OnSubscribe<String> onSubscriber1 = new Observable.OnSubscribe<String>() {

    @Override
    public void call(Subscriber<? super String> subscriber) {
        subscriber.onNext("1");
        subscriber.onCompleted();
    }
};//创建被观察者
```

```
Subscriber<String> subscriber1 = new Subscriber<String>() {
    @Override
    public void onCompleted() {

    }

    @Override
    public void onError(Throwable e) {

    }

    @Override
    public void onNext(String s) {

    }
};//创建观察者

Observable.create(onSubscriber1).subscribe(subscriber1);//被观察者订阅观察者
```

![图片](figure02.gif)

# 一、创建 Observer（观察者，订阅者）

Observer 即观察者，它决定事件触发的时候将有怎样的行为。 RxJava 中的 Observer 接口的实现方式：

```
Observer<String> observer = new Observer<String>() {// 普通事件（相当于 onClick() / onEvent()）
    @Override
    public void onNext(String s) {}

    // 事件队列异常onError() 会被触发，
    // 同时队列自动终止，不允许再有事件发出。
    @Override
    public void onError(Throwable e) {}

    // 事件队列完结，当不会再有新的 onNext() 发出时
    // 需要触发 onCompleted() 方法作为标志。
    @Override
    public void onCompleted() {}
};
```

在一个正确运行的事件序列中，`onCompleted()` 和 `onError()` 有且只有一个，并且是事件序列中的最后一个。需要注意的是，`onCompleted()` 和 `onError()` 二者也是互斥的，即在队列中调用了其中一个，就不应该再调用另一个

除了 Observer 接口之外，RxJava 还内置了一个实现了 Observer 的抽象类：

Subscriber。 Subscriber 对 Observer 接口进行了一些扩展，但他们的基本使用方式是完全一样的：

```
Subscriber<String> subscriber = new Subscriber<String>() {
    @Override
    public void onNext(String s) {  }

    @Override
    public void onCompleted() {  }

    @Override
    public void onError(Throwable e) {  }
};
```

实质上，在 RxJava 的 subscribe 过程中，Observer 也总是会先被转换成一个 Subscriber 再使用。所以如果你只想使用基本功能，选择 Observer 和 Subscriber 是完全一样的。它们的区别对于使用者来说主要有两点：

1. `onStart()`: 这是 `Subscriber` 增加的方法。它会在 subscribe 刚开始，而事件还未发送之前被调用，可以用于做一些准备工作，例如数据的清零或重置。这是一个可选方法，默认情况下它的实现为空。需要注意的是，如果对准备工作的线程有要求（例如弹出一个显示进度的对话框，这必须在主线程执行），`onStart()` 就不适用了，因为它总是在 subscribe 所发生的线程被调用，而不能指定线程。要在指定的线程来做准备工作，可以使用 `doOnSubscribe()` 方法，具体可以在后面的文中看到。
2. `unsubscribe()`: 这是 `Subscriber` 所实现的另一个接口 `Subscription` 的方法，用于取消订阅。在这个方法被调用后，`Subscriber` 将不再接收事件。一般在这个方法调用前，可以使用 `isUnsubscribed()` 先判断一下状态。`unsubscribe()` 这个方法很重要，因为在 `subscribe()` 之后，`Observable` 会持有 `Subscriber` 的引用，这个引用如果不能及时被释放，将有内存泄露的风险。所以最好保持一个原则：要在不再使用的时候尽快在合适的地方（例如 `onPause()` `onStop()` 等方法中）调用 `unsubscribe()` 来解除引用关系，以避免内存泄露的发生。

# 二、创建 Observable（被观察者）

Observable 即被观察者，它决定什么时候触发事件以及触发怎样的事件。 RxJava 使用 create() 方法来创建一个 Observable ，并为它定义事件触发规则：

```
//Observable（被观察者，事件源）
Observable observable = Observable.create(new Observable.OnSubscribe<String>() {
    //订阅事件
    @Override
    public void call(Subscriber<? super String> subscriber) {
        //Subscriber（订阅者是 Observer（观察者）的子类）
        subscriber.onNext("Hello");
        subscriber.onNext("Hi");
        subscriber.onNext("Aloha");
        subscriber.onCompleted();
    }
});
```

可以看到，这里传入了一个 `OnSubscribe` 对象作为参数。

`OnSubscribe` 会被存储在返回的 `Observable` 对象中，它的作用相当于一个计划表，当 `Observable` 被订阅的时候，`OnSubscribe` 的 `call()` 方法会自动被调用，事件序列就会依照设定依次触发（对于上面的代码，就是观察者 `Subscriber` 将会被调用三次 `onNext()` 和一次 `onCompleted()` ）。这样，由被观察者调用了观察者的回调方法，就实现了由被观察者向观察者的事件传递，即观察者模式。

重载方法：

- `Subscription  subscribe()`
- `Subscription  subscribe(Action1<? super T> onNext)`
- `Subscription  subscribe(Action1<? super T> onNext, Action1< java.lang .Throwable> onError)`
- `Subscription  subscribe(Action1<? super T> onNext, Action1< java.lang .Throwable> onError, Action0 onComplete)`
- `Subscription  subscribe(Observer<? super T> observer)`
- `Subscription  subscribe(Subscriber<? super T> subscriber)`

create() 方法是 RxJava 最基本的创造事件序列的方法。基于这个方法， RxJava 还提供了一些方法用来快捷创建事件队列，例如：

- `just(T...)`: 将传入的参数依次发送出来。

  ```
  Observable observable = Observable.just("Hello", "Hi", "Aloha");
  // 将会依次调用：
  // onNext("Hello");
  // onNext("Hi");
  // onNext("Aloha");
  // onCompleted();
  ```

- `from(T[]) / from(Iterable<? extends T>)`: 将传入的数组或 Iterable 拆分成具体对象后，依次发送出来。

  ```
  String[] words = {"Hello", "Hi", "Aloha"};
  Observable observable = Observable.from(words);
  // 将会依次调用：
  // onNext("Hello");
  // onNext("Hi");
  // onNext("Aloha");
  // onCompleted();
  ```

上面 just(T...) 的例子和 from(T[]) 的例子，都和之前的 create(OnSubscribe) 的例子是等价的。

# 三、Subscribe（订阅）

创建了 Observable 和 Observer 之后，再用 subscribe() 方法将它们联结起来，整条链子就可以工作了。代码形式很简单：

```
observable.subscribe(observer);
// 或者：
observable.subscribe(subscriber);
```

subscribe() 这个方法有点怪：它看起来是『observalbe 订阅了 observer / subscriber』而不是『observer /subscriber 订阅了 observalbe』，这看起来就像『杂志订阅了读者』一样颠倒了对象关系。Observable.subscribe(Subscriber) 的内部实现是这样的（仅核心代码）：

```
// 注意：这不是 subscribe() 的源码，
// 而是将源码中与性能、兼容性、扩展性有关的代码剔除后的核心代码。
// 如果需要看源码，可以去 RxJava 的 GitHub 仓库下载。
public Subscription subscribe(Subscriber subscriber) {
    subscriber.onStart();
    onSubscribe.call(subscriber);
    return subscriber;
}
```

可以看到，`subscriber()` 做了3件事：

1. 调用 `Subscriber.onStart()`。这个方法在前面已经介绍过，是一个可选的准备方法。
2. 调用 `Observable` 中的 `OnSubscribe.call(Subscriber)`。在这里，事件发送的逻辑开始运行。从这也可以看出，在 RxJava 中，`Observable` 并不是在创建的时候就立即开始发送事件，而是在它被订阅的时候，即当 `subscribe()` 方法执行的时候。
3. 将传入的 `Subscriber` 作为 `Subscription` 返回。这是为了方便 `unsubscribe()`.

![图片](figure03.gif)

# 四、自定义回叫

除了 `subscribe(Observer)` 和 `subscribe(Subscriber)`，`subscribe()` 还支持不完整定义的回调，RxJava 会自动根据定义创建出 `Subscriber` 。形式如下：

```
//Action1 也是一个接口，它同样只有一个方法 call(T param)，
// 这个方法也无返回值，但有一个参数
Action1<String> onNextAction = new Action1<String>() {
    // onNext()
    @Override
    public void call(String s) {
        Log.d(tag, s);
    }
};

Action1<Throwable> onErrorAction = new Action1<Throwable>() {
    // onError()
    @Override
    public void call(Throwable throwable) {
        // Error handling
    }
};

// Action0 是 RxJava 的一个接口，
// 它只有一个方法 call()，这个方法是无参无返回值的
Action0 onCompletedAction = new Action0() {
    // onCompleted()
    @Override
    public void call() {
        Log.d(tag, "completed");
    }
};

// 自动创建 Subscriber ，并使用 onNextAction 来定义 onNext()
observable.subscribe(onNextAction);

// 自动创建 Subscriber ，并使用 onNextAction
// 和 onErrorAction 来定义 onNext() 和 onError()
observable.subscribe(onNextAction, onErrorAction);

// 自动创建 Subscriber ，并使用 onNextAction、 onErrorAction
// 和 onCompletedAction 来定义 onNext()、 onError() 和 onCompleted()
observable.subscribe(onNextAction, onErrorAction, onCompletedAction);
```

简单解释一下这段代码中出现的 Action1 和 Action0。 Action0 是 RxJava 的一个接口，它只有一个方法 call()，这个方法是无参无返回值的；由于 onCompleted() 方法也是无参无返回值的，因此 Action0 可以被当成一个包装对象，将 onCompleted() 的内容打包起来将自己作为一个参数传入 subscribe() 以实现不完整定义的回调。这样其实也可以看做将 onCompleted() 方法作为参数传进了subscribe()，相当于其他某些语言中的『闭包』。 Action1 也是一个接口，它同样只有一个方法 call(T param)，这个方法也无返回值，但有一个参数；与 Action0 同理，由于 onNext(T obj) 和 onError(Throwable error) 也是单参数无返回值的，因此 Action1 可以将 onNext(obj) 和 onError(error) 打包起来传入 subscribe() 以实现不完整定义的回调。事实上，虽然 Action0 和 Action1 在 API 中使用最广泛，但 RxJava 是提供了多个 ActionX 形式的接口 (例如 Action2, Action3) 的，它们可以被用以包装不同的无返回值的方法。注：正如前面所提到的，Observer 和 Subscriber 具有相同的角色，而且 Observer 在 subscribe() 过程中最终会被转换成Subscriber 对象，因此，从这里开始，后面的描述我将用 Subscriber 来代替 Observer ，这样更加严谨。
