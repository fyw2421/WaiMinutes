---
title: "JavaScriptNotes04 : 数组"
weight: 5
description: "创建 遍历 常用方法 排序 索引 转字符串"
date: 2026-09-29
tags: ["JavaScript"]
featureimage: "covers/javascript-notes04-array.svg"
---

数组(Array)是指一组数据的集合，其中的每个数据被称作元素，在数组中可以存放任意类型的元素。数组是一种将一组数据存储在单个变量名下的优雅方式。

# 一、创建数组

JavaScript 中创建数组有两种方式：

1.  利用 new 创建数组
2.  利用数组字面量创建数组

数组中可以存放任意类型的数据,例如字符串,数字,布尔值等.

## (一) 利用new创建数组

```javascript
// 创建一个长度为5的数组
var arr = new Array(5);

console.log(arr); // 输出：[ <5 empty items> ]相当于[undefined,undefined,undefined,undefined,undefined]

console.log(arr.length); // 输出：5
console.log(arr[3]); // 输出：undefined

// 创建一个数组，并赋值
var arr2 = new Array(5,3,2,null);
console.log(arr2); // 输出：[5,3,2,null]
console.log(arr2.length); // 输出：4
console.log(arr2[2]); // 输出：2

// 创建一个空数组
var arr3 = new Array();
console.log(arr3); // 输出：[]
console.log(arr3.length); // 输出：0
console.log(arr3[3]); // 查询不存在的索引,输出：undefined
```

## (二) 利用数组字面量创建数组

```javascript
var arr4 = [];
console.log(arr4); //[]
console.log(arr4.length); //0
console.log(arr4[3]); // undefined

var arr5 = [5,3,2,null]; 
console.log(arr5); //[ 5, 3, 2, null ]
console.log(arr5.length); //4
console.log(arr5[2]); //2
```

# 二、数组访问

## (一) 通过索引(下标) 访问数组元素,从0开始

*   添加元素：

     语法：数组\[索引] = 值

*   查询元素

    语法：数组\[索引]

    如果查询不存在的索引，不会报错而是返回`undifined`

*   向数组最后一个位置添加元素：

    例如：arr\[arr.length] = 1;

```javascript
var arr6 = [];
console.log(arr6[2]); //如果查询不存在的索引，不会报错而是返回undifined
arr6[2] = 3;
console.log(arr6); //[ <2 empty items>, 3 ],自动追加前2项,undefined
```

## (二) 获取/修改数组长度

*   获取数组的长度：

    *   使用length属性获取连续数组的长度（元素的个数）

        语法：数组.length

    *   对于非连续的数组会获取数组最大索引+1

*   修改数组的长度

    *   如果修改的length大于原长度，则多余部分会空出来
    *   如果修改的length小于原长度，则多出的元素会被删除

```javascript
var arr6 = [];
arr6[arr6.length] = 3;
console.log(arr6); //[ 3 ]
console.log(arr6.length); // 1
arr6.length = 5;
console.log(arr6); //[ 3, <4 empty items> ]
arr6.length = 2;
console.log(arr6); //[ 3 , <1 empty item>]
```

# 三、数组遍历

## (一) for循环

```javascript
var arr7 = [5,3,2,null];
for(var i = 0; i < arr7.length; i++)
    arr7[i] = arr7[i] * 2;
for(var i = 0; i < arr7.length; i++)
    console.log(arr7[i]);
/*
10
6
4
0
*/
```

## (二) forEach(ES5)

```javascript
//value : 数组当前项的值
//index : 数组当前项的索引
//array : 数组对象本身
arr7.forEach(function(value,index,array){
    array[index] = value * 2;
});
arr7.forEach(function(value){
    console.log(value);
});
arr7.forEach(function(value,index){
    console.log(index + ':' + value);
});
/*
10
6
4
0

0:10
1:6
2:4
3:0
*/
```

# 四、常用方法

## (一) push()

   向数组末尾添加一个或多个元素，并返回数组的新长度
   可以将要添加的元素作为方法的参数传递，元素将自动添加到数组末尾
   该方法会将数组新的长度作为返回值返回

```javascript
var arr8 = [5,3,2,null];
var arr8Len = arr8.push('a');
console.log(arr8); //[ 5, 3, 2, null, 'a' ]
```

## (二) pop()

    删除数组的最后一个元素，并将删除的元素作为返回值返回

```javascript
var arr8 = [5,3,2,null];
var value = arr8.pop();
console.log(value); //null
console.log(arr8);
```

## (三) unshift

    向数组开头添加一个或多个元素，并返回新的数组长度

    向前边插入元素以后，其它元素索引会依次调整

```javascript
var arr8 = [5,3,2,null];
var arr8Len = arr8.unshift('a');
console.log(arr8);
//[ 'a', 5, 3, 2, null ]
```

## (四) shift

   删除数组第一个元素，并将被删除的元素作为返回值返回

```javascript
var arr8 = [5,3,2,null];
var value = arr8.shift('a');
console.log(value); //5
console.log(arr8); //[ 3, 2, null ]
```

## (五) reserve

颠倒数组中元素的顺序，无参数

```javascript
var arr8 = [5,3,2,null];

console.log(arr8); //[ 5, 3, 2, null ]
arr8.reverse();
console.log(arr8); //[ null, 2, 3, 5 ]
```

## (六) sort

对数组的元素进行冒泡排序.

```javascript
var arr8 = [5,3,2,null];

arr8.sort(); 默认升序
console.log(arr8); //[ 2, 3, 5, null ]
//a代表arr8[j],b代表arr8[j+1]
//if (b - a) > 0,采用冒泡排序,交换a,b位置,较大的数被换到前面,降序排列
//if (a - b) > 0,采用冒泡排序,交换a,b位置,较大的数被换到后面,升序排列
arr8.sort(function(a,b){
    return b - a; //降序排列
	//return a - b; //升序排列
});
console.log(arr8); //[ 5, 3, 2, null ]
```

## (七) 查询索引

### 1. indexOf()

数组中查找给定元素的第一个索引

如果存在返回索引号，如果不存在，则返回-1

### 2. lastIndexOf()

在数组的最后一个索引，从后向前索引

如果存在返回索引号，如果不存在，则返回-1

````javascript
	var arr8 = [5,3,2,null];
    // 查找arr8数组中3的索引
    console.log(arr8.indexOf(3)); // 1
     
    // 查找arr8数组中3的索引，从索引1开始查找
    console.log(arr8.indexOf(3,1)); // 1

   // 查找arr8数组中3的索引，从索引2开始查找
    console.log(arr8.indexOf(3,2)); // -1
    // 返回数组中指定元素的最后一个索引值
    console.log(arr8.lastIndexOf(3)); // 1
	console.log(arr8.lastIndexOf(3,arr8.length - 1)); // 1
 ```

```javascript
//数组去重
 function unique(arr){
    // 创建一个空数组result
    var result = [];
    // 遍历arr数组
    for(var i = 0; i < arr.length; i++){
        // 判断result数组中是否已经存在arr[i]
        if(result.indexOf(arr[i]) === -1){
            // 如果result数组中不存在arr[i]，则将arr[i]添加到result数组中
            result.push(arr[i]);
        }
    }
    // 返回result数组
    return result;
}

var arr9 = [1,2,3,4,5,4,3,2,1];
console.log(unique(arr9)); // [1,2,3,4,5]
````

## (八) 数组转化为字符串

数组转化为字符串

### 1. toString()

把数组转换成字符串，逗号分隔每一项,返回一个字符串.

### 2. join()

方法用于把数组中的所有元素转换为一个字符串,分隔符作参数,返回一个字符串.

```javascript
//toString
var arr10 = [1,2,3,4,5];
console.log(arr10.toString()); //1,2,3,4,5

//join('分隔符')
var arr11 = [1,2,3,4,5];
console.log(arr11.join('-')); //1-2-3-4-5
console.log(arr11.join());//默认为逗号,1,2,3,4,5
console.log(arr11.join('')); //12345
console.log(arr11.join('&')); //1&2&3&4&5
```

## (九) 其他方法

### 1. concat() 连接数组

连接两个或多个数组 不影响原数组,返回一个新的数组

```javascript
var arr12 = [1,2,3,4,5];
var arr13 = arr12.concat(6,7,8);
console.log(arr13); //[1,2,3,4,5,6,7,8]
var arr14 = [6,7,8,9,10];
console.log(arr12.concat(arr14).concat(11,12,13).concat(arr12));
console.log(arr12); //[ 1, 2, 3, 4, 5 ]
```

### 2. slice(begin,end)

数组截取slice(begin,end),返回被截取项目的新数组

```javascript
var arr12 = [ 1, 2, 3,  4,  5,  6, 7, 8, 9, 10, 11, 12,13];
var arr13 = arr12.slice(2,6); //从索引2开始截取，到索引6结束（不包括索引6）
console.log(arr13); //[3, 4, 5, 6]
var arr14 = arr12.slice(2); //从索引2开始截取，到数组末尾
var arr15 = arr12.slice(2,100); //从索引2开始截取，到数组末尾
console.log(arr14); //[3, 4, 5, 6, 7, 8, 9, 10]
```

### 3. splice()

数组删除splice(第几个开始,要删除的个数),返回被删除项目的新数组，影响原数组

```javascript
//定义一个数组arr12，元素为1,2,3,4,5,6,7,8,9,10,11,12,13
var arr12 = [ 1, 2, 3,  4,  5,  6, 7, 8, 9, 10, 11, 12,13];
//从arr12中取出第3个元素到第8个元素
var arr13 = arr12.splice(3,5);
console.log(arr13); //3,4,5,6
console.log(arr12); //1,2,3,9,10, 11, 12, 13
//从arr12中取出第3个元素到第5个元素，并将其赋值为100,200,300
var arr14 = arr12.splice(3,2,100,200,300);
console.log(arr12); //1,2,3,100,200,300,9,10
```

### 4. filter()(ES5)

返回一个新的数组，新数组中的元素是通过检查指定数组中符合条件的所有元素，<mark>主要用于筛选数组</mark>

```javascript
var arr8 = [5,3,2,null];
var filterArr = arr8.filter(function(value,index,arr){
       return value != null;
});
console.log(filterArr); //[5,3,3]
```

### 5. some()(ES5)

用于检测数组中的元素是否满足指定条件（查找数组中是否有满足条件的元素）

*   如果找到第一个满足条件的元素，则终止循环，不再继续查找
*   返回的是布尔值，如果查找到这个元素，就返回true，如果查找不到就返回false

```javascript
var arr8 = [5,3,2,null];

var flag = arr8.some(function(value,index,array){
   return value == null;
});
console.log(flag); // true
```
