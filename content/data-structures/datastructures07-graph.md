---
title: "Data Structures07 : Graph"
weight: 7
description: "图的邻接矩阵存储、DFS/BFS 遍历、最短路径与 C++ 实现"
date: 2026-09-24
tags: ["Data Structure"]
featureimage: "covers/datastructures07-graph.svg"
---

> 参考教材：严蔚敏《数据结构》、邓俊辉《数据结构》、清华大学数据结构 MOOC、算法导论 (CLRS)

图由顶点集合 V 和边集合 E 组成，边可有向/无向、带权/不带权。

***

# 一、存储结构

## (一) 邻接矩阵

```cpp
template<typename T>
class AdjacentMatrixGraph {
private:
    struct Vertex { T data; bool visited; };
    Vertex *mpVertex;        // 顶点表
    int **mpMatrix;          // 邻接矩阵
    int mVertexNum, mEdgeNum;
    bool mDirected;          // 有向/无向
};
```

- **空间**：O(n²)，适合**稠密图**（边数接近 n²）
- **查边/判断邻接**：O(1)
- **求度**：出度=行和，入度=列和（有向图）

## (二) 邻接表（对比）

| 操作 | 邻接矩阵 | 邻接表 |
|------|----------|--------|
| 空间 | O(n²) | O(n+e) |
| 判断邻接 | O(1) | O(deg(v)) |
| 遍历邻接点 | O(n) | O(deg(v)) |
| 适用图 | 稠密 | 稀疏 |

***

# 二、图的遍历

## (一) 深度优先搜索 (DFS)

```cpp
template<typename T>
void AdjacentMatrixGraph<T>::DFS(int v) {
    mpVertex[v].visited = true;
    visit(v); // 访问顶点
    for (int w = firstAdj(v); w >= 0; w = nextAdj(v, w)) {
        if (!mpVertex[w].visited) DFS(w);
    }
}

template<typename T>
void AdjacentMatrixGraph<T>::DFSTraverse() {
    for (int i = 0; i < mVertexNum; i++) mpVertex[i].visited = false;
    for (int i = 0; i < mVertexNum; i++)
        if (!mpVertex[i].visited) DFS(i); // 非连通图
}
```

- **递归栈深度**：O(n)
- **非连通图**：需外层循环保证全覆盖

## (二) 广度优先搜索 (BFS)

```cpp
template<typename T>
void AdjacentMatrixGraph<T>::BFS(int v) {
    queue<int> q;
    mpVertex[v].visited = true;
    visit(v);
    q.push(v);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int w = firstAdj(u); w >= 0; w = nextAdj(u, w)) {
            if (!mpVertex[w].visited) {
                mpVertex[w].visited = true;
                visit(w);
                q.push(w);
            }
        }
    }
}
```

- **队列**：O(n) 空间
- **层序访问**：适合求无权图最短路径

***

# 三、典型算法

## (一) 拓扑排序（有向无环图 DAG）

```cpp
template<typename T>
bool AdjacentMatrixGraph<T>::topologicalSort(vector<int> &result) {
    vector<int> indeg(mVertexNum, 0);
    for (int i = 0; i < mVertexNum; i++)
        for (int j = 0; j < mVertexNum; j++)
            if (mpMatrix[i][j] != 0 && mpMatrix[i][j] != INF)
                indeg[j]++;

    queue<int> q;
    for (int i = 0; i < mVertexNum; i++)
        if (indeg[i] == 0) q.push(i);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        result.push_back(u);
        for (int w = firstAdj(u); w >= 0; w = nextAdj(u, w))
            if (--indeg[w] == 0) q.push(w);
    }
    return result.size() == mVertexNum; // 有环则 false
}
```

## (二) Dijkstra 单源最短路径（非负权）

```cpp
template<typename T>
void AdjacentMatrixGraph<T>::dijkstra(int src, vector<int> &dist, vector<int> &path) {
    dist.assign(mVertexNum, INF);
    path.assign(mVertexNum, -1);
    vector<bool> collected(mVertexNum, false);
    dist[src] = 0;

    for (int i = 0; i < mVertexNum; i++) {
        int u = -1, minD = INF;
        for (int j = 0; j < mVertexNum; j++)
            if (!collected[j] && dist[j] < minD) { minD = dist[j]; u = j; }
        if (u == -1) break;
        collected[u] = true;

        for (int w = firstAdj(u); w >= 0; w = nextAdj(u, w)) {
            int weight = mpMatrix[u][w];
            if (!collected[w] && dist[u] + weight < dist[w]) {
                dist[w] = dist[u] + weight;
                path[w] = u;
            }
        }
    }
}
```

- **时间**：O(n²)（邻接矩阵 + 数组选最小），堆优化可达 O((n+e)log n)

## (三) Floyd 多源最短路径

```cpp
template<typename T>
void AdjacentMatrixGraph<T>::floyd(vector<vector<int>> &dist, vector<vector<int>> &path) {
    dist = mpMatrix; // 复制邻接矩阵
    path.assign(mVertexNum, vector<int>(mVertexNum, -1));
    for (int i = 0; i < mVertexNum; i++)
        for (int j = 0; j < mVertexNum; j++)
            if (dist[i][j] != INF && i != j) path[i][j] = i;

    for (int k = 0; k < mVertexNum; k++)
        for (int i = 0; i < mVertexNum; i++)
            for (int j = 0; j < mVertexNum; j++)
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    path[i][j] = path[k][j];
                }
}
```

- **时间**：O(n³)，适合稠密图全对最短路径

## (四) Prim 最小生成树

```cpp
template<typename T>
int AdjacentMatrixGraph<T>::prim(int src, vector<pair<int,int>> &mst) {
    vector<int> lowcost(mVertexNum, INF);
    vector<int> adjvex(mVertexNum, -1);
    vector<bool> inMST(mVertexNum, false);
    lowcost[src] = 0;

    for (int i = 0; i < mVertexNum; i++) {
        int u = -1, minC = INF;
        for (int j = 0; j < mVertexNum; j++)
            if (!inMST[j] && lowcost[j] < minC) { minC = lowcost[j]; u = j; }
        if (u == -1) break;
        inMST[u] = true;
        if (adjvex[u] != -1) mst.emplace_back(adjvex[u], u);

        for (int w = firstAdj(u); w >= 0; w = nextAdj(u, w)) {
            int weight = mpMatrix[u][w];
            if (!inMST[w] && weight < lowcost[w]) {
                lowcost[w] = weight;
                adjvex[w] = u;
            }
        }
    }
    int sum = 0;
    for (auto &e : mst) sum += mpMatrix[e.first][e.second];
    return sum;
}
```

***

# 四、源码获取

| 文件 | 说明 | 链接 |
|------|------|------|
| `AdjacentMatrixGraph.h` | 邻接矩阵图（含 DFS/BFS/拓扑/Dijkstra/Floyd/Prim） | [点击查看](datastructure/AdjacentMatrixGraph.h) |
| `AdjacentMatrixGraphTest.cpp` | 测试用例 | [点击查看](datastructure/AdjacentMatrixGraphTest.cpp) |

***

# 五、习题

1. **DFS 和 BFS 的核心区别？** 答：DFS 用栈（递归/显式），深度优先；BFS 用队列，层序展开。
2. **Dijkstra 为什么不能处理负权边？** 答：贪心策略假设“已确定的最短路径不会被更新”，负权边会破坏该性质。
3. **Floyd 算法适合什么场景？** 答：稠密图、需要所有顶点对最短路径、n 较小（≤500）。
4. **Prim 和 Kruskal 的区别？** 答：Prim 适合稠密图（O(n²)），Kruskal 适合稀疏图（O(e log e)）。

***

> 上一节：[六、线索二叉树](/data-structures/datastructures06-threaded-binary-tree/)  
> 下一节：[八、哈夫曼编码](/data-structures/datastructures08-huffman-code/)