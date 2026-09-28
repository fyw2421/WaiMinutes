---
title: "六十六、Node.js 实战"
weight: 66
description: "本章介绍 Node.js 实战。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 在 Node.js 后端开发中广泛应用，本教程介绍 Node.js 项目的 TypeScript 配置和使用。

使用 TypeScript 可以让 Node.js 代码更安全、更易维护，特别适合中大型后端项目。

## (一) 为什么需要在 Node.js 中使用 TypeScript

Node.js 项目通常涉及复杂的业务逻辑和数据处理，代码行数会快速增长。

使用 TypeScript 可以：提供编译期类型检查，减少运行时错误；利用 IDE 智能提示，提高开发效率；代码更易读和维护。

> **优势：**TypeScript 的静态类型检查可以在开发时发现潜在问题。

## (二) 项目初始化

`ash

npm init -y

npm install -D typescript @types/node ts-node nodemon

npx tsc --init

> **@types/node** 提供了 Node.js API 的类型定义，**ts-node** 可以直接运行 TypeScript 文件。

## (三) tsconfig.json 配置

```json
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
    },
    "include": ["src/**/*"],
    "exclude": ["node_modules", "dist"]
}
```

## (四) 定义类型

```typescript
// src/types/index.ts
export interface User {
    id: number;
    name: string;
    email: string;
    createdAt: Date;
}
export interface CreateUserDTO {
    name: string;
    email: string;
    password: string;
}
export interface ApiResponse {
    success: boolean;
    data?: T;
    error?: string;
}
```

## (五) 实现服务

```typescript
// src/services/userService.ts
import { User, CreateUserDTO, ApiResponse } from "../types";
class UserService {
    private users: User[] = [];
    private nextId = 1;
    createUser(dto: CreateUserDTO): ApiResponse {
        try {
            const user: User = { id: this.nextId++, name: dto.name, email: dto.email, createdAt: new Date() };
            this.users.push(user);
            return { success: true, data: user };
        } catch (error) {
            return { success: false, error: "创建用户失败" };
        }
    }
    getUser(id: number): ApiResponse {
        const user = this.users.find(u => u.id === id);
        return user ? { success: true, data: user } : { success: false, error: "用户不存在" };
    }
    getAllUsers(): ApiResponse {
        return { success: true, data: this.users };
    }
}
export default new UserService();
```

## (六) 创建 API 路由

```typescript
import express, { Request, Response } from "express";
import userService from "./services/userService";
const app = express();
app.use(express.json());
app.get("/api/users", (req: Request, res: Response) => {
    res.json(userService.getAllUsers());
});
app.get("/api/users/:id", (req: Request, res: Response) => {
    const id = parseInt(req.params.id);
    res.json(userService.getUser(id));
});
app.post("/api/users", (req: Request, res: Response) => {
    res.json(userService.createUser(req.body));
});
const PORT = 3000;
app.listen(PORT, () => { console.log(服务器运行在 http://localhost:); });
```

## (七) package.json 脚本

```json
{
    "scripts": {
        "build": "tsc",
        "start": "node dist/index.js",
        "dev": "nodemon --exec ts-node src/index.ts",
        "test": "jest"
    }
}
```

## (八) 注意事项

- 严格模式：始终启用 strict: true
- 模块选择：Node.js 项目使用 commonjs
- 类型定义：安装 @types/node 获取 API 类型
- 开发工具：使用 ts-node 实现热重载

## (九) 总结

- **项目配置：**使用 tsconfig.json 配置编译选项
- **类型定义：**在 types 目录集中管理接口
- **服务层：**业务逻辑与路由分离
- **路由：**使用 Express 创建 RESTful API
- **开发工具：**ts-node、nodemon 提高开发效率
