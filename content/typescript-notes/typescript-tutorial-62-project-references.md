---
title: "六十二、项目引用"
weight: 62
description: "本章介绍 项目引用。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

项目引用是 TypeScript 提供的组织大型项目的功能，允许将 TypeScript 项目拆分为更小的部分。

它可以实现增量构建、更好的代码组织和更快的编译速度。

## (一) 为什么需要项目引用

随着项目增长，单一的 tsconfig.json 会导致编译速度变慢。

项目引用允许将项目拆分为独立的子项目，每个子项目可以独立编译。

这不仅提高了编译速度，还提供了更好的代码组织方式。

> **概念：**项目引用允许一个 TypeScript 项目引用其他项目，实现增量编译和更好的代码组织。

## (二) 创建引用项目

```json
// packages/utils/tsconfig.json
{
    "extends": "../../tsconfig.base.json",
    "compilerOptions": {
        "outDir": "./dist",
        "declarationDir": "./dist/types",
        "declaration": true,
        "sourceMap": true,
        "composite": true
    },
    "include": ["src/**/*"],
    "exclude": ["node_modules", "dist", "**/*.test.ts"]
}
```

> **composite：**设置为 true 启用项目引用功能。

## (三) 主项目配置

```json
{
    "extends": "./tsconfig.base.json",
    "compilerOptions": {
        "outDir": "./dist",
        "declarationDir": "./dist/types",
        "declaration": true,
        "sourceMap": true
    },
    "references": [
        { "path": "./packages/utils" },
        { "path": "./packages/ui" },
        { "path": "./packages/types" }
    ],
    "include": ["src/**/*"]
}
```

## (四) 类型引用

```typescript
import { formatDate, formatCurrency } from '@my-utils/format';
import { Button, Modal, Input } from '@my-ui/core';
import { User, ApiResponse } from '@my-types/common';
const user: User = { id: 1, name: "Alice", email: "alice@example.com" };
const dateStr = formatDate(new Date(), "YYYY-MM-DD");
const price = formatCurrency(999);
```

## (五) 增量构建

`ash

## (六) 构建整个项目

npm run build

## (七) 只构建主项目

npm run build -- --build

## (八) 增量构建

npx tsc -b packages/utils

npx tsc -b packages/ui

npx tsc -b .

## (九) 注意事项

被引用的项目必须设置 composite: true 每个项目需要独立的输出目录 被引用项目需要生成声明文件 依赖的项目需要先构建

## (十) 总结

**references：**配置项目引用关系 **composite：**启用项目引用 **增量构建：**只编译修改的部分

- **代码组织：**拆分为独立模块
