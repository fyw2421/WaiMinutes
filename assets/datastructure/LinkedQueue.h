//============================
//LinkedQueue.h
//链队列
//============================
#ifndef LINKEDQUEUE_H
#define LINKEDQUEUE_H

#include <iostream>
using namespace std;

template <typename T>
class LinkedQueue{
public:
    LinkedQueue();
    ~LinkedQueue();
    void enter(T data);
    T leave();
    bool isEmpty();
    void clear();
    void print();
private:
    struct Node{
        T data;
        Node *next;
    };
    Node *mpFront;
    Node *mpRear;
};

template <typename T>
LinkedQueue<T>::LinkedQueue():mpFront(nullptr),mpRear(nullptr){
}

template <typename T>
LinkedQueue<T>::~LinkedQueue(){
    clear();
}

template <typename T>
void LinkedQueue<T>::enter(T data){
    Node *pNewNode = new Node;
    pNewNode->data = data;
    pNewNode->next = nullptr;
    if( mpRear == nullptr )
        mpFront = mpRear = pNewNode;
    else{
        mpRear->next = pNewNode;
        mpRear = pNewNode;
    }
}

template <typename T>
T LinkedQueue<T>::leave(){
    if( isEmpty()){
        cout<<"Queue is empty"<<endl;
        return -1;
    }
    T tmpData = mpFront->data;
    Node *pDelNode = mpFront;
    mpFront = mpFront->next;
    delete pDelNode;
    return tmpData;
}

template <typename T>
bool LinkedQueue<T>::isEmpty(){
    return mpFront == nullptr;
}

template <typename T>
void LinkedQueue<T>::clear(){
    Node *pNode = mpFront ;
    while( mpFront ){
        pNode = mpFront;
        mpFront = mpFront->next;
        delete pNode;
    }
    mpFront = mpRear = nullptr;
}

template <typename T>
void LinkedQueue<T>::print(){
    cout<<"LinkedQueue :"<<endl;
    cout<<"Front";
    int nCnt = 0;
    for( Node *pNode = mpFront ; pNode ; pNode = pNode->next ){
        cout<<"->"<<pNode->data;
        nCnt++;
        if( nCnt%5 ==0)
            cout<<endl;
    }
    cout<<"->Rear"<<endl<<endl;
}

#endif // LINKEDQUEUE_H
