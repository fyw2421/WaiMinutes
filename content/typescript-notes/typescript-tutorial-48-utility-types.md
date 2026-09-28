---
title: "四十八、工具类型"
weight: 48
description: "本章介绍 工具类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

工具类型是 TypeScript 内置的一系列高级类型，帮助快速创建和转换类型。

## (一) Partial- 可选属性

```typescript
interface User { id: number; name: string; email: string; }
type PartialUser = Partial;
var user: PartialUser = { name: "Alice" };
```

## (二) Required- 必填属性

```typescript
interface Config { host?: string; port?: number; }
type RequiredConfig = Required;
var config: RequiredConfig = { host: "localhost", port: 8080 };
```

## (三) Readonly- 只读属性

```typescript
type ReadonlyUser = Readonly;
var user: ReadonlyUser = { name: "Alice", age: 25 };
// user.name = "Bob"; // 错误
```

## (四) Pick- 选择属性

```typescript
type UserBasicInfo = Pick"id" | "name">;
```

## (五) Omit- 排除属性

```typescript
type UserWithoutPassword = Omit"password">;
```

## (六) Record- 构造对象类型

```typescript
type Role = "admin" | "user" | "guest";
type RolePermissions = Recordstring[]>;
```

## (七) Exclude- 排除类型

```typescript
type T = "a" | "b" | "c" | "d";
type NonABC = Exclude"a" | "b" | "c">;  // "d"
```

## (八) Extract- 提取类型

```typescript
type Letters = Extractstring>;  // "a" | "b" | "c"
```

## (九) NonNullable- 排除空值

```typescript
type NotNull = NonNullablestring | null | undefined>;  // string
```

## (十) ReturnType- 获取返回类型

```typescript
function getUser() { return { name: "Alice", age: 25 }; }
type UserType = ReturnTypetypeof getUser>;
```
