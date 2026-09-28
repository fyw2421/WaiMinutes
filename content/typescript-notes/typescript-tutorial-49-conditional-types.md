---
title: "四十九、条件类型"
weight: 49
description: "本章介绍 条件类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

条件类型（Conditional Types）允许根据条件动态地选择类型，类似于三元表达式。

## (一) 基本语法

```typescript
// T extends U ? X : Y
type IsString = T extends string ? true : false;
type A = IsStringstring>;  // true
type B = IsStringnumber>;  // false
```

## (二) 实际应用：类型过滤

```typescript
type NonNullable = T extends null | undefined ? never : T;
```

## (三) 类型推导：infer 关键字

```typescript
type ReturnType = T extends (...args: any[]) => infer R ? R : never;
function getUser() { return { name: "Alice" }; }
type R1 = ReturnTypetypeof getUser>;  // { name: string }
```

## (四) 分布条件类型

```typescript
type ToArray = T extends any ? T[] : never;
type StrOrNum = ToArraystring | number>;  // string[] | number[]
```

## (五) 条件类型与映射类型结合

```typescript
type Partial = { [P in keyof T]?: T[P]; };
type Required = { [P in keyof T]-?: T[P]; };
```

## (六) 高级示例：类型检查

```typescript
type IsAny = 0 extends (1 &amp; T) ? true : false;
type IsAssignableTo = T extends U ? true : false;
```

## (七) 注意事项

- 条件类型是延迟求值的
- 联合类型会自动触发分布机制
- infer 只能用在 extends 条件中
- 大多数内置工具类型基于条件类型实现
