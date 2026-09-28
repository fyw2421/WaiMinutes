#include "AdjacentMatrixGraph.h"

int main(){
//    src:https://www.cnblogs.com/weizhixiang/p/5815994.html
    string szUDGVexs[]={"V1","V2","V3","V4","V5","V6","V7","V8"};
    string edgeUDGHeadTailVex[][2]={
        {"V1","V2"},
        {"V1","V3"},
        {"V2","V4"},
        {"V4","V8"},
        {"V8","V5"},
        {"V5","V2"},
        {"V3","V6"},
        {"V6","V7"},
        {"V7","V3"},
    };
    //src:https://zhidao.baidu.com/question/1238441103677962139.html
    string szDGVexs[]={"a","b","c","d","e","f","g","h","i"};
    string edgeDGHeadTailVex[][2]={
        {"a","b"},
        {"a","c"},
        {"b","d"},
        {"c","d"},
        {"d","e"},
        {"e","f"},
        {"e","g"},
        {"e","h"},
        {"g","h"},
        {"f","i"},
        {"h","i"},
        {"i","g"},
    };
    AdjacentMatrixGraph adjacentMatrixGraph;
    adjacentMatrixGraph.createDG(szDGVexs,edgeDGHeadTailVex,sizeof(szDGVexs)/sizeof(szDGVexs[0]),sizeof(edgeDGHeadTailVex)/sizeof(edgeDGHeadTailVex[0]));
    cout<<"Adjacent Matrix Digraph : "<<endl;
    adjacentMatrixGraph.printMatrix();
    cout<<"Deep First Search : "<<endl;
    adjacentMatrixGraph.DFS();
    cout<<"Breadth First Search :"<<endl;
    adjacentMatrixGraph.BFS();
    cout<<endl<<endl;

    adjacentMatrixGraph.createUDG(szUDGVexs,edgeUDGHeadTailVex,sizeof(szUDGVexs)/sizeof(szUDGVexs[0]),sizeof(edgeUDGHeadTailVex)/sizeof(edgeUDGHeadTailVex[0]));
    cout<<"Adjacent Matrix undirected graph : "<<endl;
    adjacentMatrixGraph.printMatrix();
    cout<<"Deep First Search : "<<endl;
    adjacentMatrixGraph.DFS();
    cout<<"Breadth First Search :"<<endl;
    adjacentMatrixGraph.BFS();
    return 1;
}
