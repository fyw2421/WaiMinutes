---
title: "三十七、命名空间"
weight: 37
description: "本章介绍 命名空间。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

命名空间（Namespace）是 TypeScript 用来组织代码、避免命名冲突的一种方式。

## (一) 定义命名空间

### 1. 实例

```typescript
namespace StringUtils {
export function truncate(str: string, maxLength: number): string {
if (str.length <= maxLength) {
return str;
}
return str.substring(0, maxLength) + "...";
}
export function capitalize(str: string): string {
return str.charAt(0).toUpperCase() + str.slice(1);
}
}
console.log(StringUtils.truncate("Hello World", 5));
console.log(StringUtils.capitalize("typescript"));
```

## (二) 嵌套命名空间

### 1. 实例

```typescript
namespace Company {
export namespace HR {
export function hireEmployee(name: string): void {
console.log("雇佣员工: " + name);
}
}
export namespace IT {
export function assignComputer(employeeName: string): void {
console.log("为 " + employeeName + " 分配电脑");
}
}
}
Company.HR.hireEmployee("张三");
Company.IT.assignComputer("张三");
```

## (三) 命名空间别名

### 1. 实例

```typescript
namespace Shapes {
export namespace Polygons {
export class Triangle {
constructor() {
console.log("三角形已创建");
}
}
export class Square {
constructor() {
console.log("正方形已创建");
}
}
}
}
import polygons = Shapes.Polygons;
var tri = new polygons.Triangle();
var sq = new polygons.Square();
```

## (四) 命名空间分布

### 1. math.ts

```typescript
namespace MathUtils {
export function add(a: number, b: number): number {
return a + b;
}
}
```

### 2. math-advanced.ts

```typescript
namespace MathUtils {
export function multiply(a: number, b: number): number {
return a * b;
}
}
```

///

///

```typescript
console.log(MathUtils.add(5, 3));
console.log(MathUtils.multiply(5, 3));
```

## (五) 外部命名空间

### 1. 实例

```typescript
declare namespace D3 {
function select(selector: string): Selection;
interface Selection {
data(): any[];
enter(): Selection;
exit(): Selection;
}
}
```

## (六) 总结

- **定义：**使用 `namespace` 关键字
- **导出：**使用 `export` 关键字
- **嵌套：**命名空间可以嵌套
- **别名：**使用 `import` 创建别名
- **分布：**同名命名空间可以跨文件
- **声明：**`declare namespace` 声明外部类型
- **替代：**现代开发推荐使用 ES6 模块代替命名空间
