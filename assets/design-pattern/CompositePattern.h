//============================
//CompositePattern.h
//============================
#ifndef COMPOSITEPATTERN_H
#define COMPOSITEPATTERN_H

#include <iostream>
#include <list>
#include <algorithm>

using namespace std;

//Component:抽象公司类
class CCompany{
protected:
    string mStrName;
public:
    CCompany(string strName):mStrName(strName){}
    virtual ~CCompany(){}
    virtual void add(CCompany *pCompany) = 0;
    virtual void remove(CCompany *pCompany) = 0;
    virtual void display(int nDepth) = 0 ;
    virtual void lineOfDuty() = 0;
    bool operator==(const CCompany &company) const{
        return this->mStrName == company.mStrName;
    }
};

//Composite: 具体公司类
class CConcreteCompany : public CCompany{
private:
    list<CCompany *> *mpChildMemberList;

public:
    CConcreteCompany(string strName):CCompany(strName){
        mpChildMemberList = new list<CCompany *>();
    }
    virtual ~CConcreteCompany(){
        for_each(mpChildMemberList->begin() , mpChildMemberList->end() , [=](CCompany *pCompany){ delete pCompany; pCompany = nullptr; } );
        delete mpChildMemberList;
    }
    virtual void add(CCompany *pCompany){
        mpChildMemberList->push_back(pCompany);
    }
    virtual void remove(CCompany  *pCompany){
        mpChildMemberList->remove(pCompany);
    }
    virtual void display(int nDepth){
        for( int i = 0 ; i < nDepth ; i++)
            cout<<"-";
        cout<<mStrName<<endl;
        for_each(mpChildMemberList->begin() , mpChildMemberList->end() , [=](CCompany *pCompany){ pCompany->display(nDepth+1); } );
    }
    virtual void lineOfDuty(){
        for_each(mpChildMemberList->begin() , mpChildMemberList->end() , [=](CCompany *pCompany){ pCompany->lineOfDuty(); } );
    }
};

//Leaf : 人力资源部
class CHRDepartment : public CCompany{
public:
    CHRDepartment(string strName) : CCompany(strName){}
    virtual ~CHRDepartment(){}

    virtual void add(CCompany* pCompany){}
    virtual void remove(CCompany* pCompany){}
    virtual void display(int nDepth){
        for( int i = 0 ; i < nDepth ; i++)
            cout<<"-";
        cout<<mStrName<<endl;
    }
    virtual void lineOfDuty(){
        cout<<mStrName<<" Staff Manage"<<endl;
    }
};

//Leaf: 财务部
class CFinanceDepartment : public CCompany{
public:
    CFinanceDepartment(string strName) : CCompany(strName){}
    virtual ~CFinanceDepartment(){}
    virtual void add(CCompany* pCompany){}
    virtual void remove(CCompany* pCompany){}
    virtual void display(int nDepth){
        for( int i = 0 ; i < nDepth ; i++)
            cout<<"-";
        cout<<mStrName<<endl;
    }
    virtual void lineOfDuty(){
        cout<<mStrName<<" Financial Manage"<<endl;
    }
};

#endif // COMPOSITEPATTERN_H
