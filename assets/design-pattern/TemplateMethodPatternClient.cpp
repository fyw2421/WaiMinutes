//============================
//TemplateMethodPatternClient.cpp
//============================
#include "TemplateMethodPattern.h"

int main(){

    cout<<"Student A Papaer:"<<endl;
    CTestPaper *pStudentA = new CConcretePaperA();
    pStudentA->testQuestion1();
    pStudentA->testQuestion2();
    pStudentA->testQuestion3();
    cout<<endl;

    cout<<"Student B Papaer:"<<endl;
    CTestPaper *pStudentB = new CConcretePaperB();
    pStudentB->testQuestion1();
    pStudentB->testQuestion2();
    pStudentB->testQuestion3();
    return 1;
}
