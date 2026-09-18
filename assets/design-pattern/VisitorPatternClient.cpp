//============================
//VisitorPatternClient.cpp
//============================

#include "VisitorPattern.h"

int main(){
    CObjectStructure *pObjectStructure = new CObjectStructure();
    pObjectStructure->attach(new CMan());
    pObjectStructure->attach(new CWomen());

    CSuccess *pSuccess = new CSuccess();
    pObjectStructure->display(pSuccess);
    delete pSuccess;

    CFailure *pFaiure = new CFailure();
    pObjectStructure->display(pFaiure);
    delete pFaiure;

    CAmativeness *pAmativeness = new CAmativeness();
    pObjectStructure->display(pAmativeness);
    delete pAmativeness;

    delete pObjectStructure;
    return 1;
}
