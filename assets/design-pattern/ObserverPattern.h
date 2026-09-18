//============================
//ObserverPattern.h
//============================
#ifndef OBSERVERPATTERN_H
#define OBSERVERPATTERN_H

#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

//Subject抽象通知者或者主题
class CObserver;
class CSubject{
public:
    virtual void attach(CObserver* pObserver) = 0;
    virtual void detach(CObserver* pObserver) = 0;
    virtual void notify() = 0;
    string getSujectState(){ return mStrSubjectState; }
    void setSubjectState(string strSubjectState){ mStrSubjectState = strSubjectState; }
private:
    string mStrSubjectState;
};

//Observer,抽象观察者
class CObserver{
public:
    CObserver(){}
    CObserver(string strName,CSubject *pSubject):mStrName(strName),mpSubject(pSubject){}
    virtual void update() = 0;
    bool operator==(const CObserver &observer) const{
        return (mStrName==observer.mStrName && mpSubject == observer.mpSubject);
    }
protected:
    string mStrName;
    CSubject *mpSubject;
};

//ConcreteSubject,具体通知者或者具体主题
class CBoss : public CSubject{
public:
    virtual void attach(CObserver* pObserver){
        mObserverVector.push_back(pObserver);
    }
    virtual void detach(CObserver* pObserver){
        for(auto it = mObserverVector.begin() ; it != mObserverVector.end(); ){
            if( *it == pObserver){
                mObserverVector.erase(it);
            }
            else
                it++;
        }
    }
    virtual void notify(){
//        for(auto it = mObserverVector.begin() ; it != mObserverVector.end(); it++){
//            (*it)->update();
//        }
        for_each(mObserverVector.begin(),mObserverVector.end(),[](CObserver * it){ it->update(); } );
    }
private:
    string mStrAction;
    vector<CObserver *> mObserverVector;
};

//ConcreteObserver,具体观察者，股票观察者
class CStockObserver:public CObserver{
public:
    CStockObserver(){}
    CStockObserver(string strName,CSubject *pSubject):CObserver(strName,pSubject){}
    void update(){
        cout<<mpSubject->getSujectState()<<" "<<mStrName<<" Close stock,Continue work"<<endl;
    }
};

//ConcreteObserver，具体观察者，NBA观察者
class CNBAObserver:public CObserver{
public:
    CNBAObserver(){}
    CNBAObserver(string strName,CSubject *pSubject):CObserver(strName,pSubject){}
    void update(){
        cout<<mpSubject->getSujectState()<<" "<<mStrName<<" Close NBA,Continue work"<<endl;
    }
};


#endif // OBSERVERPATTERN_H
