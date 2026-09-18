//============================
//TemplateMethodPattern.h
//============================
#ifndef TEMPLATEMETHODPATTERN_H
#define TEMPLATEMETHODPATTERN_H

#include <iostream>
using namespace std;

//AbstractClass,实现了一个模板,定义了算法的骨架,组成骨架的具体步骤放在子类中实现
class CTestPaper{
public:
    void testQuestion1(){
        cout<<"How many bits per byte"<<endl;
        cout<<"A. 8 B. 4 C. 2 D. 1"<<endl;
        cout<<"answer: "<<answer1()<<endl;
    }
    void testQuestion2(){
        cout<<"How many meters per kilometers"<<endl;
        cout<<"A. 8 B. 1000 C. 2 D. 1"<<endl;
        cout<<"answer: "<<answer2()<<endl;
    }
    void testQuestion3(){
        cout<<"How many grams per kilograms"<<endl;
        cout<<"A. 8 B. 4 C. 2 D. 1000"<<endl;
        cout<<"answer: "<<answer3()<<endl;
    }
protected:
    virtual string answer1() = 0;
    virtual string answer2() = 0;
    virtual string answer3() = 0;
};


//ConcreteClass,实现具体步骤
class CConcretePaperA : public CTestPaper{
protected:
    virtual string answer1(){ return "A"; }
    virtual string answer2(){ return "B"; }
    virtual string answer3(){ return "D"; }
};

//ConcreteClass,实现具体步骤
class CConcretePaperB : public CTestPaper{
protected:
    virtual string answer1(){ return "B"; }
    virtual string answer2(){ return "C"; }
    virtual string answer3(){ return "A"; }
};


#endif // TEMPLATEMETHODPATTERN_H
