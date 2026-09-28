//============================
//DoublelinkedList.h
//Ë«ÏòÁ´±í
//============================
#ifndef DOUBLELINKEDLIST_H
#define DOUBLELINKEDLIST_H

#include <iostream>
#include <iomanip>
using namespace std;

template<typename T>
class DoubleLinkedList{
public:
    DoubleLinkedList();
    ~DoubleLinkedList();
    int length();
    bool insert(T data,int nIndex); // nIndex : [0,mLength)
    T remove(int nIndex);
    int find(T data);
    void print();
private:
    struct Node{
        T data;
        Node *prior;
        Node *next;
    };

    Node *__getNode(int nIndex); // nIndex : [-1,mLength),-1 is head
private:
    int mLength;
    Node *mpHead;
};

template<typename T>
DoubleLinkedList<T>::DoubleLinkedList():mLength(0){
    mpHead = new Node;
    mpHead->prior = nullptr;
    mpHead->next = nullptr;
}

template<typename T>
DoubleLinkedList<T>::~DoubleLinkedList(){
    Node *pNode = nullptr;
    while(mpHead->next){
        pNode = mpHead->next;
        mpHead->next = pNode->next;
        delete pNode;
        pNode = nullptr;
    }
    delete mpHead;
    mpHead = nullptr;
}

template<typename T>
int DoubleLinkedList<T>::length(){
    return mLength;
}

template<typename T>
bool DoubleLinkedList<T>::insert(T data,int nIndex){

    //move the pointer to the previous position of the destination
    Node *pNode = __getNode(nIndex-1);
    if( pNode == nullptr)
        return false;

    Node *pInsertNode = new Node;
    pInsertNode->data = data;
    pInsertNode->prior = nullptr;
    pInsertNode->next = nullptr;

    pInsertNode->next = pNode->next;
    if( pNode->next)
        pNode->next->prior = pInsertNode;;
    pInsertNode->prior = pNode;
    pNode->next = pInsertNode;

    mLength++;

    return true;
}

template<typename T>
T DoubleLinkedList<T>::remove(int nIndex){

    //move the pointer to the previous position of the destination
    Node *pNode = __getNode(nIndex-1);
    if( pNode == nullptr )
        return false;

    Node *pDelNode = pNode->next;
    pNode->next = pDelNode->next;
    pDelNode->next->prior = pNode;

    T tmpData = pDelNode->data;
    delete pDelNode;
    pDelNode = nullptr;

    mLength--;

    return tmpData;
}

template<typename T>
int DoubleLinkedList<T>::find(T data){
    Node *pNode = mpHead;
    int nIndex = 0;
    while( pNode->next ){
        pNode = pNode->next;
        if( pNode->data == data)
            return nIndex;
        nIndex++;
    }
    return -1;
}

template<typename T>
void DoubleLinkedList<T>::print(){
    int nCnt = 0;
    cout<<"Head";
    for( Node *pNode = mpHead->next ; pNode ; pNode = pNode->next){
        cout<<"->"<<left<<setw(2)<<pNode->data;
        nCnt++;
        if( nCnt % 5 == 0 )
            cout<<endl;
    }
}

template<typename T>
typename DoubleLinkedList<T>::Node *DoubleLinkedList<T>::__getNode(int nIndex){
    Node *pNode = mpHead;
    int nIdx = -1;
    while( pNode && nIdx < nIndex ){
        pNode = pNode->next;
        nIdx++;
    }
    if( pNode == nullptr || nIdx > nIndex ){
        cout<<"index error!"<<endl;
        return nullptr;
    }
    return pNode;
}
#endif // DOUBLELINKLIST_H
