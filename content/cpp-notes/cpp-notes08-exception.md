---
title: "C++Notes08 : Exception"
weight: 8
description: "throw try-catch Exception-Base-Class Common-Exception-Class"
date: 2018-05-22
tags: ["C++"]
featureimage: "covers/cpp-notes08-exception.svg"
---
## 一、异常处理

### 1.1 异常处理步骤

**堆栈解退**

　　函数出现异常,程序会释放堆栈中的内存,直到找到try后,将控制权交给异常处理程序,而不是调用函数后下一条语句.

　　C++异常机制是对程序运行过程中发生的异常响应机制,对异常的处理有三个组成部分:

1. 引发异常,throw关键字

2. 框定异常,try关键字

3. 处理异常,catch关键字

```cpp
// error3.cpp -- using an exception
#include <iostream>
double hmean(double a, double b);

int main()
{
    double x, y, z;

    std::cout << "Enter two numbers: ";
    while (std::cin >> x >> y)
    {
        try {                   // start of try block
            z = hmean(x,y);
        }                       // end of try block
        catch (const char * s)  // start of exception handler
        {
            std::cout << s << std::endl;
            std::cout << "Enter a new pair of numbers: ";
            continue;
        }                       // end of handler
        std::cout << "Harmonic mean of " << x << " and " << y
            << " is " << z << std::endl;
        std::cout << "Enter next set of numbers <q to quit>: ";
    }
    std::cout << "Bye!\n";
    return 0;
}

double hmean(double a, double b)
{
    if (a == -b)
        throw "bad hmean() arguments: a = -b not allowed";
    return 2.0 * a * b / (a + b);
}
```

### 1.2 抛出异常和捕获异常

　　异常抛出后总是沿着函数调用链往上,直到被某个函数捕捉住.一般在函数的声明和定义的头部加上可能抛出的异常集合.

```cpp
void f() throw(A,B); //可能抛出A和B类型的异常
void g();            //可能抛出任何类型的异常
void h() throw();    //不会抛出异常

cat(...)             //捕获任何异常
```

异常机制是基于类型匹配,不是基于参数匹配.

### 1.3 使用异常

**Sample01**:

```cpp
//=====================================
// f1504.cpp
// 异常方式
//=====================================
#include<fstream>
#include<iostream>
#include<string>
using namespace std;
//-------------------------------------
void procFileName(string s);
void procOpenMode(string s);
void openIn(string s);
void openOut(string s);
//-------------------------------------
int main(){
  procFileName("iabc");
  procFileName("oabc");
}//------------------------------------
void procFileName(string s){
  try{
    //for(char c='0'; c<='9'; c++) procOpenMode(s + c+".txt");
  }catch(string s){
    cout<<"error opening "<<s<<" not existed.\n";
  }
}//------------------------------------
void procOpenMode(string s){
  if(s[0]=='i') openIn(s);
  else openOut(s);
}//------------------------------------
void openIn(string s){
  const char *pFile = s.c_str();
  ifstream in(pFile);
  if(!in) throw s+" inFile";
  for(string line; getline(in, line); cout<<line<<"\n");
}//------------------------------------
void openOut(string s){
  const char *pFile = s.c_str();
  fstream out(pFile,ios::in|ios::out|ios::ate);
  if(!out) throw s+string(" outFile");
  out<<s+" outFile is ok.\n";
  cout<<s+" is here.\n";
}//====================================
```

**Sample02将对象作为异常类型**:

```cpp
// exc_mean.h  -- exception classes for hmean(), gmean()
#include <iostream>

class bad_hmean
{
private:
    double v1;
    double v2;
public:
    bad_hmean(double a = 0, double b = 0) : v1(a), v2(b){}
    void mesg();
};

inline void bad_hmean::mesg()
{
    std::cout << "hmean(" << v1 << ", " << v2 <<"): "
              << "invalid arguments: a = -b\n";
}

class bad_gmean
{
public:
    double v1;
    double v2;
    bad_gmean(double a = 0, double b = 0) : v1(a), v2(b){}
    const char * mesg();
};

inline const char * bad_gmean::mesg()
{
    return "gmean() arguments should be >= 0\n";
}
```
**测试**

```cpp
//error4.cpp ?using exception classes
#include <iostream>
#include <cmath> // or math.h, unix users may need -lm flag
#include "exc_mean.h"
// function prototypes
double hmean(double a, double b) throw(bad_hmean);
double gmean(double a, double b) throw(bad_gmean);
int main()
{
    using std::cout;
    using std::cin;
    using std::endl;

    double x, y, z;

    cout << "Enter two numbers: ";
    while (cin >> x >> y)
    {
        try {                  // start of try block
            z = hmean(x,y);
            cout << "Harmonic mean of " << x << " and " << y
                << " is " << z << endl;
            cout << "Geometric mean of " << x << " and " << y
                << " is " << gmean(x,y) << endl;
            cout << "Enter next set of numbers <q to quit>: ";
        }// end of try block
        catch (bad_hmean & bg)    // start of catch block
        {
            bg.mesg();
            cout << "Try again.\n";
            continue;
        }
        catch (bad_gmean & hg)
        {
            cout << hg.mesg();
            cout << "Values used: " << hg.v1 << ", "
                 << hg.v2 << endl;
            cout << "Sorry, you don't get to play any more.\n";
            break;
        } // end of catch block
    }
    cout << "Bye!\n";
    return 0;
}

double hmean(double a, double b) throw(bad_hmean)
{
    if (a == -b)
        throw bad_hmean(a,b);
    return 2.0 * a * b / (a + b);
}

double gmean(double a, double b) throw(bad_gmean)
{
    if (a < 0 || b < 0)
        throw bad_gmean(a,b);
    return std::sqrt(a * b);
}

```

### 1.4 catch(引用)

引发异常时编译器总是创建一个临时拷贝,即使是catch(引用).原因如下:

基类引用可以指向派生类,只需列出一个基类引用,将与任何派生类对象匹配.

```cpp
class bad_1{...};
class bad_2 : public bad_1{...};
class bad_3 : public bad_2{...};
...
void duper() throw(bad_1) //matches base and derived class object
{
...
    if(oh_no)
        throw bad_1();
    if(rats)
        throw bad_2();
    if(drat)
        throw bad_3();
}
...
try{
    duper();
}
catch(bad_3 &be){}
catch(bad_2 &be){}
catch(bad_1 &be){}  //捕获基类异常,如放在最前面,将不能捕获派生类异常
catch(...){}        //捕获任何异常
```

## 二、异常类(exception)

### 2.1 异常基类

　　在exception文件中定义了exception类,可以作为其它异常类的基类.有一个虚函数可覆盖

<center>`const char *what();`</center>

**Sample**

```cpp
#include<exception>
class bad_hmean : public std::exception{
public:
    const char *what(){
        return "bad arguments to hmean";
  }
  ...
};
class bad_gmean : public std::exception{
public:
    const char *what(){
        return "bad arguments to gmean";
  }
};

try{
...
}catch( exception &e){
    cout<<e.what()<<endl;
}
```

### 2.2 常用异常类

　　头文件stdexception定义,以公有方式从exception派生.主要包括`logic_error`和`runtime_error`

#### 2.2.1 logic_error类及其派生类

```cpp
//logic_error类,
class logic_error : public exception{
public:
    explicit logic_error(const string &what_arg);
};

//domain_error,logic_error的派生类
class domain_error : public logic_error{
public:
    explicit domain_error(const string &what_arg);
};
```

　　logic_error类的派生类,这些类的构造函数均接受一个string对象,提供了what()的返回数据:

* domain_error: 定义域异常

* invalid_argument: 参数异常,给函数传递了一个意外的值.如只接受字符0和1,当传2时引发异常

* length_error:长度异常,没有足够的空间.比string.append合并后超过最大允许长度

* out_of_bounds:索引错误.比如int A[10],使用了A[11].

```cpp
try{
...
}catch(out_of_bounds &oe){
...
}
catch(logic_error &oe){
...
}
catch(exception &oe)
{
...
}
```

#### 2.2.2 runtime_error类及其派生类

在运行期间出现错误.

* range_error:没有出现溢出错误,但是结果不在函数允许的范围内.

* overflow_error:超过类型可以表示的最大值,比如int和float越界

* underflow:超过类型可以表示的最小值,比如超过浮点数可以表示的最小值.

#### 2.2.3 bad_alloc和new

对于new产生的内存分配问题,一是返回空指针.二是抛出bad_allocy异常

```cpp
// newexcp.cpp -- the bad_alloc exception
#include <iostream>
#include <new>
#include <cstdlib>
using namespace std;

struct Big
{
    double stuff[20000];
};

int main()
{
    Big * pb;
    try {
        cout << "Trying to get a big block of memory:\n";
        pb = new Big[10000]; // 1,600,000,000 bytes
        cout << "Got past the new request:\n";
    }
    catch (bad_alloc & ba)
    {
        cout << "Caught the exception!\n";
        cout << ba.what() << endl;
        exit(EXIT_FAILURE);
    }
    if (pb != 0)
    {
        pb[0].stuff[0] = 4;
        cout << pb[0].stuff[0] << endl;
    }
    else
        cout << "pb is null pointer\n";
    delete [] pb;
    return 0;
}

```

## 三、异常注意事项

　　使用异常会增加程序代码,降低程序的运行速度.异常规范不适用于模板,因为模板引发的异常可能随特定的具体化而异.

在异常和动态内存分配时要特别注意:

```cpp
void test(int n){
    double *ar = new double[n];
    ...
    if( oh_no )
        throw exception();
    ...
    delete[] ar;  //delete将被忽略,堆栈解退时ar指针被删除,其指向的内存无法访问,造成内存泄露
    return;
}
void test2(int n){
    double *ar = new double[n];
    ...
    try{
        if( oh_no )
            throw exception();
    }catch(exception &e){
        delete[] ar;
        throw;
    }
    ...
    delete[] ar;
    return;
}
```



