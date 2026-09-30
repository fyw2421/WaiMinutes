---
title: "Data Structures03 : Linked List"
weight: 3
description: "单链表、双链表、循环链表的存储结构与 C++ 实现"
date: 2026-09-24
tags: ["Data Structure"]
featureimage: "covers/datastructures03-linked-list.svg"
---

# 一、链表

> 参考教材：严蔚敏《数据结构》、邓俊辉《数据结构》、浙江大学数据结构 MOOC（陈越）

链表用任意地址存储单元存放数据元素，通过指针链接建立逻辑关系。分为单链表、双链表、循环链表。

***

## (一)单链表

### 1. 存储结构

```cpp
template<typename T>
class SingleLinkedList {
private:
    struct Node {
        T data;
        Node *next;
    };
    Node *mpHead; // 头结点（不存数据）
    int mLength;
};
```

- **头结点**：简化插入删除边界条件，首元结点为 `mpHead->next`
- **存储密度 < 1**：每个结点额外存储 1 个指针

### 2. 基本操作

| 操作 | 时间复杂度 | 说明 |
|------|------------|------|
| 初始化 | O(1) | 创建头结点 |
| 销毁 | O(n) | 依次释放结点 |
| 求长度 | O(1) | 返回 mLength |
| 按位查找 | O(n) | 从头遍历 |
| 按值查找 | O(n) | 顺序扫描 |
| 插入 | O(n) | 定位前驱 + 修改指针 |
| 删除 | O(n) | 定位前驱 + 修改指针 + 释放结点 |

### 3. 关键代码

#### (1) 插入（带头结点，下标从 0 开始）

```cpp
template<typename T>
bool SingleLinkedList<T>::insert(T data, int nIndex) {
    Node *pNode = __getNode(nIndex - 1); // 找前驱
    if (pNode == nullptr) return false;

    Node *pInsertNode = new Node;
    pInsertNode->data = data;
    pInsertNode->next = pNode->next;
    pNode->next = pInsertNode;
    mLength++;
    return true;
}
```

#### (2) 获取第 nIndex 个结点（-1 为头结点）

```cpp
template<typename T>
typename SingleLinkedList<T>::Node *SingleLinkedList<T>::__getNode(int nIndex) {
    Node *pNode = mpHead;
    int nIdx = -1;
    while (pNode && nIdx < nIndex) {
        pNode = pNode->next;
        nIdx++;
    }
    if (pNode == nullptr || nIdx > nIndex) return nullptr;
    return pNode;
}
```

***

## (二)双链表

### 1. 存储结构

```cpp
template<typename T>
class DoubleLinkedList {
private:
    struct Node {
        T data;
        Node *prior, *next;
    };
    Node *mpHead; // 头结点，prior 指向尾结点，next 指向首元结点
    int mLength;
};
```

- **双向遍历**：前驱/后继均可 O(1) 访问
- **插入删除更简洁**：无需查找前驱

### 2. 插入操作

```cpp
template<typename T>
bool DoubleLinkedList<T>::insert(T data, int nIndex) {
    Node *pNode = __getNode(nIndex); // 目标位置结点
    if (pNode == nullptr) return false;

    Node *pInsertNode = new Node;
    pInsertNode->data = data;
    pInsertNode->prior = pNode->prior;
    pInsertNode->next = pNode;
    pNode->prior->next = pInsertNode;
    pNode->prior = pInsertNode;
    mLength++;
    return true;
}
```

***

## (三)循环链表

### 1. 存储结构

```cpp
template<typename T>
class CircularLinkedList {
private:
    struct Node {
        T data;
        Node *next;
    };
    Node *mpRear; // 尾指针，尾结点 next 指向头结点
    int mLength;
};
```

- **仅设尾指针**：`mpRear->next` 为头结点，`mpRear->next->next` 为首元结点
- **合并链表 O(1)**：只需修改尾指针

### 2. 约瑟夫环问题

循环链表经典应用：n 人围圈，报数到 m 出列，求出列顺序。

```cpp
template<typename T>
T CircularLinkedList<T>::josephus(int m) {
    Node *pPre = mpRear;
    while (mLength > 1) {
        for (int i = 1; i < m; i++) pPre = pPre->next;
        Node *pDel = pPre->next;
        pPre->next = pDel->next;
        if (pDel == mpRear) mpRear = pPre;
        T data = pDel->data;
        delete pDel;
        mLength--;
    }
    return mpRear->next->data; // 最后剩下的
}
```

***

## (四)三种链表对比

| 特性 | 单链表 | 双链表 | 循环链表 |
|------|--------|--------|----------|
| 指针域 | 1 个 | 2 个 | 1 个 |
| 反向遍历 | 不支持 | 支持 | 不支持 |
| 查找前驱 | O(n) | O(1) | O(n) |
| 合并链表 | O(n) | O(n) | O(1) |
| 适用场景 | 通用、内存受限 | 频繁双向遍历 | 循环处理、约瑟夫环 |

***

## (五)源码获取

| 文件 | 说明 | 链接 |
|------|------|------|
| `SingleLinkedList.h` | 单链表模板类 | [点击查看](datastructure/SingleLinkedList.h) |
| `SingleLinkedListTest.cpp` | 单链表测试用例 | [点击查看](datastructure/SingleLinkedListTest.cpp) |
| `DoubleLinkedList.h` | 双链表模板类 | [点击查看](datastructure/DoubleLinkedList.h) |
| `DoubleLinkedListTest.cpp` | 双链表测试用例 | [点击查看](datastructure/DoubleLinkedListTest.cpp) |
| `CircularLinkedList.h` | 循环链表模板类 | [点击查看](datastructure/CircularLinkedList.h) |
| `CircularLinkedListTest.cpp` | 循环链表测试用例 | [点击查看](datastructure/CircularLinkedListTest.cpp) |

***

## (六)习题

1. **单链表如何实现 O(1) 删除尾结点？** 答：维护尾指针 + 双链表，或用循环链表尾指针。
2. **双链表删除结点比单链表简单在哪？** 答：直接通过 `prior` 找到前驱，无需遍历。
3. **循环链表如何判断空表？** 答：`mpRear->next == mpRear`（仅头结点指向自己）。

***

> 上一节：[一、顺序表](/data-structures/datastructures02-sequence-list/)  
> 下一节：[二、栈与队列](/data-structures/datastructures04-stack-queue/)