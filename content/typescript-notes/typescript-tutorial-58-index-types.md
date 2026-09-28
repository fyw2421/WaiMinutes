---
title: "五十八、索引类型"
weight: 58
description: "本章介绍 索引类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

索引类型和 keyof 是 TypeScript 中操作对象类型的强大工具。

它们允许我们动态地访问对象的属性，并创建灵活的类型映射。

## (一) 为什么需要索引类型

在 JavaScript 中，我们经常需要动态访问对象的属性。

索引类型和 keyof 让我们能够在类型系统中表达这种动态性，同时保持类型安全。

> **概念：**索引类型是一类允许动态访问对象属性的类型，keyof 用于获取对象类型的所有键组成的联合类型。

## (二) keyof 操作符

```typescript
interface User { id: number; name: string; email: string; age?: number; }
type UserKeys = keyof User;  // "id" | "name" | "email" | "age"
function getPropertyextends keyof T>(obj: T, key: K): T[K] {
    return obj[key];
}
const user: User = { id: 1, name: "Alice", email: "alice@example.com" };
const userName: string = getProperty(user, "name");
```

## (三) 索引访问类型

```typescript
interface User { id: number; name: string; email: string; }
type UserId = User["id"];           // number
type UserName = User["name"];       // string
type UserIdAndName = User["id" | "name"];  // number | string
type AllUserValues = User[keyof User];     // number | string
function getValueextends keyof T>(obj: T, key: K): T[K] {
    return obj[key];
}
```

## (四) 映射类型基础

```typescript
interface User { id: number; name: string; email: string; age: number; }
type PartialUser = Partial;
type ReadonlyUser = Readonly;
type Stringify = { [P in keyof T]: string };
type StringifiedUser = Stringify;
const partialUser: PartialUser = { id: 1, name: "Alice" };
const readonlyUser: ReadonlyUser = { id: 1, name: "Bob", email: "bob@test.com", age: 25 };
```

## (五) 约束键的类型

```typescript
interface Config { apiUrl: string; timeout: number; retry: boolean; }
function getConfigValueextends keyof T>(config: T, key: K): T[K] {
    return config[key];
}
const config: Config = { apiUrl: "https://api.example.com", timeout: 5000, retry: true };
const url: string = getConfigValue(config, "apiUrl");
const timeoutVal: number = getConfigValue(config, "timeout");
```

## (六) 只获取特定类型的属性

```typescript
interface Mixed { id: number; name: string; age: number; email: string; active: boolean; }
type StringKeys = { [K in keyof T]: T[K] extends string ? K : never }[keyof T];
type NumberKeys = { [K in keyof T]: T[K] extends number ? K : never }[keyof T];
type StringProps = StringKeys;  // "name" | "email"
type NumberProps = NumberKeys;  // "id" | "age"
```

## (七) 遍历数组类型

```typescript
type Tuple = [string, number, boolean];
type First = Tuple[0];    // string
type Second = Tuple[1];   // number
type Third = Tuple[2];    // boolean
type AllElements = Tuple[number];  // string | number | boolean
type StringArray = string[];
type ArrayElement = StringArray[number];  // string
```

## (八) 注意事项

- keyof 返回键名的字面量联合类型
- 确保访问的键存在于目标类型中
- 使用 extends keyof 约束泛型参数
- 映射类型使用 [P in keyof T] 语法

## (九) 总结

- **keyof：**获取对象类型的所有键
- **索引访问：**通过键获取属性类型
- **映射类型：**基于现有类型创建新类型
- **类型安全：**确保动态属性访问的类型安全
