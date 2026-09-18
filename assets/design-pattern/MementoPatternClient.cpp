//============================
//MementoPatternClient.cpp
//============================
#include "MementoPattern.h"

int main(){
    cout<<"original status: "<<endl;
    CGameRole *pJetLi = new CGameRole();
    pJetLi->display();

    //save status
    CRoleStateCaretaker *pRoleStateCaretaker = new CRoleStateCaretaker();
    CRoleStateMemento roleStateMemento = pJetLi->saveState();
    pRoleStateCaretaker->setRoleMemento( &roleStateMemento ) ;

    cout<<endl;
    cout<<"Fighting status"<<endl;
    pJetLi->fight();
    pJetLi->display();

    cout<<endl;
    cout<<"resume status"<<endl;
    pJetLi->resumeState( pRoleStateCaretaker->getRoleMemento() );
    pJetLi->display();

    delete pRoleStateCaretaker;
    delete pJetLi;
    return 1;
}
