---
title: "二十六、类型断言"
weight: 26
description: "本章介绍 类型断言。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

类型断言（Type Assertion）是一种告诉编译器"我比你更清楚这个值的类型"的机制。它允许开发者手动覆盖 TypeScript 的类型推断结果，类似于其他语言的类型转换，但仅在编译阶段生效，不产生任何运行时代码。

## (一) 两种语法形式

### 1. 实例

// ① 尖括号语法（不推荐，在 .tsx 文件中无法使用）

```typescript
const str1: any = "hello";
const len1: number = (str1).length;
```

// ② as 语法（推荐）

```typescript
const str2: any = "world";
const len2: number = (str2 as string).length;
console.log(len1); // 5
console.log(len2); // 5
```

## (二) 常见使用场景

### 1. 处理 any / unknown 类型

## (三) 实例

```typescript
const response: any = { name: "Alice", age: 25 };
const user = response as { name: string; age: number };
console.log(user.name); // Alice
console.log(user.age); // 25
```

### 1. 收窄联合类型

## (四) 实例

```typescript
type Shape = | { kind: "circle"; radius: number } | { kind: "rect"; width: number; height: number };
function getArea(shape: Shape): number {
if (shape.kind === "circle") {
return Math.PI * (shape as { kind: "circle"; radius: number }).radius ** 2;
}
const rect = shape as { kind: "rect"; width: number; height: number };
return rect.width * rect.height;
}
console.log(getArea({ kind: "circle", radius: 5 }).toFixed(2));
console.log(getArea({ kind: "rect", width: 4, height: 6 }));
```

### 1. 操作 DOM 元素

## (五) 实例

```typescript
const input = document.querySelector("#username") as HTMLInputElement;
const btn = document.querySelector("#submit");
if (btn) {
(btn as HTMLButtonElement).disabled = true;
}
```

## (六) 非空断言 !

### 1. 实例

```typescript
function printLength(str?: string) {
console.log(str!.length);
}
printLength("hello");
```

## (七) 常量断言 as const

### 1. 实例

```typescript
const colors2 = ["red", "green", "blue"] as const;
const config = { host: "localhost", port: 3000 } as const;
console.log(colors2);
console.log(config.host);
console.log(config.port);
```

## (八) 断言不是类型转换

### 1. 实例

```typescript
const strNum: any = "42";
const wrongNum = strNum as number;
console.log(typeof wrongNum); // string
console.log(wrongNum + 1); // 421
const realNum = Number(strNum);
console.log(typeof realNum); // number
console.log(realNum + 1); // 43
```

## (九) 双重断言

### 1. 实例

```typescript
const num = 42;
const str = num as unknown as string;
console.log(str);
console.log(typeof str);
```

## (十) 总结

- **语法选择：**统一使用 `as` 语法
- **断言 vs 转换：**断言仅影响编译期类型
- **非空断言：**使用 `!` 前确保值真的非空
- **常量断言：**`as const` 是安全且实用的断言
- **双重断言：**是最后手段，使用时必须注释说明原因
