//============================
//Simple Factory Pattern Client
//============================
#include "SimpleFactoryPattern.h"
#include <iostream>
using namespace std;

int main(){
    auto a = 3 , b = 4;
    COperationFactory *pFactory = new COperationFactory();
    COperation *pOp = nullptr;
    if(pFactory != nullptr){
        pOp = pFactory->create('+');
    }
    if( pOp != nullptr ){
        pOp->setFirstOperand(a);
        pOp->setScondOperand(b);
        cout<<pOp->getResult()<<endl;
        delete pOp;
        pOp = nullptr;
    }
    if( pFactory != nullptr ){
        delete pFactory;
        pFactory = nullptr;
    }

    return 0;
}
