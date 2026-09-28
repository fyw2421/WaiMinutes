---
title: "五、基础语法"
weight: 5
description: "本章介绍 基础语法。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 程序由以下几个部分组成：

- 模块
- 函数
- 变量
- 语句和表达式
- 注释

## (一) 第一个 TypeScript 程序

我们可以使用以下 TypeScript 程序来输出 "Hello World"：

```typescript
const hello : string = "Hello World!"
console.log(hello)
```

以上代码首先通过 **tsc** 命令编译：

```
tsc Runoob.ts
```

得到如下 js 代码：

```javascript
var hello = "Hello World!";
console.log(hello);
```

最后我们使用 node 命令来执行该 js 代码。

```
$ node Runoob.js
Hello World
```

我们可以同时编译多个 ts 文件：

```
tsc file1.ts file2.ts file3.ts
```

tsc 常用编译参数如下表所示：

| 序号 | 编译参数说明 |
|---|---|
| 1. | **--help** 显示帮助信息 |
| 2. | **--module** 载入扩展模块 |
| 3. | **--target** 设置 ECMA 版本 |
| 4. | **--declaration** 额外生成一个 .d.ts 扩展名的文件 |
| 5. | **--removeComments** 删除文件的注释 |
| 6. | **--out** 编译多个文件并合并到一个输出的文件 |
| 7. | **--sourcemap** 生成一个 sourcemap (.map) 文件 |
| 8. | **--module noImplicitAny** 在表达式和声明上有隐含的 any 类型时报错 |
| 9. | **--watch** 在监视模式下运行编译器 |

## (二) TypeScript 保留关键字

| 关键字 | 说明 |
|---|---|
| `abstract` | 用于定义抽象类或抽象方法 |
| `any` | 表示任意类型，禁用类型检查 |
| `as` | 类型断言 |
| `await` | 用于异步函数中 |
| `boolean` | 表示布尔类型 |
| `break` | 退出循环或 switch 语句 |
| `case` | 用于 switch 语句中的分支 |
| `catch` | 用于捕获异常 |
| `class` | 用于定义类 |
| `const` | 定义常量变量 |
| `continue` | 跳过当前循环 |
| `debugger` | 启动调试器 |
| `declare` | 声明变量或模块 |
| `default` | 定义 switch 语句的默认分支 |
| `delete` | 删除对象属性 |
| `do` | 用于 do...while 循环 |
| `else` | 定义条件语句的 else 部分 |
| `enum` | 定义枚举类型 |
| `export` | 从模块中导出 |
| `extends` | 用于类的继承 |
| `false` | 布尔值 false |
| `finally` | try...catch 最终执行代码块 |
| `for` | for 循环 |
| `from` | 模块导入语句 |
| `function` | 定义函数 |
| `get` | getter 方法 |
| `if` | 条件判断 |
| `implements` | 类实现接口 |
| `import` | 从模块中导入 |
| `in` | 检查属性或 for...in 循环 |
| `infer` | 条件类型中推断类型 |
| `instanceof` | 检查对象是否是类的实例 |
| `interface` | 定义接口 |
| `let` | 定义块级作用域变量 |
| `module` | 定义模块 |
| `namespace` | 定义命名空间 |
| `new` | 创建类的实例 |
| `null` | 表示空值 |
| `number` | 表示数字类型 |
| `object` | 表示非原始类型 |
| `of` | for...of 循环 |
| `package` | 标识包 |
| `private` | 私有访问修饰符 |
| `protected` | 受保护访问修饰符 |
| `public` | 公共访问修饰符 |
| `readonly` | 只读属性 |
| `require` | 导入 CommonJS 模块 |
| `return` | 退出函数 |
| `set` | setter 方法 |
| `string` | 字符串类型 |
| `super` | 调用父类方法 |
| `switch` | switch 语句 |
| `symbol` | 符号类型 |
| `this` | 引用当前实例 |
| `throw` | 抛出异常 |
| `try` | try...catch 语句 |
| `true` | 布尔值 true |
| `type` | 定义类型别名 |
| `typeof` | 获取变量类型 |
| `undefined` | 表示未定义的值 |
| `unique` | symbol 唯一标识符 |
| `var` | 声明变量（不推荐） |
| `void` | 无返回值类型 |
| `while` | while 循环 |
| `with` | 创建作用域（不推荐） |
| `yield` | 生成器函数 |

### 1. 空白和换行

TypeScript 会忽略程序中出现的空格、制表符和换行符。

### 2. TypeScript 区分大小写

TypeScript 区分大写和小写字符。

### 3. 分号是可选的

每行指令都是一段语句，你可以使用分号或不使用，分号在 TypeScript 中是可选的，建议使用。

### 4. TypeScript 注释

TypeScript 支持两种类型的注释：

- **单行注释 ( // )** -- 在 // 后面的文字都是注释内容。
- **多行注释 (/* */)** -- 这种注释可以跨越多行。

```typescript
// 这是一个单行注释
 
/* 
 这是一个多行注释 
 这是一个多行注释 
 这是一个多行注释 
*/
```

## (三) TypeScript 与面向对象

TypeScript 是一种面向对象的编程语言。

面向对象主要有两个概念：对象和类。

- **对象**：对象是类的一个实例，有状态和行为。
- **类**：类是一个模板，描述一类对象的行为和状态。
- **方法**：方法是类的操作的实现步骤。

TypeScript 面向对象编程实例：

```typescript
class Site {
    name():void {
        console.log("Runoob")
    }
}
var obj = new Site();
obj.name();
```

编译后生成的 JavaScript 代码如下：

```javascript
var Site = /** @class */ (function () {
    function Site() {
    }
    Site.prototype.name = function () {
        console.log("Runoob");
    };
    return Site;
}());
var obj = new Site();
obj.name();
```

执行以上 JavaScript 代码，输出结果如下:

```
Runoob
```
