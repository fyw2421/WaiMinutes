//============================
//DecoratorPattern.h
//============================
#ifndef DECORATORPATTERN_H
#define DECORATORPATTERN_H

#include <iostream>
using namespace std;

//Componet
class CPerson{
public:
    CPerson(){}
    CPerson(string strName):mStrName(strName){}
    virtual ~CPerson(){}
    virtual void show(){ cout<<"Decorate "<<mStrName<<endl; }
private:
    string mStrName;
};

//Decorator
class CFinery : public CPerson{
public:
    virtual ~CFinery(){}
    void decorator(CPerson *pComponet){ mpComponent = pComponet; }
    void show(){ mpComponent->show(); }
protected:
    CPerson *mpComponent;
};

//ConcreteDecorator
class CTShirts : public CFinery{
public:
    void show(){
        cout<<"T Shirts"<<endl;
        CFinery::show();
    }
};

//ConcreteDecorator
class CTrouser : public CFinery{
public:
    void show(){
        cout<<"Trouser"<<endl;
        CFinery::show();
    }
};

//ConcreteDecorator
class CBigTrouser : public CFinery{
public:
    void show(){
        cout<<"Big Trouser"<<endl;
        CFinery::show();
    }
};

//ConcreteDecorator
class CSneakers : public CFinery{
public:
    void show(){
        cout<<"Sneakers"<<endl;
        CFinery::show();
    }
};

//ConcreteDecorator
class CSuit : public CFinery{
public:
    void show(){
        cout<<"Suit"<<endl;
        CFinery::show();
    }
};

//ConcreteDecorator
class CTie : public CFinery{
public:
    void show(){
        cout<<"Tie"<<endl;
        CFinery::show();
    }
};

//ConcreteDecorator
class CLeatherShoes : public CFinery{
public:
    void show(){
        cout<<"LeatherShoes"<<endl;
        CFinery::show();
    }
};
#endif // DECORATORPATTERN_H
