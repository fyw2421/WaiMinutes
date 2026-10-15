---
title: "DataStructuresNotes : 总目录"
weight: 1
description: "数据结构学习笔记总目录：8 篇按基础、线性结构、非线性结构三组编排，每篇附一句话说明，并提供配套 C++ 源码下载。"
date: 2026-10-15
tags: ["数据结构"]
featureimage: "covers/datastructures-index.svg"
---

# 一、这份笔记

共 8 篇。每篇都配有可运行的 **C++ 源码**（`datastructure/` 目录下，12 个 `.h` + 10 个 `.cpp`），可直接编译对照。

按**依赖顺序**编排：后面的内容默认你已经读过前面的。可以从 01 顺读到 08，也可以按结构类型直接跳。

# 二、基础（01）

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 01 | [Summarize]({{< ref "datastructures01-summarize" >}}) | 组成要素、逻辑与物理结构、算法分析 |

这一篇是其余各篇的地基——术语定义、时间复杂度分析方法都在这里，建议先读。

# 三、线性结构（02–04）

元素之间呈一对一关系，可用数组或链表存储。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 02 | [Sequence List]({{< ref "datastructures02-sequence-list" >}}) | 顺序存储、随机访问与插入删除代价 |
| 03 | [Linked List]({{< ref "datastructures03-linked-list" >}}) | 单链表、双链表与循环链表 |
| 04 | [Stack and Queue]({{< ref "datastructures04-stack-queue" >}}) | 栈、队列与双端队列 |

# 四、非线性结构（05–08）

元素之间呈一对多或多对多关系。

| 编号 | 标题 | 说明 |
| --- | --- | --- |
| 05 | [Binary Tree]({{< ref "datastructures05-binary-tree" >}}) | 二叉树性质、遍历与线索化 |
| 06 | [Threaded Binary Tree]({{< ref "datastructures06-threaded-binary-tree" >}}) | 线索二叉树与遍历回溯 |
| 07 | [Graph]({{< ref "datastructures07-graph" >}}) | 存储方式、最短路径与最小生成树 |
| 08 | [Huffman Coding]({{< ref "datastructures08-huffman-code" >}}) | 哈夫曼树、带权路径长度与前缀码 |

# 五、参考资料

- [数据结构（C++ 实现）配套源码](https://github.com/) — `datastructure/` 目录
- [算法与数据结构参考](https://www.geeksforgeeks.org/data-structures/) — 复杂度对照表
