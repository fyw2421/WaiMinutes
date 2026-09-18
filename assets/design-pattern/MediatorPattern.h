//============================
//MediatorPattern.h
//============================
#ifndef MEDIATORPATTERN_H
#define MEDIATORPATTERN_H

#include <iostream>
using namespace std;

//mediator class : abstract
class CColleague;
class CMediator{
public:
    virtual void send(string strMsg,CColleague *pColleague ) = 0;
};

//Colleague class: abstract
class CColleague{
public:
    CColleague(CMediator *pMediator):mpMediator(pMediator){}
protected:
    CMediator *mpMediator;
};

//Concrete Colleague class A
class CConcreteColleagueA : public CColleague{
public:
    CConcreteColleagueA(CMediator *pMediator):CColleague(pMediator){}
    void send(string strMsg){
        mpMediator->send(strMsg,this);
    }
    void notify(string strMsg){ cout<<"Colleague A get Message : "<<strMsg<<endl; }
};

//Concrete Colleague class B
class CConcreteColleagueB : public CColleague{
public:
    CConcreteColleagueB(CMediator *pMediator):CColleague(pMediator){}
    void send(string strMsg){
        mpMediator->send(strMsg,this);
    }
    void notify(string strMsg){ cout<<"Colleague B get Message : "<<strMsg<<endl; }
};

//concrete mediator class
class CConcreteMediator : public CMediator{
public:
    virtual void send(string strMsg, CColleague* pColleague){
        if( pColleague == mpColleagueA)
            mpColleagueA->notify(strMsg);
        else
            mpColleagueB->notify(strMsg);
    }
    void setConcreteColleagueA(CConcreteColleagueA *pConcreteColleagueA){ mpColleagueA = pConcreteColleagueA; }
    void setConcreteColleagueB(CConcreteColleagueB *pConcreteColleagueB){ mpColleagueB = pConcreteColleagueB; }
private:
    CConcreteColleagueA *mpColleagueA;
    CConcreteColleagueB *mpColleagueB;
};



#endif // MEDIATORPATTERN_H
