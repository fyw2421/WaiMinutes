---
title: "JavaScriptNotes08 : DOM节点操作"
weight: 9
description: "节点层级 创建 添加 删除 替换 复制"
date: 2026-09-29
tags: ["JavaScript"]
featureimage: "covers/javascript-notes08-dom-nodes.svg"
---

# 一、节点概述

网页中的所有内容都是节点（标签、属性、文本、注释等），在DOM 中，节点使用 node 来表示。

HTML DOM 树中的所有节点均可通过 JavaScript 进行访问，所有 HTML 元素（节点）均可被修改，也可以创建或删除

![DOM 树](javascript08-dom-tree.webp)

一般的，节点至少拥有nodeType（节点类型）、nodeName（节点名称）和nodeValue（节点值）这三个基本属性。

*   元素节点：nodeType 为1
*   属性节点：nodeType 为2
*   文本节点：nodeType 为3(文本节点包括文字、空格、换行等)

我们在实际开发中，节点操作主要操作的是元素节点

# 二、节点层级

利用 DOM 树可以把节点划分为不同的层级关系，常见的是父子兄层级关系。

## (一) 父级节点

*   parentNode属性可以返回某节点的父结点，注意是最近的一个父结点
*   如果指定的节点没有父结点则返回null

```javascript
node.parentNode;
```

## (二) 子节点

### 1. childNodes

*   返回包含指定节点的子节点的集合，该集合为即时更新的集合
*   返回值包含了所有的子结点，**包括元素节点，文本节点等**
*   如果只想要获得里面的元素节点,使用children

### 2. children

只读属性，返回所有的子元素节点

```javascript
<div class="demo">
   <div class="box">
       <span class ='qr'>x</span>
        <button>按钮1</button>
    </div>
</div>
    
var qr = document.querySelector('.qr');
console.log(qr.nodeType); // 元素1,属性2,文本3,注释4

// 2. 获取父节点
var boxNode = qr.parentNode;
console.log(boxNode);
      
// 3. 获取子节点
// 注意：childNodes属性会获取包括文本节点在内的所有节点，
console.log(boxNode.childNodes); //[#text, span.qr, #text, button, #text]
//使用children属性可以获取所有元素节点。
console.log(boxNode.children); //[span.qr, button]
```

### 3. firstChild

返回第一个子节点，找不到则返回null

### 4. lastChild

返回最后一个子节点，找不到则返回null

### 5. firstElementChild

返回第一个子元素节点，找不到则返回null,同

`node.children[0]`

### 6. lastElementChild

返回最后一个子节点，找不到则返回null.同

`node.children[node.children.length - 1]`

```html
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <ul>
        <li>1</li>
        <li>2</li>
        <li>3</li>
        <li>4</li>
    </ul>

    
    <!-- <script src="script.js"></script> -->
    <script>
      var ul = document.querySelector('ul');

      // 获取所有子节点
      console.log(ul.childNodes); //[#text, li, #text, li, #text, li, #text, li, #text]
      
      // 获取所有子元素节点
      console.log(ul.children); //[li, li, li, li]

      // 获取第一个子节点
      console.log(ul.firstChild); //#text
      
      // 获取第一个子元素节点
      console.log(ul.firstElementChild); //li
      
      // 获取最后一个子节点
      console.log(ul.lastChild); //#text
      
      // 获取最后一个子元素节点
      console.log(ul.lastElementChild); //li
      
      // 获取任意一个子节点
      console.log(ul.children[1]); //li

    </script>
  </body>
</html>
```

## (三) 兄弟节点

### 1. nextSibling

返回当前元素的下一个兄弟节点，找不到则返回null

### 2. previousSibling

返回当前元素上一个兄弟节点，找不到则返回null

### 3. nextElementSibling

返回当前元素下一个兄弟**元素节点**，找不到则返回null,IE9支持

### 4. previousElementSibling

返回当前元素上一个兄弟**元素节点**，找不到则返回null,IE9支持

```html
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <ul>
        <li>1</li>
        <li>2</li>
        <li>3</li>
        <li>4</li>
    </ul>

    
    <!-- <script src="script.js"></script> -->
    <script>
      var lis = document.querySelectorAll('li');
      console.log(lis);
      // 获取第三个li元素
      var li3 = lis[2];

      console.log(li3);
      // 获取下一个兄弟节点
      console.log(li3.nextSibling); //'\n'
      // 获取上一个兄弟节点
      console.log(li3.previousSibling); //'\n'
      // 获取下一个兄弟元素
      console.log(li3.nextElementSibling.innerHTML); //<li>4</li>
      // 获取上一个兄弟元素
      console.log(li3.previousElementSibling.innerHTML); //<li>2</li>

    </script>
  </body>
</html>
```

# 三、节点常用方法

## (一) 节点创建

```javascript
//创建由 tagName 指定的HTML 元素
document.createElement('tagName');
```

### 1. 三种动态创建元素的区别

*   `document.write()`

    直接将内容写入页面的内容流，但是文档流执行完毕，则它会导致页面全部重绘

*   `element.innerHTML`

    *   将内容写入某个 DOM 节点，不会导致页面全部重绘
    *   创建多个元素效率更高（<mark>不要拼接字符串，采取数组形式拼接</mark>），结构稍微复杂
    *   不同浏览器下， innerHTML 效率要比 createElement 高

*   `document.createElement()`

    创建多个元素效率稍低一点点，但是结构更清晰

```html
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <div class="inner"></div>
    <div class="create"></div>

    <!-- <script src="script.js"></script> -->
    <script>
      // 1. 创建元素
      document.write('<a href="#">百度</a>');
      document.write('<br>');
      // 2. innerHTML 创建元素
      var inner = document.querySelector('.inner');
      console.log(inner);
      // 2.1 innerHTML 用拼接字符串方法
      for (var i = 0; i <= 2; i++) {
        inner.innerHTML += '<a href="#">网易</a>';
      }
      // 2.2 innerHTML 用数组形式拼接
      var arr = [];
      for (var i = 0; i <= 2; i++) {
        arr.push('<a href="#">腾讯</a>');
      }
      inner.innerHTML += arr.join('');

      // 3.document.createElement() 创建元素
      var create = document.querySelector('.create');
      var a = document.createElement('a');
      create.appendChild(a);

    </script>
  </body>
</html>
```

## (二) 节点添加

```javascript
//将一个节点添加到指定父节点的子节点列表末尾
node.appendChild(child);

//将一个节点添加到父节点的指定子节点前面
node.insertBefore(child,指定元素)
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
    <ul>
        <li>1</li>
        <li>2</li>
        <li>3</li>
        <li>4</li>
    </ul>

    
    <!-- <script src="script.js"></script> -->
    <script>
      var ul = document.querySelector('ul');
      console.log('before:');
      for(var i = 0; i < ul.children.length; i++) {
        console.log(ul.children[i].innerHTML);
      }
      // 添加节点
      var appendNode= document.createElement('li');
      appendNode.innerHTML = 'appendChild node';
      ul.appendChild(appendNode);

      var insertNode = document.createElement('li');
      insertNode.innerHTML = 'insertBefore node';
      ul.insertBefore(insertNode, ul.children[1]);

      console.log('after:');
      for(var i = 0; i < ul.children.length; i++) {
        console.log(ul.children[i].innerHTML);
      }

    </script>
  </body>
</html>
```

## (三) 节点删除

```javascript
//从 DOM 中删除一个子节点，返回删除的节点
node.removechild(child);
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
    <ul>
        <li>1</li>
        <li>2</li>
        <li>3</li>
        <li>4</li>
    </ul>

    
    <!-- <script src="script.js"></script> -->
    <script>
      var ul = document.querySelector('ul');
      console.log('before:');
      for(var i = 0; i < ul.children.length; i++) {
        console.log(ul.children[i].innerHTML);
      }
      // 删除节点
      var deltedNode = ul.removeChild(ul.children[0]);
      console.log('deleted node :' + deltedNode.innerHTML);

      console.log('after:');
      for(var i = 0; i < ul.children.length; i++) {
        console.log(ul.children[i].innerHTML);
      }

    </script>
  </body>
</html>
```

## (四) 节点替换

```javascript
// 注意：replaceChild()方法用于将一个新的子节点替换掉指定位置的子节点。
var btn3 = document.createElement('button');
btn3.innerHTML = '按钮3';
boxNode.replaceChild(btn3, boxNode.lastElementChild);
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
    <ul>
        <li>1</li>
        <li>2</li>
        <li>3</li>
        <li>4</li>
    </ul>

    
    <!-- <script src="script.js"></script> -->
    <script>
      var ul = document.querySelector('ul');
      console.log('before clone :');
      for(var i = 0; i < ul.children.length; i++) {
        console.log(ul.children[i].innerHTML);
      }
      // 注意：replaceChild()方法用于将一个新的子节点替换掉指定位置的子节点。
      var newNode = document.createElement('li');
      newNode.innerHTML = 'new replace node';
      ul.replaceChild(newNode, ul.children[1]);

      console.log('after clone :');
      for(var i = 0; i < ul.children.length; i++) {
        console.log(ul.children[i].innerHTML);
      }

    </script>
  </body>
</html>
```

## (五) 节点复制

`node.cloneNode()`

*   返回调用该方法的节点的一个副本。 也称为克隆节点/拷贝节点
*   如果括号参数为空或者为 false ，则是浅拷贝，即只克隆复制节点本身，不克隆里面的子节点
*   如果括号参数为 true ，则是深度拷贝，会复制节点本身以及里面所有的子节点

```html
<!DOCTYPE html>
<html lang="zh">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
  </head>
  <body>
    <ul>
        <li>1</li>
        <li>2</li>
        <li>3</li>
        <li>4</li>
    </ul>

    
    <!-- <script src="script.js"></script> -->
    <script>
      var ul = document.querySelector('ul');
      console.log('before clone :');
      for(var i = 0; i < ul.children.length; i++) {
        console.log(ul.children[i].innerHTML);
      }
      // 复制节点
      // 注意：cloneNode()方法默认只会复制当前节点，
      //如果要复制当前节点的所有子节点，则需要将参数设置为true。
      // 注意：复制节点后，复制的节点和原始节点是两个完全独立的节点，它们之间没有任何关系。
      var li = ul.children[0].cloneNode(true);
      li.innerHTML = 'clone from 1';

      ul.appendChild(li);
      console.log('after clone :');
      for(var i = 0; i < ul.children.length; i++) {
        console.log(ul.children[i].innerHTML);
      }

    </script>
  </body>
</html>
```

## (六) 解决兼容性示例

```javascript
function getNextElementSibling(element) {
    var el = element;
    while(el = el.nextSibling) {
        if(el.nodeType === 1){
            return el;
        }
    }
    return null;
}

```
