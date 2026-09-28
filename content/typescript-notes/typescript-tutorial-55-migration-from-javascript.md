---
title: "五十五、从 JavaScript 迁移"
weight: 55
description: "本章介绍 从 JavaScript 迁移。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

将现有 JavaScript 项目逐步迁移到 TypeScript。

## (一) 迁移策略

1. 添加 tsconfig.json

2. 重命名 .js 为 .ts

3. 逐步添加类型注解

4. 启用严格模式

## (二) 配置 tsconfig.json

```json
{
    "compilerOptions": {
        "target": "ES2020",
        "module": "commonjs",
        "strict": false,
        "noImplicitAny": false,
        "strictNullChecks": false,
        "skipLibCheck": true,
        "allowJs": true,
        "checkJs": false,
        "outDir": "./dist",
        "rootDir": "./src"
    },
    "include": ["src/**/*"],
    "exclude": ["node_modules", "dist"]
}
```

## (三) 逐步启用严格检查

```json
// 阶段 1: 基础迁移
{ "compilerOptions": { "strict": false, "noImplicitAny": false } }
// 阶段 2: 启用类型检查
{ "compilerOptions": { "strict": true, "noImplicitAny": true, "strictNullChecks": true } }
// 阶段 3: 完全严格
{ "compilerOptions": { "strict": true, "noImplicitAny": true, "strictNullChecks": true, "strictFunctionTypes": true, "strictPropertyInitialization": true } }
```

## (四) JSDoc 类型注释

```javascript
/**
@param {number} a
@param {number} b
@returns {number}
 */
function add(a, b) { return a + b; }
/**
@typedef {Object} User
@property {number} id
@property {string} name
@property {string} email
 */
/**
@param {number} id
@returns {Promise<User>}
 */
function getUser(id) { return fetch(/api/users/).then(r => r.json()); }
```

## (五) 类型声明文件

```typescript
declare module "my-module" {
    export function doSomething(param: string): void;
    export class MyClass {
        constructor(options: { name: string });
        name: string;
    }
}
```

## (六) declare 关键字

```typescript
declare var GLOBAL_CONFIG: { apiUrl: string; version: string };
declare function myFunction(param: string): void;
declare namespace MyNamespace { function doSomething(): void; }
console.log(GLOBAL_CONFIG.apiUrl);
myFunction("hello");
MyNamespace.doSomething();
```

## (七) 迁移工具

- **tsc --allowJs：**编译 JS 文件
- **checkJs：**检查 JS 类型
- **// @ts-check：**单文件类型检查
- **// @ts-ignore：**忽略错误

## (八) 最佳实践

1. 从关键模块开始迁移

2. 添加单元测试

3. 逐步启用严格模式

4. 使用 JSDoc 注释

5. 创建类型声明文件

## (九) 总结

- **渐进式：**逐步迁移
- **JSDoc：**类型注释
- **声明文件：**.d.ts
- **严格模式：**分阶段启用
