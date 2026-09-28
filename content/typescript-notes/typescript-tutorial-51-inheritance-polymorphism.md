---
title: "五十一、类继承与多态"
weight: 51
description: "本章介绍 类继承与多态。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

## (一) 类的继承

使用 extends 关键字实现继承：

```typescript
class Animal {
    name: string;
    constructor(name: string) { this.name = name; }
    speak(): void { console.log(this.name + " 发出声音"); }
}
class Dog extends Animal {
    breed: string;
    constructor(name: string, breed: string) {
        super(name);
        this.breed = breed;
    }
    speak(): void { console.log(this.name + " 汪汪汪!"); }
}
```

## (二) super 关键字

```typescript
class Shape {
    color: string;
    constructor(color: string) { this.color = color; }
    describe(): string { return "这是一个 " + this.color + " 的图形"; }
}
class Circle extends Shape {
    radius: number;
    constructor(color: string, radius: number) {
        super(color);
        this.radius = radius;
    }
    describe(): string {
        return super.describe() + "，半径是 " + this.radius;
    }
}
```

## (三) 多态

子类的实例可以赋值给父类类型：

```typescript
var animals: Animal[] = [new Cat("小白"), new Dog("旺财"), new Animal("动物")];
for (var animal of animals) { animal.speak(); }
```

## (四) instanceof 检查

```typescript
if (shape instanceof Rectangle) { console.log("矩形面积: " + shape.area()); }
else if (shape instanceof Circle) { console.log("圆形面积: " + shape.area()); }
```

## (五) protected 成员

```typescript
class Person {
    protected name: string;
    constructor(name: string) { this.name = name; }
}
class Employee extends Person {
    public introduce(): string {
        return "我是 " + this.name + "，在 " + this.department + " 工作";
    }
}
```
