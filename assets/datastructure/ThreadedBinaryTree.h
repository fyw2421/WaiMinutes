//============================
//ThreadedBinaryTree.h
//线索化二叉树
// [ lchild ] [ LTag ] [ data ] [ RTag ] [ rchild ][parent]
//LTag  = { 0 : lchild 域指示结点的左孩子 1 : lchild 域指示结点的前驱 }
//RTag = { 0 : rchild 域指示结点的右孩子 1 : rchild 域指示结点的后继 }
//parent:post threading save
//============================
#ifndef THREADEDBINARYTREE_H
#define THREADEDBINARYTREE_H

#include <iostream>
#include <iomanip>
using namespace std;

enum class BinaryTreeThreadType{
    PRE_THREAD,
    IN_THREAD,
    POST_THREAD,
};

template<typename T>
class ThreadedBinaryTree{
public:
    ThreadedBinaryTree(BinaryTreeThreadType threadType = BinaryTreeThreadType::IN_THREAD);
    virtual ~ThreadedBinaryTree();

    void create(T *&pData = nullptr , T emptySubTreeValue = -1);
    void destroy();

    //recursive traversal
    void preOrder();
    void inOrder();
    void postOrder();

    //threading and traversal
    void thread();
    void traverse();
private:
    struct Node{
        T data;
        int ltag;
        Node *lChild;
        int rtag;
        Node *rChild;
        Node *parent;
    };
    Node *mpRoot;
    BinaryTreeThreadType mThreadType;
private:
    //preOrder thread
    void __preOrderThreading(Node *pNode,Node *&pPreNode);
    Node *__getPreOrderThreadedTreeFirstNode(Node *pCurNode);
    Node *__getPreOrderThreadedTreeNextNode(Node *pCurNode);
    void __traversePreOrderThreadTree(Node *pRoot);

    //inOrder thread
    void __inOrderThreading(Node *pNode,Node *&pPreNode);
    Node *__getInOrderThreadedTreeFirstNode(Node *pCurNode);
    Node *__getInOrderThreadedTreeNextNode(Node *pCurNode);
    void __traverseInOrderThreadTree(Node *pRoot);

    //postOrder thread
    void __postOrderThreading(Node *pNode,Node *&pPreNode);
    void __traversePostOrderThreadTree(Node *pRoot);

    //The recursive traversal
    void __preOrder(Node *pRoot);
    void __inOrder(Node *pRoot);
    void __postOrder(Node *pRoot);

    Node *__create(T *&pData ,T emptySubTreeValue,Node *pPreNode);
    void __destroy(Node* &pRoot);
};

template<typename T>
ThreadedBinaryTree<T>::ThreadedBinaryTree(BinaryTreeThreadType threadType):mpRoot(nullptr),mThreadType(threadType){
}

template<typename T>
ThreadedBinaryTree<T>::~ThreadedBinaryTree(){
    destroy();
}

template<typename T>
void ThreadedBinaryTree<T>::create(T *&pData,T emptySubTreeValue){
    if( mpRoot == nullptr)
        mpRoot = __create(pData,emptySubTreeValue,nullptr);
}

template<typename T>
void ThreadedBinaryTree<T>::destroy(){
    __destroy(mpRoot);
}

template<typename T>
void ThreadedBinaryTree<T>::preOrder(){
    cout<<"Recursive : ";
    __preOrder(mpRoot);
}

template<typename T>
void ThreadedBinaryTree<T>::inOrder(){
    cout<<"Recursive : ";
    __inOrder(mpRoot);
}

template<typename T>
void ThreadedBinaryTree<T>::postOrder(){
    cout<<"Recursive : ";
    __postOrder(mpRoot);
}

template<typename T>
void ThreadedBinaryTree<T>::__preOrderThreading(Node *pNode,Node *&pPreNode){
    if( pNode ){
        if( !pNode->lChild ){          //左子树为空,建立前驱线索
            pNode->lChild = pPreNode;
            pNode->ltag = 1;
        }
        if( pPreNode && !pPreNode->rChild ){ //建立前驱结点的后继线索
            pPreNode->rChild = pNode;
            pPreNode->rtag = 1;
        }

        pPreNode = pNode;                        //标记当前结点成为刚刚访问过的结点
        if( pNode->ltag == 0 )
            __preOrderThreading(pNode->lChild,pPreNode); //递归线索化左子树
        if( pNode->rtag == 0 )
            __preOrderThreading(pNode->rChild,pPreNode); //递归线索化右子树
    }
}

template<typename T>
typename ThreadedBinaryTree<T>::Node *ThreadedBinaryTree<T>::__getPreOrderThreadedTreeFirstNode(Node *pCurNode){
    return pCurNode;
}

template<typename T>
typename ThreadedBinaryTree<T>::Node *ThreadedBinaryTree<T>::__getPreOrderThreadedTreeNextNode(Node *pCurNode){
    if( pCurNode ){
        if( pCurNode->ltag == 0 )
            return pCurNode->lChild;
        else
            return pCurNode->rChild;
    }
    return nullptr;
}

template<typename T>
void ThreadedBinaryTree<T>::__traversePreOrderThreadTree(Node *pRoot){
    if( pRoot == nullptr)
        return;
    cout<<"1st method traverse"<<endl;
    for( Node *pNode = __getPreOrderThreadedTreeFirstNode(pRoot) ; pNode ; pNode = __getPreOrderThreadedTreeNextNode(pNode) )
        cout<<pNode->data<<" ";
    cout<<endl;
    cout<<"2nd method traverse"<<endl;
    Node *pCurNode = pRoot;
    while( pCurNode ){
        cout<<pCurNode->data<<" ";
        if( pCurNode->ltag == 0 )
            pCurNode = pCurNode->lChild;
        else
            pCurNode = pCurNode->rChild;
    }
}


template<typename T>
void ThreadedBinaryTree<T>::thread(){
    Node *pPreNode = nullptr;
    if( mThreadType == BinaryTreeThreadType::POST_THREAD)
        __postOrderThreading(mpRoot,pPreNode);
    else if( mThreadType == BinaryTreeThreadType::IN_THREAD)
        __inOrderThreading(mpRoot,pPreNode);
    else
        __preOrderThreading(mpRoot,pPreNode);
}

template<typename T>
void ThreadedBinaryTree<T>::traverse(){
    if( mThreadType == BinaryTreeThreadType::POST_THREAD)
        __traversePostOrderThreadTree(mpRoot);
    else if( mThreadType == BinaryTreeThreadType::IN_THREAD)
        __traverseInOrderThreadTree(mpRoot);
    else
        __traversePreOrderThreadTree(mpRoot);
}


template<typename T>
void ThreadedBinaryTree<T>::__inOrderThreading(Node *pNode,Node *&pPreNode){
    if( pNode ){
        __inOrderThreading(pNode->lChild,pPreNode); //递归线索化左子树
        if( !pNode->lChild ){          //左子树为空,建立前驱线索
            pNode->lChild = pPreNode;
            pNode->ltag = 1;
        }
        if( pPreNode && !pPreNode->rChild ){ //建立前驱结点的后继线索
            pPreNode->rChild = pNode;
            pPreNode->rtag = 1;
        }

        pPreNode = pNode;                        //标记当前结点成为刚刚访问过的结点
        __inOrderThreading(pNode->rChild,pPreNode); //递归线索化右子树
    }
}

template<typename T>
typename ThreadedBinaryTree<T>::Node *ThreadedBinaryTree<T>::__getInOrderThreadedTreeFirstNode(Node *pCurNode){
    while( pCurNode && pCurNode->ltag == 0 )
        pCurNode = pCurNode->lChild;
    return pCurNode;
}

template<typename T>
typename ThreadedBinaryTree<T>::Node *ThreadedBinaryTree<T>::__getInOrderThreadedTreeNextNode(Node *pCurNode){
    if( pCurNode ){
        if( pCurNode->rtag == 0 )
            return __getInOrderThreadedTreeFirstNode(pCurNode->rChild);
        else
            return pCurNode->rChild;
    }
    return nullptr;
}

template<typename T>
void ThreadedBinaryTree<T>::__traverseInOrderThreadTree(Node *pRoot){
    if( pRoot == nullptr)
        return;
    cout<<"1st method traverse"<<endl;
    for( Node *pNode = __getInOrderThreadedTreeFirstNode(pRoot) ; pNode ; pNode = __getInOrderThreadedTreeNextNode(pNode) )
        cout<<pNode->data<<" ";
    cout<<endl;
    cout<<"2nd method traverse"<<endl;
    Node *pCurNode = pRoot;
    while( pCurNode ){
        while( pCurNode && pCurNode->ltag == 0 )
            pCurNode = pCurNode->lChild;
        cout<<pCurNode->data<<" ";
        while( pCurNode->rChild && pCurNode->rtag == 1 ){
            pCurNode = pCurNode->rChild;
            cout<<pCurNode->data<<" ";
        }
        pCurNode = pCurNode->rChild;
    }
}

template<typename T>
void ThreadedBinaryTree<T>::__postOrderThreading(Node *pNode,Node *&pPreNode){
    if( pNode ){
        if( pNode->ltag == 0 )
            __postOrderThreading(pNode->lChild,pPreNode); //递归线索化左子树
        if( pNode->rtag == 0 )
            __postOrderThreading(pNode->rChild,pPreNode); //递归线索化右子树
        if( !pNode->lChild ){          //左子树为空,建立前驱线索
            pNode->lChild = pPreNode;
            pNode->ltag = 1;
        }
        if( pPreNode && !pPreNode->rChild ){ //建立前驱结点的后继线索
            pPreNode->rChild = pNode;
            pPreNode->rtag = 1;
        }
        pPreNode = pNode;                        //标记当前结点成为刚刚访问过的结点
    }
}

template<typename T>
void ThreadedBinaryTree<T>::__traversePostOrderThreadTree(Node *pRoot){
    if( pRoot == nullptr)
        return;
    Node *pCurNode = pRoot;
    Node *pPreNode = nullptr;
    while( pCurNode ){
        while( pCurNode && pCurNode->ltag == 0 )
            pCurNode = pCurNode->lChild;
        while( pCurNode && pCurNode->rtag == 1){
            cout<<pCurNode->data<<" ";
            pPreNode = pCurNode;
            pCurNode = pCurNode->rChild;
        }
        if( pCurNode == pRoot){
            cout<<pCurNode->data<<" ";
            break;
        }
        while( pCurNode && pCurNode->rChild == pPreNode ){
            cout<<pCurNode->data<<" ";
            pPreNode = pCurNode;
            pCurNode = pCurNode->parent;
        }
        if( pCurNode && pCurNode->rtag == 0 )
            pCurNode = pCurNode->rChild;
    }
}

template<typename T>
void ThreadedBinaryTree<T>::__preOrder(Node *pRoot){
    if( pRoot ){
        cout<<pRoot->data<<" ";
        __preOrder(pRoot->lChild);
        __preOrder(pRoot->rChild);
    }
}

template<typename T>
void ThreadedBinaryTree<T>::__inOrder(Node *pRoot){
    if( pRoot ){
        __inOrder(pRoot->lChild);
        cout<<pRoot->data<<" ";
        __inOrder(pRoot->rChild);
    }
}

template<typename T>
void ThreadedBinaryTree<T>::__postOrder(Node *pRoot){
    if( pRoot ){
        __postOrder(pRoot->lChild);
        __postOrder(pRoot->rChild);
        cout<<pRoot->data<<" ";
    }
}

template<typename T>
typename ThreadedBinaryTree<T>::Node *ThreadedBinaryTree<T>::__create(T *&pData,T emptySubTreeValue,Node *pPreNode){
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
    pNode->ltag = 0;
    pNode->rtag = 0;
    pNode->parent = pPreNode;

    pNode->lChild = __create(pData,emptySubTreeValue,pNode);
    pNode->rChild = __create(pData,emptySubTreeValue,pNode);

    return pNode;
}

template<typename T>
void ThreadedBinaryTree<T>::__destroy(Node* &pRoot){
    if( pRoot ){
        if( pRoot->ltag == 0 )
            __destroy(pRoot->lChild);
        if( pRoot->rtag == 0 )
            __destroy(pRoot->rChild);
        delete pRoot;
        pRoot = nullptr;
    }
}

#endif // THREADEDBINARYTREE_H
