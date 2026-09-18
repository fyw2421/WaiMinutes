//============================
//ObserverPatternClient.cpp
//============================
#include "ObserverPattern.h"

int main(){
    //通知者
    CSubject *pSubject = new CBoss();

    //观察者
    CObserver *pObserverA = new CStockObserver("Colleague A",pSubject);
    CObserver *pObserverB = new CStockObserver("Colleague B",pSubject);
    CObserver *pObserverC = new CNBAObserver("Colleague C",pSubject);
    CObserver *pObserverD = new CNBAObserver("Colleague D",pSubject);

    //将4个观察者都加入到通知者的通知队列中
    pSubject->attach(pObserverA);
    pSubject->attach(pObserverB);
    pSubject->attach(pObserverC);
    pSubject->attach(pObserverD);

    //不通知某人
    pSubject->detach(pObserverC);

    //通知者状态改变
    pSubject->setSubjectState("Boss Come");

    //通知所有人
    pSubject->notify();

    delete pSubject;
    delete pObserverA;
    delete pObserverB;
    delete pObserverC;
    delete pObserverD;

    return 1;
}
