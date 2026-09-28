---
title: "九、编译选项"
weight: 9
description: "本章介绍 编译选项。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 编译器（tsc）有众多编译选项，本教程详细介绍常用的编译选项及其作用。

## (一) 输出控制选项

```json
{
    "compilerOptions": {
        // 输出目录
        "outDir": "./dist",
        // 源码根目录
        "rootDir": "./src",
        // 生成 .d.ts 声明文件
        "declaration": true,
        // 声明文件输出目录
        "declarationDir": "./types",
        // 生成 source map
        "sourceMap": true,
        // 生成 .js.map 文件
        "mapRoot": "./map"
    }
}
```

## (二) 类型检查选项

```json
{
    "compilerOptions": {
        // 严格模式（推荐始终开启）
        "strict": true,
        // 检查 null 和 undefined
        "strictNullChecks": true,
        // 检查 this 参数
        "noImplicitThis": true,
        // 严格函数类型
        "strictFunctionTypes": true,
        // 严格属性初始化
        "strictPropertyInitialization": true,
        // 不允许隐式 any
        "noImplicitAny": true,
        // 不允许返回 void
        "noImplicitReturns": true
    }
}
```

## (三) 模块选项

```json
{
    "compilerOptions": {
        // 模块系统
        "module": "commonjs",
        // 模块解析策略
        "moduleResolution": "node",
        // 解析基础路径
        "baseUrl": ".",
        // 路径别名
        "paths": {
            "@/*": ["src/*"]
        },
        // ES 模块互操作
        "esModuleInterop": true,
        // 允许默认导入
        "allowSyntheticDefaultImports": true,
        // 隔离模块
        "isolatedModules": true
    }
}
```

## (四) ES 特性选项

```json
{
    "compilerOptions": {
        // 编译目标
        "target": "ES2020",
        // 启用的库
        "lib": ["ES2020", "DOM"],
        // 允许未使用的局部变量
        "noUnusedLocals": true,
        // 允许未使用的参数
        "noUnusedParameters": true,
        // 代码降级
        "downlevelIteration": true
    }
}
```

## (五) 实验性选项

```json
{
    "compilerOptions": {
        // 启用装饰器
        "experimentalDecorators": true,
        // 启用装饰器元数据
        "emitDecoratorMetadata": true,
        // 跳过库检查
        "skipLibCheck": true
    }
}
```

## (六) 常用编译选项组合

```json
// Node.js 项目推荐配置
{
    "compilerOptions": {
        "target": "ES2020",
        "module": "commonjs",
        "lib": ["ES2020"],
        "outDir": "./dist",
        "rootDir": "./src",
        "strict": true,
        "esModuleInterop": true,
        "skipLibCheck": true,
        "forceConsistentCasingInFileNames": true,
        "moduleResolution": "node",
        "declaration": true
    }
}
```

## (七) 编译选项表

| 类别 | 常用选项 | 输出 |
|---|---|---|
| outDir, rootDir, declaration, sourceMap | 类型检查 | strict, strictNullChecks, noImplicitAny |
| 模块 | module, moduleResolution, paths, esModuleInterop | ES 特性 |
| target, lib, downlevelIteration | 实验性 | experimentalDecorators, emitDecoratorMetadata |

## (八) 总结

- **严格模式：**始终启用 strict: true
- **目标版本：**根据环境选择合适的 target
- **模块系统：**根据部署环境选择 module
- **类型声明：**库开发时启用 declaration
