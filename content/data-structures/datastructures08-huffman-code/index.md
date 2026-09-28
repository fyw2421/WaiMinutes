---
title: "Data Structures08 : Huffman Coding"
weight: 8
description: "哈夫曼编码的构建原理、最优前缀码与 C++ 实现"
date: 2026-09-24
tags: ["Data Structure"]
featureimage: "covers/datastructures08-huffman-code.svg"
---

> 参考教材：严蔚敏《数据结构》、邓俊辉《数据结构》、信息论基础 (Shannon)、清华大学数据结构 MOOC

哈夫曼编码是一种**最优前缀码**，根据字符出现频率构建二叉树，频率高的字符编码短，频率低的编码长，实现无损压缩。

***

# 一、基本原理

## (一) 前缀码

**定义**：没有任何编码是另一个编码的前缀。
- **解码唯一**：从左向右读，匹配即输出，无歧义
- **二叉树表示**：字符仅在叶子结点，左分支=0，右分支=1

## (二) 哈夫曼树构建（贪心算法）

```text
输入：n 个权值 w1, w2, ..., wn
输出：带权路径长度 (WPL) 最小的二叉树

算法：
1. 将 n 个权值各自构成单结点树，放入最小堆
2. 重复 n-1 次：
   a. 取出权值最小的两棵树 T1, T2
   b. 新建根结点，权值 = T1.w + T2.w，左=T1，右=T2
   c. 新树入堆
3. 堆中剩最后一棵树即为哈夫曼树
```

**最优性证明**：贪心选择性质 + 最优子结构性质（CLRS 16.3 节）。

***

# 二、编码与解码

## (一) 编码生成（从根到叶子）

```cpp
template<typename T>
void HuffmanCode<T>::generateCodes(Node *p, string code) {
    if (p->lChild == nullptr && p->rChild == nullptr) {
        mpCodeMap[p->data] = code; // 叶子存编码
        return;
    }
    if (p->lChild) generateCodes(p->lChild, code + "0");
    if (p->rChild) generateCodes(p->rChild, code + "1");
}
```

## (二) 编码压缩

```cpp
string HuffmanCode<T>::encode(const string &text) {
    string result;
    for (char c : text) result += mpCodeMap[c];
    return result; // 如 "abac" → "010011"
}
```

## (三) 解码（从根走向叶子）

```cpp
string HuffmanCode<T>::decode(const string &bits) {
    string result;
    Node *p = mpRoot;
    for (char bit : bits) {
        p = (bit == '0') ? p->lChild : p->rChild;
        if (p->lChild == nullptr && p->rChild == nullptr) {
            result += p->data;
            p = mpRoot; // 回根继续
        }
    }
    return result;
}
```

***

# 三、关键数据结构

```cpp
template<typename T>
class HuffmanCode {
private:
    struct Node {
        T data;       // 字符（仅叶子有效）
        int weight;   // 频率/权值
        Node *lChild, *rChild;
    };
    Node *mpRoot;
    unordered_map<T, string> mpCodeMap; // 字符→编码
    unordered_map<T, int> mpFreqMap;    // 字符→频率
};
```

**构建流程**：
1. 统计频率 → `mpFreqMap`
2. 建最小堆（优先队列）存 `Node*`
3. 贪心合并 → `mpRoot`
4. 遍历生成编码表 → `mpCodeMap`

***

# 四、复杂度分析

| 操作 | 时间复杂度 | 空间复杂度 |
|------|------------|------------|
| 频率统计 | O(n) | O(σ) σ=字符集大小 |
| 建堆 | O(σ) | O(σ) |
| 合并 n-1 次 | O(σ log σ) | O(σ) |
| 生成编码 | O(σ) | O(σ) |
| 编码文本 | O(n) | O(n) |
| 解码文本 | O(m) m=编码长度 | O(1) 辅助 |

**总体**：O(n + σ log σ)，σ 通常 ≤ 256，极快。

***

# 五、带权路径长度 (WPL)

$$WPL = \sum_{i=1}^{n} w_i \times l_i$$

- $w_i$：第 i 个字符的权值（频率）
- $l_i$：第 i 个字符的编码长度（叶子深度）

**哈夫曼树使 WPL 最小**，即平均编码长度最短。

***

# 六、源码获取

| 文件 | 说明 | 链接 |
|------|------|------|
| `HuffmanCode.h` | 哈夫曼编码完整实现（统计频率、建树、编码、解码、压缩率计算） | [点击查看](datastructure/HuffmanCode.h) |
| `HuffmanCodeTest.cpp` | 测试用例 | [点击查看](datastructure/HuffmanCodeTest.cpp) |

***

# 七、习题

1. **为什么哈夫曼编码是前缀码？** 答：字符只在叶子，无字符是另一个的祖先，故无编码为前缀。
2. **权值相同的字符，哈夫曼树唯一吗？** 答：不唯一，但 WPL 相同，均为最优。
3. **如何处理只出现 1 种字符的情况？** 答：单结点树，编码为 "0"（或约定为 "1"），解码需特判。
4. **哈夫曼编码 vs 定长编码（如 ASCII）压缩率如何？** 答：频率分布越不均匀，压缩率越高；均匀分布时接近定长。

***

# 八、扩展：自适应哈夫曼编码

静态哈夫曼需两遍扫描（统计+编码），**自适应哈夫曼 (FGK 算法 / Vitter 算法)** 一遍扫描，动态维护树，适合流式压缩。

***

> 上一节：[七、图](/data-structures/datastructures07-graph/)  
> **数据结构教程完结** ✅