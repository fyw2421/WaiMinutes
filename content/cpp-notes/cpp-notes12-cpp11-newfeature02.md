---
title: "C++Notes12 : C++11/14 New Feature02"
weight: 12
description: "Lambda rvalue-reference std::move() wrapper(functional) std::bind move-constructor"
date: 2018-05-22
tags: ["C++"]
featureimage: "covers/cpp-notes12-cpp11-newfeature02.svg"
---
## 一、Lambda表达式

　　C++11的一大亮点就是引入了Lambda表达式.利用Lambda表达式,可以方便的定义和创建匿名函数.

### 1.1 Lambda表达式的声明

```cpp
//完整声明
[capture list] (params list) specifiers exception-> return type { function body }

//省略捕获列表中的值是否可修改标志,默认是const
[capture list] (params list) -> return type {function body}1
//省略返回值
[capture list] (params list) {function body}

//省略参数列表,仅若不使用 constexpr 、 mutable 、异常规定、属性或尾随返回类型之一才能使用此形式.
[capture list] {function body}
```

### 1.2 capture list(捕获外部变量列表)

捕获列表能按如下方式传递:

capture|detail
:---:|:---:
`[]`                  | 不捕获任何外部变量
`[var1,var2,....]`    | 值传递方式捕捉变量var
`[&var1,&var1,....]`  | 引用传递方式捕捉变量var
`[=]`                 | 值传递方式捕捉所有父作用域的变量
`[&]`                 | 引用传递方式捕捉所有父作用域的变量
`[this]`              | 值传递方式捕捉当前的this指针
`[=, &x]`             | 变量x以引用形式捕获,其余变量以传值形式捕获
`[&, x]`              | 变量x以值的形式捕获,其余变量以引用形式捕获

```cpp
#include<iostream>
using namespace std;

int main()
{
    int j = 10;
    auto by_val_lambda = [=]{ return j + 1; };
    auto by_ref_lambda = [&]{ return j + 1; };
    cout<<"by_val_lambda: "<<by_val_lambda()<<endl;
    cout<<"by_ref_lambda: "<<by_ref_lambda()<<endl;

    ++j;
    //j被视为一个常量,一旦初始化后不会再改变(可以认为之后只是一个跟父作用域中j同名的常量),一次捕获
    cout<<"by_val_lambda: "<<by_val_lambda()<<endl;
    cout<<"by_ref_lambda: "<<by_ref_lambda()<<endl;

    return 0;
}
/*
by_val_lambda: 11
by_ref_lambda: 11
by_val_lambda: 11
by_ref_lambda: 12
*/
```

### 1.3 params list(形参列表)

Lambda表达式的参数和普通函数的参数类似,还有一些限制,主要有以下几点:

1. 参数列表不能有默认参数

2. 不支持可变参数

3. 所有参数必须有参数名

```cpp
{
    int m = [](int x) {
        return [](int y) {
            return y * 2;
        }
        (x)+6;
    }(5);
    std::cout << "m:" << m << std::endl;            //输出m:16

    std::cout << "n:" << [](int x, int y) { return x + y; }(5, 4) << std::endl;            //输出n:9

    //function相当于函数指针
    //<https://blog.csdn.net/hanbingfengying/article/details/28651507>
    auto gFunc = [](int x) -> function<int(int)> { return [=](int y) { return x + y; }; };
    auto lFunc = gFunc(4);
    std::cout << lFunc(5) << std::endl;  //9

    auto hFunc = [](const function<int(int)>& f, int z) { return f(z) + 1; };
    auto ax = hFunc(gFunc(7), 8); //16

    int a = 111, b = 222;
    auto func = [=, &b]()mutable { a = 22; b = 333; std::cout << "a:" << a << " b:" << b << std::endl; };

    func(); //22 333
    std::cout << "a:" << a << " b:" << b << std::endl; // 111 333

    a = 333;
    auto func2 = [=, &a] { a = 444; std::cout << "a:" << a << " b:" << b << std::endl; };
    func2(); //444,333

    auto func3 = [](int x) ->function<int(int)> { return [=](int y) { return x + y; }; };


    std::function<void(int x)> f_display_42 = [](int x) { print_num(x); };
    f_display_42(44);
}
```

### 1.4 specifiers(可选的指定符序列)

   * mutable: 允许 body 修改以复制捕获的参数,及调用其非 const 成员函数.在使用该修饰符时,参数列表不可省略(即使参数为空)

   * const :默认值,不可以修改任何值

```cpp
int a = 0;
auto f1 = [=] { return a++; };                  //error
auto f2 = [=] () mutable { return a++; };       //OK

int a = 123;
auto f = [a]()mutable { cout << ++a; }; // 不会报错
cout << a << endl; // 输出:123
f(); // 输出:124
```
**Sample**

```cpp
 std::sort(x,x + N,[](float a, float b) { return std::abs(a) <std::abs(b); });
 std::cout << [](float f) -> int { return std::abs(f); } (-3.5);
```

> https://www.cnblogs.com/DswCnblog/p/5629165.html

> http://www.jb51.net/article/56147.htm

> http://zh.cppreference.com/w/cpp/language/lambda

### 1.5 函数指针/函数符/Lambda函数比较

```cpp
//=====================================
// 函数指针/函数符/lambda
//=====================================
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <ctime>

const long Size1 = 39L;
const long Size2 = 100*Size1;
const long Size3 = 100*Size2;

bool f3(int x){ return x%3 == 0 ;}
bool f13(int x){ return x%13 == 0;}

int main(){
    using namespace std;
    vector<int> numbers(Size1);

    srand(time(0));
    generate( numbers.begin(),numbers.end(),rand );

    auto div3Cnt = 0;
    auto div13Cnt = 0;

    //function pointers
    div3Cnt = count_if(numbers.begin(),numbers.end(),f3);
    div13Cnt = count_if(numbers.begin(),numbers.end(),f13);
    cout<<"FunctionPointer:"<<endl;
    cout<<"Vector Size: "<<numbers.size()<<endl;
    cout<<"Div3 Count: "<<div3Cnt<<endl;
    cout<<"Div13 Count: "<<div13Cnt<<endl<<endl;

    //resize vector
    numbers.resize(Size2);
    generate(numbers.begin(),numbers.end(),rand);

    //functor
    class f_mod{
    private:
        int dv;
    public:
        f_mod(int d = 1):dv{d}{}
        bool operator()(int x ){ return x%dv == 0 ;}
    };
    div3Cnt = count_if(numbers.begin(),numbers.end(),f_mod(3));
    div13Cnt = count_if(numbers.begin(),numbers.end(),f_mod(13));
    cout<<"Functor:"<<endl;
    cout<<"Vector Size: "<<numbers.size()<<endl;
    cout<<"Div3 Count: "<<div3Cnt<<endl;
    cout<<"Div13 Count: "<<div13Cnt<<endl<<endl;

    //resize vector
    numbers.resize(Size3);
    generate(numbers.begin(),numbers.end(),rand);
    div3Cnt = count_if(numbers.begin(),numbers.end(),[](int x){ return x%3 == 0;});
    div13Cnt = count_if(numbers.begin(),numbers.end(),[](int x){ return x%13 == 0;});
    cout<<"Lambda:"<<endl;
    cout<<"Vector Size: "<<numbers.size()<<endl;
    cout<<"Div3 Count: "<<div3Cnt<<endl;
    cout<<"Div13 Count: "<<div13Cnt<<endl;

    return 1;
}
/*output
FunctionPointer:
Vector Size: 39
Div3 Count: 11
Div13 Count: 1

Functor:
Vector Size: 3900
Div3 Count: 1263
Div13 Count: 297

Lambda:
Vector Size: 390000
Div3 Count: 129822
Div13 Count: 30030
*/
```

### 1.6 lambda优势

代码简洁,在定义处使用

```cpp
//=====================================
// 使用lambda计算能够被3和13整除的个数
//=====================================
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <ctime>

const long SIZE = 390000L;

int main(){
    using namespace std;
    vector<int> numbers(SIZE);

    srand(std::time(0));

    generate( numbers.begin(),numbers.end(),rand );
    //generate( numbers.begin() , numbers.end(),[](){ return rand(); });

    //using lambdas : divisible by three
    auto div3Cnt = count_if( numbers.begin() , numbers.end() , [](int x){ return x%3 == 0; });
    cout<<"Count of numbers divisible by 3 : "<<div3Cnt<<endl;

    //using lambdas
    auto div13Cnt = 0 ;
    for_each( numbers.begin() , numbers.end() , [&div13Cnt]( int x ){ div13Cnt += x%13 == 0 ; });
    cout<<"Count of numbers divisible by 13 : "<<div13Cnt<<endl;

    //using single lambda
    div3Cnt = div13Cnt = 0;
    for_each( numbers.begin(), numbers.end() , [&](int x){ div3Cnt += x%3 == 0 ; div13Cnt += x%13== 0;});
    cout<<"Count of numbers divisible by 3 : "<<div3Cnt<<endl;
    cout<<"Count of numbers divisible by 13 : "<<div13Cnt<<endl;
    return 1;
}
//=====================================

/**output:
Count of numbers divisible by 3 : 130189
Count of numbers divisible by 13 : 30132
Count of numbers divisible by 3 : 130189
Count of numbers divisible by 13 : 30132
**/
```

### 1.7 lambda多次调用

```cpp
//给lambda指定名称
auto mod3 = [](int x){ return x%3 == 0; };

//多次调用
div3Cnt = count_if( numbers.begin(),numbers.end(),mod3);
count3 = count_if( n2.begin(), n2.end() ,mod3);

//像常规函数一样调用,mod3的实际类型取决于编译器.
bool result = mod3( 10 );
```
## 二、右值引用

### 2.1 左值和右值

1. 左值

   能对表达式取地址、或具名对象/变量.一般指表达式结束后依然存在的持久对象.代表的是对象本身.

2. 右值

   不能对表达式取地址,或匿名对象.一般指表达式结束就不再存在的临时对象.代表的是对象的值.

```cpp
#include <iostream>
#include <type_traits>

//编译选项:g++ -std=c++11 test1.cpp -fno-elide-constructors
using namespace std;

//左/右值以及左/右值引用

struct Test
{
    int m;

public:
    Test(){cout << "Test()" << endl;}
    Test(const Test& t){cout << "Test(const Test&)" << endl;}
    Test(Test&& t){cout << "Test(Test&&)" << endl;}

    ~Test(){cout << "~Test()" << endl;}
};

Test&& func()
{
    return Test(); //不安全!返回局部对象的引用(用于演示)!
}

Test ReturnRvalue()
{
    return Test();
}

int main()
{
    //1. 左、右值判断
    int i = 0;
    int&& ri = i++; //i++为右值,表达式返回是i的拷贝,是个匿名变量.故为右值.
    int&  li = ++i; //++i返回的是i本身,是具名变量,故为左值.

    int* p = &i;
    int& lp = *p;    //*p是左值,因为可以取*p的地址,即&(*p);
    int* && rp = &i; //取地址表达式结果是个地址值,故&i是纯右值.

    int&& xi1 = std::move(i); //std::move(i)是个xvalue
    int&& xi2 = static_cast<int&&>(i); // static_cast<int&&>(i)是个xvalue

    auto&& fn = [](int x){ return x * x; }; //lambda表达式是右值,可以用来初始化右值引用
    cout << std::is_rvalue_reference<decltype(fn)>::value << endl; //1

    Test t;
    int& rm1  = t.m; //由于t是左值,而m为普通成员变量,所以m也为左值.
    int&& rm2 = Test().m; //由于Test()是个右值,所以m也是右值

    int Test::*pm = &Test::m; //定义指向成员变量的指针,指向non-static member data;
    //int&& rm3 = t.*pm; //error,由于t是左值,*pm也是左值,不能用来初始化右值引用.
    int& rm3 = t.*pm;    //ok
    int&& rm4 = Test().*pm; //ok,Test()是临时变量,为右值.所以*pm也是右值

    //2. 左/右值引用的初始化
    int a;
    int&  b = a;  //ok
    //int&& b = a;  //error,右值引用只能绑定到右值上

    Test&& t1 = ReturnRvalue();  //返回值是个临时对象(右值) 被绑定到t1上,使其“重获新生”,
                                 //生命期与t1一样.
    Test   t2 = ReturnRvalue();  //返回值是个临时对象(右值),用于构造t2,之后该临时对象
                                 //就会马上被释放.
    //Test&  t3 = ReturnRvalue();    //普通左值引用不能绑定到右值
    const Test& t4 = ReturnRvalue(); //常左值引用是个“万能引用”,可以绑定到右值


    //system("pause");
    return 0;
}
```

### 2.2 左值引用和右值引用

　　右值引用和左值引用都是属于引用类型.无论是声明一个左值引用还是右值引用,都必须立即进行初始化.

引用类型 | 非常量左值 | 常量左值 | 非常量右值 | 常量右值 | 备注
:---:|:---:|:---:|:---:|:---:|:---:
`Type&`        | Y | N | N | N | 只能绑定非常量左值
`const Type&`  | Y | Y | Y | Y | 万能类型,用于复制语义
`Type&&`       | N | N | Y | N | 只能绑定右值,用于移动语义和完美转发
`const Type &&`| N | N | Y | Y | 暂无用途

1. 左值引用

   左值引用是具名变量/对象的别名,右值引用是匿名变量/对象的别名.不能绑定到右值.

   ```cpp
   int &a = 2;       # 左值引用绑定到右值,编译失败
   int b = 2;        # 非常量左值
   const int &c = b; # 常量左值引用绑定到非常量左值,编译通过
   const int d = 2;  # 常量左值
   const int &e = c; # 常量左值引用绑定到常量左值,编译通过
   const int &b =2;  # 常量左值引用绑定到右值,编程通过
   ```

2. 右值引用

   右值值引用通常不能绑定到任何的左值,要想绑定一个左值到右值引用,通常需要std::move()将左值强制转换为右值,例如:

   ```cpp
   int a;
   int &&r1 = c;             # 编译失败
   int &&r2 = std::move(a);  # 编译通过
   ```

## 三、std::move()

　　std::move是将对象的状态或者所有权从一个对象转移到另一个对象,只是转移,没有内存的搬迁或者内存拷贝,它唯一的功能是将一个左值强制转化为右值引用,继而可以通过右值引用使用该值,以用于移动语义.从实现上讲,std::move基本等同于一个类型转换:static_cast<T&&>(lvalue);

### 3.1 移动语义

```cpp
#include <iostream>
#include <utility>
#include <vector>
#include <string>
int main()
{
    std::string str = "Hello";
    std::vector<std::string> v;

    // uses the push_back(const T&) overload, which means
    // we'll incur the cost of copying str
    v.push_back(str);
    std::cout << "After copy, str is \"" << str << "\"\n";

    // uses the rvalue reference push_back(T&&) overload,
    // which means no strings will copied; instead, the contents
    // of str will be moved into the vector.  This is less
    // expensive, but also means str might now be empty.
    v.push_back(std::move(str));
    std::cout << "After move, str is \"" << str << "\"\n";

    std::cout << "The contents of the vector are \"" << v[0]
                                         << "\", \"" << v[1] << "\"\n";
}
/*
After copy, str is "Hello"
After move, str is ""
The contents of the vector are "Hello", "Hello"
*/
```

　　`v.push_back(str);`有对象复制,调用复制构造函数,深拷贝,`v.push_back(std::move(str))`没有内存操作和深拷贝,效率更高,str被清空.

### 3.2 移动构造函数


```cpp
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>

using namespace std;

class Str{
public:
    char *value;
    Str(char s[]){
        cout<<"call constructor..."<<endl;
        int len = strlen(s);
        value = new char[len + 1];
        memset(value,0,len + 1);
        strcpy(value,s);
    }
    Str(const Str &v){
        cout<<"call copy constructor..."<<endl;
        int len = strlen(v.value);
        value = new char[len + 1];
        memset(value,0,len + 1);
        strcpy(value,v.value);
    }
    Str(Str &&s){
        cout<<"call move constructor..."<<endl;
        value = nullptr;
        value = s.value;
        s.value = nullptr;
    }

    ~Str(){
        cout<<"call destructor..."<<endl;
        if(value != nullptr){
            delete[] value;
            value = nullptr;
        }
    }
};

int main()
{
    char value[] = "I love zx";
    Str s(value);
    vector<Str> vs;
    //vs.push_back(move(s)); //call move constructor...
    vs.push_back(s); //call copy constructor...
    cout<<vs[0].value<<endl;
    if(s.value != nullptr)
    cout<<s.value<<endl;

    //右值自动调用移动构造函数,应先重载`+`操作符
    Str ss( s + s );
    return 0;
}
/*vs.push_back(s)
call constructor...
call copy constructor...
I love zx
I love zx
call destructor...
call destructor...

vs.push_back(move(s));
call constructor...
call move constructor...
I love zx
call destructor...
call destructor...
*/
```

### 3.3 移动赋值运算符

```cpp
//assignment operator
Str &Str::oprator=(const Str &s){
    if( this == &s )
        return *this;
    delete[] value;
    int len = str(s.value);
    value = new char[ len + 1 ];
    strcpy( value , s.value );
}

//move assignment operator
Str &Str::oprator=(const Str &&s){
    if( this == &s )
        return *this;
    delete[] value;
    value = s.value;
    s.value = nullptr;
    return *this;
}
```

### 3.4 移动构造函数与移动赋值运算符的使用与生成

* 如果参数与右值,优先使用移动构造和移动赋值运算符,否则使用复制构造和赋值运算符

* 如果定义了析构函数/复制构造函数或者赋值操作符,编译器不会自动生成移动构造函数和移动赋值运算符.

* 如果定义了移动构造函数,编译器不会自动生成构造函数/复制构造函数/赋值操作符.

## 四、包装器(wraper) function

　　`function<>`包装器可以将普通函数,lambad函数,函数对象(函数符)统一分装起来,虽然它们不是相同的类型,但是经过了function模板后,它们可以转化为相同的function的对象

　　可以实现类似函数指针的功能,但却比函数指针更加灵活安全

### 4.1 模板函数做参数被多次实例化

```cpp
//=====================================
// 模板函数回调造成多次实例化
//=====================================
#include <iostream>
using namespace std;
template <typename T,typename F>
T use_f(T v,F f){
    static int count = 0;
    count++;
    cout<<"  use_f count = "<<count<<",&count = "<<&count<<endl;
    return f(v);
}
class Fp{
private:
    double z;
public:
    Fp(double zz = 1.0 ): z{zz}{}
    double operator()(double p){return z*p;}
};
class Fq{
private:
    double z;
public:
    Fq(double zz = 1.0 ): z{zz}{}
    double operator()(double q){return z*q;}
};

double dub(double x){ return 2.0*x;}
double square(double x){return x*x;}


int main()
{
    double y = 1.21;
    cout<<"Function pointer dub: "<<endl;
    cout<<"  "<<use_f(y,dub)<<endl;

    cout<<"Function pointer square: "<<endl;
    cout<<"  "<<use_f(y,square)<<endl;

    cout<<"Function object Fp: "<<endl;
    cout<<"  "<<use_f(y,Fp(5.0))<<endl;

    cout<<"Function object Fq: "<<endl;
    cout<<"  "<<use_f(y,Fq(5.0))<<endl;

    cout<<"lambda expression 1: "<<endl;
    cout<<"  "<<use_f(y,[](double u){return u*u;})<<endl;

    cout<<"lambda expression 2: "<<endl;
    cout<<"  "<<use_f(y,[](double u){return u+u/2.0;})<<endl;
    return 1;
}
/*output
Function pointer dub:
  use_f count = 1,&count = 0x4b9318
  2.42
Function pointer square:
  use_f count = 2,&count = 0x4b9318
  1.4641
Function object Fp:
  use_f count = 1,&count = 0x4b9310
  6.05
Function object Fq:
  use_f count = 1,&count = 0x4b9314
  6.05
lambda expression 1:
  use_f count = 1,&count = 0x4c700c
  1.4641
lambda expression 2:
  use_f count = 1,&count = 0x4c7010
  1.815
*/
```

　　`count`有五个不同的地址,表明有五个不同的实例化.

* `dub()`和`square`类型均为double(*)(double)类型,生成同一实例化

* `Fp`和`Fq`不同,实例化两次

* 两个lambda表达式实例化两次

### 4.2 使用function包装器减少实例化次数

　　模板function是在头文件functional中声明的,它从特征标的角度定义了一个对象,可用于包装调用特征标相同的`普通函数,类成员函数,函数对象(仿函数)或lambda表达式`.

　　std::function可以绑定到全局函数/类静态成员函数(类静态成员函数与全局函数没有区别),如果要绑定到类的非静态成员函,则需要使用std::bind

　　模板function是一种类型安全的包装.函数指针(回调),是类型不安全的

```cpp
//创建一个function对象fdci
//参数为char和int
//返回值为double
std::function<double(char,int)> fdci;
```

1. 创建`function<double(double)>`对象

   用于表示函数,函数符和lambda,在对`use_f`的调用中,F的类型相同,因此只实例化一次

   ```cpp
   //创建`function<double(double)>`对象
   int main()
   {
       double y = 1.21;

       //wraper function
       function<double(double)> ef1 = dub;
       function<double(double)> ef2 = square;
       function<double(double)> ef3 = Fq(10.0);
       function<double(double)> ef4 = Fp(10.0);
       function<double(double)> ef5 = [](double u){return u*u;};
       function<double(double)> ef6 = [](double u){return u+u/2.0;};
       cout<<"Function pointer dub: "<<endl;
       cout<<"  "<<use_f(y,ef1)<<endl;

       cout<<"Function pointer square: "<<endl;
       cout<<"  "<<use_f(y,ef2)<<endl;

       cout<<"Function object Fp: "<<endl;
       cout<<"  "<<use_f(y,ef3)<<endl;

       cout<<"Function object Fq: "<<endl;
       cout<<"  "<<use_f(y,ef4)<<endl;

       cout<<"lambda expression 1: "<<endl;
       cout<<"  "<<use_f(y,ef5)<<endl;

       cout<<"lambda expression 2: "<<endl;
       cout<<"  "<<use_f(y,ef6)<<endl;
       return 1;
   }
   ```

2. 使用临时`function<double(double)>`对象

   ```cpp
   //使用临时`function<double(double)>`对象
   //typedef function<double(double)> fdd;
   using fdd = function<double(double)>;
   cout<<"  "<<use_f(y,fdd(dub))<<endl;
   cout<<"  "<<use_f(y,fdd(square))<<endl;
   cout<<"  "<<use_f(y,fdd(Fq(10.0)))<<endl;
   cout<<"  "<<use_f(y,fdd(Fp(10.0)))<<endl;
   cout<<"  "<<use_f(y,fdd([](double u){ return u*u;}))<<endl;
   cout<<"  "<<use_f(y,fdd([](double u){return u+u/2.0;}))<<endl;
   ```

3. 使用`function<double(double)>`参数

   ```cpp
   //使用`function<double(double)>`参数
   template <typename T>
   T use_f(T v,function<T(T)> f){
       static int count = 0;
       count++;
       cout<<"  use_f count = "<<count<<",&count = "<<&count<<endl;
       return f(v);
   }
   int main(){
       cout<<"  "<<use_f<double>(y,dub)<<endl;
       cout<<"  "<<use_f<double>(y,square)<<endl;
       cout<<"  "<<use_f<double>(y,Fq(10.0))<<endl;
       cout<<"  "<<use_f<double>(y,Fp(10.0))<<endl;
       cout<<"  "<<use_f<double>(y,[](double u){ return u*u;})<<endl;
       cout<<"  "<<use_f<double>(y,[](double u){return u+u/2.0;})<<endl;
    return 1;
   }
   ```

### 4.3 function使用示例

```cpp
//=====================================
// function模板用于以下类型
//1. function
//2. lambda
//3. functor
//4. class member function
//5. static class member function
//=====================================
#include <functional>
#include <iostream>
using namespace std;

std::function< int(int)> Functional;

//Function
int TestFunc(int a){ return a; }

//Lambda
auto lambda = [](int a)->int{ return a; };

//Functor
class Functor{
public:
    int operator()(int a){
        return a;
    }
};

//1.Class Member Function
//2.Static Class Member Function
class TestClass{
public:
    int ClassMember(int a) { return a; }
    static int StaticMember(int a) { return a; }
};

int main(){
    //Function
    Functional = TestFunc;
    int result = Functional(10);
    cout <<"Function: "<< result << endl;

    //Lambda
    Functional = lambda;
    result = Functional(20);
    cout <<"Lambda: "<< result << endl;

    //Functor
    Functor testFunctor;
    Functional = testFunctor;
    result = Functional(30);
    cout <<"Functor: "<< result << endl;

    //Class Member Function
    TestClass testObj;
    Functional = std::bind(&TestClass::ClassMember, testObj, std::placeholders::_1);
    result = Functional(40);
    cout <<"Class Member Function: "<< result << endl;

    // Static Class Member Function
    Functional = TestClass::StaticMember;
    result = Functional(50);
    cout <<"Static Class Member Function: "<< result << endl;

    return 0;
}
/*output
Function: 10
Lambda: 20
Functor: 30
Class Member Function: 40
Static Class Member Function: 50
*/
```

```cpp
#include <iostream>
#include <map>
#include <functional>
using namespace std;

// 普通函数
int add(int i, int j) { return i + j; }
// lambda表达式
auto mod = [](int i, int j){return i % j; };
// 函数对象类
struct divide
{
    int operator() (int denominator, int divisor)
    {
        return denominator / divisor;
    }
};

int main(int argc, char *argv[])
{
    // 受限的map
    map<char, int(*)(int, int)> binops_limit;
    binops_limit.insert({ '+', add });
    binops_limit.insert({ '%', mod });
    // 错误	1	error C2664: “void std::_Tree<std::_Tmap_traits<_Kty,_Ty,_Pr,_Alloc,false>>::insert(std::initializer_list<std::pair<const _Kty,_Ty>>)”: 无法将参数 1 从“initializer-list”转换为“std::pair<const _Kty,_Ty> &&”
    // binops_limit.insert({ '%', divide() });

    // 更灵活的map
    map<char, function<int(int, int)>> binops =
    {
        { '+', add },
        { '-', minus<int>() },
        { '*', [](int i, int j){return i - j; } },
        { '/', divide() },
        { '%', mod },
    };
    cout << binops['+'](10, 5) << endl;
    cout << binops['-'](10, 5) << endl;
    cout << binops['*'](10, 5) << endl;
    cout << binops['/'](10, 5) << endl;
    cout << binops['%'](10, 5) << endl;
    system("pause");
    return 0;
}
```

## 五、std::bind

　　bind是一组用于函数绑定的模板,位于`functional`.在对某个函数进行绑定,可以指定部分参数或全部参,也可以不指定任何参,还可以调整各个参数间的顺序.对于未指定的参,可以使用占位符_1、_2、_3来表示._1表示绑定后的函数的第1个参,_2表示绑定后的函数的第2个参,其他依次类推.

* 将函数、成员函数和闭包转成function函数对象

* 将多元(n>1)函数转成一元函数或者(n-1)元函数.

　　bind可以绑定到普通函数、类的成员函数和类的成员变量

　　std::function可以绑定到全局函数/类静态成员函数(类静态成员函数与全局函数没有区别),如果要绑定到类的非静态成员函,则需要使用std::bind

```cpp
#include <iostream>
#include <functional>
#include <string>

using namespace std;

double my_divide(double x,double y){
    return x/y;
}
class Base{
public:
    Base(int x = 0) :data(x) {}
    void show(string name) { cout << name << endl; }
    static int getNum() { return 10; }
    int operator()(int i, int j, int k) { return i + j + k; }
private:
    int data;
};
void fun(int &x){ x++; }
int main()
{
    using namespace std::placeholders;                        // adds visibility of _1, _2, _3,...

    //1. 绑定普通函数
    auto fn_five = std::bind(my_divide, 10, 2);               // returns 10/2
    std::cout << fn_five() << '\n';                           // 5

    auto fn_half = std::bind(my_divide, _1, 2);               // returns x/2
    std::cout << fn_half(10) << '\n';                         // 5

    auto fn_invert = std::bind(my_divide, _2, _1);            // returns y/x
    std::cout << fn_invert(10, 2) << '\n';                    // 0.2

    auto fn_rounding = std::bind<int>(my_divide, _1, _2);     // returns int(x/y)
    std::cout << fn_rounding(10, 3) << '\n';

    Base one(5);

    //2.绑定类成员函数
    auto show1 = bind(&Base::show, one, _1);
    show1("123456");                                       //123456

    auto show2 = bind(&Base::show, one, "123456");
    show2();                                               //123456

    //3.绑定静态成员函数
    auto getnum = bind(&Base::getNum);
    cout << getnum() << endl;                              //10

    //4.绑定operator函数
    auto oper = bind(&Base::operator(), one, _1, _2, 0);
    cout << oper(1, 2) << endl;                            //3

    auto oper1 = bind(Base(), _1, _2, 0);
    cout << oper1(1, 2) << endl;                           //3

    //5.bind中的参数传递
    //5.1 默认值传递
    auto n = 0;
    bind(fun,n)();
    cout<<n<<endl; //0

    //5.2 引用传递
    bind(fun,ref(n))();
    cout<<n<<endl; //1
    return 0;
}

```



