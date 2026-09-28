---
title: "三十八、模块"
weight: 38
description: "本章介绍 模块。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 中的模块是代码组织和复用的基本单元，完全支持 ES6 模块语法。

## (一) 导出

### 1. 实例

// utils.ts

```typescript
export function greet(name: string): string {
return "Hello, " + name;
}
export const PI = 3.14159;
export interface Person {
name: string;
age: number;
}
```

## (二) 默认导出

### 1. 实例

// logger.ts

```typescript
export default class Logger {
log(message: string): void {
console.log("日志: " + message);
}
}
```

// app.ts

```typescript
import Logger from "./logger";
var logger = new Logger();
logger.log("这是一条日志");
```

## (三) 导入

### 1. 实例

```typescript
import { greet, PI } from "./utils";
console.log(greet("TypeScript"));
console.log("PI: " + PI);
```

## (四) 重命名导入导出

### 1. 实例

// 导入时重命名

```typescript
import { greet as hello } from "./utils";
console.log(hello("TypeScript"));
```

// 导出时重命名

export { greet as sayHello };

## (五) 导出所有

### 1. 实例

// index.ts

```typescript
export * from "./utils";
export * from "./logger";
```

## (六) 动态导入

### 1. 实例

```typescript
async function loadModule() {
var module = await import("./utils");
console.log(module.greet("Dynamic Import"));
}
loadModule();
```

## (七) 总结

- **export：**导出变量、函数、类、接口等
- **export default：**默认导出，每个模块只能有一个
- **import：**按需导入
- **as：**重命名导入或导出
- **export *：**重新导出所有内容**
- 动态导入：**使用 import() 函数按需加载
