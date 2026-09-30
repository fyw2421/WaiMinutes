---
title: "Data Structures02 : Sequence List"
weight: 2
description: "顺序表的存储结构、基本操作与 C++ 实现"
date: 2026-09-24
tags: ["Data Structure"]
featureimage: "covers/datastructures02-sequence-list.svg"
---

# 一、顺序表

> 参考教材：严蔚敏《数据结构》（C语言版）、邓俊辉《数据结构》（C++版）、清华大学数据结构 MOOC（陈越）

顺序表是用一组地址连续的存储单元依次存储数据元素的线性结构，逻辑上相邻的元素在物理位置上也相邻。

***

## (一)存储结构

```cpp
template<typename T>
class SequenceList {
private:
    T *mData;           // 动态数组基址
    const int mMaxSize; // 最大容量
    int mLength;        // 当前长度
};
```

- **存储密度 = 1**（仅存储数据元素，无指针开销）
- **随机访问**：O(1) 通过下标直接访问任意元素
- **插入/删除**：O(n) 需要移动元素

***

## (二)基本操作

| 操作 | 时间复杂度 | 说明 |
|------|------------|------|
| 初始化 | O(1) | 分配内存，置空 |
| 销毁 | O(1) | 释放内存 |
| 求长度 | O(1) | 返回 mLength |
| 按值查找 | O(n) | 顺序扫描 |
| 按位查找 | O(1) | 下标直接访问 |
| 插入 | O(n) | 后移元素腾出位置 |
| 删除 | O(n) | 前移元素填补空位 |
| 遍历输出 | O(n) | 依次访问 |

***

## (三)关键代码解析

### 1. 插入操作

```cpp
template<typename T>
bool SequenceList<T>::insert(T data, int nIndex) {
    // 合法性检查
    if (nIndex < 0 || nIndex > mLength || mMaxSize == mLength)
        return false;

    // 从后向前移动元素
    for (int nIdx = mLength; nIdx >= nIndex; nIdx--) {
        mData[nIdx + 1] = mData[nIdx];
    }
    mData[nIndex] = data;
    mLength++;
    return true;
}
```

**要点**：必须从后向前移动，否则会覆盖数据。

### 2. 删除操作

```cpp
template<typename T>
bool SequenceList<T>::remove(T data) {
    int nLen = mLength;
    for (int nIndex = 0; nIndex < mLength; ) {
        if (mData[nIndex] == data) {
            for (int nIdx = nIndex; nIdx < mLength; nIdx++) {
                mData[nIdx] = mData[nIdx + 1];
            }
            mLength--;
            continue; // 同位置再次检查，处理重复值
        }
        nIndex++;
    }
    return nLen > mLength;
}
```

***

## (四)源码获取

本节完整源码位于：`assets/datastructure/`

| 文件 | 说明 | 链接 |
|------|------|------|
| `SequenceList.h` | 顺序表模板类定义 | [点击查看](datastructure/SequenceList.h) |
| `SequenceListTest.cpp` | 测试用例 | [点击查看](datastructure/SequenceListTest.cpp) |

***

## (五)习题与思考

1. **顺序表的缺点是什么？** 答：插入删除需移动大量元素；容量固定或扩容开销大；存储密度虽高但内存需连续。
2. **如何实现动态扩容？** 答：当 `mLength == mMaxSize` 时，申请 2 倍内存，拷贝数据，释放旧内存。
3. **顺序表适合哪些场景？** 答：数据量较小、查找频繁、增删较少的场景。

***

> 下一节：[二、链表](/data-structures/datastructures03-linked-list/) — 单链表、双链表、循环链表