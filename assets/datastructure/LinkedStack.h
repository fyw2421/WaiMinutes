//============================
//LinkedStack.h
//链栈
//============================
#ifndef LINKEDSTACK_H
#define LINKEDSTACK_H

#include <iostream>
using namespace std;

template <typename T>
class LinkedStack{
public:
    LinkedStack();
    ~LinkedStack();
    void push(T data);
    T pop();
    T getTop();
    void clear();
    bool isEmpty() const;
    void print() const;
private:
    struct Node{
        T data;
        Node *next;
    };
    Node *mpTop;
};

template <typename T>
LinkedStack<T>::LinkedStack():mpTop(nullptr){}

template <typename T>
LinkedStack<T>::~LinkedStack(){
    clear();
}

template <typename T>
void LinkedStack<T>::push(T data){
    Node *pNewNode = new Node;
    pNewNode->data = data;
    pNewNode->next = mpTop;
    mpTop = pNewNode;
}

template <typename T>
T LinkedStack<T>::pop(){
    if( isEmpty() ){
        cout<<"Stack is empty"<<endl;
        return -1;
    }
    T tmpData = mpTop->data;
    Node *pDelNode = mpTop;
    mpTop = mpTop->next;
    delete pDelNode;
    return tmpData;
}

template <typename T>
T LinkedStack<T>::getTop(){
    if( isEmpty() ){
        cout<<"Stack is empty"<<endl;
        return -1;
    }
    return mpTop->data;
}

template <typename T>
void LinkedStack<T>::clear(){
    Node *pDelNode = nullptr;
    while(mpTop){
        pDelNode = mpTop;
        mpTop = mpTop->next;
        delete pDelNode;
        pDelNode = nullptr;
    }
}

template <typename T>
bool LinkedStack<T>::isEmpty() const{
    return mpTop == nullptr;
}

template <typename T>
void LinkedStack<T>::print() const{
    cout<<"LinkedStatck :"<<endl;
    cout<<"Top";
    int nIdx = 0;
    for( Node *pNode = mpTop ; pNode ; pNode = pNode->next ){
        cout<<"<-"<<pNode->data;
        nIdx++;
        if( nIdx%5 == 0 )
            cout<<endl;
    }
    cout<<"<-Bottom"<<endl<<endl;
}

#endif // LINKEDSTACK_H
