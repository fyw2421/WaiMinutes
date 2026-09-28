---
title: "三十一、元组"
weight: 31
description: "本章介绍 元组。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 中的元组（Tuple）是一种特殊类型的数组，它允许在数组中存储不同类型的元素。

创建元组的语法格式如下：

let tuple: [类型1, 类型2, 类型3, ...];

## (一) 实例

```typescript
let mytuple: [number, string];
mytuple = [42, "Runoob"];
```

## (二) 访问元组

## (三) TypeScript

```typescript
let mytuple: [number, string, boolean] = [42, "Runoob", true];
let num = mytuple[0];
let str = mytuple[1];
let bool = mytuple[2];
console.log(num);
console.log(str);
console.log(bool);
```

## (四) 元组运算

### 1. TypeScript

```typescript
var tuple = [42, "Hello"];
tuple.push("World");
console.log(tuple);
```

## (五) 实例

```typescript
let tuple: [number, string, boolean] = [42, "Hello", true];
let lastElement = tuple.pop();
console.log(lastElement);
console.log(tuple);
```

## (六) 更新元组

### 1. TypeScript

```typescript
var mytuple = [42, "Runoob", "Taobao", "Google"];
console.log("元组的第一个元素为：" + mytuple[0]);
mytuple[0] = 121;
console.log("元组中的第一个元素更新为：" + mytuple[0]);
```

## (七) 解构元组

### 1. TypeScript

```typescript
let a: [number, string, boolean] = [42, "Hello", true];
var [b, c] = a;
console.log(b);
console.log(c);
```

## (八) 使用标签元组

let tuple: [id: number, name: string] = [1, "John"];

## (九) 元组的实际应用

### 1. 实例

```typescript
function getUserInfo(): [number, string] {
return [1, "John Doe"];
}
const [userId, userName] = getUserInfo();
console.log(userId);
console.log(userName);
```

## (十) 其他操作

### 1. 连接元组

```typescript
let tuple1: [number, string] = [42, "Hello"];
let tuple2: [boolean, number] = [true, 100];
let result = tuple1.concat(tuple2);
```

### 2. 切片元组

```typescript
let tuple: [number, string, boolean] = [42, "Hello", true];
let sliced = tuple.slice(1);
```

### 3. 遍历元组

```typescript
let tuple: [number, string, boolean] = [42, "Hello", true];
for (let item of tuple) { console.log(item); }
```

### 4. 扩展元组

```typescript
let tuple1: [number, string] = [42, "Hello"];
let tuple2: [boolean] = [true];
let extendedTuple: [number, string, ...typeof tuple2] = [42, "Hello", ...tuple2];
```
