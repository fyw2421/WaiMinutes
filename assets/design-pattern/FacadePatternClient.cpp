//============================
//FacadePatternClient.cpp
//============================

#include "FacadePattern.h"

int main(){
    CFacade *pFacade = new CFacade();
    pFacade->facadeMethod();
    return 1;
}
