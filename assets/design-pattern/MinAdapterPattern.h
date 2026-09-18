//============================
//MinAdapterPattern.h
//============================
#ifndef MINADAPTERPATTERN_H
#define MINADAPTERPATTERN_H

#include <iostream>
using namespace std;

//Target class
class CTarget{
public:
    virtual void request(){
        cout<<"Common Request"<<endl;
    }
};

//Adaptee class
class CAdaptee{
public:
    void specialRequest(){
        cout<<"Special Request"<<endl;
    }
};

//Adapter class
class CAdapter : public CTarget{
private:
    CAdaptee *mpDaptee;
public:
    virtual void request(){
        mpDaptee->specialRequest();
        CTarget::request();
    }
    CAdapter(){ mpDaptee = new CAdaptee(); }
    virtual ~CAdapter(){ delete mpDaptee; }
};

#endif // MINADAPTERPATTERN_H
