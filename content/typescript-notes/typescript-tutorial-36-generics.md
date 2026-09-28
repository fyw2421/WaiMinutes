---
title: "三十六、泛型"
weight: 36
description: "本章介绍 泛型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

泛型（Generics）是 TypeScript 中一个非常强大的特性，允许我们创建可复用的组件，同时保持类型安全。

## (一) 泛型函数

### 1. 实例

```typescript
function identity(arg: T): T {
return arg;
}
var output1 = identity("Hello Generics");
var output2 = identity(42);
console.log(output1);
console.log(output2);
```

## (二) 泛型约束

### 1. 实例

```typescript
interface Lengthwise {
length: number;
}
function loggingIdentity(arg: T): T {
console.log("长度: " + arg.length);
return arg;
}
loggingIdentity("Hello");
loggingIdentity([1, 2, 3]);
```

// loggingIdentity(42); // 错误

## (三) 泛型接口

### 1. 实例

```typescript
interface GenericIdentityFn{
(arg: T): T;
}
function identity2(arg: T): T {
return arg;
}
var myIdentity: GenericIdentityFn= identity2;
console.log(myIdentity(123));
```

## (四) 泛型类

### 1. 实例

```typescript
class GenericNumber{
zeroValue: T;
add: (x: T, y: T) => T;
constructor(zero: T, addFn: (x: T, y: T) => T) {
this.zeroValue = zero;
this.add = addFn;
}
}
var numberInstance = new GenericNumber(0, function(x, y) { return x + y; });
console.log(numberInstance.add(5, 3));
var stringInstance = new GenericNumber("", function(x, y) { return x + y; });
console.log(stringInstance.add("Hello", " World"));
```

## (五) 泛型与数组

### 1. 实例

```typescript
function getFirstElement(arr: T[]): T {
return arr[0];
}
var firstNumber = getFirstElement([1, 2, 3]);
var firstString = getFirstElement(["a", "b", "c"]);
console.log(firstNumber);
console.log(firstString);
```

## (六) 多类型参数

### 1. 实例

```typescript
function pair(first: T, second: U): [T, U] {
return [first, second];
}
var pair1 = pair("age", 30);
var pair2 = pair(1, true);
console.log(pair1);
console.log(pair2);
```

## (七) 总结

- 泛型函数：**使用 `` 定义类型参数**
- 泛型约束：**使用 `extends` 限制类型范围**
- 泛型接口：**接口中定义泛型**
- 泛型类：**类中定义泛型**
- 多类型参数：**使用多个类型参数**
- 数组结合：**处理数组元素类型
