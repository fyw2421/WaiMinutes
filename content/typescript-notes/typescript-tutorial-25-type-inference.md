---
title: "二十五、类型推断"
weight: 25
description: "本章介绍 类型推断。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

类型推断（Type Inference）是 TypeScript 最强大和便捷的特性之一。

它允许编译器自动分析代码的上下文，并根据变量的值、函数的返回值等自动推断出变量的类型。

这意味着开发者无需为每个变量显式声明类型，可以编写更简洁、更易于维护的代码。

## (一) 为什么需要类型推断

在 JavaScript 开发中，我们需要手动为每个变量声明类型。

这不仅增加了代码的冗余度，也降低了开发效率。

TypeScript 的类型推断功能可以在大多数情况下自动推断出变量的类型。

开发者只需要在类型复杂或不明确的情况下显式声明类型。

## (二) 基础类型推断

当声明变量并初始化时，TypeScript 会根据初始值自动推断变量的类型。

## (三) 实例

```typescript
var num = 10;
var str = "hello";
var isActive = true;
console.log("num 类型: " + typeof num);
console.log("str 类型: " + typeof str);
console.log("isActive 类型: " + typeof isActive);
```

**运行结果：**

num 类型: number

str 类型: string

isActive 类型: boolean

## (四) 函数返回类型推断

TypeScript 会根据函数的 return 语句自动推断返回类型。

## (五) 实例

```typescript
function add(a: number, b: number) {
return a + b;
}
function greet(name: string) {
return "Hello, " + name;
}
var result = add(1, 2);
var message = greet("TypeScript");
console.log("加法结果: " + result);
console.log("问候语: " + message);
```

**运行结果：**

加法结果: 3

问候语: Hello, TypeScript

## (六) 上下文类型推断

类型推断不仅基于变量本身，还会考虑变量使用的上下文环境。

## (七) 实例

```typescript
var numbers = [1, 2, 3, 4, 5];
var doubled = numbers.map(function(n) {
return n * 2;
});
console.log("翻倍数组: " + doubled);
```

## (八) 最佳通用类型推断

### 1. 实例

```typescript
var mixed = [1, "two", 3, "four"];
console.log("混合数组: " + mixed);
```

## (九) 类型推断的限制

### 1. 实例

```typescript
var unknown;
unknown = "hello";
unknown = 123;
console.log("未指定类型: " + unknown);
var fixedNumber: number = 42;
console.log("指定类型: " + fixedNumber);
```

## (十) 泛型函数推断

### 1. 实例

```typescript
function identity(arg: T): T {
return arg;
}
var str = identity("hello");
var num = identity(42);
var obj = identity({ name: "TypeScript" });
console.log("字符串: " + str);
console.log("数字: " + num);
console.log("对象: " + JSON.stringify(obj));
```

## (十一) 总结

- **基础类型推断：**根据初始值推断变量类型
- **返回类型推断：**根据 return 语句推断函数返回类型
- **上下文推断：**根据使用位置推断类型
- **最佳通用类型：**从多个候选类型中选择最合适的类型
- **泛型推断：**根据传入参数自动推断泛型类型
- **显式声明：**必要时可显式指定类型
