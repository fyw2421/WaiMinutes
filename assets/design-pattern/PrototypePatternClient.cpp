//============================
//PrototypePatternClient.cpp
//============================
#include "PrototypePattern.h"

int main(){
    CPrototype *pPrototype = new CConcretePrototype1("Wai");
    CConcretePrototype2 *pConcretePrototype2 = (CConcretePrototype2*)pPrototype->clone();
    pPrototype->show();
    pConcretePrototype2->show();
    delete pPrototype;
    delete pConcretePrototype2;
    return 1;
}
