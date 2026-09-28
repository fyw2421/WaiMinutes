---
title: "五十四、模板字面量"
weight: 54
description: "本章介绍 模板字面量。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

模板字面量类型基于字符串字面量类型构建，支持通过插值生成新的字符串类型。

## (一) 基本语法

```typescript
type World = "world";
type Greeting = \`Hello \${World}\`;  // "Hello world"
var greeting: Greeting = "Hello world";
```

## (二) 内置工具类型

```typescript
type UpperHello = Uppercase"hello">;    // "HELLO"
type LowerHELLO = Lowercase"HELLO">;    // "hello"
type CapitalizedHello = Capitalize"hello">;  // "Hello"
type UncapitalizedHello = Uncapitalize"Hello">;  // "hello"
```

## (三) 事件类型

```typescript
type EventName = \`on\${Capitalize}\`;
type Handler = \`handle\${Capitalize}\`;
var clickEvent: EventName = "onClick";
var handler: Handler = "handleSubmit";
```

## (四) 路径类型

```typescript
type HttpMethod = "get" | "post" | "put" | "delete";
type ApiEndpoint = \`/\${string}\`;
type ApiPath = \`\${HttpMethod}\${ApiEndpoint}\`;
var getUsers: ApiPath = "/get/users";
```

## (五) 复杂示例

```typescript
type Variant = "primary" | "secondary";
type Size = "sm" | "md" | "lg";
type ClassName = \`btn-\${Variant}-\${Size}\`;
// 生成 6 种组合
```

## (六) 自定义工具类型

```typescript
type Prefixextends string, P extends string> = \`\${P}\${Capitalize}\`;
type Suffix = \`\${Capitalize}\${S}\`;
type HandlerName = Prefix;  // "onClick"
```

## (七) 应用场景

- 事件名（onClick、onFocus）
- API 路径（get:/users）
- CSS 类名（btn-primary-md）
- 自定义工具类型
