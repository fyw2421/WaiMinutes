---
title: "JavaScriptNotes12 : 正则表达式"
weight: 13
description: "创建 检测 边界符 字符类 量词 预定义类 替换"
date: 2026-09-29
tags: ["JavaScript"]
featureimage: "covers/javascript-notes12-regex.svg"
---

# 一、简介

正则表达式是用于匹配字符串中字符组合的模式。在JavaScript中，正则表达式也是对象。

正则表通常被用来检索、替换那些符合某个模式（规则）的文本，例如验证表单：用户名表单只能输入英文字母、数字或者下划线， 昵称输入框中可以输入中文(匹配)。此外，正则表达式还常用于过滤掉页面内容中的一些敏感词(替换)，或从字符串中获取我们想要的特定部分(提取)等 。

# 二、创建

在JavaScript中，可以通过两种方式创建正则表达式

*   通过调用 RegExp 对象的构造函数创建

*   通过字面量创建

## (一) 调用 RegExp 对象的构造函数

```javascript
var reg = new RegExp(/expression/);
```

## (二) 通过字面量创建

```javascript
var reg = /exression/;
```

## (三) 检测正则表达式

test()正则对象方法，用于检测字符串是否符合该规则，该对象会返回true或false,其参数是测试字符串

```javascript
//reg是正则表达式实例对象
//str是测试的文本
reg.test(str);
```

```javascript
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <script>
      //利用RegExp对象来创建正则表达式
      var regExp = new RegExp(/123/);
      console.log(regExp);

      //利用字面量来创建正则表达式
      var reg = /123/;
      console.log(reg);

      //测试一个字符串是否符合正则表达式
      console.log(regExp.test("123")); //true
      console.log(reg.test("123")); //true
      console.log(regExp.test("456")); //false

    </script>
  </body>
</html>
```

# 三、正则表达式语法

## (一) 边界符

正则表达式中的边界符(位置符)用来<mark>提示字符所处的位置</mark>，主要有两个字符

| 边界符 |        说明       |
| :-: | :-------------: |
|  ^  | 表示匹配行首的文本(以谁开始) |
|  \$ | 表示匹配行尾的文本(以谁结束) |

```html
<!DOCTYPE html>
<html lang="zh">
  <title>正则表达式</title>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <script>
      var reg = /123/; //正则表达式里面不需要加引号，不管是数字型还是字符串型
      ///123/只要包含有123这个字符串返回的都是true
      console.log(reg.test('123')); //true
      console.log(reg.test('123456')); //true
      console.log(reg.test('1123abc')); //true

      //测试边界符^
      var reg1 = /^123/; //以123开头的
      console.log(reg1.test('123abc')); //true
      console.log(reg1.test('1123456')); //false
      console.log(reg1.test('abc123')); //false

      //测试边界符$
      var reg2 = /123$/; //以123结尾的
      console.log(reg2.test('abc1233')); //false
      console.log(reg2.test('123')); //true
      console.log(reg2.test('123456')); //false

      //测试边界符^和$
      var reg3 = /^123$/; //必须是123才返回true
      console.log(reg3.test('123')); //true
      console.log(reg3.test('123123')); //false
    </script>
  </body>
</html>
```

## (二) 字符类

*   字符类表示有一系列字符可供选择，只要匹配其中一个就可以了
*   <mark>所有可供选择的字符都放在方括号内</mark>

|    表达式    |                        含义                        |
| :-------: | :----------------------------------------------: |
|   \[abc]  |                 包含 `abc` 中任意一个字符                 |
| ^\[a-z]\$ | 方括号内部加上 <mark>-</mark> 表示<mark>范围</mark>，这里表示 <mark>a - z</mark> 26个英文字母都可以 |
|  \[^abc]  |  方括号内部加上 <mark>^</mark> 表示<mark>取反</mark>，只要包含方括号内的字符，都返回 `false`  |
| \[a-z1-9] |              a 到 z的26个英文字母和1到9的数字都可以             |

```html
<!DOCTYPE html>
<html lang="zh">
  <title>正则表达式</title>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <script>
      //[abc]表示匹配abc中任意字符
      //包含"hello"中任意字符都返回true
      console.log("[abc]");
      console.log(/[hello]/.test('hello world')); // 输出 true
      console.log(/[hello]/.test('greetings')); // 输出 true

      //[^abc]表示匹配除abc以外的任意字符
      //包含"hello"以外的任意字符都返回true
      console.log("[^abc]");
      console.log(/[^hello]/.test('hello world')); // 输出 true
      console.log(/[^hello]/.test('greetings')); // 输出 true
      console.log(/[^hello]/.test('hello')); // 输出 false

      //[a-z]表示匹配a到z范围内的任意字符
      //包含"hello"范围内的任意字符都返回true
      console.log("[a-z]");
      console.log(/[a-z]/.test('hello world')); // 输出 true
      console.log(/[a-z]/.test('HELLO WORLD')); // 输出 true
      console.log(/[a-z]/.test('123')); // 输出 false

      //[^a-z]表示匹配非a到z范围内的任意字符
      //包含"hello"以外的任意字符
      console.log("[^a-z]");
      console.log(/[^a-z]/.test('helloworld')); // 输出 false
      console.log(/[^a-z]/.test('HELLO WORLD')); // 输出 true
      console.log(/[^a-z]/.test('123')); // 输出 true

      //^[a-z]$表示匹配a到z范围内的任意字符，且只能出现一次
      console.log("^[a-z]$");
      console.log(/^[a-z]$/.test('a')); // 输出 true
      console.log(/^[a-z]$/.test('abc')); // 输出 false

    </script>
  </body>
</html>
```

## (三) 量词符

量词符用来<mark>设定某个模式出现的次数</mark>

|   量词  |    说明    |
| :---: | :------: |
|   \*  | 重复零次或更多次 |
|   +   | 重复一次或更多次 |
|   ?   |  重复零次或一次 |
|  {n}  |   重复n次   |
|  {n,} | 重复n次或更多次 |
| {n,m} |  重复n到m次  |

```html
<!DOCTYPE html>
<html lang="zh">
  <title>正则表达式</title>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <script>
      // 量词符: 用来设定某个模式出现的次数
      // 简单理解: 就是让下面的a这个字符重复多少次
      console.log('^a$');
      var reg = /^a$/;
      console.log(reg.test('a')); // true
      console.log(reg.test('aa')); // false

      // 重复零次或更多次
      console.log('^a*$');
      var reg = /^a*$/; // 匹配零个或多个a字符
      console.log(reg.test('')); // true
      console.log(reg.test('a')); // true

      // 重复一次或更多次
      console.log('^a+$');
      var reg = /^a+$/; // 匹配一个或多个a字符
      console.log(reg.test('')); // false
      console.log(reg.test('a')); // true
      console.log(reg.test('aba')); // false

      // 重复零次或一次
      console.log('^a?$');
      var reg = /^a?$/; // 匹配零个或一个a字符
      console.log(reg.test('')); // true
      console.log(reg.test('aa')); // false
      console.log(reg.test('a')); // true

      // 范围符: {n} 重复n次
      console.log('^a{5}$');
      var reg = /^a{5}$/; // 匹配5个a字符
      console.log(reg.test('aaaaa')); // true
      console.log(reg.test('aaaa')); // false

      // 重复零次或多次
      console.log('^a{0,3}$');
      var reg = /^a{0,3}$/; // 匹配零个或多个a字符
      console.log(reg.test('')); // true
      console.log(reg.test('a')); // true
      console.log(reg.test('aaaa')); // false
      
      // 重复一次或多次
      console.log('^a{1,}$');
      var reg = /^a{1,}$/; // 匹配一个或多个a字符
      console.log(reg.test('')); // false
      console.log(reg.test('a')); // true
      console.log(reg.test('aaaa')); // true
    </script>
  </body>
</html>
```

1.  大括号 量词符 里面面表示重复次数
2.  中括号 字符集合 匹配方括号中的任意字符
3.  小括号 表示优先级

```javascript
// 中括号 字符集合 匹配方括号中的任意字符a || b || c
var reg = /^[abc]$/;

// 大括号 量词符 里面表示重复次数
var reg = /^abc{3}$/;   // 它只是让c 重复3次 abccc

// 小括号 表示优先级
var reg = /^(abc){3}$/;  //它是让 abc 重复3次
```

## (四) 预定义类

预定义类指的是 <mark>某些常见模式的简写写法</mark>

|  预定类 |                    说明                    |
| :--: | :--------------------------------------: |
|  \d  |          匹配0-9之间的任一数字，相当于\[0-9]          |
|  \D  |         匹配所有0-9以外的字符，相当于\[^^0-9]^        |
| ^\w^ |     匹配任意的字母、数字和下划线,相当于\[A-Za-z0-9\_ ]    |
| ^\W^ | ^除所有字母、数字、和下划线以外的字符，相当于\[ ^A-Za-z0-9\_ ] |
|  \s  |   匹配空格（包括换行符，制表符，空格符等），相当于\[\t\t\n\v\f]  |
|  \S  |      \S 匹配非空格的字符，相当于\[ ^ \t\r\n\v\f]     |

# 四、常用正则表达式

*   [超全的正则表达式速查手册](https://mp.weixin.qq.com/s?__biz=MzA4Nzg5Nzc5OA==\&mid=2651720686\&idx=1\&sn=1c5e5352039cb66db413edff208b4215\&chksm=8bc8c847bcbf4151d2b2492afa483bfee45f8d7b24b3455b220e6747fa6a52a35b0272ca393e\&scene=27)

    <https://mp.weixin.qq.com/s?__biz=MzA4Nzg5Nzc5OA==&mid=2651720686&idx=1&sn=1c5e5352039cb66db413edff208b4215&chksm=8bc8c847bcbf4151d2b2492afa483bfee45f8d7b24b3455b220e6747fa6a52a35b0272ca393e&scene=27>

*   [正则表达式在线测试及常用正则表达式](https://www.jyshare.com/front-end/854/)

    <https://www.jyshare.com/front-end/854/>

# 五、参数与替换

`replace()`方法可以实现替换字符串操作，用来替换的参数可以是一个字符串或是一个正则表达式

*   regexp/substr被替换的字符串或者正则表达式
*   replacement替换为的字符串
*   返回值是一个替换完毕的新字符串

```javascript
stringObject.replace(regexp/substr,replacement)
```

通常只能替换首个匹配值,使用正则表达式参数可以匹配更多值

```javascript
/表达式/[switch]
```

switch按照什么样的模式来匹配，有三种

*   g: 全局匹配
*   i:忽略大小写
*   gi: 全局匹配 + 忽略大小写

```html
<!DOCTYPE html>
<html lang="zh">
  <title>正则表达式</title>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <script>
      var str = 'andy和red和Andy父母';
      var newStr = str.replace('andy','baby');
      var newStr2 = str.replace(/andy/gi,'baby');
      console.log(newStr); // 输出: "baby和red和Andy父母"
      console.log(newStr2); // 输出: "baby和red和baby父母"

    </script>
  </body>
</html>
```
