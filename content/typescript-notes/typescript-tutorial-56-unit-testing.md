---
title: "五十六、单元测试"
weight: 56
description: "本章介绍 单元测试。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 项目中的单元测试实践，确保代码质量。

单元测试可以验证代码的正确性，TypeScript 的类型系统与测试框架完美结合，可以编写类型安全的测试代码。

## (一) 为什么需要单元测试

单元测试是保证代码质量的重要手段，它可以验证代码的正确性，防止 bug 出现。

TypeScript 项目中，测试代码同样受益于类型系统：类型错误会在编译时被发现，IDE 提供智能提示，测试代码更可靠。

> **质量保障：**单元测试可以快速发现回归问题，确保代码改动不会破坏现有功能。

## (二) 测试框架配置

Jest 是 TypeScript 项目最流行的测试框架。

`ash

npm install --save-dev jest ts-jest @types/jest

npx ts-jest config:init

## (三) 配置 jest.config.js

```javascript
module.exports = {
    preset: 'ts-jest',
    testEnvironment: 'node',
    roots: ['<rootDir>/src'],
    testMatch: ['**/__tests__/**/*.ts'],
    moduleFileExtensions: ['ts', 'js', 'json'],
    collectCoverageFrom: [
        'src/**/*.ts',
        '!src/**/*.d.ts'
    ]
};
```

## (四) 测试函数

```typescript
// src/utils/calculator.ts
export class Calculator {
    add(a: number, b: number): number { return a + b; }
    subtract(a: number, b: number): number { return a - b; }
    multiply(a: number, b: number): number { return a * b; }
    divide(a: number, b: number): number {
        if (b === 0) throw new Error("Cannot divide by zero");
        return a / b;
    }
}
```

```typescript
// src/utils/calculator.test.ts
import { Calculator } from "./calculator";
describe("Calculator", () => {
    let calculator: Calculator;
    beforeEach(() => { calculator = new Calculator(); });
    describe("add", () => {
        it("should add two numbers", () => { expect(calculator.add(2, 3)).toBe(5); });
        it("should handle negative numbers", () => { expect(calculator.add(-1, 1)).toBe(0); });
    });
    describe("divide", () => {
        it("should divide two numbers", () => { expect(calculator.divide(10, 2)).toBe(5); });
        it("should throw error when dividing by zero", () => { expect(() => calculator.divide(10, 0)).toThrow(); });
    });
});
```

## (五) 测试 Service

```typescript
// src/services/userService.ts
export interface User { id: number; name: string; }
export class UserService {
    private users: User[] = [];
    private nextId = 1;
    createUser(name: string): User {
        const user = { id: this.nextId++, name };
        this.users.push(user);
        return user;
    }
    getUser(id: number): User | undefined {
        return this.users.find(u => u.id === id);
    }
    getAllUsers(): User[] { return [...this.users]; }
}
```

## (六) Mock

```typescript
// Mock 函数
const mockCallback = jest.fn(x => x * 2);
[1, 2, 3].forEach(mockCallback);
expect(mockCallback).toHaveBeenCalledTimes(3);
expect(mockCallback).toHaveBeenCalledWith(2);
// Mock 模块
jest.mock("./api", () => ({
    fetchUser: jest.fn(() => Promise.resolve({ id: 1, name: "Alice" }))
}));
```

## (七) 注意事项

- 测试文件放在 **tests** 目录或使用 .test.ts 后缀
- 使用描述性的测试名称
- 每个测试应该独立运行
- 关注核心业务逻辑的测试覆盖率

> **最佳实践：**测试应该快速、可靠、相互独立。遵循 AAA 原则：Arrange（准备）、Act（执行）、Assert（断言）。

## (八) 总结

- **Jest：**最流行的 TypeScript 测试框架
- **describe：**用于分组相关测试
- **it/test：**定义单个测试用例
- **expect：**断言测试结果
- **Mock：**模拟外部依赖
