---
title: "六十三、Monorepo 配置"
weight: 63
description: "本章介绍 Monorepo 配置。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

## (一) 为什么需要 Monorepo

当项目包含多个包（如工具库、组件库、应用程序）时，传统方式需要维护多个代码仓库。 Monorepo 将所有包放在同一个仓库中，共享代码更方便，版本管理更统一。 TypeScript 的项目引用功能让 Monorepo 项目的类型检查和构建更高效。

> **概念：**Monorepo（单一仓库）将多个相关项目放在同一个代码仓库中。

## (二) pnpm Workspace

```json
// package.json (根目录)
{
    "name": "my-monorepo",
    "version": "1.0.0",
    "private": true,
    "packages": ["packages/*"],
    "devDependencies": { "typescript": "^5.0.0" },
    "scripts": {
        "build": "pnpm -r run build",
        "clean": "pnpm -r run clean",
        "type-check": "pnpm -r run type-check"
    }
}
```

## (三) 项目结构

```
my-monorepo/
├── packages/
│   ├── utils/              # 工具包
│   │   ├── src/index.ts
│   │   ├── package.json
│   │   └── tsconfig.json
│   ├── ui-components/      # UI 组件包
│   │   ├── src/
│   │   ├── package.json
│   │   └── tsconfig.json
│   └── app/                # 应用程序
│       ├── src/index.tsx
│       ├── package.json
│       └── tsconfig.json
├── package.json
├── tsconfig.base.json
└── pnpm-workspace.yaml
```

## (四) 基础 TypeScript 配置

```json
// tsconfig.base.json
{
    "compilerOptions": {
        "target": "ES2020",
        "module": "ESNext",
        "strict": true,
        "skipLibCheck": true,
        "esModuleInterop": true,
        "forceConsistentCasingInFileNames": true,
        "moduleResolution": "bundler",
        "resolveJsonModule": true,
        "isolatedModules": true,
        "noEmit": true
    }
}
```

## (五) 工具包配置

```json
// packages/utils/tsconfig.json
{
    "extends": "../../tsconfig.base.json",
    "compilerOptions": {
        "outDir": "./dist",
        "declarationDir": "./dist/types",
        "declaration": true,
        "declarationMap": true,
        "module": "ESNext",
        "composite": true
    },
    "include": ["src/**/*"],
    "exclude": ["node_modules", "dist", "**/*.test.ts"]
}
```

## (六) 应用程序配置

```json
// packages/app/tsconfig.json
{
    "extends": "../../tsconfig.base.json",
    "compilerOptions": {
        "outDir": "./dist",
        "jsx": "react-jsx",
        "baseUrl": ".",
        "paths": {
            "@my-utils/*": ["../utils/src/*"],
            "@my-ui/*": ["../ui-components/src/*"]
        }
    },
    "references": [
        { "path": "../utils" },
        { "path": "../ui-components" }
    ],
    "include": ["src/**/*"],
    "exclude": ["node_modules", "dist"]
}
```

## (七) 包之间的依赖

```json
// packages/app/package.json
{
    "name": "@my-org/app",
    "dependencies": {
        "@my-org/utils": "workspace:*",
        "@my-org/ui-components": "workspace:*",
        "react": "^18.2.0"
    },
    "devDependencies": {
        "@types/react": "^18.2.0",
        "typescript": "^5.0.0"
    }
}
```

## (八) 注意事项

- 包命名规范：使用 @org-name/package 格式
- 每个包可以独立版本管理
- 使用 workspace:* 引用同仓库包
- 被依赖的包需要先构建

## (九) 总结

- **pnpm Workspace：**原生支持 Monorepo
- **项目引用：**实现增量编译
- **路径别名：**便捷引用同仓库包
- **统一管理：**共享配置和依赖
