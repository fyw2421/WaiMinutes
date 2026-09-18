//============================
//StrategyPattern.h
//============================
#ifndef STRATEGYPATTERN_H
#define STRATEGYPATTERN_H

#include <iostream>
using namespace std;

//策略基类
class COperation{
public:
    virtual ~COperation(){}
    virtual int getResult(){
        auto result = 0 ;
        return result;
    }
protected:
    int mFirstOperand = 0;
    int mSecondOperand = 0;
};

//策略具体类:加法
class CAddOperation : public COperation{
public:
    CAddOperation(int nFirstOperand,int nSecondOperand){
        mFirstOperand = nFirstOperand;
        mSecondOperand = nSecondOperand;
    }
    virtual ~CAddOperation(){}
    virtual int getResult(){
        return (mFirstOperand + mSecondOperand);
    }
};

//策略具体类:减法
class CSubOperation : public COperation{
public:
    CSubOperation(int nFirstOperand,int nSecondOperand){
        mFirstOperand = nFirstOperand;
        mSecondOperand = nSecondOperand;
    }
    virtual ~CSubOperation(){}
    virtual int getResult(){
        return (mFirstOperand - mSecondOperand);
    }
};

class CContext{
public:
    CContext(COperation *pOperation):mpOperation(pOperation){}
    int getResult(){ return mpOperation->getResult();}
private:
    COperation *mpOperation;
};

//策略模式与简单工厂模式相结合
class CCombineContext{
public:
    CCombineContext(int nFirstOperand,char cOperator,int nSecondOperand){
        mpOperation = nullptr;
        switch(cOperator){
        case '+':
            mpOperation= new CAddOperation(nFirstOperand,nSecondOperand);
            break;
        case '-':
            mpOperation = new CSubOperation(nFirstOperand,nSecondOperand);
            break;
        default:
            break;
        }
    }
    ~CCombineContext(){
        if( mpOperation != nullptr){
            delete mpOperation;
            mpOperation = nullptr;
        }
    }
    int getResult(){ return mpOperation->getResult();}
private:
    COperation *mpOperation;
};
#endif // STRATEGYPATTERN_H
