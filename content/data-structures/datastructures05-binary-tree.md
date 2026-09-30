---
title: "Data Structures05 : Binary Tree"
weight: 5
description: "二叉树的存储结构、遍历算法（递归/非递归）与 C++ 实现"
date: 2026-09-24
tags: ["Data Structure"]
featureimage: "covers/datastructures05-binary-tree.svg"
---

# 一、二叉树

> 参考教材：严蔚敏《数据结构》、邓俊辉《数据结构》、清华大学数据结构 MOOC（陈越）、LeetCode 二叉树专题

二叉树是每个结点最多有两棵子树（左、右）的树形结构，子树有左右之分，次序不能颠倒。

***

## (一)存储结构

### 1. 链式存储（二叉链表）

```cpp
template<typename T>
class BinaryTree {
private:
    struct Node {
        T data;
        Node *lChild, *rChild;
    };
    Node *mpRoot;
};
```

- **结点数 n，空链域数 = n+1**（叶子 2 个，度为 1 的结点 1 个，根 0 个）
- **遍历递归天然契合**

### 2. 顺序存储（完全二叉树/堆）

完全二叉树可用数组存储，下标关系：
- 父：`i/2`，左孩子：`2i`，右孩子：`2i+1`

***

## (二)遍历算法

| 遍历顺序 | 递归伪代码 | 非递归核心 |
|----------|------------|------------|
| **先序 (NLR)** | 访问根 → 遍历左 → 遍历右 | 栈：访问→压栈→走左，弹栈→走右 |
| **中序 (LNR)** | 遍历左 → 访问根 → 遍历右 | 栈：走左压栈到底，弹栈→访问→走右 |
| **后序 (LRN)** | 遍历左 → 遍历右 → 访问根 | 栈：需标记“右子树已访问” |
| **层序** | — | 队列：根入队，出队访问并入队子女 |

***

## (三)关键代码解析

### 1. 递归遍历

```cpp
template<typename T>
void BinaryTree<T>::__preOrder(Node *pRoot) {
    if (pRoot) {
        cout << pRoot->data << " ";
        __preOrder(pRoot->lChild);
        __preOrder(pRoot->rChild);
    }
}
```

**特点**：代码简洁，但深度过大（>1e5）会栈溢出。

### 2. 非递归先序遍历

```cpp
template<typename T>
void BinaryTree<T>::__nonRecurPreOrder(Node *pRoot) {
    stack<Node*> st;
    Node *p = pRoot;
    while (p || !st.empty()) {
        if (p) {
            cout << p->data << " "; // 访问
            st.push(p);
            p = p->lChild;          // 走左
        } else {
            p = st.top(); st.pop(); // 回溯
            p = p->rChild;          // 走右
        }
    }
}
```

### 3. 非递归中序遍历

```cpp
template<typename T>
void BinaryTree<T>::__nonRecurInOrder(Node *pRoot) {
    stack<Node*> st;
    Node *p = pRoot;
    while (p || !st.empty()) {
        if (p) {
            st.push(p);
            p = p->lChild;
        } else {
            p = st.top(); st.pop();
            cout << p->data << " "; // 访问
            p = p->rChild;
        }
    }
}
```

### 4. 非递归后序遍历（双栈法 / 单栈+标记法）

```cpp
template<typename T>
void BinaryTree<T>::__nonRecurPostOrder(Node *pRoot) {
    stack<Node*> st;
    Node *p = pRoot, *pre = nullptr;
    st.push(p);
    while (!st.empty()) {
        p = st.top();
        // 叶子 或 子树已访问
        if ((!p->lChild && !p->rChild) ||
            (pre && (pre == p->lChild || pre == p->rChild))) {
            cout << p->data << " ";
            st.pop();
            pre = p;
        } else {
            if (p->rChild) st.push(p->rChild); // 先压右
            if (p->lChild) st.push(p->lChild); // 再压左
        }
    }
}
```

**关键**：`pre` 记录上一次访问的结点，判断右子树是否已处理。

### 5. 层序遍历（队列）

```cpp
template<typename T>
void BinaryTree<T>::levelOrder() {
    queue<Node*> q;
    if (mpRoot) q.push(mpRoot);
    while (!q.empty()) {
        Node *p = q.front(); q.pop();
        cout << p->data << " ";
        if (p->lChild) q.push(p->lChild);
        if (p->rChild) q.push(p->rChild);
    }
}
```

***

## (四)树的基本性质

| 性质 | 公式/结论 |
|------|-----------|
| 第 i 层最多结点数 | 2^(i-1) |
| 深度为 h 的二叉树最多结点数 | 2^h - 1 |
| 度为 0 的结点数 = 度为 2 的结点数 + 1 | n0 = n2 + 1 |
| 满二叉树高度 h，结点数 | 2^h - 1 |
| 完全二叉树编号性质 | 父 i/2，左 2i，右 2i+1 |

***

## (五)二叉树的建立与销毁

### 1. 先序序列建立（-1 表示空）

```cpp
template<typename T>
typename BinaryTree<T>::Node *BinaryTree<T>::__create(T *&pData, T emptyVal) {
    T data = (pData == nullptr) ? input() : *pData++;
    if (data == emptyVal) return nullptr;
    Node *p = new Node{data, nullptr, nullptr};
    p->lChild = __create(pData, emptyVal);
    p->rChild = __create(pData, emptyVal);
    return p;
}
```

### 2. 销毁（后序释放）

```cpp
template<typename T>
void BinaryTree<T>::__destroy(Node* &pRoot) {
    if (pRoot) {
        __destroy(pRoot->lChild);
        __destroy(pRoot->rChild);
        delete pRoot;
        pRoot = nullptr;
    }
}
```

***

## (六)树的统计信息

| 操作 | 递归公式 |
|------|----------|
| 结点总数 | size(L) + size(R) + 1 |
| 高度 | max(height(L), height(R)) + 1 |
| 叶子数 | (L=null&&R=null)? 1 : leaf(L)+leaf(R) |

***

## (七)源码获取

| 文件 | 说明 | 链接 |
|------|------|------|
| `BinaryTree.h` | 二叉树完整实现（含递归/非递归遍历、建立、销毁、统计） | [点击查看](datastructure/BinaryTree.h) |
| `BinaryTreeTest.cpp` | 测试用例 | [点击查看](datastructure/BinaryTreeTest.cpp) |

***

## (八)习题

1. **已知先序和中序，能否唯一确定二叉树？** 答：能。先序首为根，在中序中分左右子树，递归构造。
2. **已知先序和后序，能否唯一确定？** 答：不能（除非每个结点度为 0 或 2）。
3. **非递归后序遍历为什么最难？** 答：需要判断“右子树是否已访问”，需额外标记或双栈。
4. **如何判断一棵树是否为完全二叉树？** 答：层序遍历，遇到首个空结点后，队列中不应再有非空结点。

***

> 上一节：[一、栈与队列](/data-structures/datastructures04-stack-queue/)  
> 下一节：[二、线索二叉树](/data-structures/datastructures06-threaded-binary-tree/)