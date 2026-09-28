---
title: "六十一、路径映射"
weight: 61
description: "本章介绍 路径映射。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

paths 是 tsconfig.json 中的配置选项，用于配置模块路径别名。

它可以让导入路径更简洁，同时保持代码的组织结构清晰。

## (一) 为什么需要路径映射

随着项目规模增长，文件目录会越来越深。

使用相对路径导入文件（如 ../../../components/Button）会让代码难以阅读和维护。

路径映射允许我们使用别名（如 @/components/Button）来替代冗长的相对路径。

> **别名：**路径别名让导入路径更简洁，同时便于调整项目结构。

## (二) 基本配置

```json
{
    "compilerOptions": {
        "baseUrl": ".",
        "paths": {
            "@/*": ["src/*"],
            "@components/*": ["src/components/*"],
            "@utils/*": ["src/utils/*"],
            "@services/*": ["src/services/*"],
            "@assets/*": ["src/assets/*"],
            "@types/*": ["src/types/*"]
        }
    }
}
```

> **baseUrl：**设置 baseUrl 后，paths 中的路径将相对于此目录解析。

## (三) 使用路径别名

```typescript
import { Button } from '@/components/Button';
import { User } from '@types/user';
import { fetchUser } from '@services/userApi';
import { formatDate } from '@utils/date';
import styles from '@/components/Button.module.css';
import logo from '@assets/logo.png';
```

## (四) Webpack 别名配置

```javascript
const path = require('path');
module.exports = {
    resolve: {
        alias: {
            '@': path.resolve(__dirname, 'src'),
            '@components': path.resolve(__dirname, 'src/components'),
            '@utils': path.resolve(__dirname, 'src/utils'),
            '@services': path.resolve(__dirname, 'src/services'),
            '@types': path.resolve(__dirname, 'src/types'),
            '@assets': path.resolve(__dirname, 'src/assets')
        },
        extensions: ['.ts', '.tsx', '.js', '.jsx', '.json']
    }
};
```

## (五) Vite 配置

```typescript
import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import path from 'path';
export default defineConfig({
    plugins: [react()],
    resolve: {
        alias: {
'@': path.resolve(__dirname, './src'),
'@components': path.resolve(__dirname, './src/components'),
'@utils': path.resolve(__dirname, './src/utils'),
'@services': path.resolve(__dirname, './src/services'),
'@types': path.resolve(__dirname, './src/types'),
'@assets': path.resolve(__dirname, './src/assets')
        }
    }
});
```

## (六) 多项目路径配置 (Monorepo)

```json
{
    "compilerOptions": {
        "baseUrl": ".",
        "paths": {
            "@/*": ["src/*"],
            "@my-ui/button": ["packages/ui-button/src/index.ts"],
            "@my-ui/modal": ["packages/ui-modal/src/index.ts"],
            "@my-utils/date": ["packages/utils-date/src/index.ts"],
            "@my-hooks/useFetch": ["packages/hooks-use-fetch/src/index.ts"]
        }
    }
}
```

## (七) 注意事项

- baseUrl 必需：paths 需要与 baseUrl 配合使用
- 通配符匹配：使用 * 匹配任意路径
- 保持一致：Webpack/Vite 配置必须与 tsconfig 保持一致
- 相对路径：paths 中的路径是相对于 baseUrl 的

## (八) 总结

- **paths：**配置路径别名
- **baseUrl：**设置基准路径
- **通配符：**使用 * 匹配任意字符
- **构建工具：**Webpack/Vite 需要同步配置
