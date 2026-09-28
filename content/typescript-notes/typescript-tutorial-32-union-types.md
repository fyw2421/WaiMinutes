---
title: "三十二、联合类型"
weight: 32
description: "本章介绍 联合类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

联合类型（Union Types）是 TypeScript 中非常强大的类型特性，它允许一个变量拥有多种可能的类型。

## (一) 联合类型基础

### 1. 实例

```typescript
var value: string | number;
value = "hello";
console.log("字符串值: " + value);
value = 42;
console.log("数值: " + value);
```

## (二) 联合类型与类型收窄

### 1. 实例

```typescript
function displayValue(value: string | number): void {
if (typeof value === "string") {
console.log("字符串: " + value.toUpperCase());
} else {
console.log("数字: " + value.toFixed(2));
}
}
displayValue("hello");
displayValue(42);
```

## (三) 联合类型与数组

### 1. 实例

```typescript
var mixedArray: (string | number)[] = ["hello", 42, "world", 100];
console.log(mixedArray);
function sumOrConcat(value: string | number, addValue: string | number): string | number {
if (typeof value === "number" && typeof addValue === "number") {
return value + addValue;
}
return String(value) + String(addValue);
}
console.log(sumOrConcat(10, 20));
console.log(sumOrConcat("Hello", " World"));
```

## (四) 联合类型与字面量类型

### 1. 实例

```typescript
type Status = "success" | "error" | "pending";
type ResultCode = 200 | 400 | 401 | 500;
var currentStatus: Status = "success";
var code: ResultCode = 200;
console.log("状态: " + currentStatus);
console.log("状态码: " + code);
```

## (五) 联合类型在实际代码中的应用

### 1. 实例

```typescript
type Shape =
{ kind: "circle"; radius: number }
{ kind: "square"; side: number }
{ kind: "triangle"; base: number; height: number };
```

```typescript
function getArea(shape: Shape): number {
switch (shape.kind) {
case "circle":
return Math.PI * shape.radius ** 2;
case "square":
return shape.side ** 2;
case "triangle":
return 0.5 * shape.base * shape.height;
}
}
var shapes: Shape[] = [
{ kind: "circle", radius: 5 },
{ kind: "square", side: 4 },
{ kind: "triangle", base: 3, height: 6 }
];
shapes.forEach(function(shape) {
console.log(shape.kind + " 面积: " + getArea(shape).toFixed(2));
});
```

## (六) 总结

- **基础用法：**使用 `|` 运算符创建联合类型
- **类型收窄：**使用 typeof 或判断属性区分联合成员
- **可辨别联合：**使用共同的字面量属性区分
- **实用性：**联合类型非常适用于处理多种可能的状态或结构
