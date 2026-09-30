---
title: "JavaScriptNotes03 : 流程控制"
weight: 4
description: "if switch for while break continue"
date: 2026-09-29
tags: ["JavaScript"]
featureimage: "covers/javascript-notes03-flow-control.svg"
---

# 一、条件判断语句if

## (一) if 语句

```javascript
//条件成立时,执行代码
if(条件表达式){
	//条件成立时执行的代码块
}

var age = prompt('请输入年龄');
if(age > 18){
  alert('你是成年人');
}
```

## (二) if ... else...语句

```javascript
//条件成立,执行if里的代码块,不成立则执行else里的代码块
if (条件表达式){
	//条件成立时执行的代码块
}
else{
	//条件不成立时执行的代码块
}

var year = prompt('请输入年份');
if (year % 400 == 0 || year % 4 == 0 && year % 100 != 0)
    console.log(year + "年是闰年");
else
    console.log(year + "年不是闰年");
```

## (三) if ... else if ... else...语句

```javascript
if(条件表达式1)
	语句1;
else if(条件表达式2)
	语句2;
else if(条件表达式3)
	语句3;
else
	以上条件均不成立时执行的语句;

var score = prompt('输入成绩');
if (score >= 90){
    console.log("优秀");
} else if (score >= 80){
    console.log("良好");
} else if (score >= 70){
    console.log("中等");
} else if (score >= 60){
    console.log("及格");
} else {
    console.log("不及格");
}
```

# 二、条件分支语句 switch...case...

```javascript
switch(表达式){
	case value1:
		//代码块
		break;
	case value2:
		//代码块
		break;
	default:
		//表达式不等于以上任何值
}
```

*   如果存在匹配全等(===) ，则与该 case 关联的代码块会被执行，并在遇到 break 时停止，整个 switch 语句代码执行结束
*   如果所有的 case 的值都和表达式的值不匹配，则执行 default 里的代码
*   执行case 里面的语句时，如果没有break，则继续执行下一个case里面的语句

```javascript
var num = 10;
switch(num){
    case 10:
        console.log("num等于10");
    case 20:
        console.log("num等于20");
        break;
    default:
        console.log("num不等于10或20");
        break;
}

/*
case 10没有break,继续执行下一个case里的语句
num等于10
num等于20
*/

var num = 10;
switch(num){
    case '10':
        console.log("num等于10");
		break;
    case 20:
        console.log("num等于20");
        break;
    default:
        console.log("num不等于10或20");
        break;
}
/*
case '10'为string类型,num为Number类型,两者的值不全等,执行default
num不等于10或20
*/
```

# 三、循环语句

## (一) for循环

```javascript
for(var index = 1; index < 10; index++){
    console.log(index);
}
```

```javascript
var sum = 0;
for(var i = 1; i <= 100; i++){
    sum += i;
}
console.log(sum); // 输出 5050
```

```javascript
for(var i = 1; i <= 5; i++){
    var star = '';
    for(var k = 1; k <= 5 - i; k++){
        star += ' '; 
    }
    for(var j = 1; j <= 2 * i - 1; j++){
        star +='*';
    }
   console.log(star);
}

/*
    *
   ***
  *****
 *******
*********
*/
```

## (二) do...while循环

```javascript
do{
	//循环体代码
}while(条件表达式);
```

1.  先执行一次循环体代码
2.  再执行表达式，如果结果为true，则继续执行循环体代码，如果为false，则退出循环，继续执行后面的代码
3.  先执行再判断循环体，所以do while循环语句至少会执行一次循环体代码

```javascript
do{
  var inputValue = prompt('90+10=?');
}while(inputValue !== '100');
alert("100分");
```

## (三) while 循环

```javascript
while(条件表达式){
	//循环体代码
}
```

1.  先执行条件表达式，如果结果为 true，则执行循环体代码；如果为 false，则退出循环，执行后面代码
2.  循环体代码执行完毕后，程序会继续判断执行条件表达式，如条件仍为true，则会继续执行循环体，直到循环条件为 false 时，整个循环过程才会结束
3.  while循环体代码可能会执行0次

```javascript
var sum = 0;
var i = 1;
while(i <= 100){
    sum += i;
    i++;
}
console.log(sum); // 输出 5050
```

# 四、continue

continue 关键字用于立即跳出本次循环，继续下一次循环（本次循环体中 continue 之后的代码就会少执行一次）。

```javascript
for(var i = 1; i <= 5; i++){
    if (i === 3){
        console.log("We are skipping 3.");
        continue;
    }
    console.log("Number " + i);
}

/*
Number 1
Number 2
We are skipping 3.
Number 4
Number 5
*/
```

# 五、break

break 关键字用于立即跳出整个循环

```javascript
for(var i = 1; i <= 5; i++){
    if (i === 3){
        console.log("We are skipping 3.");
        break;
    }
    console.log("Number " + i);
}

/*
Number 1
Number 2
We are skipping 3.
*/
```

# 六、代码块

*   程序由一条条语句组成，按照自上往下顺序执行
*   在JS中可以使用{ }为语句进行分组，同一个{ }内的语句要么都执行，要么都不执行，也称为代码块，在代码块后边可以不加；
*   JS中代码块只具有分组作用，代码块内的内容，在外部是完全可见的

```javascript
{
    var i = 0;
    console.log(i++);
}
console.log(i);

/*
0
1
*/
```
