//============================
//PrototypePattern.h
//============================
#ifndef PROTOTYPEPATTERN_H
#define PROTOTYPEPATTERN_H

#include <iostream>
using namespace std;

//abstract prototype
class CPrototype{
private:
    string mstrName;
public:
    CPrototype(string name=""):mstrName(name){}
    virtual ~CPrototype(){}
    void show(){ cout<<mstrName<<endl; }
    virtual CPrototype *clone() = 0;
};

//concrete prototype1
class CConcretePrototype1 : public CPrototype{
public:
    CConcretePrototype1(string name = ""):CPrototype(name){}
    virtual ~CConcretePrototype1(){}
    virtual CPrototype *clone(){
        CConcretePrototype1 *pConcretePrototype = new CConcretePrototype1();
        *pConcretePrototype = *this;
        return pConcretePrototype;
    }
};

//concrete prototype2
class CConcretePrototype2 : public CPrototype{
private:
    int sss;
public:
    CConcretePrototype2(string name = ""):CPrototype(name){}
    virtual ~CConcretePrototype2(){}
    virtual CPrototype *clone(){
        CConcretePrototype2 *pConcretePrototype = new CConcretePrototype2();
        *pConcretePrototype = *this;
        return pConcretePrototype;
    }
};

#endif // PROTOTYPEPATTERN_H
