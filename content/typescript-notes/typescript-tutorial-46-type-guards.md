---
title: "四十六、类型守卫"
weight: 46
description: "本章介绍 类型守卫。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

类型守卫是 TypeScript 中非常重要的类型缩小机制，通过运行时条件检查让编译器准确推断变量的具体类型。

## (一) typeof 类型守卫

```typescript
function printValue(value: string | number): void {
    if (typeof value === "string") {
        console.log("字符串长度: " + value.length);
    } else {
        console.log("数字翻倍: " + (value * 2));
    }
}
```

## (二) instanceof 类型守卫

```typescript
function makeSound(animal: Dog | Cat): void {
    if (animal instanceof Dog) {
        animal.bark();
    } else {
        animal.meow();
    }
}
```

## (三) 自定义类型守卫

```typescript
function isString(value: any): value is string {
    return typeof value === "string";
}
function isNumber(value: any): value is number {
    return typeof value === "number";
}
```

## (四) in 操作符类型守卫

```typescript
function process(obj: A | B): void {
    if ("a" in obj) {
        console.log("A 的属性 a: " + obj.a);
    } else {
        console.log("B 的属性 b: " + obj.b);
    }
}
```

## (五) 可辨识联合

```typescript
type Shape = Circle | Rectangle | Triangle;
function getArea(shape: Shape): number {
    switch (shape.kind) {
        case "circle": return Math.PI * shape.radius ** 2;
        case "rectangle": return shape.width * shape.height;
        case "triangle": return 0.5 * shape.base * shape.height;
    }
}
```

## (六) null/undefined 检查

```typescript
function getLength(str: string | null): number {
    if (str !== null) { return str.length; }
    return 0;
}
```
