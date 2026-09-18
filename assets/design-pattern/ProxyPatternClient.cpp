//============================
//ProxyPatternClient.cpp
//============================

#include "ProxyPattern.h"

int main(){
    CProxy *pProxy = new CProxy();
    pProxy->request();
    delete pProxy;
    pProxy = nullptr;
    return 1;
}
