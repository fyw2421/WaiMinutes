//============================
//InterpreterPatternClient.cpp
//============================

#include "InterpreterPattern.h"
#include <algorithm>

int main(){
    CPlayContext playContext;
    cout<<"ÉÏº£Ì²"<<endl;
    playContext.setText("O 2 E 0.5 G 0.5 A 3 E 0.5 G 0.5 D 3 E 0.5 G 0.5 A 0.5 O 3 C 1 O 2 A 0.5 G 1 C 0.5 E 0.5 D 3");

    CExpression *pExpression = nullptr;
    while(playContext.getText().length()>0){
        char c=playContext.getText()[0];
        switch(c){
        case 'O':
            pExpression=new CScale();
            break;
        case 'C':
        case 'D':
        case 'E':
        case 'F':
        case 'G':
        case 'A':
        case 'B':
        case 'P':
            pExpression=new CNote();
            break;
        }
        pExpression->interpret(&playContext);
        delete pExpression;
    }
    return 1;
}
