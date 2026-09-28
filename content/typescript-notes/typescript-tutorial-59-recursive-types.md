---
title: "五十九、递归类型"
weight: 59
description: "本章介绍 递归类型。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

递归类型是一种引用自身的类型，在处理树结构、嵌套数据时非常有用。

TypeScript 支持递归类型定义，可以表达无限深度的数据结构。

## (一) 为什么需要递归类型

在现实世界中，数据结构往往是嵌套的。

例如，文件系统有文件夹和子文件夹，组织架构有部门和子部门，JSON 数据可以无限嵌套。

递归类型允许我们表达这种无限嵌套的结构，是处理树形数据的基石。

> **概念：**递归类型是指在类型定义中引用自身的类型，可以表达任意深度的嵌套结构。

## (二) 树形结构

```typescript
interface TreeNode {
    id: number;
    name: string;
    children?: TreeNode[];
}
const fileSystem: TreeNode = {
    id: 1,
    name: "根目录",
    children: [
        { id: 2, name: "文件夹1", children: [{ id: 5, name: "文件A.txt" }, { id: 6, name: "文件B.txt" }] },
        { id: 3, name: "文件夹2", children: [{ id: 7, name: "文件C.txt" }] },
        { id: 4, name: "文件.txt" }
    ]
};
function traverse(node: TreeNode, depth: number = 0): void {
    const indent = "  ".repeat(depth);
    console.log(indent + "- " + node.name);
    if (node.children) { for (const child of node.children) { traverse(child, depth + 1); } }
}
```

## (三) 嵌套列表

```typescript
type NestedList = T | NestedList[];
interface Task { id: number; title: string; completed: boolean; }
const tasks: NestedList = [
    { id: 1, title: "项目A", completed: false },
    [{ id: 2, title: "子任务1", completed: true }, { id: 3, title: "子任务2", completed: false }],
    { id: 4, title: "项目B", completed: false }
];
```

## (四) 深度只读类型

```typescript
type DeepReadonly = T extends Function
    ? T
    : T extends object
        ? { readonly [P in keyof T]: DeepReadonly }
        : T;
interface User { name: string; profile: { email: string; address: { city: string; zip: string; } }; friends: User[]; }
const user: DeepReadonly = {
    name: "Alice",
    profile: { email: "alice@test.com", address: { city: "Beijing", zip: "100000" } },
    friends: []
};
// user.name = "Bob"; // Error: readonly
```

## (五) 深度可选类型

```typescript
type DeepPartial = T extends object
    ? { [P in keyof T]?: DeepPartial }
    : T;
interface AppConfig {
    database: { host: string; port: number; credentials: { username: string; password: string; }; };
    server: { port: number; ssl: boolean; };
}
const partialConfig: DeepPartial = { database: { host: "localhost" } };
```

## (六) 链式数据结构

```typescript
interface ListNode {
    value: T;
    next?: ListNode;
}
const linkedList: ListNodenumber> = {
    value: 1,
    next: { value: 2, next: { value: 3, next: { value: 4, next: undefined } } }
};
function traverseList(node: ListNode): void {
    let current: ListNode | undefined = node;
    const values: T[] = [];
    while (current) { values.push(current.value); current = current.next; }
    console.log("链表值: " + values.join(" -> "));
}
```

## (七) 联合类型的递归

```typescript
type JSONValue = string | number | boolean | null | JSONValue[] | { [key: string]: JSONValue };
const config: JSONValue = {
    name: "my-app",
    version: "1.0.0",
    settings: { debug: false, ports: [3000, 8080], metadata: { author: "Alice" } }
};
```

## (八) 注意事项

- 确保递归类型有终止条件，避免无限递归
- 递归通常与条件类型结合使用
- TypeScript 编译器对递归深度有限制
- 深度递归可能影响类型检查性能

## (九) 总结

- **自引用：**类型定义中引用自身
- **树形结构：**表达无限嵌套的数据
- **深度转换：**实现深度只读、深度可选等工具类型
- **链式结构：**表示链表等线性递归结构
