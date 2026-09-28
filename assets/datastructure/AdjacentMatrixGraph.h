//============================
//AdjacentMatrixGraph.h
//邻接矩阵图
//https://www.cnblogs.com/ECJTUACM-873284962/p/7900395.html
//https://blog.csdn.net/BastianFei/article/details/79349623
//https://www.cnblogs.com/weizhixiang/p/5815994.html
//https://zhidao.baidu.com/question/1238441103677962139.html
//============================
#ifndef ADJACENTMATRIXGRAPH_H
#define ADJACENTMATRIXGRAPH_H

#include <iostream>
#include <iomanip>
#include <string>
#include <queue>
#include <stack>
using namespace std;

class AdjacentMatrixGraph{
public:
    AdjacentMatrixGraph();
    ~AdjacentMatrixGraph();
private:
    struct MGraph{
        string *vexs;
        int **edges;
        int vexnum,arcnum;
    };
    enum Graph_TYPE{
        DG,
        UDG,
    };
    bool *mVisited;
    MGraph mMatrixGraph;
public:
    void createUDG(string szVexs[],string edgeHeadTailVex[][2],int nVexNum,int nArcNum);  //undirected graph
    void createDG(string szVexs[],string edgeHeadTailVex[][2],int nVexNum,int nArcNum);   //directed graph
    void printMatrix();
    void DFS();
    void BFS();
private:
    void __createGraph(string szVexs[],string edgeHeadTailVex[][2],int nVexNum,int nArcNum,MGraph &mGraph,Graph_TYPE eGraphType);
    int __locateVex(MGraph mGraph,string vex);
    void __print(MGraph mGraph) const;
    int __firstAdjVex(MGraph mGraph,int v);
    int __nextAdjVex(MGraph mGraph,int v,int w);
    void __dfsTraverse(MGraph mGraph);
    void __dfs(MGraph mGraph,int v);
    void __bfs(MGraph mGraph);
    void __destroy(MGraph &MGraph);

    //non-recursive deep first search
    void __dfs_non_recursive(MGraph mGraph);
};

AdjacentMatrixGraph::AdjacentMatrixGraph(){
    mMatrixGraph.vexnum = 0;
    mMatrixGraph.arcnum  = 0;

    mMatrixGraph.vexs = nullptr;
    mMatrixGraph.edges = nullptr;
    mVisited = nullptr;
}
AdjacentMatrixGraph::~AdjacentMatrixGraph(){
    __destroy(mMatrixGraph);
    if( mVisited ){
        delete[] mVisited;
        mVisited = nullptr;
    }
}

void AdjacentMatrixGraph::printMatrix(){
    cout<<setw(mMatrixGraph.vexs[0].size()+1)<<left<<" ";
    for( auto i = 0 ; i < mMatrixGraph.vexnum ; i++)
        cout<<setw(3)<<left<<mMatrixGraph.vexs[i];
    cout<<endl;
    for( auto i = 0 ; i < mMatrixGraph.vexnum ; i++){
        cout<<mMatrixGraph.vexs[i]<<" ";
        for( auto j = 0 ; j < mMatrixGraph.vexnum ; j++ )
            cout<<setw(3)<<left<<mMatrixGraph.edges[ i ][ j ];
        cout<<endl;
    }
    cout<<endl;
}

void AdjacentMatrixGraph::createUDG(string szVexs[],string edgeHeadTailVex[][2],int nVexNum,int nArcNum){
    __destroy(mMatrixGraph);
    if( mVisited ){
        delete[] mVisited;
        mVisited = nullptr;
    }
    __createGraph(szVexs,edgeHeadTailVex,nVexNum,nArcNum,mMatrixGraph,UDG);
}
void AdjacentMatrixGraph::createDG(string szVexs[],string edgeHeadTailVex[][2],int nVexNum,int nArcNum){
    __destroy(mMatrixGraph);
    if( mVisited ){
        delete[] mVisited;
        mVisited = nullptr;
    }
    __createGraph(szVexs,edgeHeadTailVex,nVexNum,nArcNum,mMatrixGraph,DG);
}

void AdjacentMatrixGraph::DFS(){
    cout<<"Recursive: "<<endl;
    __dfsTraverse(mMatrixGraph);
    cout<<"Non-Recursive: "<<endl;
    __dfs_non_recursive(mMatrixGraph);
    cout<<endl<<endl;
}

void AdjacentMatrixGraph::BFS(){
    __bfs(mMatrixGraph);
}

void AdjacentMatrixGraph::__createGraph(string szVexs[],string edgeHeadTailVex[][2],int nVexNum,int nArcNum,MGraph &mGraph,Graph_TYPE eGraphType){
    mGraph.vexnum = nVexNum;
    mGraph.arcnum  = nArcNum;
    mGraph.vexs = new string[ mGraph.vexnum ];
    mGraph.edges = new int*[ mGraph.vexnum ];
    for( int i = 0 ; i < mGraph.vexnum ; i++)
        mGraph.edges[ i ] = new int[ mGraph.vexnum ];

    for( int i = 0 ; i < mGraph.vexnum ; i++){
        mGraph.vexs[ i ] = szVexs[ i ];
            for( int j = 0 ; j < mGraph.vexnum ; j++)
        mGraph.edges[ i ][ j ] = 0;
    }
    for( int i = 0 ; i < mGraph.arcnum ; i++){

        int loc1 = __locateVex(mGraph,edgeHeadTailVex[i][0]);
        int loc2 = __locateVex(mGraph,edgeHeadTailVex[i][1]);
        mGraph.edges[loc1][loc2] = 1;
        if( eGraphType == UDG )
            mGraph.edges[loc2][loc1] = 1;
    }

    mVisited = new bool[ mGraph.vexnum ];
    for( auto i = 0 ; i < mGraph.vexnum ; i++ )
        mVisited[ i ] = false;
}
int AdjacentMatrixGraph::__locateVex(MGraph mGraph,string vex){
    for( int i = 0 ; i < mGraph.vexnum ; i++ ){
        if( mGraph.vexs[ i ] == vex )
            return i;
    }
    return 0;
}

int AdjacentMatrixGraph::__firstAdjVex(MGraph mGraph,int v){
    for( int i = 0 ; i < mGraph.vexnum ; i++){
        if( mGraph.edges[ v ][ i ] == 1 )
            return i;
    }
    return -1;
}

int AdjacentMatrixGraph::__nextAdjVex(MGraph mGraph,int v,int w){
    for( int i = w + 1 ; i < mGraph.vexnum ; i++){
        if( mGraph.edges[ v ][ i ] == 1 )
            return i;
    }
    return -1;
}

void AdjacentMatrixGraph::__dfsTraverse(MGraph mGraph){
    for( auto i = 0 ; i < mGraph.vexnum ; i++ )
        mVisited[ i ] = false;
    for( int v = 0 ; v < mGraph.vexnum ; v++){
        if( mVisited[ v ] == false )
            __dfs(mGraph,v);
    }
    cout<<endl;
}

void AdjacentMatrixGraph::__dfs(MGraph mGraph,int v){
    mVisited[ v ] = true;
    cout<<mGraph.vexs[v]<<" ";
    for( int w = __firstAdjVex(mGraph,v) ; w >= 0 ; w = __nextAdjVex(mGraph,v,w) ){
        if( mVisited[ w ] == false )
            __dfs(mGraph,w);
    }
}

void AdjacentMatrixGraph::__dfs_non_recursive(MGraph mGraph){
    stack<int> vexStack;
    for( auto i = 0 ; i < mGraph.vexnum ; i++ )
        mVisited[ i ] = false;
    vexStack.push(0);
    mVisited[ 0 ] = true;
    cout<<mGraph.vexs[0]<<" ";
    while(!vexStack.empty()){
        int v = vexStack.top();
        int w = 0 ;
        for( w = 0 ; w < mGraph.vexnum ; w++){
            if( mVisited[ w ] == false && mGraph.edges[v][w] == 1 ){
                mVisited[ w ] = true;
                vexStack.push(w);
                cout<<mGraph.vexs[ w ]<<" ";
                break;
            }
        }
        if( w == mGraph.vexnum )
            vexStack.pop();
    }
}
void AdjacentMatrixGraph::__bfs(MGraph mGraph){
    for( auto i = 0 ; i < mMatrixGraph.vexnum ; i++ )
        mVisited[ i ] = false;
    queue<int> vexQueue;
    for( auto v = 0 ; v < mGraph.vexnum ; v++){
        if( mVisited[ v ] == false){
            mVisited[ v ] = true;
            cout<<mGraph.vexs[ v ]<<" ";
            vexQueue.push(v);
            while( !vexQueue.empty() ){
                int u = vexQueue.front();
                vexQueue.pop();
                for( int w = __firstAdjVex(mGraph,u) ; w >= 0 ; w = __nextAdjVex(mGraph,u,w) ){
                    if( mVisited[ w ] == false){
                        mVisited[ w ] = true;
                        cout<<mGraph.vexs[ w ]<<" ";
                        vexQueue.push(w);
                    }
                }
            }
        }
    }
    cout<<endl;
}

void AdjacentMatrixGraph::__destroy(MGraph &mGraph){
    if( mGraph.vexs ){
        delete mGraph.vexs;
        mGraph.vexs = nullptr;
    }
    for( auto i = 0 ; i < mGraph.vexnum ; i++ ){
        if( mGraph.edges[ i ]){
            delete[] mGraph.edges[ i ];
            mGraph.edges[ i ] = nullptr;
        }
    }
    if( mGraph.edges ){
        delete[] mGraph.edges;
        mGraph.edges = nullptr;
    }
    mGraph.vexnum = 0 ;
    mGraph.arcnum = 0 ;
}

#endif // ADJACENTMATRIXGRAPH_H
