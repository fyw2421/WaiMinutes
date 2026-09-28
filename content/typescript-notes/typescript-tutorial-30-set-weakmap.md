---
title: "三十、Set / WeakMap"
weight: 30
description: "本章介绍 Set / WeakMap。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 继承自 JavaScript 的 Set 和 WeakMap 数据结构，提供了更强大的类型支持。

## (一) Set

### 1. 实例

```typescript
var numbers = new Set();
numbers.add(1);
numbers.add(2);
numbers.add(3);
```

numbers.add(1); // 重复值会被忽略

```typescript
console.log("Set 大小: " + numbers.size);
console.log("是否包含 2: " + numbers.has(2));
numbers.forEach(function(value) {
console.log("值: " + value);
});
var arr = Array.from(numbers);
console.log("转换为数组: " + arr);
```

## (二) Set 类型注解

### 1. 实例

```typescript
var stringSet: Set= new Set();
stringSet.add("a");
stringSet.add("b");
interface Person { name: string; }
var personSet: Set= new Set();
personSet.add({ name: "Alice" });
personSet.add({ name: "Bob" });
```

## (三) WeakSet

### 1. 实例

```typescript
var weakSet = new WeakSet();
var obj1 = { name: "Alice" };
var obj2 = { name: "Bob" };
weakSet.add(obj1);
weakSet.add(obj2);
console.log("是否包含 obj1: " + weakSet.has(obj1));
weakSet.delete(obj1);
console.log("删除后是否包含 obj1: " + weakSet.has(obj1));
```

## (四) Map

### 1. 实例

```typescript
var map = new Map();
map.set("one", 1);
map.set("two", 2);
map.set("three", 3);
console.log("获取 two: " + map.get("two"));
console.log("Map 大小: " + map.size);
console.log("是否包含 three: " + map.has("three"));
map.forEach(function(value, key) {
console.log(key + ": " + value);
});
console.log("转换为数组: " + Array.from(map.entries()));
```

## (五) WeakMap

### 1. 实例

```typescript
var weakMap = new WeakMap();
var keyObj = { id: 1 };
weakMap.set(keyObj, "value1");
console.log("获取值: " + weakMap.get(keyObj));
console.log("是否包含: " + weakMap.has(keyObj));
weakMap.delete(keyObj);
console.log("删除后: " + weakMap.has(keyObj));
```

## (六) 实际应用场景

### 1. 实例

```typescript
function countElements(arr: string[]): Map{
var counts = new Map();
for (var _i = 0, arr_1 = arr; _i < arr_1.length; _i++) {
var item = arr_1[_i];
var currentCount = counts.get(item) || 0;
counts.set(item, currentCount + 1);
}
return counts;
}
var fruits = ["apple", "banana", "apple", "orange", "banana", "apple"];
var result = countElements(fruits);
result.forEach(function(count, fruit) {
console.log(fruit + ": " + count);
});
```

## (七) 总结

- **Set：**值的集合，值唯一，自动去重
- **WeakSet：**对象弱引用集合，不可遍历
- **Map：**键值对集合，键可以是任意类型
- **WeakMap：**键弱引用，不可遍历，适用于缓存和私有数据
