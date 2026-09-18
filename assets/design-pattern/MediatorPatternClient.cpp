//============================
//MediatorPatternClient.cpp
//============================

#include "MediatorPattern.h"

int main(){
    CConcreteMediator *pConcreteMediator = new CConcreteMediator();

    //让同事认识中介
    CConcreteColleagueA *pColleagueA = new CConcreteColleagueA(pConcreteMediator);
    CConcreteColleagueB *pColleagueB = new CConcreteColleagueB(pConcreteMediator);

    //让中介认识同事
    pConcreteMediator->setConcreteColleagueA(pColleagueA);
    pConcreteMediator->setConcreteColleagueB(pColleagueB);

    pColleagueA->send("Are you OK?");
    pColleagueB->send("very fine");
    return 1;
}
