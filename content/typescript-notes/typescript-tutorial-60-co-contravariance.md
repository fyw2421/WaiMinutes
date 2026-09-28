---
title: "六十、协变与逆变"
weight: 60
description: "本章介绍 协变与逆变。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

协变与逆变是 TypeScript 类型系统中的重要概念，理解它们有助于编写类型安全的代码。

它们描述了泛型类型在父子类型关系中的行为。

## (一) 为什么需要协变与逆变

当我们使用泛型类或函数时，类型参数的行为并不像你想象的那么简单。

将 Dog 赋值给 Animal 是安全的，但将处理 Animal 的函数赋值给处理 Dog 的函数可能不安全。

协变与逆变规则帮助 TypeScript 捕获这些潜在的类型错误。

> **概念：**协变允许子类型向父类型转换，逆变允许父类型向子类型转换，不变则不允许任何方向转换。

## (二) 协变 (Covariant)

```typescript
class Animal { name: string = "动物"; }
class Dog extends Animal { breed: string = "田园犬"; }
type AnimalGetter = () => Animal;
type DogGetter = () => Dog;
const getDog: DogGetter = () => new Dog();
const getAnimal: AnimalGetter = getDog;  // 协变：安全
const animal: Animal = getAnimal();
```

## (三) 逆变 (Contravariant)

```typescript
class Animal { name: string = "动物"; }
class Dog extends Animal { breed: string = "田园犬"; }
type DogConsumer = (dog: Dog) => void;
type AnimalConsumer = (animal: Animal) => void;
const consumeAnimal: AnimalConsumer = (animal) => { console.log("处理动物: " + animal.name); };
const consumeDog: DogConsumer = consumeAnimal;  // 逆变：安全
```

## (四) 启用严格函数类型

```typescript
interface Animal { readonly name: string; }
interface Dog extends Animal { readonly breed: string; }
type GetName = (animal: Animal) => string;
type GetDogBreed = (dog: Dog) => string;
function printAnimalName(animal: Animal): string { return animal.name; }
// const getSpecific: GetDogBreed = printAnimalName; // 错误！（strictFunctionTypes 下）
```

## (五) 泛型类的协变

```typescript
class Animal { name: string = "动物"; }
class Dog extends Animal { breed: string = "狗"; }
class Cage { animal: T; constructor(animal: T) { this.animal = animal; } }
const dogCage = new Cage(new Dog());
const animalCage: Cage = dogCage;  // 协变：安全
```

## (六) 数组的协变

```typescript
class Animal { name: string = "动物"; }
class Dog extends Animal { breed: string = "狗"; }
const dogs: Dog[] = [{ name: "旺财", breed: "哈士奇" }, { name: "小白", breed: "萨摩耶" }];
const animals: Animal[] = dogs;  // 协变：安全
```

## (七) 使用 extends 实现安全赋值

```typescript
interface Producer { produce(): T; }
interface Consumer { consume(value: T): void; }
class DogProducer implements Producer {
    produce(): Dog { return { name: "旺财", breed: "哈士奇" }; }
}
class AnimalConsumer implements Consumer {
    consume(animal: Animal): void { console.log("消费动物: " + animal.name); }
}
const animalProducer: Producer = new DogProducer();  // 协变
const dogConsumer: Consumer = new AnimalConsumer();      // 逆变
```

## (八) 注意事项

- 返回值协变：函数返回子类型是安全的
- 参数逆变：函数参数使用父类型是安全的
- 启用 strictFunctionTypes 获得更严格检查
- 注意数组协变带来的可变性问题

## (九) 总结

- **协变：**子类型 -> 父类型，用于输出类型
- **逆变：**父类型 -> 子类型，用于输入类型
- **不变：**不能相互赋值
- **strictFunctionTypes：**启用严格函数类型检查
