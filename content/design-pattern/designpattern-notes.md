---
title: "DesignPatternNotes : 总目录"
weight: 1
description: "设计模式学习笔记总目录：9 篇按设计原则与分类、创建型、结构型、行为型四组编排，覆盖 GoF 全部 23 个模式，每篇附一句话说明。"
date: 2026-10-15
tags: ["设计模式"]
featureimage: "covers/designpattern-index.svg"
---

# 一、这份笔记

共 9 篇，收录 **GoF 全部 23 个设计模式**，按「设计原则 → 创建型 → 结构型 → 行为型」的顺序编排——后面的内容默认你已经读过前面的。

每篇都配有 UML 类图（手绘 SVG）、结构的说明、实现代码与适用场景的讨论，以及不该用这个模式的反面案例。

# 二、设计原则与分类（01）

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 01 | [Summarize]({{< ref "designpattern01-principles-types" >}}) | 七大设计原则、GoF 三大分类与选型思路 |

这是全专题的地基——六个原则（开闭、里氏、依赖、接口、迪米特、最小知识）决定了后面每个模式为什么长成那样，建议先读。

# 三、创建型模式（02–03）

解决**实例化**问题：把「怎么造」与「造什么」分开。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 02 | [Creational 2-1]({{< ref "designpattern02-simplefactory-factorymethod-abstractfactory" >}}) | 简单工厂、工厂方法与抽象工厂 |
| 03 | [Creational 2-2]({{< ref "designpattern03-builder-prototype-singleton" >}}) | 建造者、原型与单例 |

# 四、结构型模式（04–05）

解决**组装**问题：让类与类之间的组合关系更灵活。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 04 | [Structural 2-1]({{< ref "designpattern04-adapter-facade-bridge" >}}) | 适配器、外观与桥接 |
| 05 | [Structural 2-2]({{< ref "designpattern05-composite-decorator-flyweight-proxy" >}}) | 组合、装饰器、享元与代理 |

# 五、行为型模式（06–09）

解决**职责分配**问题：把「谁在什么时候响应谁」组织清楚。这一类最多，也最容易为了用而用。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 06 | [Behavioral 4-1]({{< ref "designpattern06-memento-mediator-observer" >}}) | 备忘录、中介者与观察者 |
| 07 | [Behavioral 4-2]({{< ref "designpattern07-state-visitor-interpret" >}}) | 状态、访问者与解释器 |
| 08 | [Behavioral 4-3]({{< ref "designpattern08-iterator-strategy-command" >}}) | 迭代器、策略与命令 |
| 09 | [Behavioral 4-4]({{< ref "designpattern09-templatemethod-chainofresponsibility" >}}) | 模板方法与责任链 |

# 六、参考资料

- [GoF 原始论文](https://people.cs.utexas.edu/~gwf/patterns/) — Design Patterns: Elements of Reusable Object-Oriented Software
- [refactoring.guru 设计模式](https://refactoring.guru/design-patterns) — 各模式的意图与适用场景对照
- [维基百科：设计模式](https://zh.wikipedia.org/wiki/设计模式) — 中文条目与模式目录
