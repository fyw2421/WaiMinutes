---
title: "三十九、声明文件"
weight: 39
description: "本章介绍 声明文件。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

声明文件（Declaration Files）是 TypeScript 中用于描述 JavaScript 代码类型信息的文件。

## (一) 声明变量

### 1. 实例

```typescript
declare var jQuery: (selector: string) => any;
jQuery("#app").html("Hello jQuery");
```

## (二) 声明函数

### 1. 实例

```typescript
declare function myCustomFunction(x: number, y: number): number;
var result = myCustomFunction(5, 3);
console.log(result);
```

## (三) 声明类

### 1. 实例

```typescript
declare class MyCustomClass {
constructor(name: string);
getName(): string;
setName(name: string): void;
}
var myObj = new MyCustomClass("TypeScript");
myObj.setName("JavaScript");
```

## (四) 声明模块

### 1. 实例

```typescript
declare module "my-custom-lib" {
export function doSomething(): void;
export function doSomethingElse(x: number): number;
}
import { doSomething } from "my-custom-lib";
doSomething();
```

## (五) 声明全局类型

### 1. 实例

```typescript
declare global {
interface String {
addPadding(length: number): string;
}
}
String.prototype.addPadding = function(length) {
return this + " ".repeat(length);
};
var str = "hello";
console.log("填充后: |" + str.addPadding(5) + "|");
```

## (六) 声明文件语法

### 1. 常见声明文件示例

```typescript
declare var 声明全局变量
declare function 声明全局方法
declare class 声明全局类
declare enum 声明全局枚举类型
declare namespace 声明全局对象
interface 和 type 声明全局类型
export 导出变量
export default 默认导出
export = 导出对象
export as namespace 声明 UMD 全局
```

## (七) 实际案例

### 1. 实例

// globals.d.ts

```typescript
declare var process: {
env: {
NODE_ENV: string;
};
};
```

// types.d.ts

```typescript
interface Window {
MyApp: {
version: string;
config: object;
};
}
```

// modules.d.ts

```typescript
declare module "*.png" {
const src: string;
export default src;
}
```

## (八) 总结

- **声明变量：**declare var / let / const
- **声明函数：**declare function
- **声明类：**declare class
- **声明模块：**declare module
- **声明全局：**declare global
- **文件命名：**.d.ts 后缀
- **用途：**为 JS 代码提供类型信息
