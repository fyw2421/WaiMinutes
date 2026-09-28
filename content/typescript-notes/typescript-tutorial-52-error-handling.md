---
title: "五十二、错误处理"
weight: 52
description: "本章介绍 错误处理。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

## (一) 自定义错误类型

```typescript
class AppError extends Error {
    code: string;
    constructor(message: string, code: string) {
        super(message);
        this.name = "AppError";
        this.code = code;
    }
}
function divide(a: number, b: number): number {
    if (b === 0) {
        throw new AppError("Cannot divide by zero", "DIVIDE_BY_ZERO");
    }
    return a / b;
}
```

## (二) Result 类型避免异常

```typescript
type Result =
    | { ok: true; value: T }
    | { ok: false; error: E };

function safeDivide(a: number, b: number): Resultnumber, string> {
    if (b === 0) {
        return { ok: false, error: "Cannot divide by zero" };
    }
    return { ok: true, value: a / b };
}

var result = safeDivide(10, 2);
if (result.ok) {
    console.log("结果: " + result.value);
} else {
    console.log("错误: " + result.error);
}
```

## (三) Async 函数错误处理

```typescript
async function fetchUser(id: number): Promise> {
    try {
        var response = await fetch("/api/users/" + id);
        var user = await response.json();
        return { ok: true, value: user };
    } catch (error) {
        return { ok: false, error: error as Error };
    }
}
```

## (四) 通用错误处理封装

```typescript
async function withErrorHandling(
    fn: () => Promise
): Promise> {
    try {
        var data = await fn();
        return { ok: true, value: data };
    } catch (error) {
        return { ok: false, error: error as Error };
    }
}
```

## (五) 注意事项

- 不要使用空的 catch 块忽略错误
- 尽量使用具体的错误类型
- 程序错误用异常，业务错误用 Result
