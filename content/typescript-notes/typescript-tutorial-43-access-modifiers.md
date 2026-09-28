---
title: "四十三、访问修饰符"
weight: 43
description: "本章介绍 访问修饰符。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

访问修饰符用于控制类成员的可见性，实现封装。

## (一) public 修饰符

默认修饰符，任何地方都可以访问：

```typescript
class Animal {
    public name: string;
    public constructor(name: string) {
        this.name = name;
    }
    public speak(): void {
        console.log(this.name + " 发出声音");
    }
}
```

## (二) private 修饰符

只能在定义它的类内部访问：

```typescript
class BankAccount {
    private balance: number;
    constructor(initialBalance: number) {
        this.balance = initialBalance;
    }
    public getBalance(): number {
        return this.balance;
    }
}
```

## (三) protected 修饰符

可以在类内部和子类中访问：

```typescript
class Person {
    protected name: string;
    constructor(name: string) { this.name = name; }
}
class Employee extends Person {
    public introduce(): string {
        return "我是 " + this.name; // 子类可以访问 protected
    }
}
```

## (四) readonly 修饰符

只读属性，只能在声明时或构造函数中赋值：

```typescript
class User {
    readonly id: number;
    readonly name: string;
    constructor(id: number, name: string) {
        this.id = id;
        this.name = name;
    }
}
```

## (五) 参数属性

在构造函数参数上直接使用访问修饰符：

```typescript
class Point {
    constructor(
        public x: number,
        public y: number,
        private z: number
    ) {}
}
```

## (六) 访问修饰符对比

| 修饰符 | 类内部 |
|---|---|
| 子类 | 外部 |
| public | ✓ |
| ✓ | ✓ |
| protected | ✓ |
| ✓ | ✗ |
| private | ✓ |
| ✗ | ✗ |
