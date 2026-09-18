//============================
//Abstract Factory Pattern
//============================
#ifndef ABSTRACTFACTORYPATTERN_H
#define ABSTRACTFACTORYPATTERN_H

#include <iostream>
using namespace std;

//Data Table:user
class CUserTable{
private:
    int mID;
    string mStrName;
public:
    int getID(){ return mID; }
    string getName(){ return mStrName; }
    void setID(int id){ mID = id; }
    void setName(string name){ mStrName = name; }
};

//Data Table:department
class CDepartmentTable{
private:
    int mID;
    string mStrName;
public:
    int getID(){ return mID; }
    string getName(){ return mStrName; }
    void setID(int id){ mID = id; }
    void setName(string name){ mStrName = name; }
};

//Abstract Product A: IUser
class IUSer{
public:
    virtual void insert(CUserTable user) = 0;
    virtual CUserTable *getUser(int id) = 0;
};

//Concrete Product A1: AccessUser
class CAcessUser : public IUSer{
public:
    virtual void insert(CUserTable user){
        cout<<"AccessUser add record"<<endl;
    }
    virtual CUserTable *getUser(int id){
        cout<<"AccessUser get record"<<endl;
        return nullptr;
    }
};

//Concrete Product A2: SQLserverUser
class CSQLServerUser : public IUSer{
public:
    virtual void insert(CUserTable user){
        cout<<"SQLServerUser add record"<<endl;
    }
    virtual CUserTable *getUser(int id){
        cout<<"SQLServerUser get record"<<endl;
        return nullptr;
    }
};

//Abstract Product B: IDepartment
class IDepartment{
public:
    virtual void insert(CDepartmentTable department) = 0;
    virtual CDepartmentTable *getDepartment(int id) = 0;
};

//Concrete Product B1: AccessDepartment
class CAcessDepartment : public IDepartment{
public:
    virtual void insert(CDepartmentTable department){
        cout<<"AccessDepartment add record"<<endl;
    }
    virtual CDepartmentTable *getDepartment(int id){
        cout<<"AccessDepartment get record"<<endl;
        return nullptr;
    }
};

//Concrete Product B2: SQLserverDepartment
class CSQLServerDepartment : public IDepartment{
public:
    virtual void insert(CDepartmentTable department){
        cout<<"SQLServerDepartmentr add record"<<endl;
    }
    virtual CDepartmentTable *getDepartment(int id){
        cout<<"SQLServerDepartment get record"<<endl;
        return nullptr;
    }
};

//Abstract Factory: IFactory
class IFactory{
public:
    virtual IUSer *createUser() = 0;
    virtual IDepartment *createDepartment() = 0 ;
};

//Concrete Factory:AccessFactory
class CAccessFactory : public IFactory{
public:
    virtual IUSer *createUser(){
        return new CAcessUser();
    }
    virtual IDepartment *createDepartment(){
        return new CAcessDepartment();
    }
};

//Concrete Factory: SQLServerFactory
class CSQLServerFactory : public IFactory{
public:
    virtual IUSer *createUser(){
        return new CSQLServerUser();
    }
    virtual IDepartment *createDepartment(){
        return new CSQLServerDepartment();
    }
};
#endif // ABSTRACTFACTORYPATTERN_H
