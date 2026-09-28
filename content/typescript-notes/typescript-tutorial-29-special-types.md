---
title: "二十九、特殊类型"
weight: 29
description: "本章介绍 特殊类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 有四个特殊的类型：never、void、unknown 和 any。

它们在类型系统中扮演重要角色，理解它们的区别对于编写类型安全的代码至关重要。

## (一) never 类型

### 1. 实例

```typescript
function throwError(message: string): never {
throw new Error(message);
}
function infiniteLoop(): never {
while (true) {
console.log("运行中...");
}
}
var neverValue: never;
var num: number = neverValue;
console.log("never 赋值给 number: " + num);
```

## (二) void 类型

### 1. 实例

```typescript
function logMessage(message: string): void {
console.log("日志: " + message);
}
logMessage("Hello");
var empty: void = undefined;
console.log("void 变量: " + empty);
```

## (三) unknown 类型

### 1. 实例

```typescript
var value: unknown = "hello";
value = 42;
value = true;
if (typeof value === "string") {
var str: string = value;
console.log("字符串长度: " + str.length);
}
```

## (四) any 类型

### 1. 实例

```typescript
var anything: any = "hello";
anything = 42;
anything = true;
var str: string = anything;
var num: number = anything;
console.log("字符串: " + str);
console.log("数字: " + num);
var obj: any = {};
obj.foo();
obj.bar = "value";
```

## (五) 实际应用：Exhaustive Check

### 1. 实例

```typescript
type Shape = { kind: "circle", radius: number } | { kind: "square", side: number };
function area(shape: Shape): number {
switch (shape.kind) {
case "circle":
return Math.PI * shape.radius ** 2;
case "square":
return shape.side ** 2;
default:
var _exhaustive: never = shape;
return _exhaustive;
}
}
var circle = { kind: "circle" as const, radius: 5 };
var square = { kind: "square" as const, side: 4 };
console.log("圆形面积: " + area(circle).toFixed(2));
console.log("正方形面积: " + area(square));
```

## (六) 总结

- **never：**永不返回，用于 exhaustive check，是所有类型的子类型
- **void：**无返回值，用于普通没有返回值的函数
- **unknown：**安全的任意类型，使用前必须进行类型检查
- **any：**绕过所有类型检查，应该尽量避免使用
