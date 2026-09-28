---
title: "二十三、Map"
weight: 23
description: "本章介绍 Map。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

Map 对象保存键值对，并且能够记住键的原始插入顺序。

任何值(对象或者原始值) 都可以作为一个键或一个值。

Map 是 ES6 中引入的一种新的数据结构，可以参考 ES6 Map 与 Set。

## (一) 创建 Map

TypeScript 使用 Map 类型和 new 关键字来创建 Map：

```typescript
let myMap = new Map();
```

初始化 Map，可以以数组的格式来传入键值对：

```typescript
let myMap = new Map([
        ["key1", "value1"],
        ["key2", "value2"]
    ]); 
```

Map 相关的函数与属性：

- map.clear() – 移除 Map 对象的所有键/值对 。
- map.set() – 设置键值对，返回该 Map 对象。
- map.get() – 返回键对应的值，如果不存在，则返回 undefined。
- map.has() – 返回一个布尔值，用于判断 Map 中是否包含键对应的值。
- map.delete() – 删除 Map 中的元素，删除成功返回 true，失败返回 false。
- map.size – 返回 Map 对象键/值对的数量。
- map.keys() - 返回一个 Iterator 对象， 包含了 Map 对象中每个元素的键 。
- map.values() – 返回一个新的Iterator对象，包含了Map对象中每个元素的值 。
- 
map.entries() – 返回一个包含 Map 中所有键值对的迭代器 。

### 1. 常用函数

**set(key: K, value: V): this** - 向 Map 中添加或更新键值对。

```typescript
map.set('key1', 'value1');
```

**get(key: K): V | undefined** - 根据键获取值，如果键不存在则返回 undefined。

```typescript
const value = map.get('key1');
```

**has(key: K): boolean**- 检查 Map 中是否存在指定的键。

```typescript
const exists = map.has('key1');
```

**delete(key: K): boolean**- 删除指定键的键值对，成功删除返回 true，否则返回 false。

```typescript
const removed = map.delete('key1');
```

**clear(): void**- 清空 Map 中的所有键值对。

```typescript
map.clear();
```

**size: number** - 返回 Map 中键值对的数量。

```typescript
const size = map.size;
```

### 2. 迭代方法

keys(): IterableIterator - 返回一个包含 Map 中所有键的迭代器。

```typescript
for (const key of map.keys()) {
  console.log(key);
}
```

**values(): IterableIterator** - 返回一个包含 Map 中所有值的迭代器。

```typescript
for (const value of map.values()) {
  console.log(value);
}
```

**entries(): IterableIterator<[K, V]>** - 返回一个包含 Map 中所有键值对的迭代器，每个元素是一个 [key, value] 数组。

```typescript
for (const [key, value] of map.entries()) {
  console.log(key, value);
}
```

**forEach(callbackfn: (value: V, key: K, map: Map) => void, thisArg?: any): void** - 对 Map 中的每个键值对执行一次提供的回调函数。

```typescript
map.forEach((value, key) => {
  console.log(key, value);
});
```

### 3. 实例

使用 **es6** 编译：

```typescript
tsc --target es6 test.ts
```

编译以上代码得到如下 JavaScript 代码：

执行以上 JavaScript 代码，输出结果为：

```typescript
2
true
false
3
true
Map { 'Google' => 1, 'Taobao' => 3 }
Map {}
```

### 4. 迭代 Map

Map 对象中的元素是按顺序插入的，我们可以迭代 Map 对象，每一次迭代返回 [key, value] 数组。

TypeScript使用 for...of 来实现迭代：

使用 **es6** 编译：

```typescript
tsc --target es6 test.ts
```

编译以上代码得到如下 JavaScript 代码：

执行以上 JavaScript 代码，输出结果为：

```typescript
Google
Runoob
Taobao
1
2
3
Google 1
Runoob 2
Taobao 3
Google 1
Runoob 2
Taobao 3
```
