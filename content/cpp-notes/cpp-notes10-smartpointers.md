---
title: "C++Notes10 : Smart Pointers"
weight: 10
description: "auto_ptr(obsolete) unique_ptr shared_ptr weak_ptr make_shared"
date: 2018-05-22
tags: ["C++"]
featureimage: "covers/cpp-notes10-smartpointers.svg"
---
## 一、auto_ptr类(C\++98)

　　auto_ptr是一个C\++98中提供的模板类,用来管理动态内存分配的用法.auto_ptr模板定义了类似指针的对象,可以将new获得的地址赋给这种对象.当auto_ptr对象过期时,其析构函数将调用delete来释放内存.因此将new返回的地址赋给auto_ptr对象时,无需释放这些内存.定义在头文件memory中.

[**C\++11中被弃用**](https://blog.csdn.net/haolexiao/article/details/56773039)

```cpp
//auto_ptr模板的声明
template<typename X>
class auto_ptr{
public:
  explicit auto_ptr( X *p = 0 ) throw();
};

//使用
#include <memory>
auto_ptr<double> pd(new double);
auto_ptr<string> ps(new string);
```

### 1.1 auto_ptr使用注意事项

1. new和delete对象,new[]和delete[]对应,auto_ptr模板使用的是delete,因此只能和new一起使用.

   ```cpp
   auto_ptr<int> pi(new int[200]); //error

   //智能指针只能与new联用,不能使用对象的地址
   string vacation("I wandered lonely as a cloud. ");
   auto_ptr<string> pvac(&vacation); //error,string是对象,生命周期结束自行调用析构,auto_ptr会delete已不可用的对象.
   ```

2. 智能指针不能指向另一个智能指针,auto_ptr重载了赋值运算符,去赋值的失去控制权变成空指针

   ```cpp
   auto_ptr<string> ps( new string("a"));
   auto_ptr<string> vocation;
   vocation = ps ; //ps失去控制权
   ```

### 1.2 auto_ptr解决办法

* 定义赋值操作符,使之深拷贝.

* 特定对象只能有一个智能指针

* 跟踪智能指针的个数,赋值时加1,过期时减1.仅当最后指针过期时,delete才被调用

## 二、C\++11智能指针

　　C\++11之后智能指针分为了三种,包含在`<memory>`中

### 2.1 unique_ptr

　　独占指针对象,并保证指针所指对象生命周期与其一致.

### 2.2 shared_ptr

　　共享指针对象,可以赋值给shared_ptr或weak_ptr,指针所指对象在所有相关联的shared_ptr生命周期结束时结束,是强引用.

### 2.3 weak_ptr

　　它不能决定所指对象的生命周期,引用所指对象时,需要lock()成shared_ptr才能使用.

　　C\++11之后的智能指针的构造函数都有explicit关键词修饰,表明它不能被隐式的类型转换.

```cpp
shared_ptr<int> p1 = new int(1024);  //错误,不能隐式转换
shared_ptr<int> p2(new int(1024));   //这种是直接采用了初始化的形式
```

## 三、unique_ptr

### 3.1 new分配内存

```cpp
//底层实现
unique_ptr(const _My &) = delete;
_My &operator=(const _My &) = delete;
```

　　unique_ptr中的拷贝构造和赋值操作符delete了,所以也就意味着方法私有,和auto_ptr有区别,控制权唯一,不能随意转换.

```cpp
//普通指针
unique_ptr<int> up1(new int(11));  //无法复制的unique_ptr
unique_ptr<int> up2 = up1;         //不能通过编译,可以通过move实现
```

### 3.2 new[]分配的内存

```cpp
//动态数组
unique_ptr<int[] > up(new int[10]);
up[i]=10;      //可以直接使用下标运算符
up.release(); //自动调用delete[]释放指针
```

## 四、shared_ptr

### 4.1 new分配内存

```cpp
shared_ptr<Base1>	base1(new Base1);
shared_ptr<Base1>	base2=base1;
shared_ptr<Base1>	base3;
base3 = base2;   //三个共享一个
```

　　当删除一个智能指针时,并不影响其它两个智能指针的继续使用.因为该片内存添加了一个引用计数,每shared_ptr一次,引用计数+1；每次调用析构函数,引用计数减一.直到最后一个智能指针删除,才会释放内存.智能指针类将一个计数器与类指向的对象相关联,引用计数跟踪该类有多少个对象共享同一指针.

```cpp
shared_ptr<T> a(new T());  //计数器值为1
shared_ptr<T> b(new T());  //计数器值为1
a = b; //此后 a 原先所指的对象会被销毁,b 所指的对象引用计数加 1

//获得引用计数器的值
cout<<"ptr1 cout :"<<a.use_count()<<endl;// 2
cout<<"ptr1 cout :"<<b.use_count()<<endl;// 2

//常用赋值方法
shared_ptr<T> ptr1(new T()); //
shared_ptr<T> ptr2(ptr1);    // 使用复制构造函数,会让引用计数加 1

//销毁对象a并置nullptr,b计数器减1,b对象不受影响
//销毁对象是指销毁shared_ptr对象a,而不是指向的对象.
//只有当所有指向对象的share_ptr对象删除时,释放指向对象内存
//只有b对象也被销毁,指向的对象才会删除.
a.reset();

//获得传统c指针,不能delete,也不能用get()初始化智能指针
T *p = b.get();

//重置指向对象
b.reset( new T());

//错误使用方法
T *a = new T();
shared_ptr<T> ptr1(a);
shared_ptr<T> ptr2(a);
//ptr1和ptr2的引用计数是单独计算的,任意对象析构时都会销毁a所指对象
//这个对象会被delete两次
```

### 4.2 new[] 分配内存

```cpp
//动态数组
//与unique_ptr不同,shared_ptr不支持直接管理动态数组,需要自己构造删除器
shared_ptr<int> sp(new int[10],[](int *p){delete[] p;});
*(sp.get() + i ) = 5; //shared_ptr未定义下标运算符
```

### 4.3 make_shared

　　std::make_shared 可以返回一个指定类型的 std::shared_ptr.尽量使用make_shared,不要把原生指针暴露出来.不能使用make_shared定义动态数组,因为不能构造删除器

我们在定义shared_ptr智能指针的时候通常有3种方法：

1. 先动态开辟内存,然后用局部变量接受指针.再把指针用于初始化.

   将原生指针暴露出来了,如果在外面的代码中不小心将该指针delete或者初始化其他的智能指针,容易出现错误.

2. 直接在初始化参数中写new表达式.

   直接在用new表达式作为实参,这样原生指针就匿名了.然而当你用new创建一个对象的同时创建一个shared_ptr时,这时会发生两次动态申请内存：一次是给使用new申请的对象本身的,而另一次则是由shared_ptr的构造函数引发的为资源管理对象分配的.

3. 使用make_shared函数.

   C\++编译器只会一次性分配一个足够大的内存,用来保存这个资源管理者和这个新建对象.

```cpp
//方式一
int *p = new int(3);
shared_ptr<int> sp(p);

//方式二
shared_ptr<int> sp(new int(3));

//方式三
shared_ptr<int> sp = make_shared<int>(3);
shared_ptr<string> sp = make_shared<string>("make_shared");
shared_ptr<vector<int> > spv = make_shared<vector<int> >(10, 2);
```

### 4.4 shared_ptr的陷阱(循环引用)

　　我们知道shared_ptr最引以为豪的就是其计数功能,实现了只有当无使用者才会释放掉内存.让我们使用起来管理内存十分方便,然而在使用过程中可能会不经意之间造成内存泄漏而且不容易查找.而这个问题就是:循环引用.

```cpp
class B;
class A
{
public:
    shared_ptr<B> m_b;
};
class B
{
public:
    shared_ptr<A> m_a;
};

int main()
{
    shared_ptr<A> a(new A);  //new出来的A的引用计数此时为1
    shared_ptr<B> b(new B);  //new出来的B的引用计数此时为1
    a->m_b = b;              //B的引用计数增加为2
    b->m_a = a;              //A的引用计数增加为2

//b先出作用域,B的引用计数减少为1,不为0；
//所以堆上的B空间没有被释放,且B持有的A也没有机会被析构,A的引用计数也完全没减少

//a后出作用域,同理A的引用计数减少为1,不为0,所以堆上A的空间也没有被释放
}
```

　　可以看出来以上的代码中A对象中指针引用B对象,B对象指针引用A对象,这样就导致了循环引用的出现.而代码运行到最后,由于两个指针计数都没有到0,所以资源无法释放导致了内存泄漏！

　　举个简单的例子,链表！,一旦尾首相连形成循环链表的时候那么就出现了循环引用,所以使用shared_ptr的时候一定要先判断是否会出现循环引用.

## 五、weak_ptr

　　weak_ptr是一种不控制所指向对象生存期的智能指针,它指向一个shared_ptr管理的对象,却并不拥有该对象.将一个weak_ptr绑定到shared_ptr不会改变shared_ptr的引用计数.一旦最后一个指向对象的shared_ptr被销毁,对象就会被释放,即使有weak_ptr指向对象,对象还是会被释放.

* 必须通过shared_ptr来共享内存.

* 有重载opreator*和->操作符,也就意味着即使分配到对象,他也没法使用该对象

* 不主动参与引用计数,即,share_ptr释放了,那么weak_ptr所存的对象也释放了.

* 使用成员函数use_count()可以查看当前引用计数,expired()判断引用计数是否为空.

* lock()函数,返回一个shared_ptr智能指针

```cpp
//当我们创建一个weak_ptr时,要用一个shared_ptr来初始化它:
auto p=make_shared_ptr<int>(42);

weak_ptr<int> wp(p); //wp弱共享p,p的引用计数未改变
```

```cpp
#include <memeroy>
#include <iostream>

using namespace std;

void Check(weak_ptr<int> & wp)
{
    share_ptr<int> sp = wp.lock();  //转换为share_ptr<int>
    if(sp != nullptr){
        cout << "still" << *sp << endl;
    }
    else{
        cout << "pointer is invalid." << endl;
    }
}

int main()
{
    share_ptr<int> sp1(new int(22));
    share_ptr<int> sp2 = sp1;
    weak_ptr<int> wp = sp1;//指向share_ptr<int> 所指对象

    cout << *sp1 << endl;  //22
    cout << *sp2 << endl;  //22
    Check(wp);             //still 22

    sp1.reset(); //使引用计数器降低为0,shared_ptr释放内存
    cout << *sp2 << endl;  //22
    Check(wp);        //still 22

    sp2.reset();
    Check(wp);        //pointer is invalid.

    return 0;
}
```



