//============================
//DecoratorPatternClient.cpp
//============================

#include "DecoratorPattern.h"

int main(){
    CPerson *pComponent = new CPerson("Cai");

    cout<<"First Decorator: "<<endl;
    CSneakers *pSneaker = new CSneakers();
    CBigTrouser *pBigTrouser = new CBigTrouser();
    CTShirts *pTShirts = new CTShirts();

    pSneaker->decorator(pComponent);
    pBigTrouser->decorator(pSneaker);
    pTShirts->decorator(pBigTrouser);

    pTShirts->show();

    delete pSneaker;
    delete pBigTrouser;
    delete pTShirts;

    cout<<endl;
    cout<<"Second Decorator: "<<endl;
    CLeatherShoes *pLeatherShoes = new CLeatherShoes();
    CTie *pTie = new CTie();
    CSuit *pSuit = new CSuit();

    pLeatherShoes->decorator(pComponent);
    pTie->decorator(pLeatherShoes);
    pSuit->decorator(pTie);

    pSuit->show();

    delete pSuit;
    delete pTie;
    delete pLeatherShoes;
    delete pComponent;
    return 1;
}
