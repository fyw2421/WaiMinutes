---
title: "二十七、null 和 undefined"
weight: 27
description: "本章介绍 null 和 undefined。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

在 TypeScript 中，null 和 undefined 有特殊的处理方式。

启用 strictNullChecks 后，需要显式处理这些值，这有助于编写更安全的代码。

## (一) null 和 undefined 基础

### 1. 实例

```typescript
var empty: null = null;
var notDefined: undefined = undefined;
console.log("null: " + empty);
console.log("undefined: " + notDefined);
```

## (二) 联合类型处理 null

### 1. 实例

```typescript
var name: string | null = "Alice";
name = null;
function getLength(str: string | null): number {
if (str === null) {
return 0;
}
return str.length;
}
console.log("长度: " + getLength("hello"));
console.log("长度: " + getLength(null));
```

## (三) 可选参数和属性

### 1. 实例

```typescript
function greet(name?: string): string {
if (name === undefined) {
return "Hello, stranger!";
}
return "Hello, " + name;
}
console.log(greet("Alice"));
console.log(greet());
interface User {
name: string;
age?: number;
}
var user: User = { name: "Bob" };
console.log("用户: " + JSON.stringify(user));
```

## (四) 非空断言运算符

### 1. 实例

```typescript
function getLength(str: string | null): number {
return str!.length;
}
console.log("长度: " + getLength("hello"));
```

## (五) 空值合并运算符

### 1. 实例

```typescript
var name: string | null = null;
var displayName = name ?? "Guest";
console.log("显示名称: " + displayName);
var num: number | null = 0;
var result1 = num ?? 100;
var result2 = num || 100;
console.log("?? 结果: " + result1);
console.log("|| 结果: " + result2);
```

## (六) 可选链

### 1. 实例

```typescript
interface Person {
name: string;
address?: { city: string; };
}
var person: Person = { name: "Alice" };
var city = person.address?.city;
console.log("城市: " + city);
console.log("城市: " + (person.address?.city ?? "未知"));
```

## (七) 总结

- **严格模式：**启用 strictNullChecks 后需显式处理
- **联合类型：**使用 `string | null` 声明
- **可选参数/属性：**自动包含 undefined
- **非空断言：**使用 `!`（谨慎使用）
- **空值合并：**使用 `??` 提供默认值
- **可选链：**使用 `?.` 安全访问
