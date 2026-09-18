//============================
//IteratorPatternClient.cpp
//============================

#include "IteratorPattern.h"

int main(){
    //公交车，即聚集对象
    CConcreteAggregate *pBus = new CConcreteAggregate();

    //新上来的乘客
    pBus->push("Man");
    pBus->push("Women");
    pBus->push("Baby");
    pBus->push("Boy");
    pBus->push("Thief");

    //产生从前往后的迭代器
    CIterator *pFrontIterator = pBus->createIterator();

    //告知每一位乘客买票
    cout<<"Aggregate Front Iterator: "<<endl;
    while(!pFrontIterator->isDone()){
        cout<<pFrontIterator->currentItem()<<" buy ticket "<<endl;
        pFrontIterator->next();
    }

    //产生从后往前的迭代器
    CIterator *pBackIterator = pBus->createIteratorDesc();

    //告知每一位乘客买票
    cout<<endl;
    cout<<"Aggregate Back Iterator: "<<endl;
    while(!pBackIterator->isDone()){
        cout<<pBackIterator->currentItem()<<" buy ticket "<<endl;
        pBackIterator->next();
    }

    delete pBus;
    return 1;
}
