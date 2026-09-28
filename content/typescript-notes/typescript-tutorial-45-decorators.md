---
title: "四十五、装饰器"
weight: 45
description: "本章介绍 装饰器。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

装饰器允许在不修改原类的情况下，为类、方法、属性或参数添加额外功能。

## (一) 配置启用装饰器

tsconfig.json:

```json
{
    "compilerOptions": {
        "experimentalDecorators": true,
        "emitDecoratorMetadata": true
    }
}
```

## (二) 类装饰器

```typescript
function sealed(target: Function) {
    Object.seal(target);
    Object.seal(target.prototype);
}
@sealed
class Person { name: string; }
```

## (三) 方法装饰器

```typescript
function enumerable(value: boolean) {
    return function (target: any, propertyKey: string, descriptor: PropertyDescriptor) {
        descriptor.enumerable = value;
    };
}
class Greeter {
    @enumerable(false)
    greet() { return "Hello, " + this.greeting; }
}
```

## (四) 访问器装饰器

```typescript
function configurable(value: boolean) {
    return function (target: any, propertyKey: string, descriptor: PropertyDescriptor) {
        descriptor.configurable = value;
    };
}
```

## (五) 属性装饰器

```typescript
function format(formatString: string) {
    return function (target: any, propertyKey: string) {
        Object.defineProperty(target, propertyKey + "_format", { value: formatString });
    };
}
class User { @format("YYYY-MM-DD") birthDate: string; }
```

## (六) 参数装饰器

```typescript
function logParameter(target: any, propertyKey: string, parameterIndex: number) {
    console.log("参数装饰器: " + propertyKey + " 第 " + (parameterIndex + 1) + " 个参数");
}
```

## (七) 装饰器工厂

```typescript
function color(colorCode: string) {
    return function (target: any, propertyKey: string, descriptor: PropertyDescriptor) {
        var originalMethod = descriptor.value;
        descriptor.value = function (...args: any[]) {
            var result = originalMethod.apply(this, args);
            return "\x1b[" + colorCode + "m" + result + "\x1b[0m";
        };
    };
}
```

## (八) 执行顺序

装饰器从下往上应用，同一类型的多个装饰器从右到左执行。

## (九) 实际应用

- **日志记录**: 自动记录方法调用日志
- **权限验证**: 实现方法级别的权限检查
