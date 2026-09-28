---
title: "二十八、Symbol"
weight: 28
description: "本章介绍 Symbol。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

Symbol 是 ES6 引入的原始数据类型，表示唯一的标识符。

在 TypeScript 中，Symbol 可以用作对象的属性键，确保属性的唯一性。

## (一) 创建 Symbol

### 1. 实例

```typescript
var sym1 = Symbol("description");
var sym2 = Symbol("description");
console.log("sym1 === sym2: " + (sym1 === sym2));
console.log("sym1: " + sym1.toString());
```

## (二) Symbol 作为对象属性

### 1. 实例

```typescript
var sym = Symbol("key");
var obj = {
```

name: "Alice",

[sym]: "secret value"

```typescript
};
console.log("普通属性: " + obj.name);
console.log("Symbol 属性: " + obj[sym]);
console.log("对象: " + JSON.stringify(obj));
```

## (三) 全局 Symbol 注册表

### 1. 实例

```typescript
var globalSym1 = Symbol.for("global");
var globalSym2 = Symbol.for("global");
console.log("全局 Symbol 相等: " + (globalSym1 === globalSym2));
console.log("Symbol key: " + Symbol.keyFor(globalSym1));
```

## (四) 内置 Symbol

### 1. 实例

```typescript
var arr = [1, 2, 3];
var iterator = arr[Symbol.iterator]();
console.log("第一个元素: " + iterator.next().value);
console.log("第二个元素: " + iterator.next().value);
var obj = { [Symbol.toStringTag]: "MyObject" };
console.log("对象类型: " + obj.toString());
```

## (五) Symbol 类型注解

### 1. 实例

```typescript
var sym: symbol = Symbol("key");
var obj: { [key: symbol]: string } = {};
obj[sym] = "value";
console.log("Symbol 属性值: " + obj[sym]);
```

## (六) 总结

- **唯一性：**每次创建的 Symbol 都不相等
- **属性键：**可用作对象的唯一属性键
- **全局注册：**Symbol.for() 创建/获取全局 Symbol
- **内置 Symbol：**Symbol.iterator、Symbol.toStringTag 等
- **类型注解：**使用 symbol 类型
