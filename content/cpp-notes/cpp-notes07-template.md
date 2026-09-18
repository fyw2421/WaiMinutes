---
title: "C++Notes07 : Template"
weight: 7
description: "Function-Template Function-Template-Specialization Class-Template Class-Template-Specialization Class-Template-And-Friend"
date: 2018-05-22
tags: ["C++"]
featureimage: "covers/cpp-notes07-template.svg"
---
## 一、函数模板(Function Template)

　　函数模板是通用的函数描述,它们使用通用类型来定义函数,其中通用类型可用具体的类型(int,double)来替换.通过将通用类型作为参数传递给模板,可使编译器生成该类型的函数.由于模板允许以通用类型的方式编写程序,因此称为通用编程(generic programming)或泛型编程.

### 1.1 函数模板的定义

```cpp
template <typename 模板形参>
返回值类型 函数模板名(参数变){}

template <typename T>
void swap( T &a , T &b ){
     T temp = a;
     a = b;
     b = temp;
}
```

### 1.2 函数模板的使用

　　当编译器发现有一个函数模板名为函数名的调用时,将根据实参类型,生成一个函数,称为模板函数或实例化.

```cpp
//=====================================
// f1401.cpp
// define & using function template
//=====================================
#include<iostream>
using namespace std;
template<typename T>
void swap(T& a, T& b){
  T temp=a; a=b; b=temp;
}//------------------------------------
int main(){
  double dx=3.5, dy=5.6;
  int ix=6, iy=7, ia=303, ib=505;
  string s1="good", s2="better";
  cout<<"double dx="<<dx<<",      dy="<<dy<<"\n";
  cout<<"int    ix="<<ix<<",        iy="<<iy<<"\n";
  cout<<"string s1=\""<<s1<<"\",   s2=\""<<s2<<"\"\n";
  swap(dx, dy);
  swap(ix, iy);
  swap(s1, s2);
  swap(ia, ib);
  cout<<"\nafter swap:\n";
  cout<<"double dx="<<dx<<",      dy="<<dy<<"\n";
  cout<<"int    ix="<<ix<<",        iy="<<iy<<"\n";
  cout<<"string s1=\""<<s1<<"\", s2=\""<<s2<<"\"\n";
}//====================================
```

### 1.3 函数模板的重载

```cpp
// twotemps.cpp -- using overloaded template functions
#include <iostream>
template <class Any>     // original template
void Swap(Any &a, Any &b);

template <class Any>     // new template
void Swap(Any *a, Any *b, int n);

void Show(int a[]);
const int Lim = 8;
int main()
{
    using namespace std;
    int i = 10, j = 20;
    cout << "i, j = " << i << ", " << j << ".\n";
    cout << "Using compiler-generated int swapper:\n";
    Swap(i,j);              // matches original template
    cout << "Now i, j = " << i << ", " << j << ".\n";

    int d1[Lim] = {0,7,0,4,1,7,7,6};
    int d2[Lim] = {0,6,2,0,1,9,6,9};
    cout << "Original arrays:\n";
    Show(d1);
    Show(d2);
    Swap(d1,d2,Lim);        // matches new template
    cout << "Swapped arrays:\n";
    Show(d1);
    Show(d2);

    return 0;
}

template <class Any>
void Swap(Any &a, Any &b)
{
    Any temp;
    temp = a;
    a = b;
    b = temp;
}

template <class Any>
void Swap(Any a[], Any b[], int n)
{
    Any temp;
    for (int i = 0; i < n; i++)
    {
        temp = a[i];
        a[i] = b[i];
        b[i] = temp;
    }
}

void Show(int a[])
{
    using namespace std;
    cout << a[0] << a[1] << "/";
    cout << a[2] << a[3] << "/";
    for (int i = 4; i < Lim; i++)
        cout << a[i];
    cout << endl;
}
```

## 二、函数模板的具体化(specialization)

　　隐式实例化、显式实例化和显式具体化统称为具体化.

### 2.1 隐式实例化

　　编译器使用模板为特定类型生成函数定义时,得到的是模板实例.

　　当函数通过传递的类型生成对应的模板函数,称为**隐式实例化**(implicit instantiation).

　　在使用模板之前,编译器不生成模板的声明和定义实例。只有当使用模板时,编译器才根据模板定义生成相应类型的实例。

```cpp
template <typename T>
void swap( T &a , T &b ){
     T temp = a;
     a = b;
     b = temp;
}
int a = 5 , b = 3;
swap( a, b) ; //implicit instantiation
```

### 2.2 显式实例化(instantiation)

　　程序也可以直接命令编译器创建指定的模板实例,称为**显式实例化**(explicit instantiation).

　　当显式实例化模板时,在使用模板之前,编译器根据显式实例化指定的类型生成模板实例。

　　显式实例化只需声明,不需要重新定义。编译器根据模板实现实例声明和实例定义。

```cpp
//template prototype
template <typename T>
void swap( T &,T &);

int main(){
template void swap<char>(char &,char &);
}
```

### 2.3 显式具体化(explicit specialization)

```cpp
struct job{
    char name[40];
    double salary;
    int floor;
};
```

　　假设只交换salary和floor,无法使用函数模板.所以需要**显式具体化**来匹配.显式具体化的声明和定义

```cpp
template<> void swap<job>( job & ,job &);

template<> void swap<job>( job &j1 ,job &j2){
    ...
}
```

非模板函数、模板函数和具体化的原型:

```cpp
//non-template function prototype
void swap( job &,job &);

//template prototype
template <typename T>
void swap( T &,T &);

//explicit instantiation
template void swap<char>(char &,char &);

//explicit specialization
template<> swap<job>( job &,job &);
template<> swap<job>( job &,job &);  //等价写法

//conflict
template void swap<job>(job &,job &);
//试图在同一编程单元中使用同一类型的显式实例化和显式具体化是错误的.
```

**试图在同一编程单元中使用同一类型的显式实例化和显式具体化是错误的.**

### 2.4 调用顺序

```cpp
// twoswap.cpp -- specialization overrides a template
#include <iostream>
template <class Any>
void Swap(Any &a, Any &b);

struct job
{
    char name[40];
    double salary;
    int floor;
};

// explicit specialization
template <> void Swap<job>(job &j1, job &j2);
void Show(job &j);

//explicit instantiation
template void swap<char>(char &,char &);

int main()
{
    using namespace std;
    cout.precision(2);
    cout.setf(ios::fixed, ios::floatfield);
    int i = 10, j = 20;
    cout << "i, j = " << i << ", " << j << ".\n";
    cout << "Using compiler-generated int swapper:\n";
    Swap(i,j);    // generates void Swap(int &, int &)
    cout << "Now i, j = " << i << ", " << j << ".\n";

    job sue = {"Susan Yaffee", 73000.60, 7};
    job sidney = {"Sidney Taffee", 78060.72, 9};
    cout << "Before job swapping:\n";
    Show(sue);
    Show(sidney);
    Swap(sue, sidney); // uses void Swap(job &, job &)
    cout << "After job swapping:\n";
    Show(sue);
    Show(sidney);

    return 0;
}

template <class Any>
void Swap(Any &a, Any &b)    // general version
{
    Any temp;
    temp = a;
    a = b;
    b = temp;
}

// swaps just the salary and floor fields of a job structure

template <> void Swap<job>(job &j1, job &j2)  // specialization
{
    double t1;
    int t2;
    t1 = j1.salary;
    j1.salary = j2.salary;
    j2.salary = t1;
    t2 = j1.floor;
    j1.floor = j2.floor;
    j2.floor = t2;
}

void Show(job &j)
{
    using namespace std;
    cout << j.name << ": $" << j.salary
         << " on floor " << j.floor << endl;
}
```

调用顺序如下:

1. 常规函数(非模板函数)

2. 具体化(显式实例化和显式具体化),同一类型不会同时存在,根据匹配程度调用.

3. 常规模板(隐式实例化)

## 三、类模板

### 3.1 定义类模板

因为模板不是函数,不能单独编译,必须与特定的模板实例化请求一起使用.

1. **声明与定义在同一文件中**

   ```cpp
   // stacktp.h -- a stack template
   #ifndef STACKTP_H_
   #define STACKTP_H_
   template <class Type>
   class Stack
   {
   private:
       enum {MAX = 10};    // constant specific to class
       Type items[MAX];    // holds stack items
       int top;            // index for top stack item
   public:
       Stack();
       bool isempty();
       bool isfull();
       bool push(const Type & item); // add item to stack
       bool pop(Type & item);        // pop top into item
   };

   template <class Type>
   Stack<Type>::Stack()
   {
       top = 0;
   }

   template <class Type>
   bool Stack<Type>::isempty()
   {
       return top == 0;
   }

   template <class Type>
   bool Stack<Type>::isfull()
   {
       return top == MAX;
   }

   template <class Type>
   bool Stack<Type>::push(const Type & item)
   {
       if (top < MAX)
       {
           items[top++] = item;
           return true;
       }
       else
           return false;
   }

   template <class Type>
   bool Stack<Type>::pop(Type & item)
   {
       if (top > 0)
       {
           item = items[--top];
           return true;
       }
       else
           return false;
   }

   #endif
   ```

2. **使用export关键字**,将模板方法定义放在独立文件中.每个模板声明都以export开始.**C\++11终止了这种用法**,但仍保留了export关键字

   ```cpp
   // stacktp.h -- a stack template
   #ifndef STACKTP_H_
   #define STACKTP_H_
   export template <class Type>
   class Stack
   {
       ...
   };
   ```

### 3.2 类模板使用

类模板实例化,根据传递给类模板的实参类型生成模板类.

```cpp
Stack<int> kernels;
Stack<string> colonels;
```

**Sample01:内置类型或类对象作实参**

```cpp
// stacktem.cpp -- test template stack class
#include <iostream>
#include <string>
#include <cctype>
#include "stacktp.h"
using std::cin;
using std::cout;

int main()
{
    Stack<std::string> st;   // create an empty stack
    char ch;
    std::string po;
    cout << "Please enter A to add a purchase order,\n"
         << "P to process a PO, or Q to quit.\n";
	while (cin >> ch && std::toupper(ch) != 'Q')
    {
        while (cin.get() != '\n')
            continue;
		if (!std::isalpha(ch))
        {
            cout << '\a';
            continue;
        }
        switch(ch)
        {
            case 'A':
            case 'a': cout << "Enter a PO number to add: ";
                      cin >> po;
                      if (st.isfull())
                          cout << "stack already full\n";
                      else
                          st.push(po);
                      break;
            case 'P':
            case 'p': if (st.isempty())
                          cout << "stack already empty\n";
                      else {
                          st.pop(po);
                          cout << "PO #" << po << " popped\n";
                          break;
                      }
        }
        cout << "Please enter A to add a purchase order,\n"
             << "P to process a PO, or Q to quit.\n";
    }
    cout << "Bye\n";
    return 0;
}
```
**Sample02:指针作实参**

`Stack<char *> st`指针没有空间,需要为其申请空间,必须修改类模板.

```cpp
// stcktp1.h -- modified Stack template
#ifndef STCKTP1_H_
#define STCKTP1_H_

template <class Type>
class Stack
{
private:
    enum {SIZE = 10};    // default size
    int stacksize;
    Type * items;       // holds stack items
    int top;            // index for top stack item
public:
    explicit Stack(int ss = SIZE);
    Stack(const Stack & st);
    ~Stack() { delete [] items; }
    bool isempty() { return top == 0; }
    bool isfull() { return top == stacksize; }
    bool push(const Type & item);   // add item to stack
    bool pop(Type & item);          // pop top into item
    Stack & operator=(const Stack & st);
};

template <class Type>
Stack<Type>::Stack(int ss) : stacksize(ss), top(0)
{
    items = new Type [stacksize];
}

template <class Type>
Stack<Type>::Stack(const Stack & st)
{
    stacksize = st.stacksize;
    top = st.top;
    items = new Type [stacksize];
    for (int i = 0; i < top; i++)
        items[i] = st.items[i];
}

template <class Type>
bool Stack<Type>::push(const Type & item)
{
    if (top < stacksize)
    {
        items[top++] = item;
        return true;
    }
    else
        return false;
}

template <class Type>
bool Stack<Type>::pop(Type & item)
{
    if (top > 0)
    {
        item = items[--top];
        return true;
    }
    else
        return false;
}

template <class Type>
Stack<Type> & Stack<Type>::operator=(const Stack<Type> & st)
{
    if (this == &st)
        return *this;
    delete [] items;
    stacksize = st.stacksize;
    top = st.top;
    items = new Type [stacksize];
    for (int i = 0; i < top; i++)
        items[i] = st.items[i];
    return *this;
}
#endif
```

测试方法:

```cpp
// stkoptr1.cpp -- test stack of pointers
#include <iostream>
#include <cstdlib>     // for rand(), srand()
#include <ctime>       // for time()
#include "stcktp1.h"
const int Num = 10;
int main()
{
    std::srand(std::time(0)); // randomize rand()
    std::cout << "Please enter stack size: ";
    int stacksize;
    std::cin >> stacksize;
// create an empty stack with stacksize slots
    Stack<const char *> st(stacksize);

// in basket
    const char * in[Num] = {
            " 1: Hank Gilgamesh", " 2: Kiki Ishtar",
            " 3: Betty Rocker", " 4: Ian Flagranti",
            " 5: Wolfgang Kibble", " 6: Portia Koop",
            " 7: Joy Almondo", " 8: Xaverie Paprika",
            " 9: Juan Moore", "10: Misha Mache"
            };
 // out basket
    const char * out[Num];

    int processed = 0;
    int nextin = 0;
    while (processed < Num)
    {
        if (st.isempty())
            st.push(in[nextin++]);
        else if (st.isfull())
            st.pop(out[processed++]);
        else if (std::rand() % 2  && nextin < Num)   // 50-50 chance
            st.push(in[nextin++]);
        else
            st.pop(out[processed++]);
    }
    for (int i = 0; i < Num; i++)
        std::cout << out[i] << std::endl;

    std::cout << "Bye\n";
    return 0;
}
```

### 3.3 模板参数

　　模板参数主要是类型参数和非类型参数(如值参数).

#### 3.3.1 非类型参数

　　指定特定的类型而不是用通用类型名作参数,称为非类型参数(non-type)或表达式参数(expression)

```cpp
//arraytp.h  -- Array Template
#ifndef ARRAYTP_H_
#define ARRAYTP_H_

#include <iostream>
#include <cstdlib>

template <class T, int n>  //T为类型参数,n为非类型参数
class ArrayTP
{
private:
    T ar[n];
public:
    ArrayTP() {};
    explicit ArrayTP(const T & v);
    virtual T & operator[](int i);
    virtual T operator[](int i) const;
};

template <class T, int n>
ArrayTP<T,n>::ArrayTP(const T & v)
{
    for (int i = 0; i < n; i++)
        ar[i] = v;
}

template <class T, int n>
T & ArrayTP<T,n>::operator[](int i)
{
    if (i < 0 || i >= n)
    {
        std::cerr << "Error in array limits: " << i
            << " is out of range\n";
        std::exit(EXIT_FAILURE);
    }
    return ar[i];
}

template <class T, int n>
T ArrayTP<T,n>::operator[](int i) const
{
    if (i < 0 || i >= n)
    {
        std::cerr << "Error in array limits: " << i
            << " is out of range\n";
        std::exit(EXIT_FAILURE);
    }
    return ar[i];
}

#endif

```

* 表达式参数可以是整型、枚举、引用或指针.模板不能修改参数的值或地址.

  ```cpp
  ArrayTP<double,12> eggweights;  //合法
  ArrayTP<double,5.0f> eggweights;  //表达式参数不合法
  ArrayTP<double,*m> eggweights;  //表达式参数合法
  ArrayTP<double,&m> eggweights;  //表达式参数合法

  template <class T, int n>
  n++;  //错误,不能修改参数的值
  ```

* 表达式参数不同,模板类不同.

  ```cpp
  ArrayTP<double,12> eggweights;
  ArrayTP<double,5> donuts;
  eggweights = donuts ; // 错,两个不同的模板类

  Stack<int> eggs(12);
  Stack<int> dunkers(13);  //相同的模板类
  ```

#### 3.3.2 参数的类型和数量

1. 嵌套参数

   ```cpp
   ArrayTP< ArrayTP<int,5>, 10> twodee;
   ```

   **Sample**

   ```cpp
   // twod.cpp -- making a 2-d array
   #include <iostream>
   #include "arraytp.h"
   int main(void)
   {
       using std::cout;
       using std::endl;
       ArrayTP<int, 10> sums;
       ArrayTP<double, 10> aves;
       ArrayTP< ArrayTP<int,5>, 10> twodee;

       int i, j;

       for (i = 0; i < 10; i++)
       {
           sums[i] = 0;
           for (j = 0; j < 5; j++)
           {
               twodee[i][j] = (i + 1) * (j + 1);
               sums[i] += twodee[i][j];
           }
           aves[i] = (double) sums[i] / 10;
       }
       for (i = 0; i < 10; i++)
       {
           for (j = 0; j < 5; j++)
           {
               cout.width(2);
               cout << twodee[i][j] << ' ';
           }
           cout << ": sum = ";
           cout.width(3);
           cout  << sums[i] << ", average = " << aves[i] << endl;
       }

       cout << "Done.\n";

       return 0;
   }
   ```

2. 多个类型参数

   ```cpp
   // pairs.cpp -- define and use a Pair template
   #include <iostream>
   #include <string>
   template <class T1, class T2>
   class Pair
   {
   private:
       T1 a;
       T2 b;
   public:
       T1 & first();
       T2 & second();
       T1 first() const { return a; }
       T2 second() const { return b; }
       Pair(const T1 & aval, const T2 & bval) : a(aval), b(bval) { }
       Pair() {}
   };

   template<class T1, class T2>
   T1 & Pair<T1,T2>::first()
   {
       return a;
   }
   template<class T1, class T2>
   T2 & Pair<T1,T2>::second()
   {
       return b;
   }

   int main()
   {
       using std::cout;
       using std::endl;
       using std::string;
       Pair<string, int> ratings[4] =
       {
           Pair<string, int>("The Purple Duke", 5),
           Pair<string, int>("Jake's Frisco Al Fresco", 4),
           Pair<string, int>("Mont Souffle", 5),
           Pair<string, int>("Gertie's Eats", 3)
       };

       int joints = sizeof(ratings) / sizeof (Pair<string, int>);
       cout << "Rating:\t Eatery\n";
       for (int i = 0; i < joints; i++)
           cout << ratings[i].second() << ":\t "
                << ratings[i].first() << endl;
       cout << "Oops! Revised rating:\n";
       ratings[3].first() = "Gertie's Fab Eats";
       ratings[3].second() = 6;
       cout << ratings[3].second() << ":\t "
            << ratings[3].first() << endl;

       return 0;
   }
   ```

3. 默认类型模板参数

   ```cpp
   template <typename T1,typename T2 = int>
   class Topo{}

   Topo<double,double> m1; //T1 is double,T2 is double
   Topo<double> m2;        //T1 is double,T2 is int
   ```

### 3.4 模板具体化

　　与函数模板类似,可以有隐式实例化、显式实例化和显式具体化.

#### 3.4.1 类模板的实例化过程.

　　类模板的实例化过程,是根据具体的模板实参,替换成模板形参而产生对应模板类的过程.

1. 被激活的实例化,只实例化类模板的定义部分,不将其成员函数的定义部分一起实例化.

2. 首次调用模板类成员函数时,对成员函数进行实例化.

#### 3.4.2 隐式实例化

```cpp
ArrayTP<int,100> stuff; //implicit instantiation

ArrayTP<double,30> *pt;
pt = new ArrayTP<double,30>; //编译器在需要对象之前,不会生成类的隐式实例化
```

#### 3.4.3 显式实例化

```cpp
template class ArrayTP<string,100>; //声明必须位于模板定义的名空间中.
```

#### 3.4.4 显式具体化

　　用特定类型来替换通用类型.当具体化模板和通用模板都与请求匹配时,编译器将使用具体化版本.

```cpp
template<> class Classname<specilized-type-name>{}

template<> class SortedArray<char *>{}
SortedArray<int> scores;   //use general definiation
SortedArray<char *> dates; //use specilized definiation
```

#### 3.4.5 部分具体化

```cpp
//general template
template < typename T1,typename T2> class Pair{};

//partial specilized
template <typename T1> class Pair<T1,int>{};

//explicit specialization
template <> class Pair<int,int>{};

//如果有多个模板可供选择,编译器使用具体化程度最高的模板.
```

### 3.5 成员模板

模板可用作结构、类或模板类的成员

**Sample01**

```cpp
// tempmemb.cpp -- template members
#include <iostream>
using std::cout;
using std::endl;

template <typename T>
class beta
{
private:
    template <typename V>  // nested template class member
    class hold
    {
    private:
        V val;
    public:
        hold(V v  = 0) : val(v) {}
        void show() const { cout << val << endl; }
        V Value() const { return val; }
    };
    hold<T> q;             // template object
    hold<int> n;           // template object
public:
    beta( T t, int i) : q(t), n(i) {}
    template<typename U>   // template method
    U blab(U u, T t) { return (n.Value() + q.Value()) * u / t; }
    void Show() const { q.show(); n.show();}
};
int main()
{
    beta<double> guy(3.5, 3);

    guy.Show();
    cout << guy.blab(10, 2.3) << endl;
    cout << "Done\n";
    return 0;
}
```

**Sample02**

```cpp
//=====================================
// f1406.cpp
// using class template
//=====================================
#include<iostream>
using namespace std;
//-------------------------------------
template<typename T>
struct Node{
  Node(const T& d):c(d),next(0),pref(0){}
  T c;
  Node *next, *pref;
};//-----------------------------------
template<typename T>
class List{
  Node<T> *first, *last;
public:
  List();
  void add(const T& c);
  void remove(const T& c);
  Node<T>* find(T& c)const;
  void print()const;
 ~List();
};//-----------------------------------
template<typename T>
List<T>::List():first(0),last(0){}
//-------------------------------------
template<typename T>
void List<T>::add(const T& n){
  Node<T>* p = new Node<T>(n);
  p->next = first;  first = p;
  (last ? p->next->pref : last) = p;
}//------------------------------------
template<typename T>
void List<T>::remove(const T& n){
  if(!(Node<T>* p = find(n))) return;
  (p->next ? p->next->pref : last) = p->pref;
  (p->pref ? p->pref->next : first) = p->next;
  delete p;
}//------------------------------------
template<typename T>
Node<T>* List<T>::find(T& n)const{
  for(Node<T>* p=first; p; p=p->next)
    if(p->c==n) return p;
  return 0;
}//------------------------------------
template<typename T>
List<T>::~List(){
  for(Node<T>* p; p=first; delete p)
    first = first->next;
}//------------------------------------
template<typename T>
void List<T>::print()const{
  for(Node<T>* p=first; p; p=p->next)
    cout<<p->c<<"  ";
  cout<<"\n";
}//------------------------------------
int main(){
  List<double> dList;
  dList.add(3.6);
  dList.add(5.8);
  dList.print();
  List<int> iList;
  iList.add(5);
  iList.add(8);
  iList.print();
}//====================================
```

### 3.6 类模板与友元

模板的友元分为三类:

* 非模板友元

* 约束模板友元.即友元的类型取决于被实例化时的类型.

* 非约束模板友元.即友元的所有具体化都是类的每一个具体化的友元.

#### 3.6.1 非模板友元

```cpp
template <typename T>
class HasFriend{
    friend void report( HasFriend &); //错误,不存在HasFriend这样的对象,只有特定的具体化.

    friend void report( HasFriend<T> &); //
}
//必须要为使用的友元定义显式具体化
void report( HasFriend<int> &hf){}
```

**Sample**

```cpp
// frnd2tmp.cpp -- template class with non-template friends
#include <iostream>
using std::cout;
using std::endl;

template <typename T>
class HasFriend
{
private:
    T item;
    static int ct;
public:
    HasFriend(const T & i) : item(i) {ct++;}
    ~HasFriend()  {ct--; }
    friend void counts();
    friend void reports(HasFriend<T> &); // template parameter
};

// each specialization has its own static data member
template <typename T>
int HasFriend<T>::ct = 0;

// non-template friend to all HasFriend<T> classes
void counts()
{
    cout << "int count: " << HasFriend<int>::ct << "; ";
    cout << "double count: " << HasFriend<double>::ct << endl;
}

// non-template friend to the HasFriend<int> class
void reports(HasFriend<int> & hf)
{
    cout <<"HasFriend<int>: " << hf.item << endl;
}

// non-template friend to the HasFriend<double> class
void reports(HasFriend<double> & hf)
{
    cout <<"HasFriend<double>: " << hf.item << endl;
}

int main()
{
    cout << "No objects declared: ";
    counts();
    HasFriend<int> hfi1(10);
    cout << "After hfi1 declared: ";
    counts();
    HasFriend<int> hfi2(20);
    cout << "After hfi2 declared: ";
    counts();
    HasFriend<double> hfdb(10.5);
    cout << "After hfdb declared: ";
    counts();
    reports(hfi1);
    reports(hfi2);
    reports(hfdb);

    return 0;
}
```

#### 3.6.2 约束模板友元

友元的类型取决于被实例化时的类型,类外声明,实现步骤；

```cpp
//第一步,类定义前声明函数模板
template <typename T> void counts();
template <typename T> void report( T &);

//第二步,在函数中再次声明函数模板为友元
template <typename TT>
class HasFriend{
...
    friend void count();
    friend void report<>(HasFriend<TT> &); //显示具体化,指定类型
}

//第三步,定义函数模板
template <typename T>
void counts(){}

template <typename T>
void report( T &hf ){}
```

**Sample**

```cpp
// tmp2tmp.cpp -- template friends to a template class
#include <iostream>
using std::cout;
using std::endl;

// template prototypes
template <typename T> void counts();
template <typename T> void report(T &);

// template class
template <typename TT>
class HasFriendT
{
private:
    TT item;
    static int ct;
public:
    HasFriendT(const TT & i) : item(i) {ct++;}
    ~HasFriendT() { ct--; }
    friend void counts<TT>();
    friend void report<>(HasFriendT<TT> &);
};

template <typename T>
int HasFriendT<T>::ct = 0;

// template friend functions definitions
template <typename T>
void counts()
{
    cout << "template size: " << sizeof(HasFriendT<T>) << "; ";
    cout << "template counts(): " << HasFriendT<T>::ct << endl;
}

template <typename T>
void report(T & hf)
{
    cout << hf.item << endl;
}

int main()
{
    counts<int>();
    HasFriendT<int> hfi1(10);
    HasFriendT<int> hfi2(20);
    HasFriendT<double> hfdb(10.5);
    report(hfi1);  // generate report(HasFriendT<int> &)
    report(hfi2);  // generate report(HasFriendT<int> &)
    report(hfdb);  // generate report(HasFriendT<double> &)
    cout << "counts<int>() output:\n";
    counts<int>();
    cout << "counts<double>() output:\n";
    counts<double>();

    return 0;
}
```

#### 3.6.3 非约束模板友元

友元的所有具体化都是类的每一个具体化的友元.类内声明

```cpp
template <typename T>
class ManyFriend{
...
template <typename C,typename D>
friend void show2( C &,D &);   //友元函数在类内声明,友元的所有具体是类的每一个具体化的友元
}
```

**Sample**

```cpp
// manyfrnd.cpp -- unbound template friend to a template class
#include <iostream>
using std::cout;
using std::endl;

template <typename T>
class ManyFriend
{
private:
    T item;
public:
    ManyFriend(const T & i) : item(i) {}
    template <typename C, typename D> friend void show2(C &, D &);
};

template <typename C, typename D> void show2(C & c, D & d)
{
    cout << c.item << ", " << d.item << endl;
}

int main()
{
    ManyFriend<int> hfi1(10);
    ManyFriend<int> hfi2(20);
    ManyFriend<double> hfdb(10.5);
    cout << "hfi1, hfi2: ";
    show2(hfi1, hfi2);
    cout << "hfdb, hfi2: ";
    show2(hfdb, hfi2);

    return 0;
}
```



