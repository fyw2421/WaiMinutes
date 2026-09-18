//============================
//AdapterPattern.h
//============================
#ifndef ADAPTERPATTERN_H
#define ADAPTERPATTERN_H

#include <iostream>
using namespace std;

//Target: Palyer-"运动员"
class CPlayer{
public:
    CPlayer(string strName){
        mStrName = strName;
    }
    ~CPlayer(){}
    virtual void attack() = 0;
    virtual void defense() = 0;
protected:
    string mStrName;
};

//adaptee : foreign center-"外籍中锋"
class CForeignCenter{
public:
    CForeignCenter(string strName){
        mStrName = strName;
    }
    void foreignAttack(){ cout<<"Foreign Center "<<mStrName<<" Attack!"<<endl; }
    void foreignDefense(){ cout<<"Foreign Center "<<mStrName<<" Defense!"<<endl; }
private:
    string mStrName;
};

//adapter: Translator-"翻译"
class CTranslator : public CPlayer{
public:
    CTranslator(string strName): CPlayer(strName){
        mpForeignCenter = new CForeignCenter(strName);
    }
    virtual ~CTranslator(){
        if( mpForeignCenter != nullptr){
            delete mpForeignCenter;
            mpForeignCenter = nullptr;
        }
    }
    virtual void attack(){ mpForeignCenter->foreignAttack(); }
    virtual void defense(){ mpForeignCenter->foreignDefense(); }
private:
    CForeignCenter *mpForeignCenter;
};

//common01: Forwards-"前锋"
class CForwards : public CPlayer{
public:
    CForwards(string strName) : CPlayer(strName){}
    virtual ~CForwards(){}
    virtual void attack(){ cout<<"Forwards "<<mStrName<<" Attack!"<<endl; }
    virtual void defense(){ cout<<"Forwards "<<mStrName<<" Defense!"<<endl; }
};

//common02: Center-"中锋"
class CCenter : public CPlayer{
public:
    CCenter(string strName) : CPlayer(strName){}
    virtual ~CCenter(){}
    virtual void attack(){ cout<<"Center "<<mStrName<<" Attack!"<<endl; }
    virtual void defense(){ cout<<"Center "<<mStrName<<" Defense!"<<endl; }
};

//common03: Guards-"后卫"
class CGuards : public CPlayer{
public:
    CGuards(string strName) : CPlayer(strName){}
    virtual ~CGuards(){}
    virtual void attack(){ cout<<"Guards "<<mStrName<<" Attack!"<<endl; }
    virtual void defense(){ cout<<"Guards "<<mStrName<<" Defense!"<<endl; }
};

#endif // ADAPTERPATTERN_H
