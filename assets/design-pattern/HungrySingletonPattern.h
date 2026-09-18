//============================
//HungrySingletonPattern.h
//============================
#ifndef HUNGRYSINGLETONPATTERN_H
#define HUNGRYSINGLETONPATTERN_H

#include <iostream>
using namespace std;

//singleton pattern- hungry
class CHungrySingleton{
public:
    void eat(){ cout<<" I am hungry and want to eat"<<endl; }
public:
    static CHungrySingleton hungrySingleton;
private:
    CHungrySingleton(){}
    ~CHungrySingleton(){}
    CHungrySingleton(const CHungrySingleton&){}
    CHungrySingleton &operator=(const CHungrySingleton&){ return *this; }
};

#define HUNGRY_SINGLETON CHungrySingleton::hungrySingleton

#endif // HUNGRYSINGLETONPATTERN_H
