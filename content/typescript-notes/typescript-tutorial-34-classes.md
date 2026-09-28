---
title: "三十四、类"
weight: 34
description: "本章介绍 类。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 中的类继承自 JavaScript 的 class 语法，并增加了类型注解、访问修饰符和抽象类等特性。

## (一) 基本类

### 1. 实例

```typescript
class Person {
name: string;
age: number;
constructor(name: string, age: number) {
this.name = name;
this.age = age;
}
greet(): string {
return "Hello, 我是 " + this.name + "，今年 " + this.age + " 岁";
}
}
var person = new Person("Alice", 30);
console.log(person.greet());
```

## (二) 继承

### 1. 实例

```typescript
class Animal {
name: string;
constructor(name: string) {
this.name = name;
}
move(distance: number): void {
console.log(this.name + " 移动了 " + distance + " 米");
}
}
class Dog extends Animal {
bark(): void {
console.log(this.name + " 在叫: 汪汪!");
}
}
var dog = new Dog("小黑");
dog.bark();
dog.move(10);
```

## (三) 访问修饰符

### 1. 实例

```typescript
class Employee {
public name: string;
private salary: number;
protected department: string;
constructor(name: string, salary: number, department: string) {
this.name = name;
this.salary = salary;
this.department = department;
}
public getSalary(): number {
return this.salary;
}
}
var emp = new Employee("张三", 5000, "开发部");
console.log("姓名: " + emp.name);
console.log("薪酬: " + emp.getSalary());
```

## (四) 抽象类

### 1. 实例

```typescript
abstract class Shape {
abstract getArea(): number;
display(): void {
console.log("这个形状的面积是: " + this.getArea().toFixed(2));
}
}
class Circle extends Shape {
radius: number;
constructor(radius: number) {
super();
this.radius = radius;
}
getArea(): number {
return Math.PI * this.radius ** 2;
}
}
var circle = new Circle(5);
circle.display();
```

## (五) getter 和 setter

### 1. 实例

```typescript
class Student {
private _name: string = "";
get name(): string {
return this._name;
}
set name(value: string) {
if (value.length < 2) {
console.log("名字长度不能少于2个字符");
} else {
this._name = value;
}
}
}
var student = new Student();
student.name = "李四";
console.log("学生姓名: " + student.name);
```

## (六) 静态属性和方法

### 1. 实例

```typescript
class MathUtils {
static PI: number = 3.14159;
static circleArea(radius: number): number {
return this.PI * radius ** 2;
}
}
console.log("PI: " + MathUtils.PI);
console.log("圆面积: " + MathUtils.circleArea(5).toFixed(2));
```

## (七) 总结

- 基本类：**构造函数、属性和方法**
- 继承：**extends 关键字**
- 访问修饰符：**public（默认）、private、protected**
- 抽象类：**abstract 关键字，不能实例化**
- getter/setter：**访问器**
- 静态成员：**static 关键字，类级别访问**
