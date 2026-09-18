//============================
//Factory Method Pattern
//============================
#ifndef FACTORYMETHODPATTERN_H
#define FACTORYMETHODPATTERN_H

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

//AbstractFactory class
class CAbstractFactory{
public:
    virtual ~CAbstractFactory(){}
    virtual COperation *createOperation(){
        return new COperation();
    };
};

//ConcreteFactory class
class CAddFactory : public CAbstractFactory{
public:
    virtual ~CAddFactory(){}
    virtual COperation *createOperation(){
        return new CAddOperation();
    }
};

//ConcreteFactory class
class CSubFactory : public CAbstractFactory{
public:
    virtual ~CSubFactory(){}
    virtual COperation *createOperation(){
        return new CSubOperation();
    }
};


#endif // FACTORYMETHODPATTERN_H
