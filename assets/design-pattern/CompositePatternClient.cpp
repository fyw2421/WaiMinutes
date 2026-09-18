//============================
//CompositePatternClient.cpp
//============================

#include "CompositePattern.h"

int main(){
    CCompany *pRootCompany = new CConcreteCompany("BeiJing Parent Company");
    pRootCompany->add(new CHRDepartment("BeiJing Parent Company HR"));
    pRootCompany->add(new CFinanceDepartment("BeiJing Parent Company Finance"));

    CCompany *pHDCompany = new CConcreteCompany("ShangHai Branch Company ");
    pHDCompany->add(new CHRDepartment("ShangHai Branch Company HR"));
    pHDCompany->add(new CFinanceDepartment("ShangHai Branch Company Finance"));

    pRootCompany->add(pHDCompany);

    CCompany *pNJCompany = new CConcreteCompany("NanJing Branch Company ");
    pNJCompany->add(new CHRDepartment("NanJing Branch Company HR"));
    pNJCompany->add(new CFinanceDepartment("NanJing Branch Company Finance"));

    pRootCompany->add(pNJCompany);

    CCompany *pHZCompany = new CConcreteCompany("HangZhou Branch Company ");
    pHZCompany->add(new CHRDepartment("HangZhou Branch Company HR"));
    pHZCompany->add(new CFinanceDepartment("HangZhou Branch Company Finance"));

    pRootCompany->add(pHZCompany);

    cout<<endl;
    cout<<"Company Organization: "<<endl;
    pRootCompany->display(1);

    cout<<"Responsibility: "<<endl;
    pRootCompany->lineOfDuty();

    cout<<endl<<endl;
    cout<<"Release NanJing Branch Company"<<endl;
    pRootCompany->remove(pNJCompany);

    cout<<endl<<endl;
    cout<<"Organization after release Nanjin branch company"<<endl;
    pRootCompany->display(1);

    delete pRootCompany;

    return 1;
}
