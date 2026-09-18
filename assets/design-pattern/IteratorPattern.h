//============================
//IteratorPattern.h
//============================
#ifndef ITERATORPATTERN_H_INCLUDED
#define ITERATORPATTERN_H_INCLUDED

#include <iostream>
#include <vector>
using namespace std;

class CFrontIterator;
class CBackIterator;
using object = string;

//迭代器抽象类
class CIterator{
public:
    virtual ~CIterator(){}
    virtual object first() = 0;
    virtual object next() = 0;
    virtual bool isDone() = 0;
    virtual object currentItem() = 0;
};

//聚集抽象类
class CAggregate{
public:
    virtual CIterator *createIterator() = 0;
};

//具体聚集类
class CConcreteAggregate : public CAggregate{
public:
    CConcreteAggregate():mpFrontIterator(nullptr),mpBackIterator(nullptr){
        mpObjectVector = new vector<object>();
    }
    virtual ~CConcreteAggregate(){
        if(mpObjectVector !=nullptr){
            delete mpObjectVector;
            mpObjectVector = nullptr;
        }
        if(mpFrontIterator!=nullptr){
            delete mpFrontIterator;
            mpFrontIterator = nullptr;
        }
        if( mpBackIterator != nullptr){
            delete mpBackIterator;
            mpBackIterator = nullptr;
        }
    }
    //产生从前往后的迭代器
    virtual CIterator *createIterator();

    //产生从后往前的迭代器
    CIterator *createIteratorDesc();
    int getCount(){
        return mpObjectVector->size();
    }
    object getElement(int nIndex){
        return mpObjectVector->at(nIndex);
    }
    void push(object obj){
        mpObjectVector->push_back(obj);
    }
private:
    vector<object> *mpObjectVector;
    CIterator *mpFrontIterator;
    CIterator *mpBackIterator;
};

//具体迭代器类，从前往后的迭代器
class CFrontIterator : public CIterator{
public:
    CFrontIterator(CConcreteAggregate *pConcreteAggregate):mpConcreteAggregate(pConcreteAggregate),mCurrentIndex(0){}
    virtual ~CFrontIterator(){}
    virtual object first(){
        return mpConcreteAggregate->getElement(0);
    }
    virtual object next(){
        mCurrentIndex++;
        if(mCurrentIndex< mpConcreteAggregate->getCount())
            return mpConcreteAggregate->getElement(mCurrentIndex);
        return mpConcreteAggregate->getElement( mpConcreteAggregate->getCount() <= 0 ? 0 : ( mpConcreteAggregate->getCount() - 1 ) );
    }
    virtual bool isDone(){
        return ( mCurrentIndex >= mpConcreteAggregate->getCount() );
    }
    virtual object currentItem(){
        return mpConcreteAggregate->getElement(mCurrentIndex);
    }
private:
    CConcreteAggregate *mpConcreteAggregate;
    int mCurrentIndex;
};

//具体迭代器类，从后往前的迭代器
class CBackIterator : public CIterator{
public:
    CBackIterator(CConcreteAggregate *pConcreteAggregate):mpConcreteAggregate(pConcreteAggregate){
        mCurrentIndex = mpConcreteAggregate->getCount() - 1;
    }
    virtual ~CBackIterator(){}
    virtual object first(){
        int nLastElement = mpConcreteAggregate->getCount() - 1;
        nLastElement = (nLastElement <= 0 ) ? 0 : nLastElement;
        return mpConcreteAggregate->getElement(nLastElement);
    }
    virtual object next(){
        mCurrentIndex--;
        if(mCurrentIndex >= 0)
            return mpConcreteAggregate->getElement(mCurrentIndex);
        return mpConcreteAggregate->getElement(0);
    }
    virtual bool isDone(){
        return mCurrentIndex < 0 ;
    }
    virtual object currentItem(){
        return mpConcreteAggregate->getElement(mCurrentIndex);
    }
private:
    CConcreteAggregate *mpConcreteAggregate;
    int mCurrentIndex;
};

//产生从前往后的迭代器
CIterator *CConcreteAggregate::createIterator(){
    if( mpFrontIterator == nullptr )
        mpFrontIterator = new CFrontIterator(this);
    return mpFrontIterator;
}

//产生从后往前的迭代器
CIterator *CConcreteAggregate::createIteratorDesc(){
    if( mpBackIterator == nullptr)
        mpBackIterator = new CBackIterator(this);
    return mpBackIterator;
}

#endif // ITERATORPATTERN_H_INCLUDED
