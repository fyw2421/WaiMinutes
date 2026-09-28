---
title: "四十四、混入（Mixin）"
weight: 44
description: "本章介绍 混入（Mixin）。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

Mixin 是一种代码复用模式，用于将多个独立功能模块混入到一个类中。

## (一) 基本概念

```typescript
type Constructor = new (...args: any[]) => T;

function Timestampedextends Constructor>(Base: TBase) {
    return class extends Base {
        createdAt = new Date();
    };
}

class User {
    constructor(public name: string) {}
}

const TimestampedUser = Timestamped(User);
const user = new TimestampedUser("Alice");
console.log(user.name);                    // Alice
console.log(user.createdAt instanceof Date); // true
```

## (二) 组合多个 Mixin

```typescript
function Serializableextends Constructor>(Base: TBase) {
    return class extends Base {
        serialize(): string {
            return JSON.stringify(this);
        }
    };
}
function Loggableextends Constructorserialize(): string }>>(Base: TBase) {
    return class extends Base {
        log(): void { console.log("[LOG]", this.serialize()); }
    };
}
const AdvancedProduct = Loggable(Serializable(Timestamped(Product)));
```

## (三) Mixin 与接口结合

```typescript
interface ISerializable { serialize(): string; }
function Serializableextends Constructor>(Base: TBase) {
    return class extends Base implements ISerializable {
        serialize(): string { return JSON.stringify(this); }
    };
}
```

## (四) 带约束的 Mixin

```typescript
type WithIdAndName = Constructornumber; name: string }>;
function Printableextends WithIdAndName>(Base: TBase) {
    return class extends Base {
        print(): void { console.log(\`[\${this.id}] \${this.name}\`); }
    };
}
```

## (五) Mixin 与继承对比

| 维度 | 继承 | Mixin |
|---|---|---|
| 来源数量 | 单父类 | 可叠加任意数量 |
| 耦合程度 | 强耦合 | 低耦合 |
| 复用粒度 | 整个类 | 单一功能 |
| 适用场景 | "is-a"关系 | 横切关注点 |
