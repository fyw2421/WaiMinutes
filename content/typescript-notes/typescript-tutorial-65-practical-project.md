---
title: "六十五、综合项目实战"
weight: 65
description: "本章介绍 综合项目实战。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

本教程通过一个完整的项目案例，综合运用 TypeScript 的各种特性。

从项目搭建到实际开发，完整展示 TypeScript 在实际项目中的应用。

## (一) 项目目标

创建任务管理系统，包含任务创建、查询、更新、删除功能。

## (二) 项目结构

```
task-manager/
├── src/
│   ├── types/
│   │   ├── task.ts          # 任务类型
│   │   ├── api.ts           # API 类型
│   │   └── index.ts
│   ├── services/
│   │   ├── taskService.ts   # 任务服务
│   │   └── index.ts
│   ├── components/
│   │   ├── TaskList.tsx
│   │   ├── TaskItem.tsx
│   │   ├── TaskForm.tsx
│   │   └── index.ts
│   ├── hooks/
│   │   ├── useTasks.ts
│   │   └── index.ts
│   ├── App.tsx
│   ├── App.css
│   └── main.tsx
├── index.html
├── package.json
├── tsconfig.json
└── vite.config.ts
```

## (三) 类型定义

```typescript
// src/types/task.ts
export type TaskStatus = "pending" | "in-progress" | "completed";
export type TaskPriority = "low" | "medium" | "high";
export interface Task {
    id: string;
    title: string;
    description?: string;
    status: TaskStatus;
    priority: TaskPriority;
    createdAt: string;
    updatedAt: string;
    dueDate?: string;
    tags?: string[];
}
export interface CreateTaskInput {
    title: string;
    description?: string;
    priority: TaskPriority;
    dueDate?: string;
    tags?: string[];
}
export interface UpdateTaskInput {
    title?: string;
    description?: string;
    status?: TaskStatus;
    priority?: TaskPriority;
    dueDate?: string;
    tags?: string[];
}
export interface TaskFilter {
    status?: TaskStatus;
    priority?: TaskPriority;
    search?: string;
}
```

## (四) API 类型定义

```typescript
// src/types/api.ts
export interface ApiResponse {
    success: boolean;
    data?: T;
    error?: string;
    message?: string;
}
export interface PaginationMeta {
    total: number;
    page: number;
    pageSize: number;
    totalPages: number;
}
export interface PaginatedResponse {
    items: T[];
    meta: PaginationMeta;
}
export interface ApiError {
    code: string;
    message: string;
    details?: Recordstring, string>;
}
export type HttpMethod = "GET" | "POST" | "PUT" | "PATCH" | "DELETE";
```

## (五) 任务服务层

```typescript
// src/services/taskService.ts
import { Task, CreateTaskInput, UpdateTaskInput, TaskFilter, TaskStatus } from "../types/task";
function generateId(): string {
    return Date.now().toString(36) + Math.random().toString(36).substr(2);
}
let tasks: Task[] = [
    { id: "1", title: "学习 TypeScript", status: "completed", priority: "high", createdAt: new Date().toISOString(), updatedAt: new Date().toISOString(), tags: ["学习"] },
    { id: "2", title: "开发任务管理系统", status: "in-progress", priority: "high", createdAt: new Date().toISOString(), updatedAt: new Date().toISOString(), tags: ["项目"] }
];
class TaskService {
    getAll(filter?: TaskFilter): Task[] {
        let result = [...tasks];
        if (filter) {
            if (filter.status) result = result.filter(t => t.status === filter.status);
            if (filter.priority) result = result.filter(t => t.priority === filter.priority);
            if (filter.search) {
                const search = filter.search.toLowerCase();
                result = result.filter(t => t.title.toLowerCase().includes(search));
            }
        }
        return result;
    }
    getById(id: string): Task | undefined { return tasks.find(t => t.id === id); }
    create(input: CreateTaskInput): Task {
        const now = new Date().toISOString();
        const task: Task = { id: generateId(), title: input.title, description: input.description, status: "pending", priority: input.priority, createdAt: now, updatedAt: now, dueDate: input.dueDate, tags: input.tags };
        tasks.push(task);
        return task;
    }
    update(id: string, input: UpdateTaskInput): Task | null {
        const index = tasks.findIndex(t => t.id === id);
        if (index === -1) return null;
        const updated: Task = { ...tasks[index], ...input, updatedAt: new Date().toISOString() };
        tasks[index] = updated;
        return updated;
    }
    delete(id: string): boolean {
        const index = tasks.findIndex(t => t.id === id);
        if (index === -1) return false;
        tasks.splice(index, 1);
        return true;
    }
    updateStatus(id: string, status: TaskStatus): Task | null { return this.update(id, { status }); }
}
export const taskService = new TaskService();
```

## (六) 自定义 Hook (useTasks)

```typescript
import { useState, useEffect, useCallback } from "react";
import { Task, CreateTaskInput, UpdateTaskInput, TaskFilter, TaskStatus } from "../types/task";
import { taskService } from "../services/taskService";
interface UseTasksReturn {
    tasks: Task[];
    loading: boolean;
    error: string | null;
    filter: TaskFilter;
    createTask: (input: CreateTaskInput) => Promisevoid>;
    updateTask: (id: string, input: UpdateTaskInput) => Promisevoid>;
    deleteTask: (id: string) => Promisevoid>;
    updateStatus: (id: string, status: TaskStatus) => Promisevoid>;
    setFilter: (filter: TaskFilter) => void;
    refresh: () => void;
}
export function useTasks(): UseTasksReturn {
    const [tasks, setTasks] = useState([]);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useStatestring | null>(null);
    const [filter, setFilter] = useState({});
    const loadTasks = useCallback(() => {
        setLoading(true);
        try { setTasks(taskService.getAll(filter)); }
        catch (err) { setError(err instanceof Error ? err.message : "加载失败"); }
        finally { setLoading(false); }
    }, [filter]);
    useEffect(() => { loadTasks(); }, [loadTasks]);
    const createTask = useCallback(async (input: CreateTaskInput) => {
        try { taskService.create(input); loadTasks(); }
        catch (err) { setError(err instanceof Error ? err.message : "创建失败"); }
    }, [loadTasks]);
// ... updateTask, deleteTask, updateStatus follow similar pattern
    return { tasks, loading, error, filter, createTask, updateTask, deleteTask, updateStatus, setFilter, refresh: loadTasks };
}
```

## (七) 主应用组件

```typescript
// src/App.tsx
import React, { useState } from "react";
import { TaskList } from "./components/TaskList";
import { useTasks } from "./hooks/useTasks";
import { CreateTaskInput, TaskPriority } from "./types/task";
import "./App.css";
const App: React.FC = () => {
    const { createTask, error } = useTasks();
    return (
        "app">
            TypeScript 任务管理系统
            {error && "app-error">{error}}
            
                
                
            
        
    );
};
```

## (八) tsconfig.json

```json
{
    "compilerOptions": {
        "target": "ES2020",
        "useDefineForClassFields": true,
        "lib": ["ES2020", "DOM", "DOM.Iterable"],
        "module": "ESNext",
        "skipLibCheck": true,
        "moduleResolution": "bundler",
        "allowImportingTsExtensions": true,
        "resolveJsonModule": true,
        "isolatedModules": true,
        "noEmit": true,
        "jsx": "react-jsx",
        "strict": true,
        "noUnusedLocals": true,
        "noUnusedParameters": true,
        "noFallthroughCasesInSwitch": true
    },
    "include": ["src"]
}
```

## (九) 总结

- **类型定义：**Task、CreateTaskInput、TaskFilter 等接口
- **服务层：**TaskService 封装业务逻辑
- **自定义 Hook：**useTasks 管理状态
- **React 组件：**类型安全的组件开发
- **项目配置：**严格的 TypeScript 配置
