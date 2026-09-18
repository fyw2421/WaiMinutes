//============================
//MinAdapterPatternClient.cpp
//============================

#include "MinAdapterPattern.h"

int main(){
    CAdapter *pAdapter = new CAdapter();
    pAdapter->request();
    delete pAdapter;
    return 1;
}
