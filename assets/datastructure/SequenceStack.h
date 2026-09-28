//============================
//SequenceStack.h
//À≥–Ú’ª
//============================
#ifndef SEQUENCESTACK_H
#define SEQUENCESTACK_H

#include <iostream>
using namespace std;

template <typename T>
class SequenceStack{
public:
    SequenceStack(int nMaxSize=10);
    ~SequenceStack();
    bool push(T data);
    T pop();
    T getTop();
    void clear();
    bool isEmpty() const;
    bool isFull() const;
    void print() const;
private:
    T *mData;
    const int mMaxSize;
    int mTop;
};

template <typename T>
SequenceStack<T>::SequenceStack(int nMaxSize):mMaxSize(nMaxSize),mTop(-1){
    mData = new T[nMaxSize];
}

template <typename T>
SequenceStack<T>::~SequenceStack(){
    delete[] mData;
    mData = nullptr;
    mTop = -1;
}

template <typename T>
bool SequenceStack<T>::push(T data){
    if( isFull() ){
        cout<<"Stack is Full"<<endl;
        return false;
    }
    mData[ ++mTop ] = data;
    return true;
}

template <typename T>
T SequenceStack<T>::pop(){
    if( isEmpty() ){
        cout<<"Stack is empty"<<endl;
        return -1;
    }
    T tmpData = mData[ mTop-- ];
    return tmpData;
}

template <typename T>
T SequenceStack<T>::getTop(){
    if( isEmpty() ){
        cout<<"Stack is empty"<<endl;
        return -1;
    }
    return mData[ mTop ];
}

template <typename T>
void SequenceStack<T>::clear(){
    mTop = -1;
}

template <typename T>
bool SequenceStack<T>::isEmpty() const{
    return mTop == -1;
}

template <typename T>
bool SequenceStack<T>::isFull() const{
    return mMaxSize == mTop + 1;
}

template <typename T>
void SequenceStack<T>::print() const{
    cout<<"SequenceStatck :"<<endl;
    cout<<"Top";
    for( int nIdx = mTop ; nIdx >= 0 ; nIdx-- ){
        cout<<"<-"<<mData[ nIdx ];
        if( nIdx%5 == 0 )
            cout<<endl;
    }
    cout<<"<-Bottom"<<endl<<endl;
}

#endif // SEQUENCESTACK_H
