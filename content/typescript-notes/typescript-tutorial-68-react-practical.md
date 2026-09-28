---
title: "六十八、React 实战"
weight: 68
description: "本章介绍 React 实战。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 为 React 开发提供完整的类型支持，提高代码可靠性。

使用 TypeScript 可以让 React 组件、Props、State 等都有完整的类型检查，减少运行时错误。

## (一) 为什么需要在 React 中使用 TypeScript

React 组件化开发会产生大量的 Props、State、Context 等数据流。

没有类型定义时，很难追踪数据的来源和结构，容易出现运行时错误。

TypeScript 为 React 提供了完整的类型系统：组件 Props 有类型检查、useState 有类型推断、事件处理有类型提示。

> **数据流：**React 中的数据流（Props、State、Context）都需要类型定义。

## (二) 创建项目

```bash
npx create-react-app my-app --template typescript
npm create vite@latest my-app -- --template react-ts
```

## (三) 组件类型

```typescript
import React from "react";
interface ButtonProps {
    text: string;
    onClick: () => void;
    disabled?: boolean;
    variant?: "primary" | "secondary";
}
const Button: React.FC = ({
    text, onClick, disabled = false, variant = "primary"
}) => {
    return (
        
            {text}
        
    );
};
export default Button;
```

## (四) Props 类型

```typescript
import React from "react";
interface User { id: number; name: string; email: string; avatar?: string; }
interface UserCardProps {
    user: User;
    onEdit: (user: User) => void;
    onDelete: (id: number) => void;
}
const UserCard: React.FC = ({ user, onEdit, onDelete }) => {
    return (
        "user-card">
            {user.avatar && }
            {user.name}
            {user.email}
             onEdit(user)}>编辑
             onDelete(user.id)}>删除
        
    );
};
```

## (五) useState 类型

```typescript
import React, { useState } from "react";
const Counter: React.FC = () => {
    const [count, setCount] = useStatenumber>(0);
    const [user, setUser] = useStatestring; age: number }>({ name: "Alice", age: 25 });
    return (
        
            计数: {count}
             setCount(c => c + 1)}>+1
            用户: {user.name}, {user.age}
             setUser({ ...user, age: user.age + 1 })}>年龄+1
        
    );
};
```

## (六) useEffect 类型

```typescript
import React, { useState, useEffect } from "react";
interface User { id: number; name: string; }
const DataFetcher: React.FC = () => {
    const [users, setUsers] = useState([]);
    const [loading, setLoading] = useStateboolean>(true);
    const [error, setError] = useStatestring | null>(null);
    useEffect(() => {
        fetch("/api/users")
            .then(res => res.json())
            .then(data => { setUsers(data); setLoading(false); })
            .catch(err => { setError(err.message); setLoading(false); });
    }, []);
    if (loading) return 加载中...;
    if (error) return 错误: {error};
    return ({users.map(user => {user.name})});
};
```

## (七) 事件处理

```typescript
import React, { useState } from "react";
const Form: React.FC = () => {
    const [name, setName] = useStatestring>("");
    const handleSubmit = (e: React.FormEvent) => {
        e.preventDefault();
        console.log("提交:", name);
    };
    const handleChange = (e: React.ChangeEvent) => {
        setName(e.target.value);
    };
    return (
        
            type="text" value={name} onChange={handleChange} placeholder="输入名字" />
            type="submit">提交
        
    );
};
```

## (八) 注意事项

- React.FC：推荐使用，可获得完整的类型支持
- Props 接口：为每个组件定义 Props 类型
- useState 泛型：复杂类型需要显式指定
- 事件类型：使用 React 提供的事件类型

## (九) 总结

- **React.FC：**React 函数组件的标准类型
- **Props：**使用 interface 定义组件属性
- **useState：**使用泛型参数指定状态类型
- **useEffect：**完整的类型支持
- **事件：**使用 React 事件类型避免 any

来源：[菜鸟教程 TypeScript 教程](https://www.runoob.com/typescript/ts-tutorial.html)

↑
