---
title: "十九、async/await"
weight: 19
description: "本章介绍 async/await。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

```typescript
async/await 是 ES2017 引入的异步编程语法糖，让异步代码看起来像同步代码。
async/await 执行流程
```

Promise 方式

```typescript
fetchData()
.then(result => {
console.log(result);
})
async/await 方式
async function main() {
const result = await fetchData();
console.log(result);
}
await 执行顺序
```

主线程

await

暂停

Promise

后台执行

恢复

完成

优势对比

✓ 代码更简洁

✓ 同步风格

✓ 更好的错误堆栈

✓ 易于调试

✓ try/catch 处理

上图展示了 async/await 相比传统 Promise 的优势：代码更简洁，执行流程更清晰。

## (一) Promise 基础

Promise 代表一个异步操作的最终结果。

**运行结果：**

```typescript
成功: 操作成功
```

## (二) async 函数

使用 async 关键字声明异步函数。

**运行结果：**

```typescript
结果: Hello, World!
数据: {"name":"Alice","age":25}
```

## (三) await 关键字

await 等待 Promise 完成并获取结果。

**运行结果：**

```typescript
开始...
结果: 完成!
结束
```

## (四) 错误处理

使用 try/catch 处理异步错误。

**运行结果：**

```typescript
捕获错误: 操作失败
```

## (五) 并行执行

使用 Promise.all 并行执行多个异步操作。

**运行结果：**

```typescript
串行完成: User1, User2
并行完成: User1, User2
```

## (六) async/await 相比 Promise 的优势

- 代码更简洁、更易读
- 同步代码风格
- 更好的错误堆栈
- 易于调试

## (七) 总结

- **async：**声明异步函数
- **await：**等待 Promise
- **错误处理：**try/catch
- **并行：**Promise.all
