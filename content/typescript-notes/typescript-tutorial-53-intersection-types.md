---
title: "五十三、交叉类型"
weight: 53
description: "本章介绍 交叉类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

交叉类型（Intersection Types）将多个类型合并成一个新类型，新类型包含所有成员。

## (一) 基本语法

```typescript
interface Person { name: string; age: number; }
interface Worker { company: string; salary: number; }
type Employee = Person &amp; Worker;

var employee: Employee = {
    name: "Alice",
    age: 25,
    company: "Google",
    salary: 100000
};
```

## (二) 交叉类型与接口继承

```typescript
type ABType = A &amp; B &amp; { c: boolean };
// 比接口继承更简洁
```

## (三) 类型混合（Mixin 模式）

```typescript
type Constructor = new (...args: any[]) => {};
function Timestampedextends Constructor>(Base: T) {
    return class extends Base { timestamp = Date.now(); };
}
function Serializableextends Constructor>(Base: T) {
    return class extends Base {
        serialize() { return JSON.stringify(this); }
    };
}
```

## (四) 交叉类型与联合类型

```typescript
type Combined = (A | B) &amp; C;
// 结果：{ a: string; c: boolean } | { b: number; c: boolean }
```

## (五) 实用交叉类型

```typescript
type Partial = { [P in keyof T]?: T[P] };
type Required = { [P in keyof T]-?: T[P] };
type Readonly = { readonly [P in keyof T]: T[P] };
```

## (六) 注意事项

- 不兼容类型交叉会得到 never
- 联合类型优先级高于交叉类型
- 同名方法冲突需要手动处理
