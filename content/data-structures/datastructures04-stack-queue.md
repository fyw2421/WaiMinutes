---
title: "Data Structures04 : Stack and Queue"
weight: 4
description: "栈与队列的顺序存储、链式存储及 C++ 实现"
date: 2026-09-24
tags: ["Data Structure"]
featureimage: "covers/datastructures04-stack-queue.svg"
---

# 一、栈与队列

> 参考教材：严蔚敏《数据结构》、邓俊辉《数据结构》、清华大学数据结构 MOOC

栈和队列是操作受限的线性表：
- **栈**：后进先出（LIFO），仅允许在栈顶插入/删除
- **队列**：先进先出（FIFO），仅允许队尾插入、队头删除

***

## (一)栈

### 1. 顺序栈

```cpp
template<typename T>
class SequenceStack {
private:
    T *mData;
    const int mMaxSize;
    int mTop; // 栈顶指针，-1 为空
};
```

- **入栈**：`mData[++mTop] = data` — O(1)
- **出栈**：`return mData[mTop--]` — O(1)
- **共享栈**：两个栈共享一个数组，栈顶向中间增长，提高空间利用率

### 2. 链栈

```cpp
template<typename T>
class LinkedStack {
private:
    struct Node { T data; Node *next; };
    Node *mpTop; // 栈顶指针
};
```

- **无容量上限**（内存允许范围内）
- **入栈/出栈**：头插法/头删法 — O(1)

### 3. 栈的应用

| 应用 | 原理 |
|------|------|
| 括号匹配 | 遇左括号入栈，遇右括号出栈匹配 |
| 表达式求值 | 中缀转后缀（逆波兰），再用栈求值 |
| 递归实现 | 系统调用栈保存返回地址、局部变量 |
| 回溯算法 | 迷宫求路、八皇后问题 |

***

## (二)队列

### 1. 顺序队列（循环队列）

```cpp
template<typename T>
class SequenceQueue {
private:
    T *mData;
    const int mMaxSize;
    int mFront, mRear; // 队头、队尾指针
};
```

**关键点**：
- **空**：`mFront == mRear`
- **满**：`(mRear + 1) % mMaxSize == mFront`（少用一个元素区分空满）
- **长度**：`(mRear - mFront + mMaxSize) % mMaxSize`
- **入队**：`mData[mRear] = data; mRear = (mRear + 1) % mMaxSize`
- **出队**：`data = mData[mFront]; mFront = (mFront + 1) % mMaxSize`

### 2. 链队列

```cpp
template<typename T>
class LinkedQueue {
private:
    struct Node { T data; Node *next; };
    Node *mpFront, *mpRear; // 队头、队尾指针
};
```

- **带头结点**：简化空队列操作
- **入队**：尾插法 — O(1)
- **出队**：头删法（头结点后） — O(1)

### 3. 队列的应用

| 应用 | 原理 |
|------|------|
| 树的层序遍历 | 根入队，出队访问并入队子女 |
| 图的广度优先搜索 (BFS) | 同层序遍历 |
| 进程调度 | 时间片轮转、多级反馈队列 |
| 缓冲区 | 生产者-消费者模型 |

***

## (三)源码获取

| 文件 | 说明 | 链接 |
|------|------|------|
| `SequenceStack.h` | 顺序栈 | [点击查看](datastructure/SequenceStack.h) |
| `LinkedStack.h` | 链栈 | [点击查看](datastructure/LinkedStack.h) |
| `StackTest.cpp` | 栈测试用例 | [点击查看](datastructure/StackTest.cpp) |
| `SequenceQueue.h` | 顺序队列（循环队列） | [点击查看](datastructure/SequenceQueue.h) |
| `LinkedQueue.h` | 链队列 | [点击查看](datastructure/LinkedQueue.h) |
| `QueueTest.cpp` | 队列测试用例 | [点击查看](datastructure/QueueTest.cpp) |

***

## (四)习题

1. **为什么循环队列要浪费一个存储单元？** 答：区分“空”（`front==rear`）和“满”（`(rear+1)%size==front`）。也可用标志位或计数器避免浪费。
2. **栈和队列的本质区别是什么？** 答：栈是单口操作（LIFO），队列是双口操作（FIFO）。
3. **如何用两个栈实现一个队列？** 答：栈 A 入队，栈 B 出队；B 空时将 A 全部弹出压入 B。

***

> 上一节：[一、链表](/data-structures/datastructures03-linked-list/)  
> 下一节：[二、二叉树](/data-structures/datastructures05-binary-tree/)