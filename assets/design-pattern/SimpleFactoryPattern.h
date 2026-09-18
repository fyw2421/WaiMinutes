//============================
//Simple Factory Pattern
//============================
#ifndef SIMPLEFACTORYPATTERN_H
#define SIMPLEFACTORYPATTERN_H

//Abstract Product class
class COperation{
private:
    int mFirstOperand = 0;
    int mSecondOperand = 0;
public:
    virtual ~COperation(){}
    void setFirstOperand(int firstOperand){
        mFirstOperand = firstOperand;
    }
    void setScondOperand(int secondOperand){
        mSecondOperand = secondOperand;
    }
    int getFirstOperand(){
        return mFirstOperand;
    }
    int getScondOperand(){
        return mSecondOperand;
    }
    virtual int getResult(){
        auto result = 0 ;
        return result;
    }
};

//ConcreteProduct class
class CAddOperation : public COperation{
public:
    virtual ~CAddOperation(){}
    virtual int getResult(){
        return (COperation::getFirstOperand() + COperation::getScondOperand());
    }
};

//ConcreteProduct class
class CSubOperation : public COperation{
public:
    virtual ~CSubOperation(){}
    virtual int getResult(){
        return (COperation::getFirstOperand() - COperation::getScondOperand());
    }
};

//Factory class
class COperationFactory{
public:
    COperation *create(char op){
        switch(op){
        case '+':
            return (new CAddOperation());
            break;
        case '-':
            return (new CSubOperation());
            break;
        default:
            return nullptr;
            break;
        }
    }
};

#endif // SIMPLEFACTORYPATTERN_H
