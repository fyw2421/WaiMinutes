---
title: "四十一、字面量类型"
weight: 41
description: "本章介绍 字面量类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

字面量类型（Literal Types）允许将变量类型限制为具体的值，而不是宽泛的 string、number 等类型。

## (一) 字符串字面量类型

```typescript
var direction: "up" | "down" | "left" | "right";
direction = "up"; // 正确
// direction = "upup"; // 编译错误

type Status = "pending" | "active" | "completed";
```

## (二) 数字字面量类型

```typescript
var code: 200 | 404 | 500;
code = 200;
// code = 301; // 编译错误

type Weekday = 1 | 2 | 3 | 4 | 5 | 6 | 7;
```

## (三) 布尔字面量类型

```typescript
var isActive: true | false;
isActive = true;
```

## (四) 对象字面量类型

```typescript
type ReadonlyPoint = {
    readonly x: number;
    readonly y: number;
};
```

## (五) 字面量类型与类型推断

```typescript
var colors = ["red", "green", "blue"] as const;
// 类型变为：readonly ["red", "green", "blue"]
```

## (六) 模板字面量类型

```typescript
type EventName = \`on\${string}\`;
type Handler = \`handle\${Capitalize}\`;
```

## (七) 实际应用：Redux Action

```typescript
type Action =
    | { type: "increment"; payload: number }
    | { type: "decrement"; payload: number }
    | { type: "reset" };
```
