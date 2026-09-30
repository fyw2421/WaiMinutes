---
title: "JavaScriptNotes05 : 函数与作用域"
weight: 6
description: "声明 形参实参 作用域 预解析 this指向 闭包"
date: 2026-09-29
tags: ["JavaScript"]
featureimage: "covers/javascript-notes05-function-scope.svg"
---

函数：就是封装了一段可被重复调用执行的代码块。通过此代码块可以实现大量代码的重复使用。

# 一、函数的声明和调用

函数在使用时分为两步：声明函数和调用函数

## (一) 函数的声明

函数有两种声明方式:自定义函数方式(命名函数)和函数表达式方式(匿名函数)

### 1. 自定义函数方式(命名函数)

*   因为有名字，所以也被称为命名函数
*   调用函数的代码既可以放到声明函数的前面，也可以放在声明函数的后面(声明提升)

```javascript
function 函数名(){
	//函数体代码
}
```

### 2. 函数表达式方式(匿名函数)

*   因为函数没有名字，所以也称为匿名函数
*   函数调用的代码必须写到函数体后面

```javascript
var fn = function(){};
fn();
```

### 3. new Function

*   Function 里面参数都必须是字符串格式
*   第三种方式执行效率低，也不方便书写，因此较少使用
*   所有函数都是 Function 的实例(对象)
*   函数也属于对象

```javascript
var add = new Function('a','b','return a+b');
console.log(add(1,2));
```

## (二) 函数的调用

```javascript
函数名();
```

# 二、形参和实参

*   在声明函数时，可以在函数名称后面的小括号中添加一些参数，这些参数被称为形参
*   而在调用该函数时，同样也需要传递相应的参数，这些参数被称为实参。

```javascript
function 函数名(形参1,形参2,...){
}

函数名(实参1,实参2,...);
```

1.  **实参个数等于形参个数**

    输出正确结果

2.  **实参个数多于形参个数**

    只取到形参的个数

3.  **实参个数小于形参个数**

    多的形参定义为undefined，结果为NaN

4.  **在JavaScript中，形参的默认值是undefined**

5.  **当我们把一个值类型变量作为参数传给函数的形参时，其实是把变量在栈空间里的值复制了一份给形参，那么在方法内部对形参做任何修改，都不会影响到的外部变量**

6.  **当我们把引用类型变量传给形参时，其实是把变量在栈空间里保存的堆地址复制给了形参，形参和实参其实保存的是同一个堆地址，所以操作的是同一个对象。**

# 三、arguments不确定参数个数

当我们不确定有多少个参数传递的时候，可以用 arguments 来获取。在 JavaScript 中，arguments 实际上它是当前函数的一个内置对象。所有函数都内置了一个 arguments 对象，arguments 对象中存储了传递的所有实参。

*   arguments存放的是传递过来的实参
*   arguments展示形式是一个伪数组，因此可以进行遍历。伪数组具有以下特点

    1.  具有 length 属性
    2.  按索引方式储存数据
    3.  不具有数组的 push , pop 等方法

```javascript
function getMaxValue(){
    var max = arguments[0];
    for (var i = 1; i < arguments.length; i++){
        if(arguments[i] > max){
            max = arguments[i];
        }
    }
    return max; // 返回最大值
}

console.log(getMaxValue(1, 2, 3, 4, 5)); // 输出 5
console.log(getMaxValue(10)); // 输出 10
console.log(getMaxValue()); // 输出 undefined
```

# 四、作用域

通常来说，一段程序代码中所用到的名字并不总是有效和可用的，而限定这个名字的可用性的代码范围就是这个名字的作用域。作用域的使用提高了程序逻辑的局部性，增强了程序的可靠性，减少了名字冲突。

JavaScript (ES6前) 中的作用域有两种：

*   全局作用域
*   局部作用域(函数作用域)

## (一) 全局作用域

作用于所有代码执行的环境(整个 script 标签内部)或者一个独立的 js 文件

## (二) 局部作用域(函数作用域)

作用于函数内的代码环境，就是局部作用域。 因为跟函数有关系，所以也称为函数作用域

**JS 没有块级作用域**

```javascript
if (1){
    var num = 123;
    console.log(num); // 输出 123
}
console.log(num); // 输出 123
```

## (三) 全局变量和局部变量

根据作用域的不同，变量可以分为两种：全局变量和局部变量

### 1. 全局变量

在全局作用域下声明的变量叫做全局变量（在函数外部定义的变量）

1.  全局变量在代码的任何位置都可以使用
2.  在全局作用域下 var 声明的变量 是全局变量
3.  特殊情况下，在函数内不使用 var 声明的变量也是全局变量（不建议使用)
4.  只有在浏览器关闭时才会被销毁，因此比较占内存

### 2. 局部变量

在局部作用域下声明的变量叫做局部变量（在函数内部定义的变量）

1.  局部变量只能在该函数内部使用
2.  在函数内部 var 声明的变量是局部变量
3.  函数的形参实际上就是局部变量
4.  当其所在的代码块被执行时，会被初始化；当代码块运行结束后，就会被销毁，因此更节省内存空间

## (四) 预解析(提升)

### 1. 变量预解析(变量提升)

**变量的声明会被提升到当前作用域的最上面，变量的赋值不会提升**

```javascript
console.log(num); //undefined
var num = 123; // 全局变量

//相当于

var num;
console.log(num); // 输出 undefined
num = 123; // 全局变量
```

### 2. 函数预解析(函数提升)

*   命名函数的声明会被提升到当前作用域的最上面，但是不会调用函数。
*   匿名函数的声明不会提升

```javascript
//命名函数提升
fn();
function fn(){
    console.log(1); //1
}

//匿名函数声明不会提升
fn();
var fn =function(){
    console.log(1);
}

//TypeError: fn is not a function
```

### 3. 预解析练习

```javascript
var num = 123;
fun();
function fun(){
    console.log(num);
    var num = 456;
}
//undefined
```

```javascript
fun();
console.log(c);
function fun(){
    c = 123;
    console.log(c);
}
//123
//123
//在函数内不使用 var 声明的变量也是全局变量

fun();
console.log(d); //12
console.log(c); //ReferenceError: c is not defined
function fun(){
    var c = 123;
	d = 123;
    console.log(c);
}
```

# 五、函数的调用

## (一) 函数的调用方式及this指向

|  调用方式  |         this指向        |
| :----: | :-------------------: |
| 普通函数调用 |         window        |
| 构造函数调用 | 实例对象，原型对象里面的方法也指向实例对象 |
| 对象方法调用 |        该方法所属对象        |
| 事件绑定方法 |         绑定事件对象        |
|  定时器函数 |         window        |
| 立即执行函数 |         window        |

## (二) 改变函数内部this指向

JavaScript 为我们专门提供了一些函数方法来帮我们处理函数内部 this 的指向问题，常用的有 `bind(),call(),apply()`三种方法

### 1. call

调用一个对象的一个方法，以另一个对象替换当前对象。

当我们想改变 this 指向，同时想调用这个函数的时候，可以使用 call，比如继承,参见**javascriptNotes11\_类与自定义构造函数 组合继承**

```javascript
//thisArg: 在 fun 函数运行时指定的 this 值
//arg1,arg2: 传递的其他参数
返回值就是函数的返回值，因为它就是调用函数
fun.call(thisArg,arg1,arg2,.....)
```

```html
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <script>
      var o = {
        name: '张三',
        age: 18,
        gender: '男'
      }

      function fn(a,b){
        console.log(this.name);
        console.log(a + b);
      }

      // 1. 函数调用模式
      fn.call(o,1,2);

      // 2. 方法调用模式
      function Father(name,age){
        this.name = name;
        this.age = age;
      }

      function Son(name,age){
        Father.call(this,name,age);
      }

      var f = new Father('李四',20);
      console.log(f);

      var s = new Son('张三',18);
      console.log(s);

    </script>
  </body>
</html>
```

### 2. apply

应用某一对象的一个方法，用另一个对象替换当前对象

apply和call功能一样，只是传入的参数列表形式不同

apply复杂些，要把数组转参数列表再调用内部\[\[Call]]中间步骤比用call多

```javascript
//thisArg: 在 fun 函数运行时指定的 this 值
//argsArray : 传递的值，必须包含在数组里面
fun.apply(thisArg,[argsArray])
```

```html
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <script>
      var o = {
        name: '张三',
        age: 18,
        gender: '男'
      }

      function fn(arr){
        console.log(this.name,arr);
      }

      // 1. 函数调用模式
      fn.apply(o,['pink']); //张三 pink
      fn.call(o,1); //张三 1

      // 2. 方法调用模式
      function Father(name,age){
        this.name = name;
        this.age = age;
      }

      function Son(name,age){
        Father.apply(this,[name,age]);
      }

      var f = new Father('李四',20);
      console.log(f); //Father {name: "李四", age: 20}

      var s = new Son('张三',18);
      console.log(s); //

      // 3.求最大值
      var arr = [1, 66, 3, 99, 4];
      var arr1 = ['red', 'pink'];

      function fnMax(arr){
        var max = arr[0];
        for(var i = 1; i < arr.length; i++){
          if(max < arr[i]){
            max = arr[i];
          }
        }
        return max;
      }

      var max = Math.max.apply(Math, arr); //99
      var min = Math.min.apply(Math, arr); //1
      var max1 = Math.max.call(Math, 1, 66, 3, 99, 4); //99
      var min1 = Math.min.call(Math, 1, 66, 3, 99, 4); //1
      var max2 = fnMax.call(null,arr);

      console.log(max, min, max1, min1,max2); //99 1 99 1 99

    </script>
  </body>
</html>
```

### 3. bind

不会调用函数。但是能改变函数内部 `this`指向

```javascript
//返回由指定的 this值和初始化参数改造的 原函数拷贝
//因此当我们只是想改变 this 指向，并且不想调用这个函数的时候，可以使用bind
fun.bind(thisArg,arg1,arg2,....)
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
    <button>button1</button>
    <button>button2</button>
    <button>button3</button>
    <div class="box">div</div>
    <div class="bo">bo</div>
    <script>
      var o = {
        name: '张三',
        age: 18,
        gender: '男'
      }

      function fn(arr){
        console.log(this.name,arr);
        console.log(this);
      }

      var f = fn.bind(o,[1,2,3]);
      f();

      var btns = document.querySelectorAll('button');
      for(var i = 0;i<btns.length;i++){
        btns[i].onclick = function(){
          console.log(this.innerHTML);
          //如果不使用bind方法，this指向window,输出undefined,undefined,undefined
          //使用bind方法，this指向button,输出button1,button2,button3
          setTimeout(function
            (){
              console.log(this.innerHTML);
            }.bind(this),1000);
          //setTimeout(function(){console.log(this.innerHTML)},1000);
          //setTimeout(()=>{console.log(this.innerHTML)},1000);
        }
      }

      function fn2(content){
        console.log(content);
      }

      //使用bind方法，this指向div
      var div = document.querySelector('.box');
      div.onclick = fn2.bind(div,div.innerText);

    </script>
  </body>
</html>
```

### 4. call/apply/bind异同

1.  相同点

    *   都可以改变函数内部的 `this`指向
2.  区别点

    *   `call`和`apply`会调用函数，并且改变函数内部的`this`指向
    *   `call`和`apply`传递的参数不一样，call 传递参数，apply 必须数组形式
    *   `bind`不会调用函数，可以改变函数内部`this`指向
3.  主要应用场景

    *   `call`经常做继承
    *   `apply`经常跟数组有关系，比如借助于数学对线实现数组最大值与最小值
    *   `bind`不调用函数，但是还想改变this指向，比如改变定时器内部的this指向

# 六、高阶函数(回调函数)

<mark>高阶函数</mark>是对其他函数进行操作的函数，它<mark>接收函数作为参数</mark>或<mark>将函数作为返回值输出</mark>

```html
<body>
    <div></div>
    <script>
        // 高阶函数- 函数可以作为参数传递
        function fn(a, b, callback) {
            console.log(a + b);
            callback && callback();
        }
        fn(1, 2, function() {
            console.log('我是最后调用的');

        });

    </script>
</body>
```

```html
<script>
    function fn(){
        return function() {}
    }
</script>

```

# 七、闭包

## (一) 变量的作用域

变量根据作用域的不同分为两种：全局变量和局部变量

1.  函数内部可以使用全局变量
2.  函数外部不可以使用局部变量
3.  当函数执行完毕，本作用域内的局部变量会销毁。

## (二) 闭包的含义

闭包指有权访问另一个函数作用域中的变量的函数

简单理解：一个作用域可以访问另外一个函数内部的局部变量

```html
<body>
    <script>
        // 闭包（closure）指有权访问另一个函数作用域中变量的函数。
        // 闭包: 我们fn2 这个函数作用域 访问了另外一个函数 fn1 里面的局部变量 num
        function fn1() {		// fn1就是闭包函数
            var num = 10;
            function fn2() {
                console.log(num); 	//10
            }
            fn2();
        }
        fn1();
    </script>
</body>
```

## (三) 闭包的作用

<mark>延伸变量的作用范围</mark>

### 1. 点击li输出当前li的索引号

```html
<body>
    <ul class="nav">
        <li>榴莲</li>
        <li>臭豆腐</li>
        <li>鲱鱼罐头</li>
        <li>大猪蹄子</li>
    </ul>
    <script>
        // 闭包应用-点击li输出当前li的索引号
        // 1. 我们可以利用动态添加属性的方式
        var lis = document.querySelector('.nav').querySelectorAll('li');
        for (var i = 0; i < lis.length; i++) {
            lis[i].index = i;
            lis[i].onclick = function() {
                // console.log(i);
                console.log(this.index);

            }
        }
        // 2. 利用闭包的方式得到当前小li 的索引号
        for (var i = 0; i < lis.length; i++) {
            // 利用for循环创建了4个立即执行函数
            // 立即执行函数也成为小闭包因为立即执行函数里面的任何一个函数都可以使用它的i这变量
            (function(i) {
                // console.log(i);
                lis[i].onclick = function() {
                    console.log(i);
                }
            })(i);
        }
    </script>
</body>
```

### 2. 定时器中的闭包

```html
<body>
    <ul class="nav">
        <li>榴莲</li>
        <li>臭豆腐</li>
        <li>鲱鱼罐头</li>
        <li>大猪蹄子</li>
    </ul>
    <script>
        // 闭包应用-3秒钟之后,打印所有li元素的内容
        var lis = document.querySelector('.nav').querySelectorAll('li');
        for (var i = 0; i < lis.length; i++) {
            (function(i) {
                setTimeout(function() {
                    console.log(lis[i].innerHTML);
                }, 3000)
            })(i);
        }
    </script>
</body>

```
