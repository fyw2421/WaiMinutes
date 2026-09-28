---
title: "六十四、性能优化"
weight: 64
description: "本章介绍 性能优化。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 项目的性能优化涉及编译速度、运行时性能和代码体积等多个方面。

本教程介绍常见的性能优化技巧，帮助构建更高效的 TypeScript 应用。

## (一) 为什么需要性能优化

TypeScript 虽然提供了强大的类型系统，但使用不当会影响编译速度和运行性能。

大型项目的编译可能需要数分钟，严重影响开发体验。

本教程介绍的配置和技巧可以显著提升 TypeScript 项目的性能。

> **优化目标：**更快的编译速度、更小的打包体积、更高的运行时性能。

## (二) 编译配置优化

```json
{
    "compilerOptions": {
        "incremental": true,
        "skipLibCheck": true,
        "noEmit": true,
        "assumeChangesOnlyAffectDirectDependencies": true,
        "parallel": true,
        "tsBuildInfoFile": ".tsbuildinfo",
        "exclude": ["node_modules", "dist", "build", "**/*.test.ts"]
    }
}
```

> **skipLibCheck：**最重要的优化选项，可以将编译时间减少 50% 以上。

## (三) 项目引用优化

```json
// packages/utils/tsconfig.json
{
    "extends": "../../tsconfig.base.json",
    "compilerOptions": {
        "composite": true,
        "outDir": "./dist",
        "declaration": true,
        "declarationMap": true
    },
    "include": ["src/**/*"],
    "exclude": ["node_modules", "dist"]
}
```

> **composite：**启用后，TypeScript 会生成 .tsbuildinfo 文件来加速后续编译。

## (四) 类型推断优化

```typescript
// 不好：过度标注
const name: string = "Alice";
const age: number = 25;
// 好：利用类型推断
const name = "Alice";
const age = 25;
// 函数返回值类型可以省略
function add(a: number, b: number) { return a + b; }
// 复杂对象使用类型推断
const user = { id: 1, name: "Bob", email: "bob@example.com" };
```

## (五) 避免使用 any

```typescript
// 不好：使用 any
function processData(data: any): any { return data.value; }
// 好：使用 unknown 或具体类型
function processDataextends { value: string }>(data: T): string { return data.value; }
// 使用 unknown
function parseJSON(json: string): unknown { return JSON.parse(json); }
// 使用时进行类型检查
const data = parseJSON('{"key": "value"}');
if (typeof data === "object" && data !== null) {
    const obj = data as { key: string };
}
```

## (六) 接口 vs 类型别名

```typescript
// 接口：适合对象类型，支持声明合并
interface User { id: number; name: string; }
interface User { email: string; }  // 合并
// 类型别名：适合联合类型、元组、函数类型
type ID = string | number;
type Status = "pending" | "success" | "error";
type Callback = (data: string) => void;
// 接口的编译速度通常比类型别名快
interface Point { x: number; y: number; }
```

## (七) 构建工具优化 (Vite)

```typescript
import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
export default defineConfig({
    plugins: [react()],
    build: {
        rollupOptions: {
            output: {
                manualChunks: {
'vendor': ['react', 'react-dom'],
'utils': ['lodash', 'axios']
                }
            }
        },
        minify: 'terser',
        sourcemap: false,
        chunkSizeWarningLimit: 500
    }
});
```

## (八) Tree Shaking

```typescript
// 使用具名导出，支持 Tree Shaking
export function add(a: number, b: number): number { return a + b; }
export function subtract(a: number, b: number): number { return a - b; }
// 避免默认导出（阻止 Tree Shaking）
// export default { add, subtract };
```

## (九) 注意事项

- skipLibCheck：生产环境必须开启
- 增量编译：开发环境建议开启
- 避免 any：使用 unknown 代替
- Tree Shaking：使用 ES 模块和具名导出

## (十) 总结

- **编译优化：**skipLibCheck、incremental、project references
- **类型优化：**利用推断、避免 any、选择合适的类型定义
- **构建优化：**代码分割、Tree Shaking、依赖优化
- **运行时优化：**类型安全、泛型约束
