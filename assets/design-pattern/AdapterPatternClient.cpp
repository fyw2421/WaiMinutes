//============================
//AdapterPatternClient.cpp
//============================

#include "AdapterPattern.h"

int main(){

    CPlayer *pForward = new CForwards("XXX");
    pForward->attack();

    CPlayer *pGuard = new CGuards("YYY");
    pGuard->defense();

    CPlayer *pTranslator = new CTranslator("ZZZ");
    pTranslator->attack();
    pTranslator->defense();

    delete pForward;
    delete pGuard;
    delete pTranslator;

    return 1;
}
