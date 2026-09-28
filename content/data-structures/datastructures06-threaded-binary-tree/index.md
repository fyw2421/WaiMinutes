---
title: "Data Structures06 : Threaded Binary Tree"
weight: 6
description: "线索二叉树的构建、中序/先序/后序线索化遍历与 C++ 实现"
date: 2026-09-24
tags: ["Data Structure"]
featureimage: "covers/datastructures06-threaded-binary-tree.svg"
---

# 一、线索二叉树

> 参考教材：严蔚敏《数据结构》、邓俊辉《数据结构》、清华大学数据结构 MOOC

线索二叉树利用二叉链表中 `n+1` 个空指针域，存放结点的前驱/后继信息，实现**非递归、无栈**的中序遍历。

***

## (一)基本概念

### 1. 线索定义

```cpp
template<typename T>
class ThreadedBinaryTree {
private:
    struct Node {
        T data;
        Node *lChild, *rChild;
        int lTag, rTag; // 0=指向孩子，1=指向线索（前驱/后继）
    };
    Node *mpRoot;
};
```

- **lTag = 0**：`lChild` 指向左孩子；**lTag = 1**：`lChild` 指向中序前驱
- **rTag = 0**：`rChild` 指向右孩子；**rTag = 1**：`rChild` 指向中序后继

### 2. 头结点

引入头结点 `mpHead`：
- `lChild` 指向根结点
- `rChild` 指向中序最后一个结点
- `lTag=1` 时指向中序第一个结点，`rTag=1` 时指向中序最后一个结点
- 形成**双向循环链表**，遍历更简洁

***

## (二)中序线索化构建

### 1. 中序遍历建立线索

```cpp
template<typename T>
void ThreadedBinaryTree<T>::inOrderThreading() {
    mpHead = new Node;
    mpHead->lTag = 1; mpHead->rTag = 1;
    mpHead->lChild = mpHead->rChild = mpHead;

    if (mpRoot == nullptr) {
        mpHead->lChild = mpHead;
        return;
    }

    Node *pre = mpHead; // 前驱初始为头结点
    __inThreading(mpRoot, pre);

    // 处理最后一个结点
    pre->rChild = mpHead;
    pre->rTag = 1;
    mpHead->rChild = pre; // 头结点 rChild 指向最后一个
}

template<typename T>
void ThreadedBinaryTree<T>::__inThreading(Node *p, Node *&pre) {
    if (p == nullptr) return;
    __inThreading(p->lChild, pre);

    // 建立当前结点的前驱线索
    if (p->lChild == nullptr) {
        p->lChild = pre;
        p->lTag = 1;
    }
    // 建立前驱结点的后继线索
    if (pre->rChild == nullptr) {
        pre->rChild = p;
        pre->rTag = 1;
    }
    pre = p; // 移动前驱

    __inThreading(p->rChild, pre);
}
```

**关键**：中序遍历过程中维护 `pre` 指针，`pre` 始终指向当前结点的中序前驱。

***

## (三)线索二叉树遍历

### 1. 找中序第一个结点

```cpp
Node *first(Node *p) {
    while (p->lTag == 0) p = p->lChild;
    return p;
}
```

### 2. 找中序后继

```cpp
Node *next(Node *p) {
    if (p->rTag == 1) return p->rChild; // 线索直接指向后继
    return first(p->rChild);            // 右子树最左下结点
}
```

### 3. 完整遍历（无栈、无递归）

```cpp
template<typename T>
void ThreadedBinaryTree<T>::inOrderTraverse() {
    for (Node *p = first(mpHead->lChild); p != mpHead; p = next(p)) {
        cout << p->data << " ";
    }
}
```

**复杂度**：时间 O(n)，空间 O(1) —— 最优！

***

## (四)先序线索化

### 1. 先序遍历建立线索

先序线索的前驱/后继关系与中序不同：前序先访问根节点，根的左子树的最左结点才是遍历序上的前一个结点。

```cpp
template<typename T>
void ThreadedBinaryTree<T>::preOrderThreading() {
    mpHead = new Node;
    mpHead->lTag = 1;
    mpHead->rTag = 1;
    mpHead->lChild = mpHead->rChild = mpHead;

    if (mpRoot == nullptr) {
        mpHead->lChild = mpHead;
        return;
    }

    Node *pre = nullptr;
    __preThreading(mpRoot, pre);

    // 最后一个访问的结点的后继指向头结点
    pre->rChild = mpHead;
    pre->rTag = 1;
    // 第一个结点是根节点
    mpHead->lChild = mpRoot;
    mpHead->lTag = 1;
}

template<typename T>
void ThreadedBinaryTree<T>::__preThreading(Node *p, Node *&pre) {
    if (p == nullptr) return;

    // 处理前驱线索
    if (p->lChild == nullptr) {
        p->lChild = pre;
        p->lTag = 1;
    }
    // 处理前驱结点的后继线索
    if (pre != nullptr && pre->rChild == nullptr) {
        pre->rChild = p;
        pre->rTag = 1;
    }
    pre = p;

    if (p->lTag == 0) __preThreading(p->lChild, pre);  // 左孩子
    if (p->rTag == 0) __preThreading(p->rChild, pre);  // 右孩子
}
```

**关键**：先序线索中，`pre` 指向前一轮遍历的结点。处理顺序为：**处理当前结点 → 递归左子树 → 递归右子树**。

### 2. 先序线索遍历

```cpp
template<typename T>
void ThreadedBinaryTree<T>::preOrderTraverse() {
    if (mpHead->lChild == mpHead) return;  // 空树

    Node *p = mpHead->lChild;  // 第一个结点（根节点）
    // 先序遍历顺序：根 → 左子树最左 → ...
    // 利用线索直接走向后继
    while (p != mpHead) {
        cout << p->data << " ";
        if (p->lTag == 0) p = p->lChild;  // 有左孩子，走左子树
        else p = p->rChild;               // 线索直接指向后继
    }
}
```

***

## (五)后序线索化

### 1. 后序遍历建立线索

后序线索最复杂：最后一个被访问的结点是根节点，前驱关系需要通过"左子树最右结点"或"右子树最右结点"来确定。

```cpp
template<typename T>
void ThreadedBinaryTree<T>::postOrderThreading() {
    mpHead = new Node;
    mpHead->lTag = 1;
    mpHead->rTag = 1;
    mpHead->lChild = mpHead->rChild = mpHead;

    if (mpRoot == nullptr) {
        mpHead->lChild = mpHead;
        return;
    }

    Node *pre = nullptr;
    __postThreading(mpRoot, pre);

    // 根节点是最后一个访问的结点
    if (pre != nullptr) {
        pre->rChild = mpHead;
        pre->rTag = 1;
    }

    // 第一个后序访问的结点 = 树的最左下结点
    Node *first = mpRoot;
    while (first != nullptr && first->lTag == 0) first = first->lChild;
    mpHead->lChild = first;
    mpHead->lTag = 1;
}

template<typename T>
void ThreadedBinaryTree<T>::__postThreading(Node *p, Node *&pre) {
    if (p == nullptr) return;

    if (p->lTag == 0) __postThreading(p->lChild, pre);
    if (p->rTag == 0) __postThreading(p->rChild, pre);

    // 处理前驱线索
    if (p->lChild == nullptr) {
        p->lChild = pre;
        p->lTag = 1;
    }
    // 处理前驱结点的后继线索
    if (pre != nullptr && pre->rChild == nullptr) {
        pre->rChild = p;
        pre->rTag = 1;
    }
    pre = p;
}
```

**关键**：后序遍历是**左 → 右 → 根**，所以 `pre` 移动在递归之后。

### 2. 后序线索遍历

```cpp
template<typename T>
Node* ThreadedBinaryTree<T>::getPostOrderNext(Node *p) {
    if (p == mpHead) return nullptr;

    // 后序线索的后继 = 父结点
    // 但我们没有父指针，需要反推：
    // - 若 p 是父结点的左子树，则后继 = 父结点的右子树的最左下结点（或父结点本身）
    // - 若 p 是父结点的右子树，则后继 = 父结点本身

    // 方法1：从头结点找到根，反向跟踪
    // 方法2：利用线索 —— 如果 rTag==1，直接返回 rChild
    if (p->rTag == 1) {
        return p->rChild;  // 线索指向后继（父结点）
    }

    // 若 p 没有线索后继，需要从右子树推断
    return nullptr;  // 复杂情况需额外结构
}
```

**注意**：后序线索遍历比中序/先序复杂，通常需要额外的父指针或栈结构。

***

## (六)三种线索化对比

| 特性 | 先序线索 | 中序线索 | 后序线索 |
|------|----------|----------|----------|
| **建立复杂度** | 中等 | 简单 | 复杂 |
| **遍历复杂度** | O(n) O(1) | O(n) O(1) | 困难（需父指针） |
| **前驱查找** | O(1) | O(1) | O(1) |
| **后继查找** | O(1) | O(1) | O(1) 但需额外结构 |
| **应用场景** | 先序优先遍历 | **最常用** | 后序优先遍历 |
| **实际使用** | 较少 | 广泛 | 少见 |

***

## (七)线索二叉树 vs 普通二叉树

| 特性 | 普通二叉树 | 线索二叉树 |
|------|------------|------------|
| 空指针域 | n+1 个空闲 | 全部利用存前驱/后继 |
| 中序遍历 | 栈/递归 O(h) 空间 | 无栈 O(1) 空间 |
| 查找前驱/后继 | O(h) 或需栈 | O(1) |
| 插入/删除 | 简单 | 需维护线索，较复杂 |
| 适用场景 | 通用、频繁修改 | 遍历频繁、修改少 |

***

## (八)源码获取

| 文件 | 说明 | 链接 |
|------|------|------|
| `ThreadedBinaryTree.h` | 线索二叉树完整实现 | [点击查看](datastructure/ThreadedBinaryTree.h) |
| `ThreadedBinaryTreeTest.cpp` | 测试用例 | [点击查看](datastructure/ThreadedBinaryTreeTest.cpp) |

***

## (九)习题

1. **为什么要引入头结点？** 答：统一空树/非空树处理；形成双向循环链表，遍历代码更简洁；头结点 `rChild` 直接指向最后一个结点，便于反向遍历。
2. **线索二叉树插入结点需要做什么？** 答：修改新结点的 4 个指针/标记，并更新受影响的前驱/后继线索（最多 4 处）。
3. **前序/后序线索化与中序有何不同？** 答：遍历顺序不同，`pre` 移动时机不同；前序需先处理根再递归左右，后序需最后处理根。

***

> 上一节：[一、二叉树](/data-structures/datastructures05-binary-tree/)  
> 下一节：[二、图](/data-structures/datastructures07-graph/)