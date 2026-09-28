---
title: "十六、函数重载"
weight: 16
description: "本章介绍 函数重载。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

函数重载（Function Overloading）允许为一个函数定义多个签名，编译器会根据传入的参数类型选择正确的实现。

函数重载工作原理

函数重载签名（声明）

// 签名 1

function add(a: number, b: number): number;

// 签名 2

function add(a: string, b: string): string;

编译器

匹配

函数实现（实际代码）

```typescript
function add(a: any, b: any): any {
return a + b;
}
```

必须兼容所有签名

调用时类型推断

add(1, 2) → number

add("a", "b") → string

add(true, false) → any

## (一) 基本语法

先声明多个函数签名，然后实现一个统一函数。

**运行结果：**

```typescript
数字相加: 3
字符串相加: Hello, World
```

## (二) 多参数重载

可以定义多个参数的不同组合。

**运行结果：**

```typescript
Hello, Alice!
Hi, Bob!
```

## (三) 方法重载

类中的方法也可以使用重载。

**运行结果：**

```typescript
数字: 3
字符串: HelloWorld
混合: 5 apples
```

## (四) 构造函数重载

构造函数同样可以重载。

**运行结果：**

```typescript
用户1: {"name":"Alice","age":0}
用户2: {"name":"Bob","age":25}
```

## (五) 重载与联合类型

使用重载而不是联合类型可以获得更精确的类型推断。

**运行结果：**

```typescript
数字结果: 20
字符串结果: HELLO
```

## (六) 注意事项

- 重载签名必须放在实现签名之前
- 实现签名必须兼容所有重载签名
- 重载签名只是类型声明，不生成实际代码

## (七) 总结

- **函数重载：**定义多个签名，编译器选择匹配的实现
- **方法重载：**类中同样适用
- **构造函数重载：**提供多种初始化方式
- **优于联合类型：**返回类型更精确
