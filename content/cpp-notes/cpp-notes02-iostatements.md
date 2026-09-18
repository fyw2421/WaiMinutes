---
title: "C++Notes02 : I/O Statements"
weight: 2
description: "Stream-States get()/getline() i[o]stringstream fstream seekg/seekp tellg/tellp"
date: 2018-05-22
tags: ["C++"]
featureimage: "covers/cpp-notes02-iostatements.svg"
---
## 一、流状态(Stream states)

### 1.1 流(stream)

　　流是一种抽象概念,它负责在数据的生产者和数据的消费者之间建立联系,并管理数据的流动.

　　C++程序通常在用户按下回车时刷新输入缓冲区

　　C++程序通常在用户发送换行符时刷新输出缓冲区

　　输出流 ostream ofstream ostringstream

　　预先定义的输出流对象

* cout : 标准输出

* cerr ：标准错误输出,没有缓冲,发送给它的内容立即被输出.

* clog : 类似于cerr,但是有缓冲,缓冲满时被输出.

### 1.2 常用的流状态

key                     | output
:---                    | :---
showpos/noshowpos       | 在正数(包括0)之前显示+号
showbase/noshowbase     | 在十六进制整数前加0x,八进制整数前加0
uppercase/nouppercase   | 十六进制格式字母用大写字母表示(默认为小写字母)
showpoint/noshowpoint   | 浮点输出即使小数点后都为0也加小数点
boolalpha/noboolalpha   | 逻辑值(1和0)用true和false表示
left/right              | 左/右对齐互相独立
dec/hex/oct             | 十/十六/八进制显示整数互相独立
fixed/scientific        | 定点格式输出/科学计数法格式输出互相独立

**Sample**

```cpp
#include <iostream>
using namespace std;
int main()
{
    cout<<showpos<<12;                   //output: +12
    cout<<hex<<18<<" "<<showbase<<18;    //output: 12 0x12
    cout<<hex<<255<<" "<<uppercase<<255; //output: ff FF
    cout<<123.0<<" "<<showpoint<<123.0;  //output: 123 123.000
    cout<<(2>3)<<" "<<boolalpha<<(2>3);  //output: 0 false
    cout<<fixed<<12345.678;              //output: 12345.678000
    cout<<scientific<<12345.678;         //output: 1234568e+05
}
```

### 1.3 有参数的三个常用的状态流

function                | output
:---                    | :---
cout.witdh(int);        | Cout调用方式,设置宽度
cout.fill(char);        | Cout调用方式,设置填充字符
cout.precision(int);    | Cout调用方式,设置有效位数
cout<<setw(int)         | 与<<连用,设置宽度,必须#include <iomanip>
cout<<setfill(char)     | 与<<连用,设置填充字符,必须#include <iomanip>
cout<<setprecision(int) | 与<<连用,设置有效位数,必须#include <iomanip>

**Sample01**

```cpp
cout.width(5);
cout.fill('$');
cout<<23<<23;    //output: $$$2323
```

**Sample02**

```cpp
cout<<setw(5)<<setfill('$')<<23;    //output: $$$23
```

**Sample03**

```cpp
//=====================================
// f0209.cpp
// 倒三角形流状态设置版
//=====================================
#include<iostream>
#include<iomanip>
using namespace std;
//-------------------------------------
int main()
{
    for(int n=1; n<=10; ++n)
        cout<<setfill(' ')<<setw(n)<<" " <<setfill('M')<<setw(21-2*n)<<"M"<<endl;
}
//-------------------------------------
```

### 1.4 ostream方法

除了`<<`以外,ostream类还提供了`put()`和`write()`方法.

#### 1.4.1 put

```cpp
//put()
ostream &put(char);

cout.put('w');
cout.put('w').put('a');
cout.put(65); //display A character
cout.put(66.3); //display B character
```

#### 1.4.2 write

```cpp
//write()
basic_ostream<charT,traits> &write(const char_type* s,stream_size n);

const char *state2 = "Kansas";
for( int i = 1; i <= len ; i++){
    cout.write(states2,i);
    cout<<endl;
}
/*
K
Ka
Kan
...
Kansas
*/
```

### 1.5 istream方法

#### 1.5.1 单字符输入get(char)和ch=cin.get()

```cpp
//get(char&)
int ct = 0;
char ch;
cin.get(ch);
while( ch != '\n'){
    cout<<ch;
    ct++;
    cin.get(ch);
}
cout<<endl;
//input&output: I c++ clearly.

//流提取符 >> 从流中提取数据时通常跳过流中的空格 tab键换行符等空白字符
//用cin>>  读取数据时遇到空白字符(包括空格 tab键和回车)作为终止字符.”
cin>>ch;
while(...){
    cin>>ch;
}
//output: Ic++clearly 循环不会终止且跳过空格和换行符.

cin.get(c1).get(c2);

//ch=cin.get();
while( ( ch = cin.get() )!= EOF ){}
```

#### 1.5.2 字符串输入getline()和get()

```cpp
istream &get(char *,int streamsize,char delim); //字符串地址,字符串长度,终止符
istream &get(char *,int streamsize);      //字符串地址,字符串长度
istream &getline( char *,int,char delim); //字符串地址,字符串长度,终止符
istream *getline( char *,int streamsize );  //字符串地址,字符串长度
```

**cin.getline()和cin.get()区别**

* `getline()`函数每次读取一行,通过换行符来确定结尾,但不保存换行符.在存储字符串时,用`\0`来代替换行符.

   ```cpp
   cin.getline(name1,ArSize).getline(name1,ArSize);    //接收连续输入,将第一输入放入name1,将第二输入放入name2
   ```

* `get()`函数每次读取一行,通过换行符来确定结尾,将**换行符保存在输入队列中**.

   ```cpp
   cin.get(name,ArSize);    //换行符被保存在输入队列中
   cin.get(dessert,ArSize); //dessert只接收到换行符.
   ```

   ```cpp
   cin.get(name,ArSize);    //换行符被保存在输入队列中
   cin.get();               //接收换行符
   or
   cin.get(name,ArSize).get();
   or
   char ch;
   cin.get(ch);
   ch = cin.get();

   cin.get(dessert,ArSize); //dessert接收数据.
   ```

### 1.6 内核格式化

　　C++库提供了sstream提供程序和string之间的I/O;读取string对象中的格式化信息或者将格式化的信息写入string对象称为内核格式化

#### 1.6.1 ostringstream

sstream头文件定义了ostringstream,从ostream派生而来.

```cpp
//将格式化的信息写入string对象
// strout.cpp -- incore formatting (output)
#include <iostream>
#include <sstream>
#include <string>
int main()
{
    using namespace std;
    ostringstream outstr;   // manages a string stream

    string hdisk;
    cout << "What's the name of your hard disk? ";
    getline(cin, hdisk);
    int cap;
    cout << "What's its capacity in GB? ";
    cin >> cap;
    // write formatted information to string stream
    outstr << "The hard disk " << hdisk << " has a capacity of "
            << cap << " gigabytes.\n";
    string result = outstr.str();   // save result
    cout << result;                 // show contents

    return 0;
}
/*
What's the name of your hard disk? Datarapture
What's its capacity in GB? 250
The hard disk Datarapture has a capacity of 250 gi
gabytes.
*/
```

#### 1.6.2 istringstream

读取string对象中的格式化信息

**Sample01**

```cpp
// strin.cpp -- formatted reading from a char array
#include <iostream>
#include <sstream>
#include <string>
int main()
{
    using namespace std;
    string lit = "It was a dark and stormy day, and "
                 " the full moon glowed brilliantly. ";
    istringstream instr(lit);   // use buf for input
    string word;;
    while (instr >> word)       // read a word a time
        cout << word << endl;
    return 0;
}
```

**Sample02**

```cpp
//从文件读取数字,并求和
#include<fstream>
#include<sstream>
ifstream in("aaa.txt");
for(string s; getline(in, s); ){
  int a, sum=0;
  for(istringstream sin(s); sin>>a; sum += a);
  cout<<sum<<endl;
}
```

**Sample03**

```cpp
//=====================================
// f0306.cpp
// 整行读入再分解读入
//=====================================
#include<iostream>
#include<sstream>
#include<fstream>
using namespace std;
//-------------------------------------
int main(){
  ifstream in("aaa.txt");
  for(string s; getline(in, s); ){
    int a, sum=0;
    for(istringstream sin(s); sin>>a; sum += a);
    cout<<sum<<endl;
  }
}//====================================
```

## 二、文件流

### 2.1 文本类型

1. 文本文件,其任何内容总是与字符码表(如ASCII码)对应.

2. 二进制文件0/1.

### 2.2 文件打开模式及语句

statement        | detail
:---:            | :---:
ios_base::in     | 打开文件,以便读取
ios_base::out    | 打开文件,以便写入
ios_base::ate    | 打开文件,并移到文件尾
ios_base::app    | 追加到文件尾
ios_base::trun   | 如果文件存在,截短文件
ios_base::binary | 二进制文件

```cpp
#include <fstream>
ifstream fin( filename , openmode = ios_base::in );

ifstream fin;
fin.open( filename , openmode = ios_base::in );
fin.close();

fin.open( filename , ios_base:: out | ios_base::app );

ofstream fout( filename , openmode = ios_base::out );

ofstream fout;
fout.open( filename , openmode = ios_base::out );
fout.close();
```

**Sample追加文件**

```cpp
// append.cpp -- appending information to a file
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>      // (or stdlib.h) for exit()

const char * file = "guests.txt";
int main()
{
    using namespace std;
    char ch;

// show initial contents
    ifstream fin;
    fin.open(file);

    if (fin.is_open())
    {
        cout << "Here are the current contents of the "
             << file << " file:\n";
        while (fin.get(ch))
            cout << ch;
        fin.close();
    }

// add new names
    ofstream fout(file, ios::out | ios::app);
    if (!fout.is_open())
    {
        cerr << "Can't open " << file << " file for output.\n";
        exit(EXIT_FAILURE);
    }

    cout << "Enter guest names (enter a blank line to quit):\n";
    string name;
    while (getline(cin,name) && name.size() > 0)
    {
          fout << name << endl;
    }
    fout.close();

// show revised file
    fin.clear();    // not necessary for some compilers
    fin.open(file);
    if (fin.is_open())
    {
        cout << "Here are the new contents of the "
             << file << " file:\n";
        while (fin.get(ch))
            cout << ch;
        fin.close();
   }
    cout << "Done.\n";
    return 0;
}

```

### 2.3 文件读写

```cpp
ifstream fin;
fin.open("jellyjar.dat");
char ch;
fin>>ch;      //read a character from file
char buf[80];
fin>>buf;     //read a word from file
fin.getline( buf , 80); //read line from filename
string s;
getline( fin , s) //read line from file to s

//从文件读取数字,并求和
#include<fstream>
#include<sstream>
ifstream in("aaa.txt");
for(string s; getline(in, s); ){
  int a, sum=0;
  for(istringstream sin(s); sin>>a; sum += a);
  cout<<sum<<endl;
}
```

**Sample文件读写**

```cpp
//=====================================
// f0211.cpp
// 拷贝文件
//=====================================
#include<fstream>
using namespace std;
//-------------------------------------
int main()
{
    ifstream in("a.in");
    ofstream out("a.out");
    for(string str; getline(in, str); ) //getline从输入文件流中读入一行数据,丢掉回车,放入string中.
      out<<str<<endl;                  //为了原样输出,补上回车.
}
//====================================
```

### 2.4 文件是否打开成功

```cpp
//旧式方法,无法检测出以不合适的文件模式打开导致的错误
fin.open("a.data");
if( fin.fail() ){}
if( !fin ){}
if( fin.good() ){}

//新式
if(fin.is_open())

//打开多个文件
ifstream fin;
fin.open("fat.data");
...
fin.close(); //显式关闭文件,fin消亡时会自动调用.
fin.clear();

fin.open("rat.data");
...
fin.close();
```

### 2.5 二进制文件的读写

```cpp
const int LIM = 20;
struct planet{
    char name[ LIM ];
    double population;
    double g;
};
planet p1;
//二进制文件的读操作
ifstream fin("planets.dat",ios_base::in|ios_base::binary);
fin.read((char*)&p1,sizoef(p1));

//二进制文件的写操作
ofstream fout("planets.data",ios_base::out|ios_base::binary|ios_base::app);
fout.write((char*)&p1,sizoef(p1));
```

**Sample二进制文件的读写Sample**

```cpp
// binary.cpp -- binary file I/O
#include <iostream> // not required by most systems
#include <fstream>
#include <iomanip>
#include <cstdlib>  // (or stdlib.h) for exit()

inline void eatline() { while (std::cin.get() != '\n') continue; }
struct planet
{
    char name[20];      // name of planet
    double population;  // its population
    double g;           // its acceleration of gravity
};

const char * file = "planets.dat";

int main()
{
    using namespace std;
    planet pl;
    cout << fixed << right;

// show initial contents
    ifstream fin;
    fin.open(file, ios_base::in |ios_base::binary);  // binary file
    //NOTE: some systems don't accept the ios_base::binary mode
    if (fin.is_open())
    {
    cout << "Here are the current contents of the "
        << file << " file:\n";
    while (fin.read((char *) &pl, sizeof pl))
    {
        cout << setw(20) << pl.name << ": "
              << setprecision(0) << setw(12) << pl.population
              << setprecision(2) << setw(6) << pl.g << endl;
    }
    fin.close();
    }

// add new data
    ofstream fout(file,
             ios_base::out | ios_base::app | ios_base::binary);
    //NOTE: some systems don't accept the ios::binary mode
    if (!fout.is_open())
    {
        cerr << "Can't open " << file << " file for output:\n";
        exit(EXIT_FAILURE);
    }

    cout << "Enter planet name (enter a blank line to quit):\n";
    cin.get(pl.name, 20);
    while (pl.name[0] != '\0')
    {
        eatline();
        cout << "Enter planetary population: ";
        cin >> pl.population;
        cout << "Enter planet's acceleration of gravity: ";
        cin >> pl.g;
        eatline();
        fout.write((char *) &pl, sizeof pl);
        cout << "Enter planet name (enter a blank line "
                "to quit):\n";
        cin.get(pl.name, 20);
    }
    fout.close();

// show revised file
    fin.clear();    // not required for some implementations, but won't hurt
    fin.open(file, ios_base::in | ios_base::binary);
    if (fin.is_open())
    {
        cout << "Here are the new contents of the "
             << file << " file:\n";
        while (fin.read((char *) &pl, sizeof pl))
        {
            cout << setw(20) << pl.name << ": "
                 << setprecision(0) << setw(12) << pl.population
                 << setprecision(2) << setw(6) << pl.g << endl;
        }
        fin.close();
    }
    cout << "Done.\n";
    return 0;
}

```

### 2.6 随机存取(文件定位)

　　随机存取是直接移动到文件的任何位置.fstream类继承了两个方法`seekg()`和`seekp()`.fstream使用缓冲区来存储中间数据,因而指针指向的是缓冲区的位置,而不是文件的实际位置.

#### 2.6.1 seekg()

将输入流指针移动到指定的文件位置.原型如下

```cpp
//定位到指定位置特定距离
//streamoff是与stream中指定点的相对位置,可以是负数
istream &seekg(streamoff offset,seek_dir origin);

//定位到文件开始位置特定距离
//streampos是在stream中的绝对位置,非负
istream &seekg (streampos pos);

//获得输入指针的当前位置
streampos tellg();
```

seekdir参数是ios_base类中定义的另一种整型,有三个可能的值:

* ios_base::cur : 当前位置

* ios_base::beg : 文件开始位置

* ios_base::end : 文件结束位置

```cpp
fin.seekg(30,ios_base::beg); //30 bytes beyond the beginning.
fin.seekg(-1,ios_base::cur); //back up one bytes
fin.seekg(0,ios_base::end);  //go the end of file
streampos = fin.tellg();
```

#### 2.6.2 seekp()

将输出流指针移动到指定的文件位置.原型如下

```cpp
//定位到指定位置特定距离
//streamoff是与stream中指定点的相对位置,可以是负数
ostream &seekp(streamoff offset,seek_dir origin);

//定位到文件开始位置特定距离
//streampos是在stream中的绝对位置,非负
ostream &seekp(streampos pos);

//获得输入指针的当前位置
streampos tellp();
```

#### 2.6.3 文件随机读取sample

```cpp
// random.cpp -- random access to a binary file
#include <iostream>     // not required by most systems
#include <fstream>
#include <iomanip>
#include <cstdlib>      // (or stdlib.h) for exit()
const int LIM = 20;
struct planet
{
    char name[LIM];      // name of planet
    double population;  // its population
    double g;           // its acceleration of gravity
};

const char * file = "planets.dat";  // ASSUMED TO EXIST (binary.cpp example)
inline void eatline() { while (std::cin.get() != '\n') continue; }

int main()
{
    using namespace std;
    planet pl;
    cout << fixed;

// show initial contents
    fstream finout;     // read and write streams
    finout.open(file,
           ios_base::in | ios_base::out | ios_base::binary);
    //NOTE: Some Unix systems require omitting | ios::binary
    int ct = 0;
    if (finout.is_open())
    {
        finout.seekg(0);    // go to beginning
        cout << "Here are the current contents of the "
             << file << " file:\n";
        while (finout.read((char *) &pl, sizeof pl))
        {
            cout << ct++ << ": " << setw(LIM) << pl.name << ": "
                 << setprecision(0) << setw(12) << pl.population
                 << setprecision(2) << setw(6) << pl.g << endl;
        }
        if (finout.eof())
            finout.clear(); // clear eof flag
        else
        {
            cerr << "Error in reading " << file << ".\n";
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        cerr << file << " could not be opened -- bye.\n";
        exit(EXIT_FAILURE);
    }

// change a record
    cout << "Enter the record number you wish to change: ";
    long rec;
    cin >> rec;
    eatline();              // get rid of newline
    if (rec < 0 || rec >= ct)
    {
        cerr << "Invalid record number -- bye\n";
        exit(EXIT_FAILURE);
    }
    streampos place = rec * sizeof pl;  // convert to streampos type
    finout.seekg(place);    // random access
    if (finout.fail())
    {
        cerr << "Error on attempted seek\n";
        exit(EXIT_FAILURE);
    }

    finout.read((char *) &pl, sizeof pl);
    cout << "Your selection:\n";
    cout << rec << ": " << setw(LIM) << pl.name << ": "
         << setprecision(0) << setw(12) << pl.population
         << setprecision(2) << setw(6) << pl.g << endl;
    if (finout.eof())
        finout.clear();     // clear eof flag

    cout << "Enter planet name: ";
    cin.get(pl.name, LIM);
    eatline();
    cout << "Enter planetary population: ";
    cin >> pl.population;
    cout << "Enter planet's acceleration of gravity: ";
    cin >> pl.g;
    finout.seekp(place);    // go back
    finout.write((char *) &pl, sizeof pl) << flush;
    if (finout.fail())
    {
        cerr << "Error on attempted write\n";
        exit(EXIT_FAILURE);
    }

// show revised file
    ct = 0;
    finout.seekg(0);            // go to beginning of file
    cout << "Here are the new contents of the " << file
         << " file:\n";
    while (finout.read((char *) &pl, sizeof pl))
    {
        cout << ct++ << ": " << setw(LIM) << pl.name << ": "
             << setprecision(0) << setw(12) << pl.population
             << setprecision(2) << setw(6) << pl.g << endl;
    }
    finout.close();
    cout << "Done.\n";
    return 0;
}

```

## 三、素数判定

### 3.1 筛选法

```cpp
//=====================================
// f0212.cpp
// 用筛法判断素数
//=====================================
#include<iostream>
#include<vector>
#include<fstream>
using namespace std;
//-------------------------------------
int main(){
    vector<int> prime(10000,1);
    for(int i=2; i<100; ++i)   // 构造素数集合
      if(prime[i])
        for(int j=i; i*j<10000; ++j)
          prime[i*j]=0;         //i的所有倍数均不是素数,从素数集合中删除.
    ifstream in("a.txt");
    for(int a; in>>a && a>1 && a<10000; )  // 判断素数
      cout<<a<<" is"<<(prime[a] ? "":" not")<<" a prime.\n";
    getchar(); //system("pause");
}
```

### 3.2 素数定义

```cpp
//=====================================
// f0207.cpp
// 按素数定义判断一个整数是否素数
//=====================================
#include<iostream>
using namespace std;
//-------------------------------------
int main(){
    cout<<"please input a number: \n";
    int m;
    cin>>m;
    for(int i=2; i<m; ++i)
      if(m%i==0){
        cout<<m<<" isn't a prime.\n";
        return 1;
      }
    cout<<m<<" is a prime.\n";
}
//====================================
```

### 3.3 微优化版

推导过程:

如果m不是素数,则可以表示成两个因子的积.

m=i*j 假定 i<=j;

则   i² <= i *j <= j²

则   i <= √m <= j

即如果m不是素数,其必有一个因子不大于其平方根.

```cpp
//=====================================
// f0208.cpp
// 判断一个整数是否素数的稍微优化版
//=====================================
#include<iostream>
#include<cmath>   // sqrt()
using namespace std;
//-------------------------------------
int main(){
    cout<<"please input a number: \n";
    int m;
    cin>>m;
    double sqrtm=sqrt(m*1.0);
    for(int i=2; i<=sqrtm; ++i)
      if(m%i==0){
        cout<<m<<" isn't a prime.\n";
        return 1;
      }
    cout<<m<<" is a prime.\n";
}//====================================
```



