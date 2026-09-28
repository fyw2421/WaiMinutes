---
title: "六、基本结构"
weight: 6
description: "本章介绍 基本结构。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 程序的基本结构可以分为几个部分，每个部分都有特定的作用。

以下是 TypeScript 程序的常见组成部分：

- **声明部分**：包括类型声明、接口声明等。
- **变量声明**：包括 `let`, `const` 和 `var` 的使用。
- **函数声明**：包括普通函数和箭头函数。
- **类声明**：用于定义类及其成员。
- **接口与类型别名**：描述类型的结构。
- **模块化**：通过 `import` 和 `export` 组织代码。
- **类型断言**：强制类型转换。
- **泛型**：使代码具备更多的复用性。
- **注释**：增加代码的可读性。
- **类型推断**：自动推断类型。
- **类型守卫**：缩小类型范围。
- **异步编程**：支持 `async/await`。
- **错误处理**：通过 `try/catch` 进行错误捕捉。

## (一) 声明部分（Declarations）

**类型声明：**TypeScript 是一种静态类型的语言，可以通过类型声明来定义变量、函数、类等的类型。

```typescript
let name: string = "Alice";
let age: number = 30;
```

**接口声明：**用于定义对象的结构，包括对象的属性和方法。

```typescript
interface Person {
    name: string;
    age: number;
}
```

## (二) 变量声明（Variable Declarations）

在 TypeScript 中，可以使用 let, const, 和 var 来声明变量。推荐使用 let 和 const。

```typescript
let age: number = 25;
const pi: number = 3.14;
```

## (三) 函数声明（Function Declarations）

函数声明：TypeScript 允许声明带有类型注解的函数，包括参数类型和返回值类型。

```typescript
function greet(name: string): string {
    return "Hello, " + name;
}
```

**箭头函数：**TypeScript 同样支持 ES6 的箭头函数。

```typescript
const greet = (name: string): string => "Hello, " + name;
```

## (四) 类声明（Class Declarations）

TypeScript 提供对面向对象编程的支持，允许定义类和类的方法、属性。

```typescript
class Person {
    name: string;
    age: number;
    
    constructor(name: string, age: number) {
        this.name = name;
        this.age = age;
    }
    
    greet() {
        return "Hello, my name is " + this.name;
    }
}
```

## (五) 接口与类型别名（Interfaces & Type Aliases）

接口（Interface）：用于描述对象的形状，接口可以继承和扩展。

```typescript
interface Animal {
    name: string;
    sound: string;
    makeSound(): void;
}
```

**类型别名（Type Alias）：**允许为对象类型、联合类型、交叉类型等定义别名。

```typescript
type ID = string | number;
```

## (六) 模块和导入导出（Modules & Imports/Exports）

TypeScript 支持模块化编程，可以使用 import 和 export 来组织代码。

**导出：**

```typescript
export class Person {
    constructor(public name: string) {}
}
```

**导入：**

```typescript
import { Person } from './person';
```

## (七) 类型断言（Type Assertions）

开发者可以使用类型断言来强制指定类型。

```typescript
let value: any = "hello";
let strLength: number = (value as string).length;
```

## (八) 泛型（Generics）

泛型允许在定义函数、接口或类时不指定具体类型，而是使用占位符。

```typescript
function identity(arg: T): T {
    return arg;
}
```

## (九) 注释（Comments）

**单行注释：**

```typescript
// 这是一个单行注释
```

**多行注释：**

```typescript
/* 这是一个
    多行注释 */
```

## (十) 类型推断（Type Inference）

TypeScript 在某些情况下会自动推断变量的类型。

```typescript
let num = 10;  // TypeScript 推断 num 为 number 类型
```

## (十一) 类型守卫（Type Guards）

TypeScript 提供了类型守卫（如 typeof 和 instanceof），用于在运行时缩小变量的类型范围。

```typescript
function isString(value: any): value is string {
    return typeof value === 'string';
}
```

## (十二) 异步编程（Asynchronous Programming）

TypeScript 完全支持异步编程，可以使用 async/await 语法来处理异步操作。

```typescript
async function fetchData(): Promisestring> {
    const response = await fetch("https://example.com");
    const data = await response.text();
    return data;
}
```

## (十三) 错误处理（Error Handling）

TypeScript 允许使用 try/catch 块进行错误处理。

```typescript
try {
    throw new Error("Something went wrong");
} catch (error) {
    if (error instanceof Error) {
        console.error(error.message);
    }
}
```
