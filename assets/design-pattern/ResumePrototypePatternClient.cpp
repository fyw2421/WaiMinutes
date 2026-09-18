//============================
//ResumePrototypePatternClient.cpp
//============================

#include "ResumePrototypePattern.h"

int main(){

    CPrototype *pResumeOne = new CResume("Wai");
    pResumeOne->setPersonalInfo("male","20");
    pResumeOne->setWorkExperience("1988-1999","XXX Company");

    CPrototype *pResumeTwo = pResumeOne->clone();
    pResumeTwo->setWorkExperience("1999-2000","YYY Company");

    CPrototype *pResumeThree = pResumeOne->clone();
    pResumeThree->setWorkExperience("2000-20001","ZZZ Company");

    pResumeOne->display();
    pResumeTwo->display();
    pResumeThree->display();

    delete pResumeOne;
    delete pResumeTwo;
    delete pResumeThree;
    return 1;
}
