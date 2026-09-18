---
title: "C++Notes11 : C++11/14 New Feature01"
weight: 11
description: "auto decltype nullptr enum-class Uniform-iniitialization manage-virtual-method"
date: 2018-05-22
tags: ["C++"]
featureimage: "covers/cpp-notes11-cpp11-newfeature01.svg"
---
## 一、标准化

### 1.1 C++ 98 标准

　　C\++标准第一版,1998年发布.绝大多数编译器都支持C\++98标准.不过当时错误地引入了export关键字.由于技术上的实现难度,除了Comeau C\++编译器export关键字以外,没有任何编译器支持export关键字.并且这个标准对现代的一些编译理念有相当的差距,有很多在高级语言都应当有的功能,它都没有.这也正是后来需要制定C++11标准的原因所在.

### 1.2 C++ 03 标准

　　C\++标准第二版,2003年发布.这个标准仅仅是C\++98修订版,与C\++98几乎一样,没做什么修改.仅仅是对C\++98做了一些“勘误”,就连主流编译器（受C99标准影响）都已支持的long long都没有被加入C++03标准.

### 1.3 C++ 11 标准

　　C\++标准第三版,2011年8月12日发布.C\++11包含了核心语言的新机能,并且拓展C\++标准程序库,并且加入了大部分的C\++ Technical Report 1程序库(数学上的特殊函数除外).此次标准为C\++98发布后13年来第一次重大修正.

### 1.4 C++ 14 标准

　　C\++标准第四版,2014年8月18日发布.主要的编译器开发商已经实现了 C\++ 14 规格.C\++ 14 是 C\++ 11 的增量更新,主要是支持普通函数的返回类型推演、泛型、lambda、扩展的 lambda 捕获、对 constexpr 函数限制的修订,constexpr变量模板化等等.

### 1.5 C++17 标准

　　C\++标准第五版,2017年12月5日发布.基于 C\++ 11,C\++ 17 旨在使 C\++ 成为一个不那么臃肿复杂的编程语言,以简化该语言的日常使用,使开发者可以更简单地编写和维护代码.C\++ 17 是对 C\++ 语言的重大更新,引入了许多新的语言特性:

* UTF-8 字符文字；

* 折叠表达式 (fold expressions):用于可变的模板；

* 内联变量 (inline variables):允许在头文件中定义变量；

* 在 if 和 switch 语句内可以初始化变量；

* 结构化绑定 (Structured Binding):for (auto [key,value] : my_map) {…}；

* 类模板参数规约 (Class Template Argument Deduction):用 pair p{1, 2.0}; 替代 pair<int, double>{1, 2.0};；

## 二、auto(类型推断)

### 2.1 自动类型推导

　　auto的自动类型推导,用于从初始化表达式中推断出变量的数据类型.通过auto的自动类型推导,可以大大简化我们的编程工作.

　　auto实际上是在编译时对变量进行了类型推导,所以不会对程序的运行效率造成不良影响.

　　另外,似乎auto并不会影响编译速度,因为编译时本来也要右侧推导然后判断与左侧是否匹配.

```cpp
auto a; // 错误,auto是通过初始化表达式进行类型推导,如果没有初始化表达式,就无法确定a的类型
auto i = 1;
auto d = 1.0;
auto str = "Hello World";
auto ch = 'A';
auto func = less<int>();
vector<int> iv;
auto ite = iv.begin();
auto p = new foo() // 对自定义类型进行类型推导

int a = 10;
auto au_a = a;//自动类型推断,au_a为int类型
cout << typeid(au_a).name() << endl; //int
```

### 2.2 模板应用

```cpp
//如果不用auto,必须声明Product模板参数
template <typename Product,typename Creator>
void processProduct(const creator &creator){
    Product *val = creator.makeObject();
    ...
}

//使用auto
template <typename Creator>
void processProduct(const creator &creator){
    auto val = creator.makeObject();
    ...
}
```

### 2.3 注意事项

使用auto关键字做类型自动推导时,遵循以下规则:

1. 如果初始化表达式是引用,则去除引用语义.

   ```cpp
   int a = 10;
   int &b = a;

   auto c = b;//c的类型为int而非int&（去除引用）
   auto &d = b;//此时c的类型才为int&

   c = 100;//a =10;
   d = 100;//a =100;
   ```

2. 如果初始化表达式为const或volatile（或者两者兼有）,则除去const/volatile语义.

   ```cpp
   const int a1 = 10;
   auto  b1= a1; //b1的类型为int而非const int（去除const）
   const auto c1 = a1;//此时c1的类型为const int
   b1 = 100;//合法
   c1 = 100;//非法
   ```

3. 如果auto关键字带上&号,则不去除const语意.

   ```cpp
   const int a2 = 10;
   auto &b2 = a2;//因为auto带上&,故不去除const,b2类型为const int
   b2 = 10; //非法
   ```

4. 初始化表达式为数组时,auto关键字推导类型为指针.

   ```cpp
   int a3[3] = { 1, 2, 3 };
   auto b3 = a3;
   cout << typeid(b3).name() << endl; //int *
   ```

5. 若表达式为数组且auto带上&,则推导类型为数组类型.

   ```cpp
   int a7[3] = { 1, 2, 3 };
   auto & b7 = a7;
   cout << typeid(b7).name() << endl; //int [ 3 ]
   ```

6. 函数或者模板参数不能被声明为auto(c\++11),c\++14可以

   ```cpp
   void func(auto a){} //error

   //c\++14
   auto closure = [](auto x, auto y) { return x * y;}

   ////c\++11
   template<class T, class U>
   auto mul(T x, U y) -> decltype(x * y) {
      return x*y;
   }

   ////c\++14
   template<class T, class U>
   auto mul(T x, U y){
      return x*y;
   }
   ```

7. auto不是真正的类型,仅仅是占位符,不能使用sizeof或者typeid的操作符

   ```cpp
   cout << sizeof(auto) << endl;//错误
   cout << typeid(auto).name() << endl;//错误
   ```

8. auto 变量必须在定义时初始化,这类似于const关键字.

9. 定义在一个auto序列的变量必须始终推导成同一类型.例如:

  ```cpp
  auto a4 = 10, a5 = 20, a6 = 30;//正确
  auto b4 = 10, b5 = 20.0, b6 = 'a';//错误,没有推导为同一类型
  ```

## 三、decltype(类型获取)

　　decltype在C++11标准制定时引入,主要是为泛型编程而设计,以解决泛型编程中,由于有些类型由模板参数决定,而难以（甚至不可能）表示之的问题.

　　decltype实际上有点像auto的反函数,auto可以让你声明一个变量,而decltype则可以从一个变量或表达式中得到类型,有实例如下:

```cpp
int x = 3;
delctype(x) y = x;

//如果上文中的加工产品的例子中我们想把产品作为返回值该怎么办呢？我们可以这样写:
template <typename Creator>
//用了C++11标准中的“返回类型后置”（trailing-return-type）语法
auto processProduct(const Creator& creator) -> decltype(creator.makeObject()) {
    auto val = creator.makeObject();
    // do somthing with val
}
```

　　类似于sizeof操作符,decltype也不需对其操作数求值.粗略来说,decltype(e)返回类型前,进行了如下推导:

1. 若表达式e指向一个局部变量、命名空间作用域变量、静态成员变量或函数参数,那么返回类型即为该变量（或参数）的“声明类型”；

2. 若e是一个左值（lvalue,即“可寻址值”）,则decltype(e)将返回T&,其中T为e的类型；

3. 若e是一个x值（xvalue）,则返回值为T&&；

4. 若e是一个纯右值（prvalue）,则返回值为T.

　　这些语义是为满足通用库编写者的需求而设计,但由于decltype的返回类型总与对象（或函数）的定义类型相匹配,这对编程新手来说也更为直观.更正式地说,规则1适用于不带括号的标识符表达式（id-expression）与类成员访问表达式.示例如下:

```cpp
const int&& foo();
const int bar();
int i;
struct A { double x; };
const A* a = new A();
decltype(foo()) x1; // 类型为const int&&
decltype(bar()) x2; // 类型为int
decltype(i) x3; // 类型为int
decltype(a->x) x4; // 类型为double
decltype((a->x)) x5; // 类型为const double&
//最后两个对decltype的调用,返回结果有所不同.
/*
这是因为,带括号的表达式(a->x)既非“标识符表达式”,亦非类访问表达式,因而未指向一个命名对象,而是一个左值,于是推导类型便为“指向表达式类型的引用”,亦即const double&.
*/
```

**Sample**

```cpp
#include <algorithm>
#include <iostream>
#include <iterator>
#include <ostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

template <typename T, typename U>
struct Plus {
    //C++11
    //auto operator()(T&& t,U&& u) const-> decltype( forward<T>(t) + forward<U>(u) )
    //C++14
    auto operator()(T&& t,U&& u) const {
        return forward<T>(t) + forward<U>(u);
    }
};

int main() {
    vector<int> i;
    i.push_back(1);
    i.push_back(2);
    i.push_back(3);

    vector<int> j;
    j.push_back(40);
    j.push_back(50);
    j.push_back(60);

    vector<int> k;
    vector<string> s;

    s.push_back("cut");
    s.push_back("flu");
    s.push_back("kit");

    vector<string> t;
    t.push_back("e");
    t.push_back("ffy");
    t.push_back("tens");

    vector<string> u;
    transform(i.begin(), i.end(), j.begin(), back_inserter(k), Plus());
    transform(s.begin(), s.end(), t.begin(), back_inserter(u), Plus());
    for_each(k.begin(), k.end(), [](int n) { cout << n << " "; });
    cout << endl;
    for_each(u.begin(), u.end(), [](const string& r) { cout << r << " "; });
    cout << endl;
}
```

## 四、nullptr

　　nullptr是为了解决原来C++中NULL的二义性问题而引进的一种新的类型,因为NULL实际上代表的是0.

```cpp
void F(int a){
    cout<<a<<endl;
}

void F(int *p){
    assert(p != NULL);

    cout<< p <<endl;
}

int main(){

    int *p = nullptr;
    int *q = NULL;
    bool equal = ( p == q ); // equal的值为true,说明p和q都是空指针
    int a = nullptr; // 编译失败,nullptr不能转型为int
    F(0); // 在C++98中编译失败,有二义性；在C++11中调用F（int）
    F(nullptr);

    return 0;
}
```

## 五、区间迭代（range-based for loop)

　　for循环可以使用类似java的简化的for循环,可以用于遍历数组,容器,string以及由begin和end函数定义的序列(即有Iterator)

```cpp
#include <iostream>
#include <map>
#include <string>
using namespace std;

int main()
{
	map<string, int> ms;
	ms.insert(make_pair("a", 1));
	ms.insert(make_pair("b", 2));
	ms.insert(make_pair("c", 3));
	ms.insert(make_pair("d", 4));

	for (auto itr: ms)
		cout << itr.first << ":" << itr.second << endl;

	int a[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	for (auto itr: a)
		cout << itr << endl;

	char str[10] = "Hello";
	for (auto itr : str)
		cout << itr;
	cout << endl;

	string _str = "Hello";
	for (auto itr : _str)
		cout << itr;
	cout << endl;

    std::vector<int> arr;
    arr.push_back(1);
    arr.push_back(2);

    for (auto n : arr){//只读方式
        std::cout << n << std::endl;
    }
    for (auto &n : arr){//使用引用方式,可写
    ...
    }
	return 0;
}
```

## 六、强类型枚举(枚举类)

### 6.1 传统枚举

* 定义不同的枚举类型,枚举中的元素不能重复出现(作用域是全局)

* 枚举默认转换为int,`cout<<ENUM<<endl;`合法

* 可以给int赋值,自动转换为int型再赋值

```cpp
enum ANIMAL{
    MONKEY,
    HORSE,
    LION,
};
enum KPANIMAL{
    MONKEY, //全局作用域,重复声明错误
    A,
    B,
};
int main()
{
    int a = A;      //自动类型转换为int
    cout<<A<<endl;
    return 0;
}
```

### 6.2 强枚举类型

* 不同名称的枚举类型,元素可以相同,不是全局作用域

* 可以指定底层类型,默认为int

  ```cpp
  enum class LETTER:char{
      A = 1,
      B = 2,
  };
  enum class LETTER:unsingned int{
      A = 1,
      B = 2,
  };
  ```
* 枚举元素不会自动类型转换

* 通过::引用ANIMAL::MONKEY

```cpp
enum class ANIMAL{
    MONKEY,
    HORSE,
    LION,
};
enum class KPANIMAL{
    MONKEY,  //内部类型,不同名称枚举类型,元素可以相同
    A,
    B,
};
int main()
{
    int b = A;               //error,非全局,通过KPANIMAL::A引用
    int a = KPANIMAL::A;     //error,不能自动转换,需强制转换
    cout<<KPANIMAL::A<<endl; //error,不能自动转换,需强制转换

    int a = (int)KPANIMAL::A;      //correct
    cout<<(int)KPANIMAL::A<<endl;  //correct
}
```

## 七、新的类功能

### 7.1 显式使用默认函数(6个)

　　如果没有定义相应的函数,C\++编译器会默认生成**构造,复制构造,移动构造,赋值操作符,移动赋值操作符,析构函数**.如果定义了,编译器将不会提供.

　　如果要使用某个默认的函数,而这个函数由于某种原因不会自动创建.如**定义了移动构造函数,编译器不会自动创建构造函数,复制构造函数和赋值操作符**.

　　可以使用`default`显式声明这些方法的默认版本,**只能用于6个特殊的成员函数**

```cpp
class SomeClass
{
public:
    SomeClass(SomeClass &&) = default;
    SomeClass &operator=(SomeClass &&) = default;
    SomeClass() = default;
    SomeClass(const SomeClass &) = default;
    SomeClass &operator=(const SomeClass &) = default;
    ~SomeClass() = default;
};
```

### 7.2 禁止使用某函数

相当于放在`private`部分,`delete`实现,可以用于任何成员函数.

```cpp
class SomeClass
{
public:
    SomeClass(SomeClass &&) = delete;
    SomeClass &operator=(SomeClass &&) = delete;
    SomeClass() = default;
    SomeClass(const SomeClass &) = default;
    SomeClass &operator=(const SomeClass &) = delete;

    //禁止特定类型的转换
    void redo(int) = delete;
};

int main(){
    SomeClass one;
    SomeClass two;
    SomeClass three(one);           //not allowed
    SomeClass four( one + two ) ;   // allowed , expression is an rvalue.

    SomeClass sc;
    sc.redo( 5 );                   //compile error
}
```

## 八、统一的初始化

可以统一使用花括号进行初始化

```cpp
int x = {5};
double y{2.75}; //可以添加=,也可以不添加.
short qura[5]{4,5,2,76,1};

//init array
int *ar = new int[4]{2,4,6,7};

//init class
class Stump{
private:
    int roots;
    double weight;
public:
    Stump(int r, double w) :roots{r},weight{w}{}
};
Stump s1(3,15.6);
Stump s2{5,43.4};
Stump s3 = {4,32.1};
```

### 8.1 禁止不恰当的类型转换

```cpp
char c1 = 1.57e27;    //undefined behavior
char c2 = 45958521;   //undefined behavior

char c1{57e27};       //compile-time error
char c2{45958521}     //compile-time error

char c1{66};         //allowed
char c2 = {66}      //allowed
```
### 8.2 std::initializer_list

C\++11提供了模板类initializer_list,可将其用作构造函数的参数.

```cpp
vector<int> a1(10); // 10 elements(unintialized)

vector<int> a2{10}; //initializer_list,1 element set to 10
vector<int> a3{4,6,1}; //3 elements set to 4,6,1
```

initializer_list用作常规函数的参数

```cpp
#include <initializer_list>
double sum(std::initializer_list<double> il);
int main(){
    double total = sum({2.5,3.1,4});
    ...
}
double sum(std::initializer_list<double> il){
    double tot = 0;
    for(auto p = il.begin() ; p != il.end(); p++)
        tot += *p;
    return tot;
}
```

## 九、返回类型后置

C\++新增了一种函数声明语法:在函数名和参数列表后面指定返回类型.

```cpp
double f1(double,int); //traditinal

auto f2(double,int)->double;

//一般用于指定模板的返回类型
//c++11
template<typename T,typename U>
auto eff( T t,U u)->decltype(T*U){
}

//c++14,增强了auto,可以直接推断返回值类型
template<typename T,typename U>
auto eff( T t,U u ){
}
```

## 十、模板别名

```cpp
//traditinal
typedef std::vector<std::string>::iterator itType;

//c++11
using itType = std::vector<std::string>::iterator;

//using 可用于模板部分具体化,而typedef不能
using arr12 = std::array<T,12>;
arr12<double> a1;<==>std::array<double,12 > a1;
```

## 十一、类成员的初始化

C\++11 可以在类定义中初始化类成员

```cpp
class Session{
    int mem1 = 10;
    double mem2{1966.54};
    short mem3;
public:
    Session();
    Session(short s):mem3{3}{}
    Session(int n,double d,short s) : mem1{n},mem2{d},mem3{s}{}
};
```

* 可以使用`=`和`{}`初始化,但不能用`()`进行初始化.

* 如果构造函数成员初始化列表提供了相应值,默认值将被覆盖,如mem3;

## 十二、委托构造函数

　　如果类提供了多个构造函数,C\++11允许在一个构造函数定义中使用另一个构造函数,这被称为委托.

```cpp
clas Notes{
    int k;
    double x;
    std::string st;
public:
    Notes();
    Notes(int);
    Notes(int ,double);
    Notes(int,double,std::string);
};
Notes::Notes(int kk,double xx,std::string stt):k{kk},x{xx},st{stt}{}
Notes::Notes():Notes{0,0.01,"oh"}{}
Notes::Notes(int kk):Notes{kk,0.01,"oh"}{}
Notes::Notes(int kk,double xx):Notes{kk,xx,"oh"}{}
```

## 十三、管理虚方法(override 和 final )

　　虚方法派生过程中,如果派生的方法与基类方法的参数不同,基类方法将被隐藏而不是覆盖.

```cpp
class Action{
    int a;
public:
    Action(int i = 0):a{i}{}
    int val() const { return a;}
    virtual void f(char ch) const { std::cout<<val()<<ch<<endl;}
};
class Bingo : public Action{
public:
    Bingo( int i = 0 ) : Action{i}
    virtual void f(char *ch) const { std::cout<<val()<<ch<<endl;}
};
```
　　由于`f()`参数不同,`Bingo::f(chr *ch)将隐藏`Action::f(char ch)`

### 13.1 override(覆盖)

　　使用`override`指出要覆盖一个虚函数.如果声明与基类方法不匹配,编译将出现错误.

```cpp
//重写Bingo::f()
virtual void f(char *ch) const override{
    std:cout<<val()<<ch<<endl;
}
//生成编译错误
```

### 13.2 final

禁止派生类覆盖特定的虚方法.

```cpp
//禁止Action的派生类重新定义f()
virtual void f(char ch) const final {
    std::cout<<val()<<ch<,endl;
}
```

　　`override`和`final`并非关键字,而是具有特殊意义的标识符,编译器根据上下文确定他们是否有特殊含义.



