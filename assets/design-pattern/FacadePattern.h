//============================
//FacadePattern.h
//============================
#ifndef FACADEPATTERN_H
#define FACADEPATTERN_H

#include <iostream>
using namespace std;

class CSubSysOne{
public:
    void methodOne(){ cout<<"Method One"<<endl; }
};

class CSubSysTwo{
public:
    void methodTwo(){ cout<<"Method Two"<<endl; }
};

class CSubSysThree{
public:
    void methodThree(){ cout<<"Method Three"<<endl; }
};

class CFacade{
private:
    CSubSysOne *mpSubSysOne;
    CSubSysTwo *mpSubSysTwo;
    CSubSysThree *mpSubSysThree;
public:
    CFacade(){
        mpSubSysOne = new CSubSysOne();
        mpSubSysTwo = new CSubSysTwo();
        mpSubSysThree = new CSubSysThree();
    }
    ~CFacade(){
        delete mpSubSysOne;
        delete mpSubSysTwo;
        delete mpSubSysThree;
    }
    void facadeMethod(){
        mpSubSysOne->methodOne();
        mpSubSysTwo->methodTwo();
        mpSubSysThree->methodThree();
    }
};
#endif // FACADEPATTERN_H
