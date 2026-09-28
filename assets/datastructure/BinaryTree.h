//============================
//BinaryTree.h
//二叉树
//============================
#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <iostream>
#include <stack>
#include <iomanip>
using namespace std;

template<typename T>
class BinaryTree{
public:
    BinaryTree();
    virtual ~BinaryTree();

    void create(T *&pData = nullptr , T emptySubTreeValue = -1);
    void destroy();

    void preOrder();
    void inOrder();
    void postOrder();

    int getSize();
    int getHeight();
    int getLeaf(); //lchild and rchild = null
private:
    struct Node{
        T data;
        Node *lChild;
        Node *rChild;
    };
    Node *mpRoot;
private:
    //The recursive traversal
    void __preOrder(Node *pRoot);
    void __inOrder(Node *pRoot);
    void __postOrder(Node *pRoot);

    //Non-recursive traversal
    void __nonRecurPreOrder(Node *pRoot);
    void __nonRecurInOrder(Node *pRoot);
    void __nonRecurPostOrder(Node *pRoot);

    int __getSize(Node *pRoot);
    int __getHeight(Node *pRoot);
    int __getLeaf(Node *pRoot);

    Node *__create(T *&pData ,T emptySubTreeValue);
    void __destroy(Node* &pRoot);
};

template<typename T>
BinaryTree<T>::BinaryTree():mpRoot(nullptr){
}

template<typename T>
BinaryTree<T>::~BinaryTree(){
    destroy();
}

template<typename T>
void BinaryTree<T>::create(T *&pData,T emptySubTreeValue){
    if( mpRoot == nullptr)
        mpRoot = __create(pData,emptySubTreeValue);
}

template<typename T>
void BinaryTree<T>::destroy(){
    __destroy(mpRoot);
}

template<typename T>
void BinaryTree<T>::preOrder(){
    cout<<setw(17)<<left<<"Recursive : ";
    __preOrder(mpRoot);
    cout<<endl;
    cout<<setw(17)<<left<<"Non-recursive : ";
    __nonRecurPreOrder(mpRoot);
}

template<typename T>
void BinaryTree<T>::inOrder(){
    cout<<setw(17)<<left<<"Recursive : ";
    __inOrder(mpRoot);
    cout<<endl;
    cout<<setw(17)<<left<<"Non-recursive : ";
    __nonRecurInOrder(mpRoot);
}

template<typename T>
void BinaryTree<T>::postOrder(){
    cout<<setw(17)<<left<<"Recursive : ";
    __postOrder(mpRoot);
    cout<<endl;
    cout<<setw(17)<<left<<"Non-recursive : ";
    __nonRecurPostOrder(mpRoot);
}

template<typename T>
int BinaryTree<T>::getSize(){
    return __getSize(mpRoot);
}

template<typename T>
int BinaryTree<T>::getHeight(){
    return __getHeight(mpRoot);
}

template<typename T>
int BinaryTree<T>::getLeaf(){
    return __getLeaf(mpRoot);
}

template<typename T>
void BinaryTree<T>::__preOrder(Node *pRoot){
    if( pRoot ){
        cout<<pRoot->data<<" ";
        __preOrder(pRoot->lChild);
        __preOrder(pRoot->rChild);
    }
}

template<typename T>
void BinaryTree<T>::__inOrder(Node *pRoot){
    if( pRoot ){
        __inOrder(pRoot->lChild);
        cout<<pRoot->data<<" ";
        __inOrder(pRoot->rChild);
    }
}

template<typename T>
void BinaryTree<T>::__postOrder(Node *pRoot){
    if( pRoot ){
        __postOrder(pRoot->lChild);
        __postOrder(pRoot->rChild);
        cout<<pRoot->data<<" ";
    }
}

template<typename T>
void BinaryTree<T>::__nonRecurPreOrder(Node *pRoot){
    stack<Node*> nodeStack;
    Node *pNode = pRoot;
    while( pNode || !nodeStack.empty()){
        if( pNode ){
            cout<<pNode->data<<" ";
            nodeStack.push(pNode);
            pNode = pNode->lChild;
        }
        else{
            pNode = nodeStack.top();
            nodeStack.pop();
            pNode = pNode->rChild;
        }
    }
}

template<typename T>
void BinaryTree<T>::__nonRecurInOrder(Node *pRoot){
    stack<Node*> nodeStack;
    Node *pNode = pRoot;
    while( pNode || !nodeStack.empty()){
        if( pNode ){
            nodeStack.push(pNode);
            pNode = pNode->lChild;
        }
        else{
            pNode = nodeStack.top();
            cout<<pNode->data<<" ";
            nodeStack.pop();
            pNode = pNode->rChild;
        }
    }
}

template<typename T>
void BinaryTree<T>::__nonRecurPostOrder(Node *pRoot){
    stack<Node*> nodeStack;

    //current node poionter
    Node *pNode = pRoot;

    //node pointer that had been accessed
    Node *pPreNode = nullptr;

    //push root node into stack
    nodeStack.push(pNode);
    while( !nodeStack.empty()){
        pNode = nodeStack.top();

        //have no lchild&&rchild or node had been accessed
        if( ( pNode->lChild == nullptr && pNode->rChild == nullptr ) || ( pPreNode && ( pPreNode == pNode->lChild || pPreNode == pNode->rChild ) ) ){
            cout<<pNode->data<<" ";
            nodeStack.pop();
            pPreNode = pNode;
        }
        else{
            if( pNode->rChild )
                nodeStack.push(pNode->rChild);
            if( pNode->lChild )
                nodeStack.push(pNode->lChild);
        }
    }
}

template<typename T>
int BinaryTree<T>::__getSize(Node *pRoot){
    if( pRoot ){
        return __getSize(pRoot->lChild) + __getSize(pRoot->rChild) + 1;
    }
    return 0;
}

template<typename T>
int BinaryTree<T>::__getHeight(Node *pRoot){
    if( pRoot ){
        int nLeftHeight = __getHeight(pRoot->lChild);
        int nRightHeight = __getHeight(pRoot->rChild);
        return ( nLeftHeight >= nRightHeight ) ? nLeftHeight + 1 : nRightHeight + 1;
    }
    return 0;
}

template<typename T>
int BinaryTree<T>::__getLeaf(Node *pRoot){
    if( pRoot ){
        if( pRoot->lChild == nullptr && pRoot->rChild == nullptr)
            return 1;
        return __getLeaf(pRoot->lChild) + __getLeaf(pRoot->rChild);
    }
    return 0;
}

template<typename T>
typename BinaryTree<T>::Node *BinaryTree<T>::__create(T *&pData,T emptySubTreeValue){
    T data;

    if( pData == nullptr)
        cin>>data;
    else{
        data = *(pData);
        (pData)++;
    }

    if( data == emptySubTreeValue )
        return nullptr;

    Node *pNode = new Node;
    pNode->data = data;

    pNode->lChild = __create(pData,emptySubTreeValue);
    pNode->rChild = __create(pData,emptySubTreeValue);

    return pNode;
}

template<typename T>
void BinaryTree<T>::__destroy(Node* &pRoot){
    if( pRoot ){
        __destroy(pRoot->lChild);
        __destroy(pRoot->rChild);
        delete pRoot;
        pRoot = nullptr;
    }
}

#endif // BINARYTREE_H
