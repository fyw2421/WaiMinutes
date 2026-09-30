---
title: "JavaScriptNotes02 : 基本数据类型及运算符"
weight: 3
description: "注释 变量 数据类型 类型转换 运算符 深浅拷贝"
date: 2026-09-29
tags: ["JavaScript"]
featureimage: "covers/javascript-notes02-data-types-operators.svg"
---

# 一、注释

*   **单行注释**

    ```javascript
    //单行注释
    ```

*   **多行注释**

    ```javascript
    /*
    多行注释
    */
    ```

# 二、输入输出语句

```javascript
alert(msg)          //浏览器弹出警示框
console.log(msg)    //浏览器控制台打印输出信息
prompt(info)        //浏览器弹出输入框,用户可以输入
```

```javascript
var age = parseInt(prompt('请输入年龄'));
if(age > 18){
  alert('你是成年人');
}
else{
  console.log('你不是从成年人');
}
```

# 三、变量和字面量

*   **字面量**

    不可改变的值。比如：1,2,3，…，“a”,“b”,“c”…，字面量是可以直接使用的

*   **变量**

    值可以任意改变。在JS中通常使用var(variable)关键字来声明一个变量,使用该关键字声明变量后，计算机会自动为变量分配内存空间。

    ```javascript
    var age = 18; //声明变量同时赋值
    console.log(age); //控制端输出18
    ```

# 四、标识符

在JS中所有可由我们自主命名的都可以称为标识符,例如：**变量名**，**函数名**，**属性名**

命名要遵循如下规则：

1.  可以含有字母，数字，下划线，\$,如\:usrAge,num01,\_\_name
2.  严格区分大小写。 var app; 和 var App; 是两个变量
3.  标识符不能以数字开头
4.  标识符不能是ES中的关键字或保留字 例如：`var var = 123 （错误）`
5.  标识符一般采用驼峰命名法，除首字母外每个单词首字母大写，其余字母小写 例如：xxYyZz

*JS底层保存标识符实际上都是采用Unicde编码，所以理论上讲，所有utf-8都可以作为标识符。（例如： var 锄禾日当午 = 789；但不建议，推荐使用英文*

# 五、数据类型

## (一) 分类

简单类型又叫做基本数据类型或者值类型，复杂类型又叫做引用类型。

### 1. 基本数据类型

简单数据类型/基本数据类型，在存储时变量中存储的是值本身，因此叫做值类型
string ，number，boolean，undefined，null

### 2. 引用类型

复杂数据类型，在存储时变量中存储的仅仅是地址（引用），因此叫做引用数据类型
通过 new 关键字创建的对象（系统对象、自定义对象），如 Object、Array、Date等

## (二) 堆和栈

### 1. 栈

由操作系统自动分配释放存放函数的参数值、局部变量的值等。其操作方式类似于数据结构中的栈

**简单数据类型存放到栈里面,如**string ，number，boolean，undefined，null

### 2. 堆

存储复杂类型(对象)，一般由程序员分配释放，若程序员不释放，由垃圾回收机制回收.

**复杂数据类型存放到堆里面**

*   通过 new 关键字创建的对象（系统对象、自定义对象），如 Object、Array、Date等

*   引用类型变量（栈空间）里存放的是地址，真正的对象实例存放在堆空间中

## (三) 基本数据类型

JS数据类型是指字面量的类型，一共有6中数据类型：

### 1. string 字符串

   string字符串需要用引号引起来，使用双引号或单引号都可以，但不能引号的嵌套使用.一般在htmll里使用双引号,js里使用单引号.

*   字符串嵌套

    ```javascript
    //JS可以用 单引号嵌套双引号，或者用 双引号嵌套单引号（外双内单，外单内双）
    var strMsg = '我是一个"字符串"';
    var strMsg = "我是一个'字符串'";
    ```

*   转移字符

    转义符都是 \ 开头的,常用的转义符及其说明如下:

|  转义符 |       说明      |
| :--: | :-----------: |
|  \n  | 换行符,n是newline |
| \ \\ |      斜杠\\     |
|  \ ’ |     ’ 单引号     |
| \ ‘’ |      双引号      |
|  \ t |     tab 缩进    |
|  \ b | 空格，b是blank的意思 |

### 2. Number 数字

*     数字型，包含整型值和浮点型值，如21，0.21

```javascript
console.log(Number.MAX_VALUE); //JS中表示的数字最大值,1.7976931348623157e+308 
console.log(Number.MIN_VALUE); //JS中表示的最小正数值,5e-324
console.log(Infinity); //无穷大
console.log(-Infinity); //无穷小
console.log(NaN); //Not a Number,代表任意非数值,不和任何值相等
console.log(isNan(x)); //判断非数字,如果是数字返回的是false，如果不是数字返回的是true
```

### 3. Boolean 布尔值

   布尔值类型，如true，false ，等价于1和0

### 4. Null 空值

```javascript
var a = null; //声明了变量a为空值
```

### 5. Undefined 未定义

```javascript
var a; //声明了变量a但是没有赋值，此时a=undefined
```

## (四) typeof

   可用来检测变量的数据类型

```javascript
console.log(typeof 'a'); // 输出 "string"
console.log(typeof 123); // 输出 "number"
console.log(typeof true); // 输出 "boolean"
console.log(typeof undefined); // 输出 "undefined"
console.log(typeof null); // 输出 "object"
```

## (五) 数据类型转换

我们通常会实现3种方式的转换：

### 1. 转换为字符串型

*   `toString()`

*   `String()` 强制转换

*   加号拼接字符串(隐式转换)

    ```javascript
    var num = 1;
    console.log(num.toString());
    console.log(String(num));
    console.log(num + '转换为在字符');
    ```

### 2. 转换为数字型

*   `parseInt(string)` 将string转换为整型
*   `parseFloat(string)` 将string转换为浮点型
*   `Number(string)` 强制转换
*   **算术运算符隐式转换**

    ```javascript
    var age = '12';
    console.log(parseInt(age)); //输出12

    var height = '1.75';
    console.log(parseFloat(height)); //输出1.75

    var numStr = '123';
    console.log(Number(numStr)); // 输出 123

    var numStr = '123abc';
    console.log(Number(numStr)); // 输出 NaN

    console.log('12' - 0); // 12
    console.log('123' * 1); // 123
    console.log('123' - '120'); // 3
    ```

### 3. 转换为布尔型

*   **Boolean() 强制转换**

    否定的值会被转换为false，如 ’ ’ , 0, NaN , null , undefined

    其余的值都会被被转换为true

    ```javascript
    console.log(Boolean('')); // false
    console.log(Boolean(NaN)); // false
    console.log(Boolean(0)); // false
    console.log(Boolean(null)); // false
    console.log(Boolean(undefined)); // false
    console.log(Boolean(false)); // false
    console.log(Boolean('false')); // true
    console.log(Boolean('0')); // true
    ```

# 六、运算符

JavaScript 中常用的运算符有：

*   算数运算符
*   递增和递减运算符
*   比较运算符
*   逻辑运算符
*    赋值运算符
*   条件运算符

## (一) 算术运算符

*   \+ 加
*   \- 减
*   \* 乘
*   / 除
*   % 模
*      浮点数的精度问题

    浮点数值的最高精度是17位小数，但在进行算数计算时其精确度远远不如整数

    ```javascript
    var result = 0.1 + 0.2;
    console.log(result); // 输出 0.30000000000000004
    console.log(0.07*100); //7.000000000000001
    console.log(result === 0.3); // 输出 false
    console.log(Math.abs(result - 0.3) < 0.00000000000001);
    ```

## (二) 递增和递减运算符

```javascript
var i = 1;
console.log(i++ + 1); //2,后置递增运算符,先返回原值，后自加
console.log(++i + 1); //4,前置递增运算符,先自加，后返回值
```

## (三) 比较(关系)运算符

*   <

*   \>

*   <=

*   \>=

*   !=

*   \==

    判断两边值是否相等(注意此时有隐士转换)

*   \===

    判断两边的值和数据类型是否完全相同

*   !==

    ```javascript
    var a = 1;
    var b = '1';
    var c = Boolean(true);
    console.log(a == b); // 输出 true
    console.log(a === b); // 输出 false
    console.log(a != c); // 输出 false
    console.log(a !== c); // 输出 true
    console.log(NaN == NaN); //NaN不和任何值相等
    ```

## (四) 逻辑运算符

*   && 与

    两边都是 true才返回 true，否则返回 false

*   || 或

    两边都为 false 才返回 false，否则都为true

*   ! 非

    也叫作取反符，用来取一个布尔值相反的值，如 true 的相反值是 false

### 1. 短路运算

当有多个表达式（值）时,左边的表达式值可以确定结果时,就不再继续运算右边的表达式的值

#### (1) 与

   语法：表达式1 && 表达式2

*   如果第一个表达式的值为真，则返回表达式2

*   如果第一个表达式的值为假，则返回表达式1

    ```javascript
    var res = 2 > 1 && 3 > 1;
    console.log(res); //true
    console.log(123 && 456); //456
    console.log(0 && 456); //0
    console.log(123 && 456 && 789); //789
    ```

#### (2) 或

   语法：表达式1 || 表达式2

*   如果第一个表达式的值为真，则返回表达式1

*   如果第一个表达式的值为假，则返回表达式2

    ```javascript
    var res = 2 > 1 || 3 > 1;
    console.log(res); //true
    console.log(123 || 456); //123
    console.log(0 || 456); //456
    console.log(123 || 456 || 789); //123
    ```

## (五) 赋值运算符

*   \=
*   \+=
*   \-=
*   \*=
*   /=
*   %=

## (六) 条件运算符

条件运算符又称为三元运算符，可以嵌套使用

```javascript
语法格式：条件表达式？语句1：语句2；
```

步骤：首先对条件表达式进行求值，如果该值为true，则执行语句1，并返回结果，如果该值为fasle，则执行语句2，并返回结果

如果条件表达式的求值结果是一个非布尔值，则会将其转换为布尔值然后再运算

```javascript
var a = 10;
var b = 20;
var c = 30;

var result = " " ? a : b;
console.log(result); // 输出 10
console.log(""? a : b); // 输出 20
console.log(""? a : b ? c : 0); // 输出 30
```

## (七) 运算符优先级

1.  小括号 ()
2.  一元运算符 ++--!
3.  算术运算符 \*/+-
4.  关系运算符 > < >= <=
5.  相等运算符 ==、===、!==
6.  逻辑运算符 && || !
7.  赋值运算符 =
8.  逗号

# 七、深拷贝与浅拷贝

## (一) 定义

浅拷贝是指**将原对象或原数组的引用直接赋给新对象，新数组，新对象／数组只是原对象的一个引用**

深拷贝是指\*\* 创建一个新的对象和数组，将原对象的各项属性的“值”（数组的所有元素）拷贝过来，是“值”而不是“引用”\*\*

`Object.assign(target,....sources)` ES6新增方法可以浅拷贝
