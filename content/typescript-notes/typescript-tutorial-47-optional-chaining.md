---
title: "四十七、可选链"
weight: 47
description: "本章介绍 可选链。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

可选链（Optional Chaining）是一种安全的属性访问方式。当访问路径中的属性为 null 或 undefined 时，短路返回 undefined。

## (一) 基本语法

```typescript
var user = { name: "RUNOOB", address: { city: "Beijing" } };
var city = user?.address?.city;
```

## (二) 处理不存在的属性

```typescript
var user = { name: "RUNOOB" };
var city = user?.address?.city;  // undefined
var country = user?.address?.country?.name;  // undefined
```

## (三) 可选链与数组结合

```typescript
var users = [{ name: "Alice" }, { name: "Bob" }];
var firstUser = users?.[0]?.name;   // "Alice"
var tenthUser = users?.[9]?.name;   // undefined
```

## (四) 可选链与方法调用

```typescript
var user = {
    name: "Alice",
    greet: function() { return "Hello, " + this.name; }
};
var message1 = user.greet?.();    // 正常调用
var message2 = user.sayHello?.(); // undefined
```

## (五) 可选链赋值

```typescript
var user = { name: "Alice" };
user?.address?.city = "Beijing";  // 跳过赋值，不会创建中间对象
```

## (六) 空值合并与可选链

```typescript
var city = user?.address?.city ?? "未知城市";
```

## (七) 三种形式

- `obj?.prop` — 属性访问
- `arr?.[index]` — 数组访问
- `obj?.method()` — 方法调用
