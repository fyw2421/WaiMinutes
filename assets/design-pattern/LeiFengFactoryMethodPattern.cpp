//============================
//Factory Method Pattern-LeiFeng Factory Client
//============================
#include "LeiFengFactoryMethodPattern.h"

int main(){

    CAbstractFactory *pAbstractFactory = new CVolunteerFactory();
    CLeiFeng *pLeiFeng = nullptr;
    if( pAbstractFactory != nullptr)
       pLeiFeng = pAbstractFactory->create();

    pLeiFeng->sweep();
    pLeiFeng->wash();
    pLeiFeng->buyRice();


    if( pLeiFeng != nullptr){
        delete pLeiFeng;
        pLeiFeng = nullptr;
    }
    if( pAbstractFactory != nullptr){
        delete pAbstractFactory;
        pAbstractFactory = nullptr;
    }

    return 1;
}
