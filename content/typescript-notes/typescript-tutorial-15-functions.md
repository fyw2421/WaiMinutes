---
title: "十五、函数"
weight: 15
description: "本章介绍 函数。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

函数是一组一起执行一个任务的语句。

您可以把代码划分到不同的函数中。如何划分代码到不同的函数中是由您来决定的，但在逻辑上，划分通常是根据每个函数执行一个特定的任务来进行的。

函数声明告诉编译器函数的名称、返回类型和参数。函数定义提供了函数的实际主体。

## (一) 函数定义

函数就是包裹在花括号中的代码块，前面使用了关键词 function：

语法格式如下所示：

```typescript
function function_name()
{
    // 执行代码

}
```

### 1. 实例

## (二) 调用函数

函数只有通过调用才可以执行函数内的代码。

语法格式如下所示：

```typescript
function_name()
```

### 1. 实例

## (三) 函数返回值

有时，我们会希望函数将执行的结果返回到调用它的地方。

通过使用 return 语句就可以实现。

在使用 return 语句时，函数会停止执行，并返回指定的值。

语法格式如下所示：

```typescript
function function_name():return_type { 
    // 语句

    return value; 
}
```

- return_type 是返回值的类型。
- return 关键词后跟着要返回的结果。
- 一般情况下，一个函数只有一个 return 语句。
- 返回值的类型需要与函数定义的返回类型(return_type)一致。

### 1. 实例

- 实例中定义了函数 ，返回值的类型为 string。
- 函数通过 return 语句返回给调用它的地方，即变量 msg，之后输出该返回值。。

编译以上代码，得到以下 JavaScript 代码：

## (四) 带参数函数

在调用函数时，您可以向其传递值，这些值被称为参数。

这些参数可以在函数中使用。

您可以向函数发送多个参数，每个参数使用逗号 , 分隔：

语法格式如下所示：

```typescript
function func_name( param1 [:datatype], param2 [:datatype]) {   
}
```

- param1、param2 为参数名。
- datatype 为参数类型。

### 1. 实例

- 实例中定义了函数 ，返回值的类型为 number。
- 函数中定义了两个 number 类型的参数，函数内将两个参数相加并返回。

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
3
```

## (五) 可选参数和默认参数

### 1. 可选参数

在 TypeScript 函数里，如果我们定义了参数，则我们必须传入这些参数，除非将这些参数设置为可选，可选参数使用问号标识 ？。

**实例**

以下实例，我们将 lastName 设置为可选参数：

可选参数必须跟在必需参数后面。 如果上例我们想让 firstName 是可选的，lastName 必选，那么就要调整它们的位置，把 firstName 放在后面。

如果都是可选参数就没关系。

### 2. 默认参数

我们也可以设置参数的默认值，这样在调用函数的时候，如果不传入该参数的值，则使用默认参数，语法格式为：

```typescript
function function_name(param1[:type],param2[:type] = default_value) { 
}
```

注意：参数不能同时设置为可选和默认。

**实例**

以下实例函数的参数 rate 设置了默认值为 0.50，调用该函数时如果未传入参数则使用该默认值：

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
计算结果:  500
计算结果:  300
```

## (六) 剩余参数

有一种情况，我们不知道要向函数传入多少个参数，这时候我们就可以使用剩余参数来定义。

剩余参数语法允许我们将一个不确定数量的参数作为一个数组传入。

函数的最后一个命名参数 restOfName 以 ... 为前缀，它将成为一个由剩余参数组成的数组，索引值从0（包括）到 restOfName.length（不包括）。

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
和为： 6
和为： 50
```

## (七) 匿名函数

匿名函数是一个没有函数名的函数。

匿名函数在程序运行时动态声明，除了没有函数名外，其他的与标准函数一样。

我们可以将匿名函数赋值给一个变量，这种表达式就成为函数表达式。

语法格式如下：

```typescript
var res = function( [arguments] ) { ... }
```

### 1. 实例

不带参数匿名函数：

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
hello world
```

带参数匿名函数：

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
24
```

### 2. 匿名函数自调用

匿名函数自调用在函数后使用 () 即可：

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
Hello!!
```

## (八) 构造函数

TypeScript 也支持使用 JavaScript 内置的构造函数 Function() 来定义函数：

语法格式如下：

```typescript
var res = new Function ([arg1[, arg2[, ...argN]],] functionBody)
```

参数说明：

- **arg1, arg2, ... argN**：参数列表。
- **functionBody**：一个含有包括函数定义的 JavaScript 语句的字符串。

### 1. 实例

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
12
```

## (九) 递归函数

递归函数即在函数内调用函数本身。

> 举个例子： 
>  从前有座山，山里有座庙，庙里有个老和尚，正在给小和尚讲故事呢！故事是什么呢？"从前有座山，山里有座庙，庙里有个老和尚，正在给小和尚讲故事呢！故事是什么呢？'从前有座山，山里有座庙，庙里有个老和尚，正在给小和尚讲故事呢！故事是什么呢？……'"

### 1. 实例

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
720
```

## (十) Lambda 函数

Lambda 函数也称之为箭头函数。

箭头函数表达式的语法比函数表达式更短。

函数只有一行语句：

```typescript
( [param1, param2,…param n] )=>statement;
```

### 1. 实例

以下实例声明了 lambda 表达式函数，函数返回两个数的和：

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
110
```

函数是一个语句块：

```typescript
( [param1, param2,…param n] )=> {
 
    // 代码块

}
```

### 2. 实例

以下实例声明了 lambda 表达式函数，函数返回两个数的和：

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
110
```

我们可以不指定函数的参数类型，通过函数内来推断参数类型:

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
12 是一个数字
Tom 是一个字符串
```

单个参数 () 是可选的：

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
输出为 12
```

无参数时可以设置空括号：

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
调用函数
```

## (十一) 函数重载

重载是方法名字相同，而参数不同，返回类型可以相同也可以不同。

每个重载的方法（或者构造函数）都必须有一个独一无二的参数类型列表。

参数类型不同：

```typescript
function disp(string):void; 
function disp(number):void;
```

参数数量不同：

```typescript
function disp(n1:number):void; 
function disp(x:number,y:number):void;
```

参数类型顺序不同：

```typescript
function disp(n1:number,s1:string):void; 
function disp(s:string,n:number):void;
```

如果参数类型不同，则参数类型应设置为 **any**。

参数数量不同你可以将不同的参数设置为可选。

### 1. 实例

以下实例定义了参数类型与参数数量不同：

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
abc
undefined
1
xyz
```

- 定义函数重载需要定义**重载签名**和一个**实现签名**。
  重载签名定义函数的形参和返回类型，没有函数体。一个函数可以**有多个**重载签名(不可调用)
```typescript
let suits = ["hearts", "spades", "clubs", "diamonds"];
// 定义重载签名

function greet(person: string): string;
function greet(persons: string[]): string[];
// 定义实现签名

function greet(person: unknown): unknown {
    if (typeof person === 'string') {
        return `Hello, ${person}!`;
    } else if (Array.isArray(person)) {
        return person.map(name => `Hello, ${name}!`);
    }
    throw new Error('Unable to greet');
}
console.log(greet(suits[0]));
console.log(greet(suits));
```
```typescript
var suits = ["hearts", "spades", "clubs", "diamonds"];
// 实现签名

function greet(person) {
    if (typeof person === 'string') {
        return "Hello, ".concat(person, "!");
    }
    else if (Array.isArray(person)) {
        return person.map(function (name) { return "Hello, ".concat(name, "!"); });
    }
    throw new Error('Unable to greet');
}
console.log(greet(suits[0]));
console.log(greet(suits));
```
