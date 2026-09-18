//============================
//CommandPattern.h
//============================
#ifndef COMMANDPATTERN_H
#define COMMANDPATTERN_H

#include <iostream>
#include <vector>
#include <typeinfo>
#include <ctime>
using namespace std;

//Receiver:øæ—Ú»‚¥Æ’ﬂ
class CBarbecuer{
public:
    void bakeMutton(){ cout<<"Barbecuer bake mutton"<<endl; }
    void bakeChickenWing(){ cout<<"Barbecuer bake chicken wing"<<endl; }
};

//Command:≥ÈœÛ√¸¡Ó
class CCommand{
public:
    CCommand(CBarbecuer *pBarbecuer):mpBarbecuer(pBarbecuer){}
    virtual void execute() = 0 ;
protected:
    CBarbecuer *mpBarbecuer;
};

//ConcreteCommand: æﬂÃÂ√¸¡Ó:øæ—Ú»‚¥Æ
class CBakeMuttonCmd : public CCommand{
public:
    CBakeMuttonCmd(CBarbecuer *pBarbecuer):CCommand(pBarbecuer){}
    virtual void execute(){ mpBarbecuer->bakeMutton(); }
};

//ConcreteCommand: æﬂÃÂ√¸¡Ó:øæº¶≥·∞Ú
class CBakeChickerWingCmd : public CCommand{
public:
    CBakeChickerWingCmd(CBarbecuer *pBarbecuer):CCommand(pBarbecuer){}
    virtual void execute(){ mpBarbecuer->bakeChickenWing(); }
};

//Invoker:∑˛ŒÒ‘±
class CWaiter{
public:
    CWaiter(){
        mpCommandVector = new vector<CCommand *>();
    }
    ~CWaiter(){
        if( mpCommandVector != nullptr){
            delete mpCommandVector;
            mpCommandVector = nullptr;
        }
    }

    //…Ë÷√≤Àµ•
    void order(CCommand *pCommand){
        if( typeid(*pCommand) == typeid(CBakeChickerWingCmd) )
            cout<<"Log: Waiter :Chicken Wings are gone ,please order something else"<<endl;
        else if( typeid(*pCommand) == typeid(CBakeMuttonCmd) ){
            mpCommandVector->push_back(pCommand);
            time_t now = time(0);
            cout<<"Log: Waiter : add order: bake mutton . Time : "<<asctime(gmtime(&now));
        }
        else
            cout<<"Log: No Service"<<endl;
    }

    void notify(){
        for( auto it = mpCommandVector->begin(); it != mpCommandVector->end(); it++)
            (*it)->execute();
    }
private:
    vector<CCommand *> *mpCommandVector;
};
#endif // COMMANDPATTERN_H
