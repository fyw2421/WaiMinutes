---
title: "JavaScriptNotes01 : 简介"
weight: 2
description: "主要特点 浏览器执行 JS的组成 代码形式"
date: 2026-09-29
tags: ["JavaScript"]
featureimage: "covers/javascript-notes01-introduction.svg"
---

# 一、主要特点

JavaScript是Web页面中的一种脚本编程语言，也是一种通用的、跨平台的、基于对象和事件驱动并具有安全性的脚本语言。它不需要进行编译，而是直接嵌入在HTML页面中，把静态页面转变成支持用户交互并响应相应事件的动态页面。

**解释性**

JavaScript不同于一些编译性的程序语言如，C、C++等，是一种解释性的程序语言，其源代码不需要经过编译，而是直接在浏览器中运行时被解释。

**基于对象**

JavaScript是一种基于对象的语言，这意味着它能运用自己已经创建的对象。因此，许多功能可以来自于脚本环境中对象的方法与脚本的相互作用。，

**事件驱动**

JavaScript可以直接对用户或客户输入做出响应，无须经过Web服务程序。它对用户的响应，是 以事件驱动的方式进行的。所谓事件驱动，就是指在主页中执行了某种操作所产生的动作，此动作称为“事件”。例如，按下鼠标、移动窗口、选择菜单等都可以视为事件。当事件发生后，可能会引起 相应的事件响应。

**跨平台**

JavaScript依赖于浏览器本身，与操作环境无关，只要能运行浏览器的计算机，并支持JavaScript
的浏览器就可以正确执行。

**安全性**

JavaScript是-种安全性语言，不允许访问本地的硬盘，不能将数据存入服务器上，不允许对网络
文档进行修改和删除，只能通过浏览器实现信息浏览或动态交互。这样可有效地防止数据的丢失。

# 二、浏览器执行JS简介

浏览器分成两部分：**渲染引擎**和 **JS 引擎**

*   **渲染引擎**：

    用来解析HTML与CSS，俗称内核，比如 chrome 浏览器的 blink ，老版本的 webkit

*   **JS 引擎**：

    也称为 JS 解释器。 用来读取网页中的JavaScript代码，对其处理后运行，比如 chrome 浏览器的 V8

# 三、JS的组成

**JavaScript 包括 ECMAScript、DOM、BOM**

*   **ECMAScript**

    ECMAScript 是由ECMA 国际（ 原欧洲计算机制造商协会）进行标准化的一门编程语言，这种语言在万维网上应用广泛，它往往被称为 JavaScript 或 JScript，但实际上后两者是 ECMAScript 语言的实现和扩展。ECMAScript 规定了JS的编程语法和基础核心知识，是所有浏览器厂商共同遵守的一套JS语法工业标准。

*   **DOM文档对象模型**

    文档对象模型（Document Object Model，简称DOM），是W3C组织推荐的处理可扩展标记语言的标准编程接口。通过 DOM 提供的接口可以对页面上的各种元素进行操作（大小、位置、颜色等）。

*   **BOM浏览器对象模型**

    BOM (Browser Object Model，简称BOM) 是指浏览器对象模型，它提供了独立于内容的、可以与浏览器窗口进行互动的对象结构。通过BOM可以操作浏览器窗口，比如弹出框、控制浏览器跳转、获取分辨率等。

# 四、JS形式

*   **行内式JS**

    ```html
    <input type="button" value="点我试试" onClick="javascript:alert('Hello World')"/>
    ```

    1.  可以将单行或少量JS代码写在HTML标签的事件属性中(以on开头的属性)，如： onclink
    2.  注意单双引号的使用：在HTML中我们推荐使用双引号，JS中我们推荐使用单引号
    3.  可读性差，在 HTML 中编入 JS 大量代码时，不方便阅读
    4.  特殊情况下使用

*   **内嵌式JS**

    ```html
    <script
          alert('Hello World')
    </script>
    ```

    1.  可以将多行JS代码写到\<script>标签中
    2.  内嵌 JS 是学习时常用的方式

*   **外部JS**

    ```html
    <script src="my.js"></script>
    ```

    1.  利于HTML页面代码结构化，把单独JS代码独立到HTML页面之外，既美观，又方便
    2.  引用外部JS文件的script标签中间不可以写代码
    3.  适合于JS代码量比较大的情况
