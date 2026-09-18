//============================
//FlyweightPattern.h
//============================
#ifndef FLYWEIGHTPATTERN_H
#define FLYWEIGHTPATTERN_H

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//user class
class CUser{
public:
    CUser(string strName):mStrName(strName){}
    string getName(){ return mStrName; }
private:
    string mStrName;
};

//flyweight class
class CWebsite{
public:
    virtual void use(CUser user) = 0;
    virtual string getCategory(){ return "Empty website";}
    virtual ~CWebsite(){}
};

//concreteFlyweight class
class CConcreteWebsite:public CWebsite{
public:
    CConcreteWebsite(string strCategory):mStrCategory(strCategory){}
    virtual void use(CUser user){
        cout<<"website category: "<<mStrCategory<<" User :"<<user.getName()<<endl;
    }
    virtual string getCategory(){ return mStrCategory; }
    virtual ~CConcreteWebsite(){}
private:
    string mStrCategory;
};

//unshareConcreteFlyweight class
class UnShareWebsite:public CWebsite{
public:
    UnShareWebsite(string strCategory):mStrCategory(strCategory){}
    virtual void use(CUser user){
        cout<<"website category: "<<mStrCategory<<" User :"<<user.getName()<<endl;
    }
    virtual ~UnShareWebsite(){}
private:
    string mStrCategory;
};

//flyweight factory class
class CWebsiteFactory{
public:
    CWebsiteFactory(){}
    virtual ~CWebsiteFactory(){
        for_each(mWebsiteVector.begin(),mWebsiteVector.end(),[&](CWebsite *pWebsitePair){ delete pWebsitePair; });
    }

    CWebsite *getWebsiteCategory(string strCatetory){
        for (auto pWebsite:mWebsiteVector ){
            if(pWebsite->getCategory() == strCatetory)
                return pWebsite;
        }
        CWebsite *pWebsite = new CConcreteWebsite(strCatetory);
        mWebsiteVector.push_back(pWebsite);
        return pWebsite;
    }
private:
    vector<CWebsite *> mWebsiteVector;
};

#endif // FLYWEIGHTPATTERN_H
