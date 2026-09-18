//============================
//BridgePatternClient.cpp
//============================

#include "BridgePattern.h"

int main(){
    cout<<"Handset Brand M :"<<endl;

    CHandsetBrand *pHandsetBrand = new CHandsetBrandM();
    pHandsetBrand->setHandsetSoft(new CHandsetGame());
    pHandsetBrand->run();

    pHandsetBrand->setHandsetSoft(new CHandsetAddressList());
    pHandsetBrand->run();

    delete pHandsetBrand;

    cout<<endl;
    cout<<"Handset Brand N :"<<endl;

    pHandsetBrand = new CHandsetBrandN();
    pHandsetBrand->setHandsetSoft(new CHandsetGame());
    pHandsetBrand->run();

    pHandsetBrand->setHandsetSoft(new CHandsetAddressList());
    pHandsetBrand->run();

    delete pHandsetBrand;
    return 1;
}
