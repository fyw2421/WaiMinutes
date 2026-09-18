//============================
//CommandPatternClient.cpp
//============================
#include "CommandPattern.h"

int main(){
    //开店前的准备
    CBarbecuer *pBarbecuer = new CBarbecuer();
    CWaiter *pWaiter = new CWaiter();

    //开门营业,顾客点菜
    CCommand *pFirstOrder = new CBakeMuttonCmd(pBarbecuer);
    pWaiter->order(pFirstOrder);

    CCommand *pSecondOrder = new CBakeChickerWingCmd(pBarbecuer);
    pWaiter->order(pSecondOrder);

    CCommand *pThirdOrder = new CBakeMuttonCmd(pBarbecuer);
    pWaiter->order(pThirdOrder);

    //点菜完毕，通知厨房
    pWaiter->notify();

    delete pWaiter;
    delete pThirdOrder;
    delete pSecondOrder;
    delete pFirstOrder;
    delete pBarbecuer;
    return 1;
}
