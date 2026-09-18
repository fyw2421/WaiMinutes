//============================
//StatePattern.h
//============================
#ifndef STATEPATTERN_H
#define STATEPATTERN_H

#include <iostream>
using namespace std;

class CWork;

//抽象状态类
class CState{
public:
    virtual void writeProgram(CWork *pWork) = 0;
    virtual ~CState(){}
};

//context类
class CWork{
public:
    CWork();
    ~CWork(){
        if( mpState != nullptr){
            delete mpState;
            mpState = nullptr;
        }
    }
    void setState(CState *pState){
        if( mpState != nullptr )
            delete mpState;
        mpState = pState;
    }
    void writeProgram(){ mpState->writeProgram(this); }

    void setTime(double dTime){ mTime = dTime; }
    double getTime(){ return mTime; }
private:
    CState *mpState;
    double mTime;
};

//具体状态类:睡眠状态
class CSleepingState : public CState{
public:
    virtual void writeProgram(CWork* pWork){
        cout<<"Time: "<<pWork->getTime()<<" , Sleeping...."<<endl;
    }
};

//具体状态类:晚上状态
class CEveningState : public CState{
public:
    virtual void writeProgram(CWork* pWork){
        if( pWork->getTime() < 21 )
            cout<<"Time: "<<pWork->getTime()<<" , Sport...."<<endl;
        else{
            pWork->setState(new CSleepingState());
            pWork->writeProgram();
        }
    }
};

//具体状态类:下午状态
class CAfternoonState : public CState{
public:
    virtual void writeProgram(CWork* pWork){
        if( pWork->getTime() < 18 )
            cout<<"Time: "<<pWork->getTime()<<" , afternoon work...."<<endl;
        else{
            pWork->setState(new CEveningState());
            pWork->writeProgram();
        }
    }
};

//具体状态类:上午状态
class CForenoonState : public CState{
public:
    virtual void writeProgram(CWork* pWork){
        if( pWork->getTime() < 12 )
            cout<<"Time: "<<pWork->getTime()<<" , forcenoon work...."<<endl;
        else{
            pWork->setState(new CAfternoonState());
            pWork->writeProgram();
        }
    }
};
CWork::CWork(){
    mpState = new CForenoonState();
    mTime = 9;
}


#endif // STATEPATTERN_H
