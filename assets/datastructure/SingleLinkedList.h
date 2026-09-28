//============================
//SingleLinkedList.h
//单链表
//============================
#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H

#include <iostream>
#include <iomanip>
using namespace std;

template<typename T>
class SingleLinkedList{
public:
    SingleLinkedList();
    ~SingleLinkedList();
    int length();
    bool insert(T data,int nIndex); // nIndex : [0,mLength)
    T remove(int nIndex);
    int find(T data);
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
SingleLinkedList<T>::SingleLinkedList():mLength(0){
    mpHead = new Node;
    mpHead->next = nullptr;
}

template<typename T>
SingleLinkedList<T>::~SingleLinkedList(){
    Node *pNode = nullptr;
    while(mpHead->next){
        //Always delete mpHead->next
        pNode = mpHead->next;
        //set mpHead->next to next node
        mpHead->next = pNode->next;
        delete pNode;
        pNode = nullptr;
        mLength--;
    }
    delete mpHead;
    mpHead = nullptr;
}

template<typename T>
int SingleLinkedList<T>::length(){
    return mLength;
}

template<typename T>
bool SingleLinkedList<T>::insert(T data,int nIndex){

    //move the pointer to the previous position of the destination
    Node *pNode = __getNode(nIndex-1);
    if( pNode == nullptr)
        return false;

    Node *pInsertNode = new Node;
    pInsertNode->data = data;
    pInsertNode->next = nullptr;

    pInsertNode->next = pNode->next;
    pNode->next = pInsertNode;

    mLength++;
    return true;
}

template<typename T>
T SingleLinkedList<T>::remove(int nIndex){

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
int SingleLinkedList<T>::find(T data){
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
void SingleLinkedList<T>::print(){
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
typename SingleLinkedList<T>::Node *SingleLinkedList<T>::__getNode(int nIndex){
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
#endif // SINGLELINKLIST_H
