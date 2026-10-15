---
title: "TypeScriptNotes : 总目录"
weight: 1
description: "TypeScript 学习笔记总目录：68 篇按入门与环境、基本类型与语法、异步编程、内置类型、类型系统、高级类型、工程与实战七组编排，每篇附一句话说明。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

# 一、这份笔记

共 68 篇。本教程由 [菜鸟教程](https://www.runoob.com/typescript/ts-tutorial.html) 内容整理而成，保存日期 2026-06-26。

按**依赖顺序**编排而非主题分类：后面的内容默认你已经读过前面的。可以从 01 顺读到 68，也可以直接跳到需要的分组。

# 一、入门与环境（01–09）

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 01 | [TypeScript 简介]({{< ref "typescript-tutorial-01-introduction" >}}) | 语言定位，以及与 JavaScript 的差异 |
| 02 | [TypeScript 安装]({{< ref "typescript-tutorial-02-installation" >}}) | 全局安装、本地安装与版本管理 |
| 03 | [VS Code]({{< ref "typescript-tutorial-03-vs-code" >}}) | 编辑器配置与必要插件 |
| 04 | [TypeScript 特性]({{< ref "typescript-tutorial-04-features" >}}) | 类型系统与编译期检查的价值 |
| 05 | [基础语法]({{< ref "typescript-tutorial-05-basic-syntax" >}}) | 与 JavaScript 共通的语法部分 |
| 06 | [基本结构]({{< ref "typescript-tutorial-06-basic-structure" >}}) | 程序入口与代码组织方式 |
| 07 | [TypeScript vs JavaScript]({{< ref "typescript-tutorial-07-typescript-vs-javascript" >}}) | 两者的能力边界与取舍 |
| 08 | [tsconfig.json]({{< ref "typescript-tutorial-08-tsconfig-json" >}}) | 配置文件结构与常用字段 |
| 09 | [编译选项]({{< ref "typescript-tutorial-09-compile-options" >}}) | target、module 与严格模式开关 |

# 二、基本类型与语法（10–18）

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 10 | [基础类型]({{< ref "typescript-tutorial-10-basic-types" >}}) | string、number、boolean 与字面量 |
| 11 | [变量声明]({{< ref "typescript-tutorial-11-variable-declarations" >}}) | let、const 与类型标注 |
| 12 | [运算符]({{< ref "typescript-tutorial-12-operators" >}}) | 算术、比较与逻辑运算 |
| 13 | [条件语句]({{< ref "typescript-tutorial-13-conditionals" >}}) | if、三元与 switch |
| 14 | [循环]({{< ref "typescript-tutorial-14-loops" >}}) | for、while 与遍历写法 |
| 15 | [函数]({{< ref "typescript-tutorial-15-functions" >}}) | 声明、参数与返回值类型 |
| 16 | [函数重载]({{< ref "typescript-tutorial-16-function-overloads" >}}) | 同名多签名与实现签名 |
| 17 | [箭头函数]({{< ref "typescript-tutorial-17-arrow-functions" >}}) | 表达式体与 this 绑定 |
| 18 | [迭代器]({{< ref "typescript-tutorial-18-iterators-generators" >}}) | Iterable、Iterator 与 yield |

# 三、异步编程（19–20）

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 19 | [async/await]({{< ref "typescript-tutorial-19-async-await" >}}) | 异步函数的写法与错误处理 |
| 20 | [Promise]({{< ref "typescript-tutorial-20-promise" >}}) | then、catch 与组合 |

# 四、内置类型（21–31）

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 21 | [Number]({{< ref "typescript-tutorial-21-number" >}}) | 数值方法与浮点精度问题 |
| 22 | [Array]({{< ref "typescript-tutorial-22-array" >}}) | 常用方法与不可变操作 |
| 23 | [Map]({{< ref "typescript-tutorial-23-map" >}}) | 键值映射与遍历顺序 |
| 24 | [枚举]({{< ref "typescript-tutorial-24-enums" >}}) | 数字、字符串与联合类型枚举 |
| 25 | [类型推断]({{< ref "typescript-tutorial-25-type-inference" >}}) | 编译器如何推断出类型 |
| 26 | [类型断言]({{< ref "typescript-tutorial-26-type-assertion" >}}) | as 与尖括号两种写法 |
| 27 | [null 和 undefined]({{< ref "typescript-tutorial-27-null-undefined" >}}) | 严格模式下的处理方式 |
| 28 | [Symbol]({{< ref "typescript-tutorial-28-symbol" >}}) | 唯一标识与迭代协议 |
| 29 | [特殊类型]({{< ref "typescript-tutorial-29-special-types" >}}) | any、unknown 与 never 的取舍 |
| 30 | [Set / WeakMap]({{< ref "typescript-tutorial-30-set-weakmap" >}}) | 集合与弱引用映射 |
| 31 | [元组]({{< ref "typescript-tutorial-31-tuples" >}}) | 定长数组与具名元素 |

# 五、类型系统（32–47）

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 32 | [联合类型]({{< ref "typescript-tutorial-32-union-types" >}}) | 用竖线组合多个类型 |
| 33 | [接口]({{< ref "typescript-tutorial-33-interfaces" >}}) | 结构化契约与可选属性 |
| 34 | [类]({{< ref "typescript-tutorial-34-classes" >}}) | 属性、方法与继承 |
| 35 | [对象]({{< ref "typescript-tutorial-35-objects" >}}) | 字面量类型与索引签名 |
| 36 | [泛型]({{< ref "typescript-tutorial-36-generics" >}}) | 类型参数与约束 |
| 37 | [命名空间]({{< ref "typescript-tutorial-37-namespaces" >}}) | 内部模块与全局声明 |
| 38 | [模块]({{< ref "typescript-tutorial-38-modules" >}}) | import、export 与模块解析 |
| 39 | [声明文件]({{< ref "typescript-tutorial-39-declaration-files" >}}) | 给 JavaScript 补上类型 |
| 40 | [type 别名]({{< ref "typescript-tutorial-40-type-alias" >}}) | 类型的组合与复用 |
| 41 | [字面量类型]({{< ref "typescript-tutorial-41-literal-types" >}}) | 把取值范围限定为字面量 |
| 42 | [抽象类]({{< ref "typescript-tutorial-42-abstract-classes" >}}) | 抽象成员与模板方法 |
| 43 | [访问修饰符]({{< ref "typescript-tutorial-43-access-modifiers" >}}) | public、private、protected |
| 44 | [混入（Mixin）]({{< ref "typescript-tutorial-44-mixin" >}}) | 用交叉类型复用实现 |
| 45 | [装饰器]({{< ref "typescript-tutorial-45-decorators" >}}) | 实验特性与元编程 |
| 46 | [类型守卫]({{< ref "typescript-tutorial-46-type-guards" >}}) | 类型收窄与自定义守卫函数 |
| 47 | [可选链]({{< ref "typescript-tutorial-47-optional-chaining" >}}) | ?. 与 ?? 的短路取值 |

# 六、高级类型（48–54）

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 48 | [工具类型]({{< ref "typescript-tutorial-48-utility-types" >}}) | Partial、Pick、Omit 等内置工具 |
| 49 | [条件类型]({{< ref "typescript-tutorial-49-conditional-types" >}}) | extends 与分发机制 |
| 50 | [映射类型]({{< ref "typescript-tutorial-50-mapped-types" >}}) | keyof 与属性变换 |
| 51 | [类继承与多态]({{< ref "typescript-tutorial-51-inheritance-polymorphism" >}}) | 泛型约束下的继承 |
| 52 | [错误处理]({{< ref "typescript-tutorial-52-error-handling" >}}) | 错误类型与联合返回值 |
| 53 | [交叉类型]({{< ref "typescript-tutorial-53-intersection-types" >}}) | 多个类型的合并 |
| 54 | [模板字面量]({{< ref "typescript-tutorial-54-template-literals" >}}) | 字面量拼接成新类型 |

# 七、工程与实战（55–68）

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 55 | [从 JavaScript 迁移]({{< ref "typescript-tutorial-55-migration-from-javascript" >}}) | 渐进式改造的步骤 |
| 56 | [单元测试]({{< ref "typescript-tutorial-56-unit-testing" >}}) | 类型测试与 ts-jest 配置 |
| 57 | [infer 关键字]({{< ref "typescript-tutorial-57-infer-keyword" >}}) | 从已有类型中提取子类型 |
| 58 | [索引类型]({{< ref "typescript-tutorial-58-index-types" >}}) | keyof、typeof 与索引访问 |
| 59 | [递归类型]({{< ref "typescript-tutorial-59-recursive-types" >}}) | 递归约束与深度限制 |
| 60 | [协变与逆变]({{< ref "typescript-tutorial-60-co-contravariance" >}}) | 函数参数与返回的类型关系 |
| 61 | [路径映射]({{< ref "typescript-tutorial-61-path-mapping" >}}) | paths 与 baseUrl |
| 62 | [项目引用]({{< ref "typescript-tutorial-62-project-references" >}}) | 多包之间的相互引用 |
| 63 | [Monorepo 配置]({{< ref "typescript-tutorial-63-monorepo" >}}) | 工作区与统一构建 |
| 64 | [性能优化]({{< ref "typescript-tutorial-64-performance-optimization" >}}) | 类型检查与编译速度 |
| 65 | [综合项目实战]({{< ref "typescript-tutorial-65-practical-project" >}}) | 从零搭建一个完整项目 |
| 66 | [Node.js 实战]({{< ref "typescript-tutorial-66-nodejs-practical" >}}) | 后端类型定义与运行 |
| 67 | [Vue3 实战]({{< ref "typescript-tutorial-67-vue3-practical" >}}) | 组件 props 与组合式 API |
| 68 | [React 实战]({{< ref "typescript-tutorial-68-react-practical" >}}) | 组件类型与 Hooks |

# 八、参考资料

- [菜鸟教程 TypeScript 教程](https://www.runoob.com/typescript/ts-tutorial.html) — 内容来源
- [TypeScript Handbook](https://www.typescriptlang.org/docs/handbook/intro.html) — 官方手册
- [TypeScript 文档](https://www.typescriptlang.org/docs/) — 类型系统与编译器选项
