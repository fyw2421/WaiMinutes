---
title: "五十、映射类型"
weight: 50
description: "本章介绍 映射类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

映射类型（Mapped Types）基于已有类型创建新类型，通过批量修改属性特性实现类型转换。

## (一) 基础映射类型

```typescript
interface User { id: number; name: string; email: string; }
type Partial = { [P in keyof T]?: T[P]; };
type PartialUser = Partial;
```

## (二) 属性修饰符

```typescript
type Readonly = { readonly [P in keyof T]: T[P]; };
type Optional = { [P in keyof T]?: T[P]; };
type Required = { [P in keyof T]-?: T[P]; };
```

## (三) 键名映射

```typescript
type WithPrefixextends string> = {
    [P in keyof T as \`\${Prefix}\${Capitalize}\`]: T[P];
};
type PrefixedUser = WithPrefix"user">;
// { userId: number; userName: string; userEmail: string }
```

## (四) 键过滤

```typescript
type Omitextends keyof T> = {
    [P in keyof T as P extends K ? never : P]: T[P];
};
```

## (五) 条件映射

```typescript
type FunctionToVoid = {
    [P in keyof T]: T[P] extends (...args: any[]) => any ? () => void : T[P];
};
```

## (六) 内置映射类型

- `Partial
  ` - 所有属性可选
- `Required
  ` - 所有属性必填
- `Readonly
  ` - 所有属性只读
- `Pick
  ` - 选择指定属性
- `Omit
  ` - 排除指定属性
