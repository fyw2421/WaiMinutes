//============================
//Abstract Factory Pattern-client
//============================
#include "AbstractFactoryPattern.h"

int main(){

    CUserTable userTable;
    CDepartmentTable departmentTable;

    //Concrete Factory
    IFactory *factory = new CAccessFactory();

    //Concrete Product A1
    IUSer *user = nullptr;
    if( factory != nullptr)
        user = factory->createUser();
    if( user != nullptr){
        user->insert(userTable);
        user->getUser(0);
    }

    //Concrete product B1
    IDepartment *department = nullptr;
    if( factory != nullptr)
        department = factory->createDepartment();
    if( department != nullptr){
        department->insert(departmentTable);
        department->getDepartment(0);
    }

    if( factory != nullptr){
        delete factory;
        factory = nullptr;
    }
    if( user != nullptr ){
        delete user;
        user = nullptr;
    }
    if( department != nullptr ){
        delete department;
        department = nullptr;
    }
    return 1;
}
