//============================
//ResumePrototypePattern.h
//============================
#ifndef RESUMEPROTOTYPEPATTERN_H
#define RESUMEPROTOTYPEPATTERN_H

#include<iostream>>
using namespace std;

//Work Experience class
class CWorkExperience{
private:
    string mStrWorkDate;
    string mStrCompany;
public:
    CWorkExperience(){}
    ~CWorkExperience(){}

    string getWorkDate(){ return mStrWorkDate; }
    string getCompany(){ return mStrCompany; }
    void setWorkDate(string strWorkDate){ mStrWorkDate = strWorkDate; }
    void setCompany(string strCompany){ mStrCompany = strCompany; }
    void display(){
        cout<<"Work Experience "<<endl;
        cout<<"  "<<mStrWorkDate<<"  "<<mStrCompany<<endl;
    }
};

//Abstract Prototype
class CPrototype{
protected:
    string mstrName;
    string mstrSex;
    string mstrAge;
public:
    virtual ~CPrototype(){}
    virtual void setPersonalInfo(string strSex,string strAge) = 0;
    virtual void setWorkExperience(string strWorkDate,string strCompany) = 0;
    virtual void display() = 0;
    virtual CPrototype *clone() = 0 ;
};

//Concrete Prototype
class CResume : public CPrototype{
private:
    CWorkExperience *mpWorkExperience;
public:
    CResume(string strName){
        mstrName = strName;
        mpWorkExperience = new CWorkExperience();
    }
    virtual ~CResume(){
        if( mpWorkExperience != nullptr){
            delete mpWorkExperience;
            mpWorkExperience = nullptr;
        }
    }

    virtual void setPersonalInfo(string strSex, string strAge){
        mstrSex = strSex;
        mstrAge = strAge;
    }
    virtual void setWorkExperience(string strWorkDate,string strCompany){
        mpWorkExperience->setWorkDate(strWorkDate);
        mpWorkExperience->setCompany(strCompany);
    }
    virtual void display(){
        cout<<"Name : "<<mstrName<<endl;
        cout<<"Age  : "<<mstrAge<<endl;
        mpWorkExperience->display();
    }
    virtual CResume *clone(){
        CResume *pResume = new CResume(this->mstrName);
        pResume->setPersonalInfo(this->mstrSex,this->mstrAge);
        pResume->setWorkExperience(this->mpWorkExperience->getWorkDate(),this->mpWorkExperience->getCompany());
        return pResume;
    }
};
#endif // RESUMEPROTOTYPEPATTERN_H
