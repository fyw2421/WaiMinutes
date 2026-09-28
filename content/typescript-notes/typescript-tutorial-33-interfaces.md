---
title: "三十三、接口"
weight: 33
description: "本章介绍 接口。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

接口（Interface）是 TypeScript 中的一个核心概念，用于定义对象的结构和类型。

## (一) 基本接口

### 1. 实例

```typescript
interface Person {
name: string;
age: number;
}
function greet(person: Person): string {
return "Hello, " + person.name + "! 年龄: " + person.age;
}
var alice: Person = { name: "Alice", age: 30 };
console.log(greet(alice));
```

## (二) 可选属性

### 1. 实例

```typescript
interface Config {
url: string;
method?: string;
timeout?: number;
}
function createRequest(config: Config): void {
var method = config.method || "GET";
var timeout = config.timeout || 5000;
console.log("请求: " + config.url + ", 方法: " + method + ", 超时: " + timeout);
}
createRequest({ url: "https://api.example.com" });
createRequest({ url: "https://api.example.com", method: "POST", timeout: 10000 });
```

## (三) 只读属性

### 1. 实例

```typescript
interface Point {
readonly x: number;
readonly y: number;
}
var p1: Point = { x: 10, y: 20 };
console.log("坐标: (" + p1.x + ", " + p1.y + ")");
```

// p1.x = 5; // 错误!

## (四) 函数类型接口

### 1. 实例

```typescript
interface SearchFunc {
(source: string, subString: string): boolean;
}
var search: SearchFunc = function(source, subString) {
return source.indexOf(subString) !== -1;
};
console.log(search("Hello World", "World"));
console.log(search("Hello World", "TypeScript"));
```

## (五) 索引签名

### 1. 实例

```typescript
interface StringArray {
[index: number]: string;
}
var myArray: StringArray = ["Apple", "Banana", "Orange"];
console.log("第一个元素: " + myArray[0]);
```

## (六) 类类型接口

### 1. 实例

```typescript
interface ClockInterface {
currentTime: Date;
setTime(d: Date): void;
}
class Clock implements ClockInterface {
currentTime: Date = new Date();
setTime(d: Date): void {
this.currentTime = d;
}
}
var clock = new Clock();
console.log("当前时间: " + clock.currentTime);
clock.setTime(new Date("2024-01-01"));
console.log("设置时间: " + clock.currentTime);
```

## (七) 接口继承

### 1. 实例

```typescript
interface Shape {
color: string;
}
interface Square extends Shape {
sideLength: number;
}
var square: Square = { color: "red", sideLength: 10 };
console.log("正方形: " + square.color + ", 边长: " + square.sideLength);
```

## (八) 接口合并

### 1. 实例

```typescript
interface Box {
height: number;
width: number;
}
interface Box {
depth: number;
}
var box: Box = { height: 10, width: 20, depth: 30 };
console.log("盒子: " + box.height + "x" + box.width + "x" + box.depth);
```

## (九) 总结

- **基本接口：**定义对象的结构和类型
- **可选属性：**使用 `?` 标记可选属性
- **只读属性：**使用 `readonly` 关键字
- **函数类型：**定义函数签名
- **索引签名：**定义索引类型
- **类类型：**类可以实现接口
- **继承：**接口可以继承多个接口
- **合并：**同名接口自动合并
