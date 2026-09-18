---
title: "C++Notes09 : STL"
weight: 9
description: "containers iterators adapters algorithms functors"
date: 2018-05-22
tags: ["C++"]
featureimage: "covers/cpp-notes09-stl.svg"
---
　　STL可分为容器(containers)、迭代器(iterators)、空间配置器(allocator)、适配器(adapters)、算法(algorithms)、仿函数(functors)六个部分.

## 一、容器(containers)

　　STL容器对最常用的数据结构提供了支持,这些模板的参数允许我们指定容器中元素的数据类型,实现了数据的存储方式,可以将我们许多重复而乏味的工作简化.

　　容器部分主要由头文件`<vector>,<list>,<deque>,<set>,<map>,<stack>和<queue>`组成.

### 1.1 序列式容器

#### 1.1.1 vector(向量)

　　底层是数组,随机存取数据元素,在尾部添加和移除很快,在头部和中部插入耗时

#### 1.1.2 list(列表)

　　底层数据结构是双向链表,不提供随机存取数据元素(需要按顺序走到要存取的元素,在任何位置插入和删除都很快,只需要简单的移动一下指针

```cpp
// list.cpp -- using a list
#include <iostream>
#include <list>
#include <iterator>

int main()
{
    using namespace std;
    list<int> one(5, 2); // list of 5 2s
    int stuff[5] = {1,2,4,8, 6};
    list<int> two;
    two.insert(two.begin(),stuff, stuff + 5 );
    int more[6] = {6, 4, 2, 4, 6, 5};
    list<int> three(two);
    three.insert(three.end(), more, more + 6);

    cout << "List one: ";
    ostream_iterator<int,char> out(cout, " ");
    copy(one.begin(), one.end(), out);
    cout << endl << "List two: ";
    copy(two.begin(), two.end(), out);
    cout << endl << "List three: ";
    copy(three.begin(), three.end(), out);
    three.remove(2);
    cout << endl << "List three minus 2s: ";
    copy(three.begin(), three.end(), out);
    three.splice(three.begin(), one);
    cout << endl << "List three after splice: ";
    copy(three.begin(), three.end(), out);
    cout << endl << "List one: ";
    copy(one.begin(), one.end(), out);
    three.unique();
    cout << endl << "List three after unique: ";
    copy(three.begin(), three.end(), out);
    three.sort();
    three.unique();
    cout << endl << "List three after sort & unique: ";
    copy(three.begin(), three.end(), out);
    two.sort();
    three.merge(two);
    cout << endl << "Sorted two merged into three: ";
    copy(three.begin(), three.end(), out);
    cout << endl;

    return 0;
}
/*
Set A: buffoon can for heavy thinkers
Set B: any deliver elegant food for metal
Union of A and B:
any buffoon can deliver elegant food for heavy met
al thinkers
Intersection of A and B:
for
Difference of A and B:
buffoon can heavy thinkers
Set C:
any buffoon can deliver elegant food for heavy met
al thinkers
Set C after insertion:
any buffoon can deliver elegant food for grungy he
avy metal thinkers
Showing a range:
grungy heavy metal
*/
```

#### 1.1.3 deque(双端队列)

　　底层数据结构是数组,可以随机存取数据元素,在数组的头部和尾部插入和删除元素很快

### 1.2 关联容器

　　元素位置取决于特定的排序准则,和插入的顺序无关,底层数据结构为二叉树

#### 1.2.1 set(集合)

内部元素依据其值自动排序,set内相同的数值元素只能出现一次

```cpp
// setops.cpp -- some set operations
#include <iostream>
#include <string>
#include <set>
#include <algorithm>
#include <iterator>

int main()
{
    using namespace std;
    const int N = 6;
    string s1[N] = {"buffoon", "thinkers", "for", "heavy", "can", "for"};
    string s2[N] = {"metal", "any", "food", "elegant", "deliver","for"};

    set<string> A(s1, s1 + N);
    set<string> B(s2, s2 + N);

    ostream_iterator<string, char> out(cout, " ");
    cout << "Set A: ";
    copy(A.begin(), A.end(), out);
    cout << endl;
    cout << "Set B: ";
    copy(B.begin(), B.end(), out);
    cout << endl;

    cout << "Union of A and B:\n";
    set_union(A.begin(), A.end(), B.begin(), B.end(), out);
    cout << endl;

    cout << "Intersection of A and B:\n";
    set_intersection(A.begin(), A.end(), B.begin(), B.end(), out);
    cout << endl;

    cout << "Difference of A and B:\n";
    set_difference(A.begin(), A.end(), B.begin(), B.end(), out);
    cout << endl;

    set<string> C;
    cout << "Set C:\n";
    set_union(A.begin(), A.end(), B.begin(), B.end(),
        insert_iterator<set<string> >(C, C.begin()));
    copy(C.begin(), C.end(), out);
    cout << endl;

    string s3("grungy");
    C.insert(s3);
    cout << "Set C after insertion:\n";
    copy(C.begin(), C.end(),out);
    cout << endl;

    cout << "Showing a range:\n";
    copy(C.lower_bound("ghost"),C.upper_bound("spook"), out);
    cout << endl;

    return 0;
}
/*
Set A: buffoon can for heavy thinkers
Set B: any deliver elegant food for metal
Union of A and B:
any buffoon can deliver elegant food for heavy met
al thinkers
Intersection of A and B:
for
Difference of A and B:
buffoon can heavy thinkers
Set C:
any buffoon can deliver elegant food for heavy met
al thinkers
Set C after insertion:
any buffoon can deliver elegant food for grungy he
avy metal thinkers
Showing a range:
grungy heavy metal
*/
```

#### 1.2.2 multiset(多重集合)

　　内部元素依据其值自动排序,set内允许出现重复的元素

#### 1.2.3 map(映射)

　　map的元素是成对的键值对,内部元素的值依据键自动排序,键只允许出现一次

#### 1.2.4 multimap(多重映射)

　　多重映射是map的增强版,允许键出现多次

　　STL使用`pair<class T,class U>`模板类存储到映射中,`pair<const keytype,datatype>`

```cpp
//为multimap对象插入内容
multimap<int,string> codes;
pair<const int,string> item(213,"Los");
codes.insert(item);

codes.insert(pair<const int,string> (213,"Los"));

//访问pair的元素
cout<<item.first<<ite.second;
```

**sample**

```cpp
// multmap.cpp -- use a multimap
#include <iostream>
#include <string>
#include <map>
#include <algorithm>

typedef int KeyType;
typedef std::pair<const KeyType, std::string> Pair;
typedef std::multimap<KeyType, std::string> MapCode;

int main()
{
    using namespace std;
    MapCode codes;

    codes.insert(Pair(415, "San Francisco"));
    codes.insert(Pair(510, "Oakland"));
    codes.insert(Pair(718, "Brooklyn"));
    codes.insert(Pair(718, "Staten Island"));
    codes.insert(Pair(415, "San Rafael"));
    codes.insert(Pair(510, "Berkeley"));

    cout << "Number of cities with area code 415: "
         << codes.count(415) << endl;
    cout << "Number of cities with area code 718: "
         << codes.count(718) << endl;
    cout << "Number of cities with area code 510: "
         << codes.count(510) << endl;
    cout << "Area Code   City\n";
    MapCode::iterator it;
    for (it = codes.begin(); it != codes.end(); ++it)
        cout << "    " << (*it).first << "     "
            << (*it).second    << endl;

    pair<MapCode::iterator, MapCode::iterator> range
         = codes.equal_range(718); //返回该关键字的迭代器
    cout << "Cities with area code 718:\n";
    for (it = range.first; it != range.second; ++it)
        cout <<  (*it).second    << endl;

    return 0;
}
/*
Number of cities with area code 415: 2
Number of cities with area code 718: 2
Number of cities with area code 510: 2
Area Code   City
    415     San Francisco
    415     San Rafael
    510     Oakland
    510     Berkeley
    718     Brooklyn
    718     Staten Island
Cities with area code 718:
Brooklyn
Staten Island
*/
```

## 二、适配器(adapters)

　　适配器是标准库中通用的概念,包括容器适配器、迭代器适配器和函数适配器.

### 2.1 容器适配器

　　并没有提供与元素的保存形式有关的真正数据结构实现,并且适配器不支持迭代器.适配器对容器进行包装,使其表现出另外一种行为

#### 2.1.1 stack(栈)

　　栈可以使用序列式容器中的vector,deque,list中的任意一种作为其底层的数据结构.默认是使用deque来实现的stack.

#### 2.1.2 queue(队列)

队列可以使用deque和list中的任意一种作为其底层的数据结构.默认是使用deque来实现queue.

#### 2.1.3 priority_queue(优先队列)

　　优先队列也是一种队列,不过在进入队列之后会对元素进行排序,可以使用vector和deque来实现其底层结构,默认是使用vector来实现priority_queue.

### 2.2 迭代适配器

　　STL提供了许多基于迭代器的适配器,如back_insert_iterator,front_insert_iterator, inser_iterator, reverse_iterator, istream_iterator, ostream_iterator, istreambuf_iterator, ostreambuf_iterator等.

> https://blog.csdn.net/effective_coder/article/details/8733853

```cpp
// copyit.cpp -- copy() and iterators
#include <iostream>
#include <iterator>
#include <vector>

int main()
{
    using namespace std;

    int casts[10] = {6, 7, 2, 9 ,4 , 11, 8, 7, 10, 5};
    vector<int> dice(10);
    // copy from array to vector
    copy(casts, casts + 10, dice.begin());
    cout << "Let the dice be cast!\n";
    // create an ostream iterator
    ostream_iterator<int, char> out_iter(cout, " ");
    // copy from vector to output
    copy(dice.begin(), dice.end(), out_iter);
    cout << endl;
    cout <<"Implicit use of reverse iterator.\n";
    copy(dice.rbegin(), dice.rend(), out_iter);
    cout << endl;
    cout <<"Explicit use of reverse iterator.\n";
    vector<int>::reverse_iterator ri;
    for (ri = dice.rbegin(); ri != dice.rend(); ++ri)
        cout << *ri << ' ';
    cout << endl;

    return 0;
}
/*
Let the dice be cast!
6 7 2 9 4 11 8 7 10 5
Implicit use of reverse iterator.
5 10 7 8 11 4 9 2 7 6
Explicit use of reverse iterator.
5 10 7 8 11 4 9 2 7 6
*/
```

### 2.3 函数适配器

　　函数适配器是用来让一个函数对象表现出另外一种类型的函数对象的特征.因为,许多情况下,我们所持有的函数对象或普通函数的参数个数或是返回值类型并不是我们想要的,这时候就需要函数适配器来为我们的函数进行适配

1. 一元函数一个参数

2. 二元函数 两个参数

3. 一元谓词 一个参数,返回类型为bool型

4. 二元谓词 两个参数,返回类型为bool型

expression|effect
:---:|:---:
`bind1st(op,value)` | op(value,param)
`bind2nd(op,value)` | op(param,value)
`not_1(op)`         | !op(param)
`not2(op)`          | !op(param1,param2)

#### 2.3.1 绑定器

　　该类适配器用于将二元函数适配成一元函数

　　将二元函数的一个参数绑定到一个特定的值上,将二元函数对象转换成一元函数对象.

　　[绑定器适配器](https://blog.csdn.net/simahao/article/details/405455)有两种:

* bind1st将给定值绑定到二元函数对象的第一个实参

* bind2nd将给定值绑定到二元函数对象的第二个实参

　　函数绑定器是函数对象,它能结合函数对象、于特定值或函数在一起使用.它们也是被定义在`<functional>`.

```cpp
find_if (coll.begin(),coll.end(), bind2nd (greater<int>(),42))`
//这个例子bind2nd把greater<int>()于42结合起来用来检查大于42的值.
```


```cpp
int a[] = {1, 2, 100, 200};

std::vector< int> arr(a, a + 4);

// 移除所有小于100的元素,这里的比较表达式相当于arr.value < 100
arr.erase( std::remove_if( arr.begin(),  arr.end(),
    std::bind2nd( std::less< int>(), 100)), arr.end());

//移除所有大于100的元素,这里的表达式相当于100 < arr.value

arr.erase( std::remove_if( arr.begin(),  arr.end(),
    std::bind1st( std::less< int>(), 100)), arr.end());
```

#### 2.3.2 取反器

　　将函数对象的结果真值求反

　　取反器有两种:not1和not2

* not1是对一元函数对象求反的取反器,传递给函数对象的只有一个参数,则要使用这个not1

* not2是对二元函数对象求反的取反器

```cpp
int* where=find_if(&array[0],&array[100],not1(bind2nd(greater<int>(),200)))
```

## 三、迭代器(iterators)

迭代器提供对一个容器中的对象的访问方法.迭代器共分为五种:

* 输入迭代器(Input iterator):提供只读操作,可读取其指向元素的值,但
不可以改变该元素的值(还包括trivial迭代器中的所有操作(可能是用继承)

* 输出迭代器(Output Iterator):提供只写操作,可改变

* 前进迭代器(Forward Iterator):可以改变所指向元素的值,但不可以读取

* 双向迭代器(Bidirectional Iterator):可以双向移动

* 随机访问迭代器(Random Iterator):韩乾各种指针算数能力,可以像访问顺序表一样随机访问

```cpp
for(vector::iterator it = v.begin() ; it != v.end(); it++)
  count<<*it<<endl;
```

## 四、算法(algorithms)

　　STL算法部分主要由头文件`<algorithm>,<numeric>,<functional>`组成.要使用 STL中的算法函数必须包括头文件`<algorithm>`,对于数值算法须包括`<numeric>`,`<functional>`中则定义了一些模板类,用来声明函数对象.

　　[STL中算法大致分为四类](https://blog.csdn.net/tick_tock97/article/details/71316372)

### 4.1 非可变序列算法

指不直接改动其所操作的容器内容的算法.

```cpp
//for_each()
for( vector<Review>::iterator pr = books.begin(); pr != books.end(); pr++ )
  showReview(*pr);

//等价于
for_each(books.begin(),books.end(); showReview);

//find(first,end,value)
vector<int>::iterator result=find(books.begin(),books.end(),2);
```

name|detail
:---:|:---:
adjacent_find  | 查找两个相邻(Adjacent)的等价(Identical)元素
all_of (C\++11) | 检测在给定范围中是否所有元素都满足给定的条件
any_of (C\++11) | 检测在给定范围中是否存在元素满足给定条件
count          | 返回值等价于给定值的元素的个数
count_if       | 返回值满足给定条件的元素的个数
equal          | 返回两个范围是否相等
find           | 返回第一个值等价于给定值的元素
find_end       | 查找范围 A 中与范围 B 等价的子范围最后出现的位置
find_first_of  | 查找范围 A 中第一个与范围 B 中任一元素等价的元素的位置
find_if        | 返回第一个值满足给定条件的元素
find_if_not(C\++11) | 返回第一个值不满足给定条件的元素
for_each       | 对范围中的每个元素调用指定函数
mismatch       | 返回两个范围中第一个元素不等价的位置
none_of(C\++11) | 检测在给定范围中是否不存在元素满足给定的条件
search         | 在范围 A 中查找第一个与范围 B 等价的子范围的位置
search_n       |   在给定范围中查找第一个连续 n 个元素都等价于给定值的子范围的位置

### 4.2 可变序列算法

指能够改动它们所操作的容器内容的算法.

name|detail
:---:|:---:
copy            | 将一个范围中的元素拷贝到新的位置处
copy_backward   | 将一个范围中的元素按逆序拷贝到新的位置处
copy_if (C\++11) | 将一个范围中满足给定条件的元素拷贝到新的位置处
copy_n (C\++11)  | 拷贝 n 个元素到新的位置处
fill            | 将一个范围的元素赋值为给定值
fill_n          | 将某个位置开始的 n 个元素赋值为给定值
generate        | 将一个函数的执行结果保存到指定范围的元素中,用于批量赋值范围中的元素
generate_n      | 将一个函数的执行结果保存到指定位置开始的 n 个元素中
iter_swap       | 交换两个迭代器(Iterator)指向的元素
move (C\++11)    | 将一个范围中的元素移动到新的位置处
move_backward (C\++11) | 将一个范围中的元素按逆序移动到新的位置处
random_shuffle  | 随机打乱指定范围中的元素的位置
remove          | 将一个范围中值等价于给定值的元素删除
remove_if       | 将一个范围中值满足给定条件的元素删除
remove_copy     | 拷贝一个范围的元素,将其中值等价于给定值的元素删除
remove_copy_if  | 拷贝一个范围的元素,将其中值满足给定条件的元素删除
replace         | 将一个范围中值等价于给定值的元素赋值为新的值
replace_copy    | 拷贝一个范围的元素,将其中值等价于给定值的元素赋值为新的值
replace_copy_if | 拷贝一个范围的元素,将其中值满足给定条件的元素赋值为新的值
replace_if      | 将一个范围中值满足给定条件的元素赋值为新的值
reverse         | 反转排序指定范围中的元素
reverse_copy    | 拷贝指定范围的反转排序结果
rotate          | 循环移动指定范围中的元素
rotate_copy     | 拷贝指定范围的循环移动结果
shuffle (C\++11) | 用指定的随机数引擎随机打乱指定范围中的元素的位置
swap            | 交换两个对象的值
swap_ranges     | 交换两个范围的元素
transform       | 对指定范围中的每个元素调用某个函数以改变元素的值
unique          | 删除指定范围中的所有连续重复元素,仅仅留下每组等值元素中的第一个元素.
unique_copy     | 拷贝指定范围的唯一化(参考上述的 unique)结果

```cpp
//random_shuffle()接受两个指定区间的迭代器,并随机排列区间中的元素
random_shuffle(books.begin(),books.end());

//copy()复制元素
copy(v.begin(),v.end(),l.begin()) //将v的元素复制到l中

//transform() //元素按照某种方式变换
int square(int x){
return x*x;
}
transform(v.begin(),v.end(),l.begin(),square);

```

### 4.3 排序算法

包含对序列进行排序和合并的算法、搜索算法以及有序序列上的集合操作.

name|detail
:---:|:---:
is_sorted (C\++11) | 检测指定范围是否已排序
is_sorted_until (C\++11) | 返回最大已排序子范围
nth_element       | 部份排序指定范围中的元素,使得范围按给定位置处的元素划分
partial_sort      | 部份排序
partial_sort_copy | 拷贝部分排序的结果
sort              | 排序(快速排序)
stable_sort       | 稳定排序

```cpp

//sort()默认使用<操作符对值进行比较,默认升序,可以重载<操作符
sort(books.begin(),books.end()); //按照升序排列

//重载<操作符
bool operator<(const Review &r1,const Review &v2){
  return (r1.title <= r2.title);
}

//使用函数指针的返回值
sort(books.begin(),books.end(),worthThan)
bool worthThan(const Review &r1,const Review &v2){
  return (r1.title <= r2.title);
}
```

### 4.4 数值算法

对容器内容进行数值计算.

name|detail
:---:|:---:
is_permutation (C\++11) | 判断一个序列是否是另一个序列的一种排序
max                    | 返回两个元素中值最大的元素
max_element            | 返回给定范围中值最大的元素
min                    | 返回两个元素中值最小的元素
min_element            | 返回给定范围中值最小的元素
minmax (C\++11)         | 返回两个元素中值最大及最小的元素
minmax_element (C\++11) | 返回给定范围中值最大及最小的元素


### 4.5 sample

```cpp
//usealgo.cpp -- using several STL elements
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <map>
#include <iterator>
#include <algorithm>
#include <cctype>
using namespace std;

char toLower(char ch) { return tolower(ch); }
string & ToLower(string & st);
void display(const string & s);

int main()
{
    vector<string> words;
    cout << "Enter words (enter quit to quit):\n";
    string input;
    while (cin >> input && input != "quit")
        words.push_back(input);

    cout << "You entered the following words:\n";
    for_each(words.begin(), words.end(), display);
    cout << endl;

    // place words in set, converting to lowercase
    set<string> wordset;
    transform(words.begin(), words.end(),
        insert_iterator<set<string> > (wordset, wordset.begin()),
        ToLower);
    cout << "\nAlphabetic list of words:\n";
    for_each(wordset.begin(), wordset.end(), display);
    cout << endl;

    // place word and frequency in map
    map<string, int> wordmap;
    set<string>::iterator si;
    for (si = wordset.begin(); si != wordset.end(); si++)
        wordmap[*si] = count(words.begin(), words.end(), *si);

    // display map contents
    cout << "\nWord frequency:\n";
    for (si = wordset.begin(); si != wordset.end(); si++)
        cout << *si << ": " << wordmap[*si] << endl;

    return 0;
}

string & ToLower(string & st)
{
    transform(st.begin(), st.end(), st.begin(), toLower);
    return st;
}

void display(const string & s)
{
     cout << s << " ";
}
/*
Enter words (enter quit to quit):
The dog saw the cat and thought the cat fat
the cat thought the cat perfect
quit
You entered the following words:
The dog saw the cat and thought the cat fat the ca
t thought the cat perfect

Alphabetic list of words:
and cat dog fat perfect saw the thought

Word frequency:
and: 1
cat: 4
dog: 1
fat: 1
perfect: 1
saw: 1
the: 5
thought: 2
*/
```

## 五、仿函数(functors)

　　仿函数(functor)就是使一个类的使用看上去象一个函数.其实现就是类中实现一个`operator()`,这个类就有了类似函数的行为.

仿函数的优势:

1. 仿函数比一般函数灵活.

2. 仿函数有类型识别.可以用作模板参数.

3. 执行速度上仿函数比函数和指针要更快.


### 5.1 预定义仿函数

operator|functor
:---:|:---:
`+`  | plus
`-`  | minus
`*`  | multiplies
`/`  | divides
`%`  | modulus
`-`  | negate
`==` | equal_to
`!=` | not_equal_to
`>`  | greater
`<`  | less
`>=` | greater_equal
`<=` | less_equal
`&&` | logical_and
`或` | logical_or
`!`  | logical_not

```cpp
// funadap.cpp -- using function adapters
#include <iostream>
#include <vector>
#include <iterator>
#include <algorithm>
#include <functional>

void Show(double);
const int LIM = 5;
int main()
{
    using namespace std;
    double arr1[LIM] = {36, 39, 42, 45, 48};
    double arr2[LIM] = {25, 27, 29, 31, 33};
    vector<double> gr8(arr1, arr1 + LIM);
    vector<double> m8(arr2, arr2 + LIM);
    cout.setf(ios_base::fixed);
    cout.precision(1);
    cout << "gr8:\t";
    for_each(gr8.begin(), gr8.end(), Show);
    cout << endl;
    cout << "m8: \t";
    for_each(m8.begin(), m8.end(), Show);
    cout << endl;

    vector<double> sum(LIM);
    transform(gr8.begin(), gr8.end(), m8.begin(), sum.begin(),
              plus<double>());
    cout << "sum:\t";
    for_each(sum.begin(), sum.end(), Show);
    cout << endl;

    vector<double> prod(LIM);
    transform(gr8.begin(), gr8.end(), prod.begin(),
              bind1st(multiplies<double>(), 2.5));
    cout << "prod:\t";
    for_each(prod.begin(), prod.end(), Show);
    cout << endl;

    return 0;
}

void Show(double v)
{
    std::cout.width(6);
    std::cout << v << ' ';
}
/*
gr8:      36.0   39.0   42.0   45.0   48.0
m8:       25.0   27.0   29.0   31.0   33.0
sum:      61.0   66.0   71.0   76.0   81.0
prod:     90.0   97.5  105.0  112.5  120.0
*/
```

### 5.2 自定义仿函数

```cpp
// functor.cpp --- using a functor
#include <iostream>
#include <list>
#include <iterator>

template<class T>  // functor class defines operator()()
class TooBig
{
private:
    T cutoff;
public:
    TooBig(const T & t) : cutoff(t) {}
    bool operator()(const T & v) { return v > cutoff; }
};

int main()
{
    using std::list;
    using std::cout;
    using std::endl;

    TooBig<int> f100(100); // limit = 100
    list<int> yadayada;
    list<int> etcetera;
    int vals[10] = {50, 100, 90, 180, 60, 210, 415, 88, 188, 201};

    yadayada.insert(yadayada.begin(), vals, vals + 10);
    etcetera.insert(etcetera.begin(), vals, vals + 10);
    std::ostream_iterator<int, char> out(cout, " ");
    cout << "Original lists:\n";
    copy(yadayada.begin(), yadayada.end(), out);
    cout << endl;
    copy(etcetera.begin(), etcetera.end(), out);
    cout << endl;
    yadayada.remove_if(f100);               // use a named function object
    etcetera.remove_if(TooBig<int>(200));   // construct a function object
    cout <<"Trimmed lists:\n";
    copy(yadayada.begin(), yadayada.end(), out);
    cout << endl;
    copy(etcetera.begin(), etcetera.end(), out);
    cout << endl;

    return 0;
}
/*
Original lists:
50 100 90 180 60 210 415 88 188 201
50 100 90 180 60 210 415 88 188 201
Trimmed lists:
50 100 90 60 88
50 100 90 180 60 88 188
*/
```



