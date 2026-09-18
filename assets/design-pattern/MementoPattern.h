//============================
//MementoPattern.h
//============================
#ifndef MEMENTOPATTERN_H
#define MEMENTOPATTERN_H

#include <iostream>
using namespace std;

//Memento类,备忘录,此处为角色状态存储箱RoleStateMemento class
class CRoleStateMemento{
public:
    CRoleStateMemento(int nVitality,int nAttrack,int nDefence):mVitality(nVitality),mAttrack(nAttrack),mDefence(nDefence){}

    void setVitality(int nVitality){ mVitality = nVitality; }
    int getVitality(){ return mVitality; }

    void setAttrack(int nAttrack){ mAttrack = nAttrack; }
    int getAttrack(){ return mAttrack; }

    void setDefence(int nDefence){ mDefence = nDefence; }
    int getDefence(){ return mDefence; }
private:
    int mVitality;
    int mAttrack;
    int mDefence;
};

//Originator,发起人,此处为游戏角色,GameRole class
class CGameRole{
public:
    CGameRole(){ mVitality = 100; mAttrack = 100 ; mDefence = 100; }
    void fight(){ mVitality = 0; mAttrack = 0 ; mDefence = 0; }
    void display(){
        cout<<"Vitality: "<<mVitality<<endl;
        cout<<"Attrack: "<<mAttrack<<endl;
        cout<<"Defence: "<<mDefence<<endl;
    }
    CRoleStateMemento saveState(){ return CRoleStateMemento(mVitality, mAttrack, mDefence);}
    void resumeState(CRoleStateMemento *pRoleMemento){
        mVitality = pRoleMemento->getVitality();
        mAttrack = pRoleMemento->getAttrack();
        mDefence = pRoleMemento->getDefence();
    }

    void setVitality(int nVitality){ mVitality = nVitality; }
    int getVitality(){ return mVitality; }

    void setAttrack(int nAttrack){ mAttrack = nAttrack; }
    int getAttrack(){ return mAttrack; }

    void setDefence(int nDefence){ mDefence = nDefence; }
    int getDefence(){ return mDefence; }
private:
    int mVitality;
    int mAttrack;
    int mDefence;
};

//Caretaker,管理者,此处为游戏角色管理类,RoleStateCaretaker class
class CRoleStateCaretaker{
public:
    CRoleStateCaretaker(): mpRoleStateMemento(nullptr){}
    ~CRoleStateCaretaker(){
        if( mpRoleStateMemento != nullptr){
            delete mpRoleStateMemento;
            mpRoleStateMemento = nullptr;
        }
    }

    void setRoleMemento(CRoleStateMemento *pRoleStateMemento){ mpRoleStateMemento = pRoleStateMemento; }
    CRoleStateMemento *getRoleMemento(){ return mpRoleStateMemento; }
private:
    CRoleStateMemento *mpRoleStateMemento;
};


#endif // MEMENTOPATTERN_H
