//============================
//InterpreterPattern.h
//============================
#ifndef INTERPRETERPATTERN_H
#define INTERPRETERPATTERN_H

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

//Context,此处为演奏内容类
class CPlayContext{
public:
    void setText(string strText){ mStrText = strText; }
    string getText(){ return mStrText; }
private:
    string mStrText;
};

//AbstractExpression,此处为表达式类
class CExpression{
public:
    void interpret(CPlayContext *pPlayContext){
        string strText = pPlayContext->getText();

        if(strText.length() <= 0 )
            return;
        else{
            vector<string> strVector;
            string strTemp,strBuffer;
            for(istringstream ss(strText);ss>>strTemp;)
                strVector.push_back(strTemp);
            execute(strVector.at(0),strVector.at(1));
            strVector.erase(strVector.begin(),strVector.begin() + 2);
            ostringstream oss;
            for(auto it = strVector.begin() ; it != strVector.end() ; it++)
                oss<<*it<<" ";
            pPlayContext->setText(oss.str());
        }
    }
    virtual void execute(string strKey,string strValue) = 0;
};
//ConcreteExpression,此处为音符类
class CNote : public CExpression{
public:
    virtual void execute(string strKey, string strValue){
        string strNote;
        switch (strKey[0]){
        case 'C':
            strNote="1";
            break;
        case 'D':
            strNote="2";
            break;
        case 'E':
            strNote="3";
            break;
        case 'F':
            strNote="4";
            break;
        case 'G':
            strNote="5";
            break;
        case 'A':
            strNote="6";
            break;
        case 'B':
            strNote="7";
            break;
        }
        cout<<strNote<<" ";
    }
};

//ConcreteExpression,此处为音阶类
class CScale : public CExpression{
public:
    virtual void execute(string strKey, string strValue){
        string strScale;
        switch(strValue[0])  {
        case '1':
            strScale="low";
            break;
        case '2':
            strScale="medium";
            break;
        case'3':
            strScale="high";
            break;
        }
        cout<<endl<<strScale<<" ";
    }
};
#endif // INTERPRETERPATTERN_H
