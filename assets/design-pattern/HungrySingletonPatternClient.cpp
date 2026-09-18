//============================
//HungrySingletonPatternClient.cpp
//============================
#include "HungrySingletonPattern.h"

CHungrySingleton CHungrySingleton::hungrySingleton;

int main(){
    CHungrySingleton::hungrySingleton.eat();
    HUNGRY_SINGLETON.eat();
    return 1;
}
