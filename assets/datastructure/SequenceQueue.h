//============================
//SequenceStack.h
//À≥–Ú∂”¡–
//============================
#ifndef SEQUENCEQUEUE_H
#define SEQUENCEQUEUE_H

#include <iostream>
using namespace std;

template <typename T>
class SequenceQueue{
public:
    SequenceQueue(int nMaxSize = 10);
    ~SequenceQueue();
    bool enter(T data);
    T leave();
    bool isFull();
    bool isEmpty();
    void clear();
    void print();
private:
    T *mData;
    const int mMaxSize;
    int mFront;
    int mRear;
};

template <typename T>
SequenceQueue<T>::SequenceQueue(int nMaxSize):mMaxSize(nMaxSize),mFront(0),mRear(0){
    mData = new T[nMaxSize];
}

template <typename T>
SequenceQueue<T>::~SequenceQueue(){
    delete[] mData;
    mData = nullptr;

    mFront = 0;
    mRear = 0;
}

template <typename T>
bool SequenceQueue<T>::enter(T data){
    if( isFull()){
        cout<<"Queue is Full"<<endl;
        return false;
    }
    mData[ mRear ] = data;
    mRear = (mRear+1)%mMaxSize;
    return true;
}

template <typename T>
T SequenceQueue<T>::leave(){
    if( isEmpty()){
        cout<<"Queue is empty"<<endl;
        return -1;
    }
    T tmpData = mData[ mFront ];
    mFront = (mFront+1)%mMaxSize;
    return tmpData;
}

template <typename T>
bool SequenceQueue<T>::isFull(){
    return mFront == (mRear + 1)%mMaxSize;
}

template <typename T>
bool SequenceQueue<T>::isEmpty(){
    return mFront == mRear;
}

template <typename T>
void SequenceQueue<T>::clear(){
    mFront = mRear;
}

template <typename T>
void SequenceQueue<T>::print(){
    cout<<"SequenceQueue :"<<endl;
    cout<<"Front";
    int nCnt = 0;
    for( int nIdx = mFront ; nIdx != mRear ; nIdx = (nIdx + 1)%mMaxSize ){
        cout<<"->"<<mData[ nIdx ];
        nCnt++;
        if( nCnt%5 ==0)
            cout<<endl;
    }
    cout<<"->Rear"<<endl<<endl;
}

#endif // SEQUENCEQUEUE_H
