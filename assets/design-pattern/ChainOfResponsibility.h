//============================
//ChainOfResponsibility.h
//============================
#ifndef CHAINOFRESPONSIBILITY_H
#define CHAINOFRESPONSIBILITY_H

#include <iostream>
using namespace std;


//请求类
class CRequest{
public:
    void setType(string strType){ mStrRequestType = strType; }
    string getType(){ return mStrRequestType; }

    void setContent(string strContent){ mStrRequestContent = strContent; }
    string getContent(){ return mStrRequestContent; }

    void setNumber(int nNumber){ mNumber = nNumber; }
    int getNumber(){ return mNumber; }
private:
    string mStrRequestType;
    string mStrRequestContent;
    int mNumber;
};


//Handler类抽象类,此处为Manager类
class CManager{
public:
    CManager(){}
    CManager(string strName):mStrName(strName),mpManager(nullptr){}
    virtual ~CManager(){}

    //设置继任者
    void setSuperior(CManager *pSuperior){ mpManager = pSuperior; }
    //处理请求的抽象方法
    virtual void hanldeRequest(CRequest *pRequest) = 0;
protected:
    string mStrName;
    CManager *mpManager;
};

//ConcreteHandler1:此处为经理CommonManager
class CCommonManager : public CManager{
public:
    CCommonManager(string strName):CManager(strName){}
    virtual void hanldeRequest(CRequest* pRequest){
        if( pRequest->getType() == "Ask For Leave" && pRequest->getNumber() <= 2 )
            cout<<mStrName<<":"<<pRequest->getContent()<<" Number:"<<pRequest->getNumber()<<" was Approved"<<endl;
        //自己处理不了,转移到下一位进行处理
        else
            mpManager->hanldeRequest(pRequest);
    }
};

//ConcreteHandler2:此处为总监,Majordomo
class CMajorDomo : public CManager{
public:
    CMajorDomo(string strName):CManager(strName){}
    virtual void hanldeRequest(CRequest* pRequest){
        if( pRequest->getType() == "Ask For Leave" && pRequest->getNumber() <= 5 )
            cout<<mStrName<<":"<<pRequest->getContent()<<" Number:"<<pRequest->getNumber()<<" was Approved"<<endl;
        //自己处理不了,转移到下一位进行处理
        else
            mpManager->hanldeRequest(pRequest);
    }
};

//ConcreteHandler3:此处为总经理,GeneralManager
class CGeneralManager : public CManager{
public:
    CGeneralManager(string strName):CManager(strName){}
    virtual void hanldeRequest(CRequest* pRequest){
        string strType = pRequest->getType();
        if( strType == "Ask For Leave" )
            cout<<mStrName<<":"<<pRequest->getContent()<<" Number:"<<pRequest->getNumber()<<" was Approved"<<endl;
        else if( strType == "Salary Increase" && pRequest->getNumber() <= 500 )
            cout<<mStrName<<":"<<pRequest->getContent()<<" Number:"<<pRequest->getNumber()<<" was Approved"<<endl;
        else if( strType == "Salary Increase" && pRequest->getNumber() > 500 )
            cout<<mStrName<<":"<<pRequest->getContent()<<" Number:"<<pRequest->getNumber()<<" was Rejected"<<endl;
        else
            cout<<mStrName<<":"<<pRequest->getContent()<<" Number:"<<pRequest->getNumber()<<" was Rejected"<<endl;
    }
};
#endif // CHAINOFRESPONSIBILITY_H
