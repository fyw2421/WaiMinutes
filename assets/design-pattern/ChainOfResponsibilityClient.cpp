//============================
//ChainOfResponsibilityClient.cpp
//============================

#include "ChainOfResponsibility.h"

int main(){

    CManager *pCommonManager = new CCommonManager("CommonManaer");
    CManager *pMajorDomo = new CMajorDomo("MajorDomo");
    CManager *pGeneralManager = new CGeneralManager("GeneralManager");

    //设置上级,完全可以按照实际需求来进行更改
    pCommonManager->setSuperior(pMajorDomo);
    pMajorDomo->setSuperior(pGeneralManager);

    //请求1:请假1天
    CRequest *pRequest = new CRequest();
    pRequest->setType("Ask For Leave");
    pRequest->setContent("Ask For Leave");
    pRequest->setNumber(1);
    //客户端的申请都是由“经理”发起,但实际谁来决策由具体管理类来处理,客户端不知道
    pCommonManager->hanldeRequest(pRequest);

    //请求2：请假4天
    pRequest->setNumber(4);
    pCommonManager->hanldeRequest(pRequest);

    //请求3:加薪500元
    pRequest->setType("Salary Increase");
    pRequest->setContent("Salary Increase");
    pRequest->setNumber(450);
    pCommonManager->hanldeRequest(pRequest);

    //请求4:加薪1000元
    pRequest->setNumber(1000);
    pCommonManager->hanldeRequest(pRequest);

    //请求5:升职
    pRequest->setType("Promotion");
    pRequest->setContent("Promotion");
    pRequest->setNumber(1);
    pCommonManager->hanldeRequest(pRequest);

    delete pRequest;
    delete pCommonManager;
    delete pMajorDomo;
    delete pGeneralManager;
    return 1;
}
