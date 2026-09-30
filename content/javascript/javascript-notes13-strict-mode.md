---
title: "JavaScriptNotes13 : 严格模式"
weight: 14
description: "开启方式 变量 this 函数的变化"
date: 2026-09-29
tags: ["JavaScript"]
featureimage: "covers/javascript-notes13-strict-mode.svg"
---

# 一、严格模式

*   javaScript 除了提供正常模式外，还提供了严格模式
*   ES5 的严格模式是采用具有限制性 JavaScript 变体的一种方式，即在严格的条件下运行 JS 代码
*    严格模式在IE10 以上版本的浏览器才会被支持，旧版本浏览器会被忽略
*   严格模式对正常的JavaScript语义做了一些更改：

    *   消除了Javascript 语法的一些不合理、不严谨之处，减少了一些怪异行为
    *   消除代码运行的一些不安全之处，保证代码运行的安全
    *   提高编译器效率，增加运行速度
    *   禁用了在 ECMAScript 的未来版本中可能会定义的一些语法，为未来新版本的 Javascript 做好铺垫。比如一些保留字如：class, enum, export, extends, import, super 不能做变量名

## (一) 开启严格模式

*   严格模式可以应用到<mark>整个脚本</mark>或<mark>个别函数</mark>中。
*   因此在使用时，我们可以将严格模式分为<mark>为脚本开启严格模式</mark>和<mark>为函数开启严格模式两种情况</mark>

### 1. 脚本开启严格模式

```html
<script>
    'use strict'
     console.log('严格模式');
</script>
```

因为`"use strict"`加了引号，所以老版本的浏览器会把它当作一行普通字符串而忽略。

有的 script 基本是严格模式，有的 script 脚本是正常模式，这样不利于文件合并，所以可以将整个脚本文件放在一个立即执行的匿名函数之中。这样独立创建一个作用域而不影响其他 script 脚本文件。

```html
<script>
	(function (){
    	'use strict';
    	 var num = 10;
    	 function fn() {}
	})();   
</script>
```

### 2. 函数开启严格模式

若要给某个函数开启严格模式，需要把`"use strict"`或`'use strict'`声明放在函数体所有语句之前

```html
<body>
    <!-- 为整个脚本(script标签)开启严格模式 -->
    <script>
        'use strict';
        //   下面的js 代码就会按照严格模式执行代码
    </script>
    <script>
        (function() {
            'use strict';
        })();
    </script>
    <!-- 为某个函数开启严格模式 -->
    <script>
        // 此时只是给fn函数开启严格模式
        function fn() {
            'use strict';
            // 下面的代码按照严格模式执行
        }

        function fun() {
            // 里面的还是按照普通模式执行
        }
    </script>
</body>

```

## (二) 严格模式的变化

### 1. 变量

在正常模式中，如果一个变量没有声明就赋值，默认是全局变量,在严格模式下

*   变量都必须先用var 命令声明，然后再使用
*   严禁删除已经声明变量，例如，\`\`delete x\` 语法是错误的

### 2. this指针

*   全局作用域函数中的`this`指向`window`对象,严格模式下全局作用域中函数中的`this` 是 <mark>undefined</mark>

*   构造函数时不加 `new` 也可以调用，当普通函数，`this`指向全局对象.严格模式下，如果构造函数不加 `new` 调用，`this`指向的是 `undefined` ，如果给它赋值，会报错

*   `new` 实例化的构造函数指向创建的对象实例

*   定时器`this` 还是指向`window`

*   事件、对象还是指向调用者

```html
<body>
    <script>
        'use strict';
		//3. 严格模式下全局作用域中函数中的 this 是 undefined。
        function fn() {
            console.log(this); // undefined。

        }
        fn();
        //4. 严格模式下,如果 构造函数不加new调用, this 指向的是undefined 如果给他赋值则 会报错.
        function Star() {
            this.sex = '男';
        }
        // Star();
        var ldh = new Star();
        console.log(ldh.sex);
        //5. 定时器 this 还是指向 window 
        setTimeout(function() {
            console.log(this);

        }, 2000);
        
    </script>
</body>
```

### 3. 函数

*   函数不能有重名的**参数**
*   函数必须声明在顶层，新版本的JavaScript会引入“块级作用域”（ES6中已引入）。为了与新版本接轨，**不允许在非函数的代码块内声明函数**

```html
<body>
    <script>
        'use strict';
        // 6. 严格模式下函数里面的参数不允许有重名
        function fn(a, a) {
           console.log(a + a);

        };
        // fn(1, 2);
        function fn() {}
    </script>
</body>
```
