//============================
//VisitorPattern.h
//============================
#ifndef VISITORPATTERN_H
#define VISITORPATTERN_H

#include <iostream>
#include <vector>
#include <typeinfo>
#include <algorithm>
using namespace std;

class CPerson;
class CMan;
class CWomen;

//Visitor:抽象类
class CAction{
public:
    virtual void getManConclusion(CPerson *pMan) = 0;
    virtual void getWomenConclusion(CPerson *pWomen) = 0;
    virtual ~CAction(){}
};

//Element:抽象类,接收Visitor,人
class CPerson{
public:
    virtual ~CPerson(){}
    virtual void accept(CAction *pVisitor) = 0 ;
};

//concreteElement:男人
class CMan : public CPerson{
public:
    virtual ~CMan(){}
    virtual void accept(CAction* pVisitor){
        pVisitor->getManConclusion(this);
    }
};

//concreteElement:女人
class CWomen : public CPerson{
public:
    virtual ~CWomen(){}
    virtual void accept(CAction* pVisitor){
        pVisitor->getWomenConclusion(this);
    }
};

//Concrete Visitor:成功
class CSuccess : public CAction{
public:
    virtual void getManConclusion(CPerson* pMan){
        cout<<typeid(*pMan).name()<<" "<<typeid(*this).name()<<" There is a great women in the back"<<endl;
    }
    virtual void getWomenConclusion(CPerson* pWomen){
        cout<<typeid(*pWomen).name()<<" "<<typeid(*this).name()<<" There is a useless men in the back"<<endl;
    }
};

//concrete visitor:失败
class CFailure : public CAction{
public:
    virtual void getManConclusion(CPerson* pMan){
       cout<<typeid(*pMan).name()<<" "<<typeid(*this).name()<<" Drink "<<endl;
    }
    virtual void getWomenConclusion(CPerson* pWomen){
        cout<<typeid(*pWomen).name()<<" "<<typeid(*this).name()<<" Cry"<<endl;
    }
};

//concrete visitor::恋爱
class CAmativeness : public CAction{
public:
    virtual void getManConclusion(CPerson* pMan){
        cout<<typeid(*pMan).name()<<" "<<typeid(*this).name()<<" give gift "<<endl;
    }
    virtual void getWomenConclusion(CPerson* pWomen){
        cout<<typeid(*pWomen).name()<<" "<<typeid(*this).name()<<" smile always"<<endl;
    }
};



//objectStructure
class CObjectStructure{
public:
    CObjectStructure(){}
    ~CObjectStructure(){
        for_each(mPersonVector.begin(),mPersonVector.end(),[](CPerson *p){ delete p;});
    }
    void attach(CPerson *pElement){
        mPersonVector.push_back(pElement);
    }
    void detach(CPerson *pElement){
        for(auto it = mPersonVector.begin(); it != mPersonVector.end(); ){
            if( *it == pElement )
                mPersonVector.erase(it);
            else
                it++;
        }
    }
    void display(CAction *pVisitor){
        for_each(mPersonVector.begin(),mPersonVector.end(),[pVisitor](CPerson *p){ p->accept(pVisitor); } );
    }
private:
    vector<CPerson *> mPersonVector;
};
#endif // VISITORPATTERN_H
