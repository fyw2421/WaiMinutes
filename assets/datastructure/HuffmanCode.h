//============================
//HuffmanCode.h
//============================
#ifndef HUFFMANCODE_H
#define HUFFMANCODE_H

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <algorithm>
#include <iomanip>
using namespace std;

class HuffmanCode{
public:
    HuffmanCode():mpHuffmanTree(nullptr){}
    ~HuffmanCode(){
        if( mpHuffmanTree )
            delete[] mpHuffmanTree;
            mpHuffmanTree = nullptr;
    }
    void code(string codedStr);
    void code(int weightArr[],string strCode,int nStrCodeCount);
    void decode(string encodeStr);
private:
    struct Node{
        char code;
        int weight;
        int parent,lchild,rchild;
    };
    Node *mpHuffmanTree;
    int mStrCodeCount;
private:
    void __count(int *&weightArr,string &strCode,int &nStrCodeCount,string codeStr);
    void __createHuffmanTree(Node *&pHuffmanTree,int weightArr[],string strCode,int nStrCodeCount);
    void __reverseHuffmanCode(Node *pHuffmanTree,vector<pair<string,char>> &huffmanCodeVec,int nWeightArrSize);
    void __forwardHuffmanCode(Node *pHuffmanTree,vector<pair<string,char>> &huffmanCodeVec,int nWeightArrSize);
    void __select(Node *pHuffmanTree,int nRange,int &n1stMinWeightIndex,int &n2ndMinWightIndex);
    void __printHuffmanTree(Node *pHuffmanTree,int nHuffmanTreeSize);
};

void HuffmanCode::code(string codedStr){
    string strCode;
    int *weightArr = nullptr;
    __count(weightArr,strCode,mStrCodeCount,codedStr);
    cout<<"weight and strCode "<<endl;
    for( int i = 0 ; i < mStrCodeCount ; i++ ){
        cout<<strCode.at(i)<<" ";
        cout<<weightArr[ i ]<<endl;
    }
    cout<<endl;
    __createHuffmanTree(mpHuffmanTree,weightArr,strCode,mStrCodeCount);
    if( weightArr ){ delete[] weightArr; weightArr = nullptr; }

    cout<<endl;
    cout<<"reverse code-(key,code)"<<endl;
    vector<pair<string,char>> huffmanCodeVec;
    __reverseHuffmanCode(mpHuffmanTree,huffmanCodeVec,mStrCodeCount);
    for( auto it : huffmanCodeVec)
        cout<<it.second<<"---"<<it.first<<endl;

    __forwardHuffmanCode(mpHuffmanTree,huffmanCodeVec,mStrCodeCount);
    cout<<endl;
    cout<<"forward code"<<endl;
    for( auto it : huffmanCodeVec)
        cout<<it.second<<"---"<<it.first<<endl;
}

void HuffmanCode::code(int weightArr[],string strCode,int nStrCodeCount){

    mStrCodeCount = nStrCodeCount;
    __createHuffmanTree(mpHuffmanTree,weightArr,strCode,nStrCodeCount);

    cout<<endl;
    cout<<"reverse code-(key,code)"<<endl;
    vector<pair<string,char>> huffmanCodeVec;
    __reverseHuffmanCode(mpHuffmanTree,huffmanCodeVec,nStrCodeCount);
    for( auto it : huffmanCodeVec)
        cout<<it.second<<"---"<<it.first<<endl;

    __forwardHuffmanCode(mpHuffmanTree,huffmanCodeVec,nStrCodeCount);
    cout<<endl;
    cout<<"forward code"<<endl;
    for( auto it : huffmanCodeVec)
        cout<<it.second<<"---"<<it.first<<endl;
}

void HuffmanCode::decode(string decodeStr){
    if( mpHuffmanTree == nullptr)
        return;
    vector<char> codeVec;
    auto strIt = decodeStr.begin();
    while( strIt != decodeStr.end() ){
        int nTreeIndex = mStrCodeCount*2 - 1;
        while( mpHuffmanTree[ nTreeIndex ].lchild != 0 && mpHuffmanTree[ nTreeIndex ].rchild != 0 ){
            if( *strIt == '0' )
                nTreeIndex = mpHuffmanTree[ nTreeIndex ].lchild;
            else
                nTreeIndex = mpHuffmanTree[ nTreeIndex ].rchild;
            strIt++;
        }
        codeVec.push_back(mpHuffmanTree[ nTreeIndex ].code);
    }
    cout<<"Decode : "<<decodeStr<<endl;
    for( auto it : codeVec)
        cout<<it;
}

void HuffmanCode::__count(int *&weightArr,string &strCode,int &nStrCodeCount,string codeStr){
    map<char,int> weightCodeMap;
    for( unsigned nIdx = 0 ; nIdx < codeStr.length() ; nIdx++ ){
        auto ch = codeStr.at(nIdx);
        auto it = weightCodeMap.find(ch);
        if( it != weightCodeMap.end() )
            it->second += 1;
        else
            weightCodeMap.insert( make_pair(ch,1));
    }

    nStrCodeCount = weightCodeMap.size();
    if( weightArr ) { delete weightArr; }
    weightArr = new int[ nStrCodeCount ];
    int *p = weightArr;
    for( auto it = weightCodeMap.begin() ; it != weightCodeMap.end() ; it++){
        *p++ = it->second;
        strCode += it->first;
    }
}

void HuffmanCode::__createHuffmanTree(Node *&pHuffmanTree,int weightArr[],string strCode,int nStrCodeCount){
    if( nStrCodeCount < 1 ) return;
    if( pHuffmanTree ) { delete[] pHuffmanTree; }

    //malloc and init
    int nHuffmanTreeSize = nStrCodeCount*2 - 1 + 1 ;
    pHuffmanTree = new Node[ nHuffmanTreeSize  ];   // index 0 is unused
    auto strIter = strCode.begin();

    //set weight for leaf
    Node *pHT = pHuffmanTree + 1;
    for( int nIdx = 1 ; nIdx <= nStrCodeCount && strIter != strCode.end(); nIdx++ , pHT++ , strIter++ ,weightArr++)
        *pHT = { *strIter , *weightArr , 0 , 0 , 0 };
    for( int nIdx = nStrCodeCount + 1 ; nIdx < nHuffmanTreeSize ; nIdx++ , pHT++)
        *pHT = { '.',0,0,0,0};

    cout<<"Forest :"<<endl;
    __printHuffmanTree(pHuffmanTree,nHuffmanTreeSize);

    //create huffman tree
    int n1stMinWeightIndex = 0,n2ndMinWightIndex=0;
    for( int nIdx = nStrCodeCount + 1 ; nIdx < nHuffmanTreeSize ; nIdx++){
        __select(pHuffmanTree,nIdx-1,n1stMinWeightIndex,n2ndMinWightIndex);
        pHuffmanTree[ n1stMinWeightIndex ].parent = nIdx;
        pHuffmanTree[ n2ndMinWightIndex ].parent = nIdx;
        pHuffmanTree[ nIdx ].weight = pHuffmanTree[ n1stMinWeightIndex ].weight + pHuffmanTree[ n2ndMinWightIndex ].weight;
        pHuffmanTree[ nIdx ].lchild = n1stMinWeightIndex;
        pHuffmanTree[ nIdx ].rchild = n2ndMinWightIndex;
    }

    cout<<endl;
    cout<<"Huffman Tree : "<<endl;
    __printHuffmanTree(pHuffmanTree,nHuffmanTreeSize);
}

void HuffmanCode::__reverseHuffmanCode(Node *pHuffmanTree,vector<pair<string,char>> &huffmanCodeVec,int nStrCodeCount){
    if( nStrCodeCount <= 1 ) return;
    if( pHuffmanTree == nullptr ) return;
    huffmanCodeVec.clear();

    //huffman coding from leaf
    string cdStr;
    for( int nLeafIdx = 1 ; nLeafIdx <= nStrCodeCount ; nLeafIdx++){
        cdStr.clear();
        for( int nChildIdx = nLeafIdx , nParentIdx = pHuffmanTree[ nChildIdx ].parent ; nParentIdx != 0 ; nChildIdx = nParentIdx , nParentIdx = pHuffmanTree[ nChildIdx ].parent){
            if( pHuffmanTree[ nParentIdx ].lchild == nChildIdx )
                cdStr.insert(0,"0");
            else
                cdStr.insert(0,"1");
        }
        huffmanCodeVec.push_back( make_pair(cdStr,pHuffmanTree[nLeafIdx].code) );
    }
}

void HuffmanCode::__forwardHuffmanCode(Node *pHuffmanTree,vector<pair<string,char>> &huffmanCodeVec,int nStrCodeCount){
    if( nStrCodeCount <= 1 ) return;
    if( pHuffmanTree == nullptr ) return;
    huffmanCodeVec.clear();

    int nHuffmanTreeSize = nStrCodeCount*2;
    int nParent = nHuffmanTreeSize - 1;
    string cdStr;
    for( int nIdx = 1; nIdx < nHuffmanTreeSize ; nIdx++) pHuffmanTree[ nIdx ].weight = 0;
    while( nParent ){
        if( pHuffmanTree[ nParent].weight == 0 ){
            pHuffmanTree[ nParent ].weight = 1;
            if( pHuffmanTree[ nParent ].lchild != 0 ){
                nParent = pHuffmanTree[ nParent ].lchild;
                cdStr += "0";
            }else if( pHuffmanTree[ nParent].rchild == 0 )
                huffmanCodeVec.push_back(make_pair(cdStr,pHuffmanTree[ nParent].code));
        }else if( pHuffmanTree[ nParent ].weight == 1){
            pHuffmanTree[ nParent ].weight = 2;
            if( pHuffmanTree[ nParent ].rchild != 0 ){
                nParent = pHuffmanTree[ nParent ].rchild;
                cdStr += "1";
            }
        }else{
            pHuffmanTree[ nParent ].weight = 0;
            nParent = pHuffmanTree[ nParent ].parent;
            cdStr.pop_back();
        }
    }
}
//1st and 2nd min weight if parent == 0 form huffmantree[1,nRange]
void HuffmanCode::__select(Node *pHuffmanTree,int nRange,int &n1stMinWeightIndex,int &n2ndMinWightIndex){
    n1stMinWeightIndex = n2ndMinWightIndex = -1;
    for( int nIdx = 1; nIdx <= nRange ; nIdx++){
        if( pHuffmanTree[ nIdx ].parent == 0 && n1stMinWeightIndex == -1 )
            n1stMinWeightIndex = nIdx;
        if( pHuffmanTree[ nIdx ].parent == 0 && pHuffmanTree[ nIdx ].weight <= pHuffmanTree[ n1stMinWeightIndex ].weight )
            n1stMinWeightIndex = nIdx;
    }
    for( int nIdx = 1; nIdx <= nRange ; nIdx++){
        if( nIdx == n1stMinWeightIndex) continue;
        if( pHuffmanTree[ nIdx ].parent == 0 && n2ndMinWightIndex == -1 )
            n2ndMinWightIndex = nIdx;
        if( pHuffmanTree[ nIdx ].parent == 0 && pHuffmanTree[ nIdx ].weight <= pHuffmanTree[ n2ndMinWightIndex ].weight )
            n2ndMinWightIndex = nIdx;
    }
    int tmp;
    if( n1stMinWeightIndex > n2ndMinWightIndex ){
        tmp = n1stMinWeightIndex;
        n1stMinWeightIndex = n2ndMinWightIndex;
        n2ndMinWightIndex = tmp;
    }
}

void HuffmanCode::__printHuffmanTree(Node *pHuffmanTree,int nHuffmanTreeSize){
    cout<<setw(5)<<left<<"i"<<setw(5)<<left<<"w"<<setw(5)<<left<<"p"<<setw(5)<<left<<"l"<<setw(5)<<left<<"r"<<endl;
    for( int nIdx = 1; nIdx < nHuffmanTreeSize ; nIdx++ ){
        cout<<setw(5)<<left<<nIdx;
        cout<<setw(5)<<left<<pHuffmanTree[ nIdx ].weight;
        cout<<setw(5)<<left<<pHuffmanTree[ nIdx ].parent;
        cout<<setw(5)<<left<<pHuffmanTree[ nIdx ].lchild;
        cout<<setw(5)<<left<<pHuffmanTree[ nIdx ].rchild;
        cout<<endl;
    }
}
#endif // HUFFMANCODE_H
