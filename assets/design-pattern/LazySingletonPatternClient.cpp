//============================
//LazySingletonPatternClient.cpp
//============================

#include "SingletonPattern.h"

pthread_mutex_t CLazySingleton::mutex = PTHREAD_MUTEX_INITIALIZER;

int main(){

    //mutex lock lazy singleton
    CLazySingleton::getInstance().eat();
    MUTEX_LAZY_SINGLETON.eat();

    //c++11 lazy singleton
    CMeyersSingleton::getInstance().drink();
    C11_LAZY_SINGLETON.drink();
    return 1;
}
