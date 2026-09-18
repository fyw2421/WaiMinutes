//============================
//StrategyPatternClient.cpp
//============================

#include "StrategyPattern.h"

int main(){
    int a = 0, b = 0;
    cout<<"please input two integers"<<endl;
    cin>>a>>b;
    cout<<"please input operator,+ or -"<<endl;
    char c = '+';
    cin>>c;
    COperation *pOperation = nullptr;
    switch(c){
    case '+':
        pOperation= new CAddOperation(a,b);
        break;
    case '-':
        pOperation = new CSubOperation(a,b);
        break;
    default:
        break;
    }
    //策略模式
    cout<<"strategy"<<endl;
    if( pOperation != nullptr ){
        CContext *pContext = new CContext( pOperation );
        cout<<pContext->getResult()<<endl;
        delete pOperation;
        pOperation = nullptr;
    }

    //策略模式与简单工厂模式相结合
    cout<<"combine Strategy with SimpleFactory"<<endl;
    CCombineContext *pCombineContext = new CCombineContext(a,c,b);
    cout<<pCombineContext->getResult()<<endl;
    delete pCombineContext;
    pCombineContext = nullptr;
    return 1;
}
