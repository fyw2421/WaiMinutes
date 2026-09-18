//============================
//Factory Method Pattern Client
//============================
#include <iostream>
#include "FactoryMethodPattern.h"

CAbstractFactory *getFactory(char op){
    switch( op ){
    case '+':
        return  new CAddFactory();
        break;
    case '-':
        return new CSubFactory();
        break;
    default:
        return nullptr;
        break;
    }
}

int main(){
    using namespace std;
    auto a = 3 , b = 4;
    CAbstractFactory *pFactory = dynamic_cast<CAbstractFactory *>(getFactory('+'));
    COperation *pOp = nullptr;
    if( pFactory != nullptr )
        pOp = pFactory->createOperation();
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
