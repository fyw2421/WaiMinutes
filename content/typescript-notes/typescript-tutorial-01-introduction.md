---
title: "一、TypeScript 简介"
weight: 1
description: "本章介绍 TypeScript 简介。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 是由微软开发并开源的编程语言，它是 JavaScript 的超集，在完全兼容 JavaScript 语法的基础上，增加了可选的静态类型系统和基于类的面向对象编程能力。

TypeScript 代码最终会被编译为纯 JavaScript，可以运行在任何支持 JavaScript 的环境中，包括浏览器、Node.js 和移动端。

## (一) 为什么需要 TypeScript

JavaScript 是一门动态类型语言，变量的类型在运行时才能确定。

这种灵活性在小型项目中表现出色，但随着项目规模扩大，问题逐渐暴露：

| 问题场景 | JavaScript 的困境 | TypeScript 的解决方式 |
|---|---|---|
| 函数参数传错类型 | 运行时才报错，难以提前发现 | 编译阶段即报错，IDE 实时提示 |
| 访问不存在的属性 | 返回 undefined，行为难以预测 | 编译器直接报错，拒绝通过 |
| 大型项目重构 | 改一处，不知道哪里会崩 | 类型系统自动追踪所有引用 |
| 团队协作 | 函数接受什么参数、返回什么全靠注释或文档 | 类型签名即文档，IDE 自动补全 |
| 代码可读性 | 看函数定义无法直接知道数据结构 | 接口和类型别名让数据结构一目了然 |

> TypeScript 的本质目标不是取代 JavaScript，而是让大规模 JavaScript 工程变得可管理、可维护。它是 JavaScript 的"安全网"，而不是竞争对手。

看一个最直观的例子：

```typescript
// JavaScript：运行时才会报错
function greet(name) {
    return "Hello, " + name.toUpperCase();
}
greet(123); // 运行时报错：name.toUpperCase is not a function
// TypeScript：编译时就能发现错误
function greet(name: string): string {
    return "Hello, " + name.toUpperCase();
}
greet(123); // 编译错误：Argument of type 'number' is not assignable to parameter of type 'string'
greet("runoob"); // 正确：Hello, RUNOOB
```

## (二) TypeScript 与 JavaScript 的关系

TypeScript 和 JavaScript 之间的关系，可以用一句话概括：TypeScript 是 JavaScript 的超集，即所有合法的 JavaScript 代码，同时也是合法的 TypeScript 代码。

**集合关系示意**

JavaScript（所有 JS 代码）

⊂ TypeScript（JS + 类型系统 + 新特性）

⊂ 编译产物（标准 JavaScript，可在任意环境运行）

两者的核心差异对比：

| 对比项 | JavaScript | TypeScript |
|---|---|---|
| 类型系统 | 动态类型，运行时确定 | 静态类型，编译时检查（可选） |
| 运行方式 | 直接在浏览器 / Node.js 中运行 | 需先编译为 JS 再运行 |
| 错误发现时机 | 运行时 | 编译时（提前发现） |
| IDE 支持 | 基础补全 | 强类型推断、精准补全、重构支持 |
| 学习曲线 | 较平缓 | 需额外学习类型系统 |
| 现有 JS 兼容性 | — | 完全兼容，可渐进式迁移 |
| 文件扩展名 | .js | .ts 或 .tsx（含 JSX） |

> TypeScript 支持"渐进式采用"：你不必一次性重写整个项目。可以先把 .js 改为 .ts，逐步为关键模块添加类型，现有代码照常运行。

## (三) 核心特性

TypeScript 在 JavaScript 的基础上引入了一套完整的类型系统，以下是最常用的核心特性。

### 1. 基础类型注解

```typescript
// 基础类型：number、string、boolean、null、undefined、symbol、bigint
let age: number = 25;
let username: string = "runoob";
let isActive: boolean = true;
// 数组类型，两种等价写法
let scores: number[] = [90, 85, 92];
let tags: Arraystring> = ["typescript", "javascript"];
// 元组：固定长度和类型的数组
let point: [number, number] = [10, 20];
let entry: [string, number] = ["RUNOOB", 100];
// 函数：参数类型 + 返回值类型
function add(a: number, b: number): number {
    return a + b;
}
// void：函数无返回值
function log(msg: string): void {
    console.log(msg);
}
// 可选参数：参数名后加 ?
function greet(name: string, title?: string): string {
    return title ? ${title}  : name;
}
console.log(greet("RUNOOB"));          // 输出：RUNOOB
console.log(greet("runoob", "Mr."));  // 输出：Mr. runoob
```

### 2. 接口（Interface）

```typescript
interface User {
    id: number;
    name: string;
    email?: string;
    readonly role: string;
}
function printUser(user: User): void {
    console.log(ID: , Name: );
    if (user.email) {
        console.log(Email: );
    }
}
const admin: User = {
    id: 1,
    name: "RUNOOB",
    email: "runoob@example.com",
    role: "admin",
};
printUser(admin);
// 接口继承
interface AdminUser extends User {
    permissions: string[];
}
```

### 3. 类型别名（Type Alias）

```typescript
// 联合类型
type ID = string | number;
let userId: ID = "abc-123";
userId = 456;
// 字面量类型
type Direction = "up" | "down" | "left" | "right";
type Status = "pending" | "active" | "inactive";
function move(dir: Direction): void {
    console.log(Moving );
}
move("up");
// 交叉类型
type WithTimestamp = {
    createdAt: Date;
    updatedAt: Date;
};
type UserRecord = User & WithTimestamp;
// 函数类型别名
type Transformer = (input: T) => U;
const toNumber: Transformerstring, number> = (s) => parseInt(s, 10);
```

### 4. 泛型（Generics）

```typescript
function first(arr: T[]): T {
    return arr[0];
}
const n = firstnumber>([1, 2, 3]);
const s = firststring>(["a", "b"]);
const inferred = first([true, false]);
interface ApiResponse {
    data: T;
    status: number;
    message: string;
}
const userResponse: ApiResponse = {
    data: { id: 1, name: "RUNOOB", role: "admin" },
    status: 200,
    message: "success",
};
function getByIdextends { id: number }>(items: T[], id: number): T | undefined {
    return items.find(item => item.id === id);
}
```

### 5. 枚举（Enum）

```typescript
// 数字枚举
enum Direction {
    Up,     // 0
    Down,   // 1
    Left,   // 2
    Right,  // 3
}
console.log(Direction.Up);    // 输出：0
console.log(Direction[0]);    // 输出："Up"
// 字符串枚举
enum Color {
    Red = "RED",
    Green = "GREEN",
    Blue = "BLUE",
}
function paint(color: Color): void {
    console.log(Painting in );
}
paint(Color.Red);
// const 枚举
const enum HttpStatus {
    OK = 200,
    NotFound = 404,
    InternalError = 500,
}
const status: HttpStatus = HttpStatus.OK;
```

### 6. 类型推断

```typescript
let count = 0;            // 推断为 number
let name = "RUNOOB";      // 推断为 string
let flag = true;          // 推断为 boolean
let numbers = [1, 2, 3];  // 推断为 number[]
function double(n: number) {
    return n * 2;
}
const config = {
    host: "localhost",
    port: 3000,
    debug: false,
};
```

### 7. 类与访问修饰符

```typescript
class Animal {
    readonly name: string;
    private age: number;
    protected species: string;
    constructor(name: string, age: number, species: string) {
        this.name = name;
        this.age = age;
        this.species = species;
    }
    public introduce(): string {
        return I'm , a .;
    }
    get info(): string {
        return ${this.name} ( years old);
    }
}
class Dog extends Animal {
    private breed: string;
    constructor(name: string, age: number, breed: string) {
        super(name, age, "Canis lupus familiaris");
        this.breed = breed;
    }
    describe(): string {
        return ${this.name} is a , breed: ;
    }
}
const dog = new Dog("RUNOOB", 3, "Labrador");
console.log(dog.introduce());
console.log(dog.info);
```

### 8. 装饰器（Decorator）

```typescript
function sealed(constructor: Function) {
    Object.seal(constructor);
    Object.seal(constructor.prototype);
}
function log(target: any, propertyKey: string, descriptor: PropertyDescriptor) {
    const original = descriptor.value;
    descriptor.value = function (...args: any[]) {
        console.log(Calling  with args:, args);
        const result = original.apply(this, args);
        console.log(${propertyKey} returned:, result);
        return result;
    };
}
@sealed
class Calculator {
    @log
    add(a: number, b: number): number {
        return a + b;
    }
}
const calc = new Calculator();
calc.add(1, 2);
```

## (四) 应用领域

### 1. 前端 Web 开发

| 框架 | TypeScript 支持情况 | 典型场景 |
|---|---|---|
| Angular | 官方语言，默认使用 TypeScript | 企业级 SPA、后台管理系统 |
| React | 官方提供 @types/react | 电商、内容平台、中后台应用 |
| Vue | Vue 3 核心用 TypeScript 重写 | 中小型项目、渐进式迁移 |
| Next.js / Nuxt | 内置 TypeScript 支持 | 全栈 SSR/SSG 应用 |

### 2. 后端 Node.js 开发

TypeScript 在 Node.js 后端开发中同样普及。NestJS 是目前最流行的 TypeScript 后端框架，采用与 Angular 相近的模块化架构，内置依赖注入和装饰器支持。

### 3. 命令行工具与脚本

借助 s-node、 sx 等工具，TypeScript 代码可以在不预编译的情况下直接执行。

### 4. 移动端与跨平台

React Native 完整支持 TypeScript，大量企业级移动应用均采用此技术栈。

### 5. 游戏开发与图形

Babylon.js 完全用 TypeScript 编写。Phaser 等 2D 游戏引擎同样提供完整的类型定义。

## (五) 发展历史

### 1. 起源（2010 - 2012）

设计者 **Anders Hejlsberg** 是业界传奇——C# 的首席架构师，也是 Turbo Pascal 和 Delphi 的创造者。

2012 年 10 月，TypeScript 0.8 公开发布。

### 2. 早期版本（2013 - 2015）

| 时间 | 版本 | 重要事件 |
|---|---|---|
| 2013-06 | 0.9 | 正式稳定版发布 |
| 2014-04 | 1.0 | 第一个正式稳定版 |
| 2014-07 | — | TypeScript 编译器代码开源 |
| 2015-04 | — | Visual Studio Code 发布，内置深度 TS 支持 |
| 2015-07 | 1.5 | ES6 模块语法、装饰器、命名空间 |

### 3. 高速成长（2016 - 2019）

| 时间 | 版本/事件 | 里程碑意义 |
|---|---|---|
| 2016-09 | TypeScript 2.0 | 非空类型、标记联合类型 |
| 2016 | Angular 2 发布 | 采用 TypeScript 作为官方语言 |
| 2017 | TypeScript 2.x | 条件类型、映射类型、infer |
| 2018-07 | TypeScript 3.0 | 项目引用、unknown 类型 |
| 2019 | DefinitelyTyped | 超过 7000 个包 |

### 4. 成熟与普及（2020 - 2022）

| 时间 | 版本/事件 | 重要内容 |
|---|---|---|
| 2020-08 | TypeScript 4.0 | 可变元组类型 |
| 2021 | TypeScript 4.x | 模板字符串类型 |
| 2022 | State of JS 调查 | TS 使用率首次超越纯 JS |
| 2022 | Vue 3 / Vite 普及 | 前端 TS 体验达到新高度 |

### 5. 现代阶段（2023 至今）

| 时间 | 版本 | 重要特性 |
|---|---|---|
| 2023-03 | TypeScript 5.0 | 现代化装饰器、const 类型参数 |
| 2023-08 | TypeScript 5.2 | Explicit Resource Management |
| 2024-03 | TypeScript 5.4 | NoInfer 工具类型 |
| 2024-06 | TypeScript 5.5 | 类型谓词推断 |
| 2024-11 | TypeScript 5.7 | --target es2024 |
| 2025-03 | TypeScript 5.8 | require() 加载 ESM 模块 |

### 6. 未来展望：用 Go 重写编译器

2025 年初，微软宣布以 Go 语言重写 TypeScript 编译器，项目代号 **Corsa**。预计性能提升超过 10 倍，将在 TypeScript 7.x 正式发布。

## (六) TypeScript 与其他强类型语言的对比

| 对比项 | TypeScript |
|---|---|
| Java / C# | Go |
| 类型系统 | 结构化类型（鸭子类型） |
| 名义类型 | 结构化类型 |
| 空值安全 | 开启 strictNullChecks 后支持 |
| Java 需 Optional，C# 8+ 支持 | 通过 error 值和 nil 检查 |
| 泛型 | 支持 |
| 支持 | Go 1.18+ 支持 |
| 编译产物 | JavaScript |
| 字节码 | 原生机器码 |
| 运行时类型检查 | 无 |
| 有（反射机制） | 无 |
| 学习曲线 | 对 JS 开发者友好 |
| 较陡 | 中等 |

## (七) TypeScript 的局限性

| 局限 | 说明 | 应对思路 |
|---|---|---|
| 运行时无类型 | 类型信息编译后全部抹除 | 使用 zod、io-ts 等库 |
| 编译步骤 | 多了一个编译环节 | Vite、esbuild 已有良好优化 |
| any 类型逃生舱 | 滥用 ny 会让类型检查形同虚设 | 开启 |

oImplicitAny 和 strict 模式 |

| 类型体操门槛 | 复杂类型对初学者不友好 | 循序渐进即可 |
|---|---|---|
| 第三方库支持 | 少数老旧库缺少类型定义 | 先查 @types/* |
