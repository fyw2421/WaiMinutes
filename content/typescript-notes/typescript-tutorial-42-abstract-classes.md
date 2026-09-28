---
title: "四十二、抽象类"
weight: 42
description: "本章介绍 抽象类。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

抽象类（Abstract Class）是一种不能被直接实例化的类，只能作为基类供其他类继承。

## (一) 抽象类基础

```typescript
abstract class Animal {
    name: string;
    constructor(name: string) {
        this.name = name;
    }
    abstract speak(): void;
    move(): void {
        console.log(this.name + " 在移动");
    }
}

class Dog extends Animal {
    speak(): void {
        console.log(this.name + " 汪汪汪!");
    }
}

var dog = new Dog("旺财");
dog.speak();
dog.move();
```

## (二) 抽象方法

子类必须实现所有抽象方法：

```typescript
abstract class Shape {
    abstract area(): number;
    abstract perimeter(): number;
    describe(): void {
        console.log("面积: " + this.area().toFixed(2));
    }
}
```

## (三) 抽象类作为类型

```typescript
function makeSpeak(animal: Animal): void {
    animal.speak();
}
makeSpeak(new Cat());
makeSpeak(new Dog());
```

## (四) 抽象类与接口的区别

| 特性 | 抽象类 | 接口 |
|---|---|---|
| 实例化 | 不能 | 不能 |
| 方法实现 | 可以有 | 不能 |
| 成员修饰符 | public/protected/private | 仅 readonly |
| 继承 | 单继承 | 多实现 |
| 构造函数 | 可以有 | 不能 |

## (五) 完整示例：支付系统

```typescript
abstract class Payment {
    abstract process(amount: number): boolean;
    validate(): void {
        console.log("验证支付信息");
    }
}
class CreditCardPayment extends Payment {
    process(amount: number): boolean {
        console.log("处理信用卡支付: " + amount);
        return true;
    }
}
class PayPalPayment extends Payment {
    process(amount: number): boolean {
        console.log("处理 PayPal 支付: " + amount);
        return true;
    }
}
```
