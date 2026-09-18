//============================
//BridgePattern.h
//============================
#ifndef BRIDGEPATTERN_H
#define BRIDGEPATTERN_H

#include <iostream>
using namespace std;

//Implementor:手机软件抽象类
class CHandsetSoft{
public:
    virtual void run() = 0 ;
    virtual ~CHandsetSoft(){}
};

//ConcreteImplementorA: 手机游戏
class CHandsetGame : public CHandsetSoft {
public:
    virtual void run(){ cout<<"Run Handset Game"<<endl; }
    virtual ~CHandsetGame(){}
};

//ConcreteImplementorB: 手机通讯录
class CHandsetAddressList : public CHandsetSoft{
public:
    virtual void run(){ cout<<"Run Handset Address List"<<endl; }
    virtual ~CHandsetAddressList(){}
};

//Abstraction: 手机品牌抽象类
class CHandsetBrand{
protected:
    CHandsetSoft *mpHandsetSoft = nullptr;
public:
    void setHandsetSoft(CHandsetSoft *pHandsetSoft){
        if( mpHandsetSoft != nullptr){
            delete mpHandsetSoft;
            mpHandsetSoft = nullptr;
        }
        mpHandsetSoft = pHandsetSoft;
    }
    virtual void run() = 0 ;
    virtual ~CHandsetBrand(){
        if( mpHandsetSoft != nullptr ){
            delete mpHandsetSoft;
            mpHandsetSoft = nullptr;
        }
    }
};

//Redefine Abstraction: 手机品牌M
class CHandsetBrandM : public CHandsetBrand{
public:
    virtual void run(){
        mpHandsetSoft->run();
    }
};

//Redefine Abstraction: 手机品牌N
class CHandsetBrandN : public CHandsetBrand{
public:
    virtual void run(){
        mpHandsetSoft->run();
    }
};

#endif // BRIDGEPATTERN_H
