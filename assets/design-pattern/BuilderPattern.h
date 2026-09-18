//============================
//BuilderPattern.h
//============================
#ifndef BUILDERPATTERN_H
#define BUILDERPATTERN_H

#include<iostream>
#include<vector>
using namespace std;

//Product class
class CProduct{
private:
    vector<string> mParts;
public:
    void add(string part){ mParts.push_back(part); }
    void show(){
        cout<<"Product Build------"<<endl;
        for(auto part : mParts)
            cout<<part<<endl;
    }
};

//abstract builder
class CBuilder{
public:
    virtual void buildPartA() = 0;
    virtual void buildPartB() = 0;
    virtual CProduct *getProduct() = 0;
};

//concrete builder 1
class CConcreteBuilder1 : public CBuilder{
private:
    CProduct *mpProduct;
public:
    CConcreteBuilder1(){ mpProduct = new CProduct(); }
    virtual ~CConcreteBuilder1(){
        if( mpProduct != nullptr ){
            delete mpProduct;
            mpProduct = nullptr;
        }
    }
    virtual void buildPartA(){ mpProduct->add("One"); }
    virtual void buildPartB(){ mpProduct->add("Two"); }
    virtual CProduct *getProduct(){ return mpProduct; }
};

//concrete builder 2
class CConcreteBuilder2 : public CBuilder{
private:
    CProduct *mpProduct;
public:
    CConcreteBuilder2(){ mpProduct = new CProduct(); }
    virtual ~CConcreteBuilder2(){
        if( mpProduct != nullptr ){
            delete mpProduct;
            mpProduct = nullptr;
        }
    }
    virtual void buildPartA(){ mpProduct->add("A"); }
    virtual void buildPartB(){ mpProduct->add("B"); }
    virtual CProduct *getProduct(){ return mpProduct; }
};

//director class
class CDirector{
public:
    void construct(CBuilder *builder){
        builder->buildPartA();
        builder->buildPartB();
    }
};

#endif // BUILDERPATTERN_H
