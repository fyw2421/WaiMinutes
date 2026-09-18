//============================
//StatePatternClient.cpp
//============================

#include "StatePattern.h"

int main(){
    CWork *pWork = new CWork();
    pWork->setTime(10);
    pWork->writeProgram();

    pWork->setTime(22);
    pWork->writeProgram();
    return 1;
}
