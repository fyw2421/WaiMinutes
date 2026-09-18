//============================
//Factory Method Pattern-LeiFeng Factory
//============================
#ifndef LEIFENGFACTORYMETHODPATTERN_H
#define LEIFENGFACTORYMETHODPATTERN_H

#include <iostream>
using namespace std;

//AbstractProduct LeiFeng
class CLeiFeng{
public:
    virtual void sweep(){
        cout<<"LeiFeng Seep"<<endl;
    }
    virtual void wash(){
        cout<<"LeiFeng Wash"<<endl;
    }
    virtual void buyRice(){
        cout<<"LeiFeng Buy Rice"<<endl;
    }
};

//ConcreteProduct
class CUndergraduate : public CLeiFeng{
public:
    void sweep(){
        cout<<"Undergraduate Sweep"<<endl;
    }
    void wash(){
        cout<<"Undergraduate wash"<<endl;
    }
    void buyRice(){
        cout<<"Undergraduate Buy Rice"<<endl;
    }
};

//ConcreteProduct
class CVolunteer : public CLeiFeng{
public:
    void sweep(){
        cout<<"Volunteer Sweep"<<endl;
    }
    void wash(){
        cout<<"Volunteer wash"<<endl;
    }
    void buyRice(){
        cout<<"Volunteer Buy Rice"<<endl;
    }
};

//AbstractFactory
class CAbstractFactory {
public:
    virtual CLeiFeng *create(){
        return new CLeiFeng();
    }
};

//ConcreteFactory
class CUndergraduateFactory : public CAbstractFactory{
public:
    virtual CUndergraduate *create(){
        return new CUndergraduate();
    }
};
//ConcreteFactory
class CVolunteerFactory : public CAbstractFactory{
public:
    virtual CVolunteer *create(){
        return new CVolunteer();
    }
};
#endif // LEIFENGFACTORYMETHODPATTERN_H
