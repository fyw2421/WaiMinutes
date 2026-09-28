---
title: "五十七、infer 关键字"
weight: 57
description: "本章介绍 infer 关键字。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

infer 是 TypeScript 条件类型中的关键字，用于从类型中推断出新的类型。

它允许在泛型条件类型中提取和推导类型，实现强大的类型操作。

## (一) 为什么需要 infer 关键字

在泛型编程中，我们经常需要从复杂的类型中提取出部分类型。

例如，从 Promise中提取 string，从 Array中提取 User。

infer 关键字让这种类型提取变得优雅和类型安全。

> **概念说明：**infer 是"infer"的缩写，意为"推断"。它只能在条件类型的 extends 子句中使用，用于声明一个待推断的类型变量。

## (二) 基本用法

```typescript
type ValueOf = T extends Promise ? V : never;
type Str = ValueOfPromisestring>>;  // string
type Num = ValueOfPromisenumber>>; // number
type NotPrm = ValueOfstring>;       // never
var promise: Promisestring> = Promise.resolve("hello");
var value: ValueOftypeof promise> = "world";
```

## (三) 提取数组元素类型

```typescript
type ArrayElement = T extends (infer V)[] ? V : never;
type User = { name: string };
type Users = User[];
type E1 = ArrayElementstring[]>;   // string
type E2 = ArrayElement;      // User
type E3 = ArrayElementnumber>;     // never
```

## (四) 提取函数返回类型

```typescript
type ReturnType = T extends (...args: any[]) => infer R ? R : any;
function getData() { return { id: 1, name: "Alice" }; }
function fetchUser(id: number): Promise { return Promise.resolve({ id, name: "Bob" }); }
type R1 = ReturnTypetypeof getData>;   // { id: number; name: string }
type R2 = ReturnTypetypeof fetchUser>; // Promise
```

## (五) 提取函数参数类型

```typescript
type FirstParameter = T extends (first: infer P, ...rest: any[]) => any ? P : never;
function createUser(name: string, age: number): User { return { id: 1, name }; }
function logMessage(msg: string): void { console.log(msg); }
type P1 = FirstParametertypeof createUser>; // string
type P2 = FirstParametertypeof logMessage>;  // string
type P3 = FirstParameter void>;         // never
```

## (六) 多个 infer

```typescript
type FirstTwo = T extends [infer A, infer B, ...rest: any[]] ? [A, B] : never;
type Tuple = [string, number, boolean];
type FirstTwoTypes = FirstTwo; // [string, number]
type ObjectValue = T extends { value: infer V } ? V : never;
type WithValue = { value: string; name: string };
type ExtractedValue = ObjectValue; // string
```

## (七) 在递归类型中使用 infer

```typescript
type DeepReadonly = T extends Function
    ? T
    : T extends object
        ? { readonly [P in keyof T]: DeepReadonly }
        : T;
type FlattenPromise = T extends Promise
    ? U extends Promiseany> ? FlattenPromise : U
    : T;
type Nested = PromisePromisestring>>;
type Flat = FlattenPromise; // string
```

## (八) 注意事项

- 只能用在 extends 右侧
- 使用 infer V 声明待推断的类型变量
- 一个条件类型中可以使用多个 infer
- 推断失败返回 never

## (九) 总结

- **类型提取：**从复杂类型中提取部分类型
- **条件推断：**在条件类型中推断类型
- **函数相关：**提取函数参数、返回类型
- **递归工具：**实现深度类型转换
