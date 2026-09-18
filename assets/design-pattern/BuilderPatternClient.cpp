//============================
//BuilderPatternClient.cpp
//============================
#include "BuilderPattern.h"

int main(){

    CDirector *pDirector = new CDirector();

    CConcreteBuilder1 *pConcreteBuilder1 = new CConcreteBuilder1();
    CConcreteBuilder2 *pConcreteBuilder2 = new CConcreteBuilder2();

    cout<<"Direnctor Construct through ConcreteBuilder1"<<endl;
    pDirector->construct(pConcreteBuilder1);
    CProduct *pProduct = pConcreteBuilder1->getProduct();
    pProduct->show();
    cout<<endl;

    cout<<"Direnctor Construct through ConcreteBuilder2"<<endl;
    pDirector->construct(pConcreteBuilder2);
    pProduct = pConcreteBuilder2->getProduct();
    pProduct->show();
    cout<<endl;

    delete pDirector;
    delete pConcreteBuilder1;
    delete pConcreteBuilder2;
    return 1;
}
