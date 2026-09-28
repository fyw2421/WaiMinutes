//============================
//SequenceList.h
//Ë³Ðò±í
//============================
#ifndef SEQUENCELIST_H
#define SEQUENCELIST_H

#include <iostream>
using namespace std;

const int nDEFAULT_SIZE= 100;

template<typename T>
class SequenceList{
public:
    SequenceList(int nMaxSize = nDEFAULT_SIZE);
    ~SequenceList();
    int length() const;
    int find(T data) const;
    bool insert(T data,int nIndex);
    bool remove(T data);
    bool isEmpty() const;
    bool isFull() const;
    T getData(int nIndex) const;
    void print() const;
private:
    T *mData;
    const int mMaxSize;
    int mLength;
};

template<typename T>
SequenceList<T>::SequenceList(int nMaxSize):mData(nullptr),mMaxSize(nMaxSize),mLength(0){
    if( nMaxSize > 0 )
        mData = new T[ nMaxSize ];
}

template<typename T>
SequenceList<T>::~SequenceList(){
    if( mData != nullptr){
        delete[] mData;
        mData = nullptr;
    }
}

template<typename T>
int SequenceList<T>::length() const{
    return mLength ;
}

template<typename T>
int SequenceList<T>::find(T data) const{
    for( int nIndex = 0 ; nIndex < length() ; nIndex++ ){
        if( data == mData[ nIndex ])
            return nIndex;
    }
    return -1;
}

template<typename T>
bool SequenceList<T>::insert(T data,int nIndex){
    if( nIndex < 0 || nIndex > mLength || mMaxSize == mLength )
        return false;
    for( int nIdx = mLength ; nIdx >= nIndex ; nIdx-- ){
        mData[ nIdx + 1 ] = mData[ nIdx ];
    }
    mData[ nIndex ] = data;
    mLength++;
    return true;
}

template<typename T>
bool SequenceList<T>::remove(T data){
    int nLen = mLength;
    for( int nIndex = 0 ; nIndex < mLength ;){
        if( mData[ nIndex ] == data ){
            for( int nIdx = nIndex ; nIdx < mLength ; nIdx++ ){
                mData[ nIdx ] = mData[ nIdx + 1 ];
            }
            mLength--;
            continue;
        }
        nIndex++;
    }
    return nLen > mLength;
}

template<typename T>
bool SequenceList<T>::isEmpty() const{
    return mLength == -1;
}

template<typename T>
bool SequenceList<T>::isFull() const{
    return mLength == mMaxSize - 1;
}

template<typename T>
T SequenceList<T>::getData(int nIndex) const{
    if( nIndex > 0 && nIndex < length() )
        return mData[ nIndex ];
    return 0;
}

template<typename T>
void SequenceList<T>::print() const{
    int nCnt = 0;

    cout<<"Sequence List:"<<endl;
    for( int nIndex = 0 ; nIndex < length(); nIndex++ ){
        cout<<mData[nIndex] ;
        nCnt++;
        if( nCnt% 5 == 0 )
            cout<<endl;
        else
            cout<<" ";
    }
    cout<<endl;
}

#endif // SEQLIST_H
