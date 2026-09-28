---
title: "十、基础类型"
weight: 10
description: "本章介绍 基础类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

基础类型可以开发者更准确地描述数据的结构和意图。

TypeScript 包含的数据类型如下表:

| 类型 | 描述 | 示例 |
|---|---|---|
| `string` | 表示文本数据 | `let name: string = "Alice";` |
| `number` | 表示数字，包括整数和浮点数 | `let age: number = 30;` |
| `boolean` | 表示布尔值 `true` 或 `false` | `let isDone: boolean = true;` |
| `array` | 表示相同类型的元素数组 | `let list: number[] = [1, 2, 3];` |
| `tuple` | 表示已知类型和长度的数组 | `let person: [string, number] = ["Alice", 30];` |
| `enum` | 定义一组命名常量 | `enum Color { Red, Green, Blue };` |
| `any` | 任意类型，不进行类型检查 | `let value: any = 42;` |
| `void` | 无返回值（常用于函数） | `function log(): void {}` |
| `null` | 表示空值 | `let empty: null = null;` |
| `undefined` | 表示未定义 | `let undef: undefined = undefined;` |
| `never` | 表示不会有返回值 | `function error(): never { throw new Error("error"); }` |
| `object` | 表示非原始类型 | `let obj: object = { name: "Alice" };` |
| `union` | 联合类型，表示可以是多种类型之一 | `let id: string |
| `unknown` | 不确定类型，需类型检查后再使用 | `let value: unknown = "Hello";` |

**注意：**TypeScript 和 JavaScript 没有整数类型。

## (一) string 字符串

表示文本数据，只能存储字符串，通常用于描述文字信息。

```typescript
let message: string = "Hello, TypeScript!";
```

**模板字符串：**TypeScript 支持 **模板字符串**，用反引号 `（记住不是单引号 '）来定义，允许在字符串中插入变量或表达式，非常适合多行文本和拼接变量。

```typescript
let name: string = "Alice";
let greeting: string = `Hello, ${name}! Welcome to TypeScript.`;
console.log(greeting); // 输出：Hello, Alice! Welcome to TypeScript.
```

## (二) number 数字

TypeScript 使用 number 表示所有数字，包括整数和浮点数。

```typescript
let age: number = 25;
let temperature: number = 36.5;
```

## (三) boolean 布尔值

表示逻辑值 true 或 false，用于条件判断。

```typescript
let isCompleted: boolean = false;
```

## (四) array 数组

可以表示一组相同类型的元素。可以使用 type[] 或 Array两种方式表示。

```typescript
let numbers: number[] = [1, 2, 3];
let names: Array<string> = ["Alice", "Bob"];
```

## (五) tuple 元组

表示已知数量和类型的数组。每个元素可以是不同的类型，适合表示固定结构的数据。

```typescript
let person: [string, number] = ["Alice", 25];
```

## (六) enum 枚举

用来定义一组命名常量。默认情况下枚举的值从 0 开始递增。

```typescript
enum Color {
  Red,
  Green,
  Blue,
}
let favoriteColor: Color = Color.Green;
```

## (七) any 类型

以表示任何类型。适合不确定数据类型的情况，但使用时需谨慎，因为 any 会绕过类型检查。

```typescript
let randomValue: any = 42;
randomValue = "hello";
```

任意值是 TypeScript 针对编程时类型不明确的变量使用的一种数据类型，它常用于以下三种情况。

1、变量的值会动态改变时，比如来自用户的输入，任意值类型可以让这些变量跳过编译阶段的类型检查，示例代码如下：

```typescript
let x: any = 1;    // 数字类型

x = 'I am who I am';    // 字符串类型

x = false;    // 布尔类型
```

改写现有代码时，任意值允许在编译时可选择地包含或移除类型检查，示例代码如下：

```typescript
let x: any = 4;
x.ifItExists();    // 正确，ifItExists方法在运行时可能存在，但这里并不会检查

x.toFixed();    // 正确
```

定义存储各种类型数据的数组时，示例代码如下：

```typescript
let arrayList: any[] = [1, false, 'fine'];
arrayList[1] = 100;
```

## (八) void 空类型

用于没有返回值的函数。声明变量时，类型 void 意味着只能赋值 null 或 undefined。

```typescript
function logMessage(message: string): void {
  console.log(message);
}
```

## (九) null 和 undefined

null 和 undefined分别表示"空值"和"未定义"。在默认情况下，它们是所有类型的子类型，但可以通过设置 strictNullChecks 严格检查。

```typescript
let empty: null = null;
let notAssigned: undefined = undefined;
```

**null**

在 JavaScript 中 null 表示 "什么都没有"。

null是一个只有一个值的特殊类型。表示一个空对象引用。

用 typeof 检测 null 返回是 object。

**undefined**

在 JavaScript 中, undefined 是一个没有设置值的变量。

typeof 一个没有值的变量会返回 undefined。

Null 和 Undefined 是其他任何类型（包括 void）的子类型，可以赋值给其它类型，如数字类型，此时，赋值后的类型会变成 null 或 undefined。而在TypeScript中启用严格的空校验（--strictNullChecks）特性，就可以使得null 和 undefined 只能被赋值给 void 或本身对应的类型，示例代码如下：

```typescript
// 启用 --strictNullChecks

let x: number;
x = 1; // 编译正确

x = undefined;    // 编译错误

x = null;    // 编译错误
```

上面的例子中变量 x 只能是数字类型。如果一个类型可能出现 null 或 undefined， 可以用 | 来支持多种类型，示例代码如下：

```typescript
// 启用 --strictNullChecks

let x: number | null | undefined;
x = 1; // 编译正确

x = undefined;    // 编译正确

x = null;    // 编译正确
```

更多内容可以查看：JavaScript typeof, null, 和 undefined

## (十) never 类型

表示不会有返回值，通常用于抛出错误或进入无限循环的函数，表示该函数永远不会正常结束。

```typescript
function throwError(message: string): never {
  throw new Error(message);
}
```

never 是其它类型（包括 null 和 undefined）的子类型，代表从不会出现的值。这意味着声明为 never 类型的变量只能被 never 类型所赋值，在函数中它通常表现为抛出异常或无法执行到终止点（例如无限循环），示例代码如下：

```typescript
let x: never;
let y: number;

// 编译错误，数字类型不能转为 never 类型

x = 123;

// 运行正确，never 类型可以赋值给 never类型

x = (()=>{ throw new Error('exception')})();

// 运行正确，never 类型可以赋值给 数字类型

y = (()=>{ throw new Error('exception')})();

// 返回值为 never 的函数可以是抛出异常的情况

function error(message: string): never {
    throw new Error(message);
}

// 返回值为 never 的函数可以是无法被执行到的终止点的情况

function loop(): never {
    while (true) {}
}
```

## (十一) object 对象类型

表示非原始类型的值，适用于复杂的对象结构。

```typescript
let person: object = { name: "Alice", age: 30 };
```

## (十二) 联合类型 (Union)

表示一个变量可以是多种类型之一。通过 | 符号实现。

```typescript
let id: string | number;
id = "123";
id = 456;
```

## (十三) unknown 不确定的类型

与 any 类似，但更严格。必须经过类型检查后才能赋值给其他类型变量。

```typescript
let value: unknown = "Hello";
if (typeof value === "string") {
  let message: string = value;
}
```

## (十四) 类型断言 (Type Assertions)

类型断言可以让开发者明确告诉编译器变量的类型，常用于无法推断的情况。可以使用 as 或尖括号语法。

```typescript
let someValue: any = "this is a string";
let strLength: number = (someValue as string).length;
```

## (十五) 字面量类型

字面量类型可以让变量只能拥有特定的值，用于结合联合类型定义变量的特定状态。

```typescript
let direction: "up" | "down" | "left" | "right";
direction = "up";
```

通过这些类型，TypeScript 提供了更强的类型安全性和代码检查能力，使开发者能够更清晰、准确地表达数据和意图，减少运行时错误。

## (十六) 实例

以下实例展示了 TypeScript 中主要基础类型的定义和使用，模拟一个用户对象和相关的操作函数：

**说明：**

- **枚举类型 (`Role`)**：用于定义用户角色的命名常量 `Admin`、`User` 和 `Guest`。
- **接口 (`User`)**：定义了 `User` 对象的结构，包括 `id`、`username`、`isActive` 等属性，展示了 `string`、`number`、`boolean`、`array` 和 `tuple` 类型的使用。
- **函数 `getUserInfo`**：返回用户的描述信息，使用了字符串插值并结合 `enum`。
- **函数 `printUserInfo`**：类型为 `void`，因为它仅输出信息，不返回任何值。
- **联合类型 (`number | string`)**：用于 `findUser` 函数的参数，可以接受数字或字符串。
- **`never` 类型**：`throwError` 函数抛出错误，不会正常返回，因此使用 `never` 类型。
- **`any` 类型**：展示如何声明一个可以接收任何类型的变量。
- **`unknown` 类型**：类似 `any`，但更加安全，示例中在类型断言之前进行类型检查。
- **`null` 和 `undefined`**：演示空值的使用。

- 
  ```typescript
  const getValue = () => {
  </ul>
    return 0
  }

  enum List {
    A = getValue(),
    B = 2,  // 此处必须要初始化值，不然编译不通过

    C
  }
  console.log(List.A) // 0

  console.log(List.B) // 2

  console.log(List.C) // 3
  ```
  A 的值是被计算出来的。注意注释部分，如果某个属性的值是计算出来的，那么它后面一位的成员必须要初始化值。
  Andy Zhao
- **布尔型：**
  ```typescript
  let isDone: boolean = false;
  </ul>
  ```
  **数字：**
  ```typescript
  let decLiteral: number = 6;
  let hexLiteral: number = 0xf00d;
  let binaryLiteral: number = 0b1010;
  let octalLiteral: number = 0o744;
  ```
  **字符串：**
  ```typescript
  let name: string = "bob";
  name = "smith";

  let name: string = `Gene`;
  let age: number = 37;
  let sentence: string = `Hello, my name is ${ name }.

  I'll be ${ age + 1 } years old next month.`;

  let sentence: string = "Hello, my name is " + name + ".\\n\\n" +
      "I'll be " + (age + 1) + " years old next month."; 
  ```
  **数组：**
  ```typescript
  let list: number[] = [1, 2, 3]; //数字类型的数组

  //第二种方式是使用数组泛型，Array<元素类型>：

  let list: Array<number> = [1, 2, 3];
  ```
  **元组 Tuple：（数量和类型有限的数组———元组）**
  ```typescript
  let x: [string, number];
  x = ['hello', 10];
  ```
  **枚举：**
  ******`enum`类型是对JavaScript标准数据类型的一个补充(**使用枚举类型可以为一组数值赋予友好的名字 )****
  ```typescript
  enum Color {Red, Green, Blue}
  let c: Color = Color.Green;

  //默认情况下，从0开始为元素编号。 你也可以手动的指定成员的数值。 

  //例如，我们将上面的例子改成从 1开始编号：

  enum Color {Red = 1, Green, Blue}
  let c: Color = Color.Green;

  //或者，全部都采用手动赋值：

  enum Color {Red = 1, Green = 2, Blue = 4}
  let c: Color = Color.Green;

  //枚举类型提供的一个便利是你可以由枚举的值得到它的名字。 

  //例如，我们知道数值为2，但是不确定它映射到Color里的哪个名字，我们可以查找相应的名字：

  enum Color {Red = 1, Green, Blue}
  let colorName: string = Color[2];

  console.log(colorName);  // 显示'Green'因为上面代码里它的值是2
  ```
  小袁
