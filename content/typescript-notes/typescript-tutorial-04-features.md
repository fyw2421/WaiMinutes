---
title: "四、TypeScript 特性"
weight: 4
description: "本章介绍 TypeScript 特性。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

相对 JavaScript，TypeScript 增加了许多关键功能，特别是围绕类型系统和代码结构的增强功能。

TypeScript 的一些关键特性：

- **静态类型检查**：TypeScript 在编译时就会检查代码的类型是否匹配，能够发现很多潜在的错误。
- **类型推断**：TypeScript 能够自动推断变量的类型。
- **接口和类型定义**：TypeScript 提供了 interface 和 ype 关键字，允许你定义复杂的数据结构。
- **类和模块支持**：TypeScript 支持面向对象编程中的类（class）概念，增加了构造函数、继承、访问控制修饰符。
- **工具和编辑器支持**：TypeScript 拥有良好的编辑器支持，特别是与 Visual Studio Code 集成时，能提供智能提示、自动补全、重构等工具。
- **兼容 JavaScript**：TypeScript 是 JavaScript 的超集，所有合法的 JavaScript 代码都是合法的 TypeScript 代码。

以下是 TypeScript 增加的主要功能：

## (一) 静态类型

TypeScript 的最大特性就是增加了静态类型系统。开发者可以显式地声明变量、参数、返回值的类型，这样可以在编译时捕获很多潜在的类型错误。

```typescript
let name: string = "Alice";
let age: number = 25;
```

## (二) 类型推断

TypeScript 可以自动推断变量类型，即使不显式声明类型。

```typescript
let name = "Alice"; // 推断为 string
```

## (三) 接口 (Interfaces)

TypeScript 提供了接口，允许定义复杂的对象结构。

```typescript
interface Person {
  name: string;
  age: number;
  greet(): void;
}
class Student implements Person {
  constructor(public name: string, public age: number) {}
  greet() {
    console.log(Hello, my name is );
  }
}
```

## (四) 类型别名 (Type Aliases)

类型别名 ( ype) 可以为复杂的类型定义简短的别名。

```typescript
type StringOrNumber = string | number;
let value: StringOrNumber = 42;
```

## (五) 枚举 (Enums)

TypeScript 引入了 enum 类型，用于定义一组命名的常量。

```typescript
enum Direction {
  Up,
  Down,
  Left,
  Right,
}
let dir: Direction = Direction.Up;
```

## (六) 元组 (Tuples)

元组允许定义具有固定数量和类型的数组。

```typescript
let point: [number, number] = [10, 20];
```

## (七) 访问控制修饰符 (Access Modifiers)

TypeScript 在类中提供了 public、private 和 protected 修饰符。

```typescript
class Person {
  private name: string;
  protected age: number;
  public constructor(name: string, age: number) {
    this.name = name;
    this.age = age;
  }
}
```

## (八) 抽象类 (Abstract Classes)

TypeScript 支持抽象类，抽象类不能直接实例化，需要由子类实现。

```typescript
abstract class Animal {
  abstract makeSound(): void;
}
class Dog extends Animal {
  makeSound() {
    console.log("Woof!");
  }
}
```

## (九) 泛型 (Generics)

TypeScript 支持泛型，允许在类、接口和函数中使用参数化类型。

```typescript
function identity(value: T): T {
  return value;
}
let num = identitynumber>(42);
```

## (十) 模块和命名空间

TypeScript 提供了基于 ES6 的模块系统，使用 import 和 export 导入和导出模块。

```typescript
// math.ts
export function add(a: number, b: number): number {
  return a + b;
}
// main.ts
import { add } from "./math";
console.log(add(2, 3));
```

## (十一) 类型守卫 (Type Guards)

TypeScript 提供了类型守卫，可以在代码中检查变量类型，帮助编译器推断更加具体的类型。

```typescript
function printId(id: string | number) {
  if (typeof id === "string") {
    console.log(id.toUpperCase());
  } else {
    console.log(id.toFixed(2));
  }
}
```

## (十二) 可选链和空值合并运算符

TypeScript 增加了 JavaScript 的可选链 (?.) 和空值合并运算符 (??)。

```typescript
let user = { name: "Alice", address: { city: "Wonderland" } };
console.log(user?.address?.city);
let value = null;
console.log(value ?? "default");
```

## (十三) 类型兼容性和工具类型

TypeScript 提供了一些工具类型，如 Partial、Pick、Readonly、Record 等。

```typescript
interface Todo {
  title: string;
  description: string;
}
let partialTodo: Partial = { title: "Learn TypeScript" };
```

## (十四) 编译期错误检查

TypeScript 提供的编译期错误检查可以捕获 JavaScript 中不易发现的错误，如拼写错误、类型不匹配等。

## (十五) ES 新特性支持

TypeScript 提前支持了一些还未在所有环境中普及的 ES 特性，如装饰器（Decorators）、异步迭代器等，且能够将其编译成兼容 JavaScript 版本。
