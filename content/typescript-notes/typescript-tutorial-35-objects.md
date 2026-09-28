---
title: "三十五、对象"
weight: 35
description: "本章介绍 对象。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 中的对象类型定义和操作与 JavaScript 类似，但提供了更强大的类型安全。

## (一) 对象字面量

### 1. 实例

var person = {

name: "Alice",

age: 30

```typescript
};
console.log("姓名: " + person.name);
console.log("年龄: " + person.age);
```

## (二) 对象类型定义

### 1. 实例

```typescript
var book: {
title: string;
author: string;
year: number;
} = {
```

title: "TypeScript 入门",

author: "张三",

year: 2024

```typescript
};
console.log("书籍: " + JSON.stringify(book));
```

## (三) 对象作为函数返回值

### 1. 实例

```typescript
function createPerson(name: string, age: number): object {
return {
```

name: name,

age: age

```typescript
};
}
var person = createPerson("Bob", 25);
console.log("创建的人: " + JSON.stringify(person));
```

## (四) 对象解构

### 1. 实例

var user = {

id: 1,

username: "admin",

email: "admin@example.com",

role: "管理员"

```typescript
};
var username = user.username, email = user.email;
console.log("用户名: " + username);
console.log("邮箱: " + email);
```

## (五) 扩展运算符

### 1. 实例

var baseConfig = {

host: "localhost",

port: 3000

```typescript
};
var config = Object.assign({}, baseConfig, { protocol: "https" });
console.log("配置: " + JSON.stringify(config));
```

## (六) 只读属性

### 1. 实例

var config: { readonly url: string; port: number } = {

url: "https://api.example.com",

port: 443

```typescript
};
console.log(config.url);
```

// config.url = "https://new-url.com"; // 编译错误

## (七) 可选属性

### 1. 实例

```typescript
var options: {
url: string;
method?: string;
timeout?: number;
} = { url: "https://api.example.com" };
console.log(options.url);
console.log("方法: " + (options.method || "GET"));
```

## (八) 总结

- 对象字面量：**可以直接创建**
- 类型注解：**使用类型别名或内联类型**
- 解构：**快速提取属性**
- 扩展运算符：**合并对象**
- 只读属性：**使用 readonly**
- 可选属性：**使用 `?`**
