//============================
//LazySingletonPattern.h
//============================
#ifndef SINGLETONPATTERN_H
#define SINGLETONPATTERN_H

#include<iostream>
#include<pthread.h>
using namespace std;

//Lazy Singleton Pattern-set mutex lock(before C++11)
class CLazySingleton{
public:
    static CLazySingleton& getInstance(){
        pthread_mutex_lock(&mutex);
        static CLazySingleton lazySingleton; //C++11标准下local static对象初始化在多线程条件下安全
        pthread_mutex_unlock(&mutex);
        return lazySingleton;
    }
    void eat(){ cout<<"I am mutex lock lazy and want to eat"<<endl; }
private:
    static pthread_mutex_t mutex;
private:
    CLazySingleton(){}
    CLazySingleton( const CLazySingleton&){}
    CLazySingleton &operator=(const CLazySingleton&){ return *this; }
    ~CLazySingleton(){}
};
#define MUTEX_LAZY_SINGLETON CLazySingleton::getInstance()

//Lazy Singleton Pattern-thread safe in C++11
class CMeyersSingleton{
public:
    static CMeyersSingleton& getInstance(){
        static CMeyersSingleton meyersSingleton;
        return meyersSingleton;
    }
    void drink(){ cout<<"I am meyers In c++11 lazy and want to drink "<<endl; }
private:
    CMeyersSingleton(){}
    ~CMeyersSingleton(){}
    CMeyersSingleton(const CMeyersSingleton&){}
    CMeyersSingleton &operator=(const CMeyersSingleton&){ return *this; }
};
#define C11_LAZY_SINGLETON CMeyersSingleton::getInstance()

#endif // SINGLETONPATTERN_H
