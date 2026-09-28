---
title: "八、tsconfig.json"
weight: 8
description: "本章介绍 tsconfig.json。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

tsconfig.json 是 TypeScript 项目的配置文件，用于指定编译选项和项目设置。

## (一) 基本配置

最基础的 tsconfig.json 文件：

```json
{
    "compilerOptions": {
        "target": "ES2020",
        "module": "commonjs",
        "strict": true,
        "outDir": "./dist"
    },
    "include": ["src/**/*"],
    "exclude": ["node_modules", "dist"]
}
```

配置说明：

- **target：**编译目标 JavaScript 版本
- **module：**使用的模块系统
- **strict：**启用所有严格类型检查
- **outDir：**输出目录

## (二) 编译目标版本

使用 target 指定编译到的 JavaScript 版本。

```json
{
    "compilerOptions": {
        "target": "ES2020"
    }
}
```

不同目标的输出差异：

```javascript
// target: ES5 - 使用 var
var greeting = "Hello";
// target: ES2020 - 使用 let/const
let greeting = "Hello";
```

## (三) 模块系统

配置模块化方案。

```json
{
    "compilerOptions": {
        "module": "commonjs"
    }
}
```

可选值: none, commonjs, amd, umd, es6, es2020, esnext

## (四) 严格模式

strict 选项启用所有严格类型检查。

```json
{
    "compilerOptions": {
        "strict": true
    }
}
```

> **建议：**始终启用 strict: true，这是最佳实践。

## (五) 路径别名

配置路径别名简化导入。

```json
{
    "compilerOptions": {
        "baseUrl": ".",
        "paths": {
            "@/*": ["src/*"],
            "@components/*": ["src/components/*"]
        }
    }
}
```

使用方式：

```typescript
import Button from "@components/Button";
import { Header } from "@/components";
```

## (六) 文件包含与排除

控制哪些文件包含在编译中。

```json
{
    "include": ["src/**/*"],
    "exclude": [
        "node_modules",
        "dist",
        "**/*.test.ts",
        "**/*.spec.ts"
    ]
}
```

## (七) 常用编译选项一览

| 选项 | 说明 | target |
|---|---|---|
| 编译目标版本 | module | 模块系统 |
| strict | 严格模式 | outDir |
| 输出目录 | rootDir | 源码根目录 |
| esModuleInterop | 允许 ES 模块互操作 | skipLibCheck |
| 跳过库检查 | declaration | 生成 .d.ts 声明文件 |

## (八) 总结

- **tsconfig.json：**TypeScript 项目配置文件
- **compilerOptions：**编译选项核心配置
- **include/exclude：**控制文件范围
- **路径别名：**简化导入路径
- **strict: true：**始终启用严格模式
