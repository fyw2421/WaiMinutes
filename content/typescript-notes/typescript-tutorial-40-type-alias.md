---
title: "四十、type 别名"
weight: 40
description: "本章介绍 type 别名。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

type 别名（Type Alias）用于为现有类型创建别名，让代码更简洁、更易读。

通过类型别名，可以为复杂类型定义一个简短的名字，提高代码的可维护性。

## (一) 基本用法

使用 type 关键字为类型定义别名。

```typescript
type ID = string | number;
type Point = { x: number; y: number };

var userId: ID = "123";
var productId: ID = 456;
var point: Point = { x: 10, y: 20 };
```

## (二) 接口 vs 类型别名

类型别名和接口非常相似，但有一些区别：

- type 可以定义任何类型（联合类型、元组、函数类型等）
- 接口主要用于定义对象类型，支持声明合并

## (三) 类型别名与联合类型

```typescript
type Status = "pending" | "success" | "error";
type Result = string | number | boolean;
```

## (四) 类型别名与元组

```typescript
type Coordinate = [number, number];
type NameAge = [string, number];
```

## (五) 类型别名与函数

```typescript
type Callback = (result: string) => void;
type MathOperation = (a: number, b: number) => number;
```

## (六) 类型别名与泛型

```typescript
type Result = { success: boolean; data?: T; error?: string };
type Pair = { key: K; value: V };
```

## (七) 类型别名与映射类型

```typescript
type Readonly = { readonly [P in keyof T]: T[P] };
type Partial = { [P in keyof T]?: T[P] };
```
