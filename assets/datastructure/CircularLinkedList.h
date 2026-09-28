//============================
//CircularLinkedList.h
//循环链表-带头结点
//============================
#ifndef CIRCULARLINKEDLIST_H
#define CIRCULARLINKEDLIST_H

#include <iostream>
#include <iomanip>
using namespace std;

template<typename T>
class CircularLinkedList{
public:
    CircularLinkedList();
    ~CircularLinkedList();
    int length();
    bool insert(T data,int nIndex); // nIndex : [0,mLength)
    T remove(int nIndex);
    int find(T data);
    void clear();
    void print();
private:
    struct Node{
        T data;
        Node *next;
    };
    Node *__getNode(int nIndex); // nIndex : [-1,mLength),-1 is head
private:
    int mLength;
    Node *mpHead;
};

template<typename T>
CircularLinkedList<T>::CircularLinkedList():mLength(0){
    mpHead = new Node;
    mpHead->next = mpHead;
}

template<typename T>
CircularLinkedList<T>::~CircularLinkedList(){
    clear();
    delete mpHead;
    mpHead = nullptr;
}

template<typename T>
int CircularLinkedList<T>::length(){
    return mLength;
}

template<typename T>
bool CircularLinkedList<T>::insert(T data,int nIndex){

    //move the pointer to the previous position of the destination
    Node *pNode = __getNode(nIndex-1);
    if( pNode == nullptr)
        return false;

    Node *pInsertNode = new Node;
    pInsertNode->data = data;
    pInsertNode->next = mpHead;

    pInsertNode->next = pNode->next;
    pNode->next = pInsertNode;

    mLength++;
    return true;
}

template<typename T>
T CircularLinkedList<T>::remove(int nIndex){

    //move the pointer to the previous position of the destination
    Node *pNode = __getNode(nIndex-1);
    if( pNode == nullptr )
        return false;

    Node *pDelNode = pNode->next;
    pNode->next = pDelNode->next;
    T tmpData = pDelNode->data;

    delete pDelNode;
    pDelNode = nullptr;

    mLength--;

    return tmpData;
}

template<typename T>
int CircularLinkedList<T>::find(T data){
    Node *pNode = mpHead;
    int nIndex = 0;
    while( pNode->next != mpHead ){
        pNode = pNode->next;
        if( pNode->data == data)
            return nIndex;
        nIndex++;
    }
    return -1;
}

template<typename T>
void CircularLinkedList<T>::clear(){
    Node *pNode = nullptr;
    while( mpHead->next != mpHead ){
        pNode = mpHead->next;
        mpHead->next = pNode->next;
        delete pNode;
        pNode = nullptr;
        mLength--;
    }
}

template<typename T>
void CircularLinkedList<T>::print(){
    int nCnt = 0;
    cout<<"Head";
    for( Node *pNode = mpHead->next ; pNode != mpHead ; pNode = pNode->next){
        cout<<"->"<<left<<setw(2)<<pNode->data;
        nCnt++;
        if( nCnt % 5 == 0 )
            cout<<endl;
    }
    cout<<"->Head"<<endl;
}

template<typename T>
typename CircularLinkedList<T>::Node *CircularLinkedList<T>::__getNode(int nIndex){
    if( nIndex < -1 || nIndex > mLength ){
        cout<<"index error!"<<endl;
        return nullptr;
    }
    Node *pNode = mpHead;
    int nIdx = -1;
    while( pNode->next != mpHead && nIdx < nIndex ){
        pNode = pNode->next;
        nIdx++;
    }
    if( nIdx > nIndex ){
        cout<<"index error!"<<endl;
        return nullptr;
    }
    return pNode;
}

#endif // CIRCULARLINKLIST_H
