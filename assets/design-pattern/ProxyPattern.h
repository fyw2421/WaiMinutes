//============================
//ProxyPattern.h
//============================
#ifndef PROXYPATTERN_H
#define PROXYPATTERN_H

#include <iostream>
using namespace std;

//interface
class CInterface{
public:
    virtual void request() = 0;
};

//realized class
class CRealClass : public CInterface{
public:
    virtual void request(){
        cout<<"Real request"<<endl;
    }
};

//proxy class
class CProxy : public CInterface{
public:
    CProxy(){ mpRealClass = nullptr; }
    virtual void request(){
        if( mpRealClass == nullptr )
            mpRealClass = new CRealClass();
        mpRealClass->request();
        delete mpRealClass;
        mpRealClass = nullptr;
    }
private:
    CRealClass *mpRealClass;
};



#endif // PROXYPATTERN_H
