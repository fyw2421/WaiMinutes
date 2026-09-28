---
title: "二十一、Number"
weight: 21
description: "本章介绍 Number。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

TypeScript 与 JavaScript 类似，支持 Number 对象。

在 TypeScript 中，Number 对象用于包装数值类型。

Number 对象是原始数值的包装对象。

类似于 String 对象，Number 对象是引用类型，与基本的 number 类型有所不同。

尽管 Number 对象提供了一些额外的属性和方法，但在 TypeScript 中更推荐直接使用基本的 number 类型，因为 Number 对象会带来性能开销和类型混淆。

## (一) 语法

```typescript
var num = new Number(value);
```

需要注意的是，这会创建一个引用类型的对象，而非基本的 number 类型。

**注意：** 如果一个参数值不能转换为一个数字将返回 NaN (非数字值)。

## (二) Number 对象与基本 number 类型的区别

- **基本类型 `number`**：原始数据类型，用于存储数值。
- **`Number` 对象**：引用类型，是一个包装对象，用于包装基本数值。

## (三) Number 对象属性

下表列出了 Number 对象支持的属性：

| 序号 | 属性 & 描述 |
|---|---|
| 1. | **MAX_VALUE** 可表示的最大的数，MAX_VALUE 属性值接近于 1.79E+308。大于 MAX_VALUE 的值代表 "Infinity"。 |
| 2. | **MIN_VALUE** 可表示的最小的数，即最接近 0 的正数 (实际上不会变成 0)。最大的负数是 -MIN_VALUE，MIN_VALUE 的值约为 5e-324。小于 MIN_VALUE ("underflow values") 的值将会转换为 0。 |
| 3. | **NaN** 非数字值（Not-A-Number）。 |
| 4. | **NEGATIVE_INFINITY** 负无穷大，溢出时返回该值。该值小于 MIN_VALUE。 |
| 5. | **POSITIVE_INFINITY** 正无穷大，溢出时返回该值。该值大于 MAX_VALUE。 |
| 6. | **prototype** Number 对象的静态属性。使您有能力向对象添加属性和方法。 |
| 7. | **constructor** 返回对创建此对象的 Number 函数的引用。 |

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
TypeScript Number 属性:
最大值为: 1.7976931348623157e+308
最小值为: 5e-324
负无穷大: -Infinity
正无穷大:Infinity
```

## (四) NaN 实例

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
月份是：NaN
```

## (五) prototype 实例

编译以上代码，得到以下 JavaScript 代码：

输出结果为：

```typescript
员工号: 123
员工姓名: admin
员工邮箱: admin@runoob.com
```

## (六) Number 对象方法

Number对象 支持以下方法：

| 序号 | 方法 & 描述 | 实例 |
|---|---|---|
| 1. | toExponential() 把对象的值转换为指数计数法。 | 示例 1 |
| 2. | toFixed() 把数字转换为字符串，并对小数点指定位数。 | 示例 2 |
| 3. | toLocaleString() 把数字转换为字符串，使用本地数字格式顺序。 | 示例 3 |
| 4. | toPrecision() 把数字格式化为指定的长度。 | 示例 4 |
| 5. | toString() 把数字转换为字符串，使用指定的基数。数字的基数是 2 ~ 36 之间的整数。若省略该参数，则使用基数 10。 | 示例 5 |
| 6. | valueOf() 返回一个 Number 对象的原始数字值。 | 示例 6 |
| 类型本质 | 原始值 | 引用类型（对象） |
| 性能 | 高效，无额外内存开销 | 低效，创建对象实例 |
| 类型校验（TypeScript） | 符合 TS 基础类型规范 | 类型不匹配（如 string 类型变量无法赋值 String 对象） |
| 比较方式 | 直接比较值 | 比较引用地址（需用 valueOf() 取原始值） |
| 1. | constructor 对创建该对象的函数的引用。 | 输出结果： 示例 7 示例 8 |
| 2. | length 返回字符串的长度。 | 示例 9 |
| 3. | prototype 允许您向对象添加属性和方法。 | 示例 10 |

**示例 1**

```typescript
//toExponential() 

var num1 = 1225.30 
var val = num1.toExponential(); 
console.log(val) // 输出： 1.2253e+3
```

**示例 2**

```typescript
var num3 = 177.234 
console.log("num3.toFixed() 为 "+num3.toFixed())    // 输出：177

console.log("num3.toFixed(2) 为 "+num3.toFixed(2))  // 输出：177.23

console.log("num3.toFixed(6) 为 "+num3.toFixed(6))  // 输出：177.234000
```

**示例 3**

```typescript
var num = new Number(177.1234); 
console.log( num.toLocaleString());  // 输出：177.1234
```

**示例 4**

```typescript
var num = new Number(7.123456); 
console.log(num.toPrecision());  // 输出：7.123456 

console.log(num.toPrecision(1)); // 输出：7

console.log(num.toPrecision(2)); // 输出：7.1
```

**示例 5**

```typescript
var num = new Number(10); 
console.log(num.toString());  // 输出10进制：10

console.log(num.toString(2)); // 输出2进制：1010

console.log(num.toString(8)); // 输出8进制：12
```

**示例 6**

```typescript
var num = new Number(10); 
console.log(num.valueOf()); // 输出：10
```

**示例 7**

```typescript
var str = new String( "This is string" ); 
console.log("str.constructor is:" + str.constructor)
```

**示例 8**

```typescript
str.constructor is:function String() { [native code] }
```

**示例 9**

```typescript
var uname = new String("Hello World") 
console.log("Length "+uname.length)  // 输出 11
```

**示例 10**

```typescript
function employee(id:number,name:string) { 
    this.id = id 
    this.name = name 
 } 
 var emp = new employee(123,"admin") 
 employee.prototype.email="admin@runoob.com" // 添加属性 email

 console.log("员工号: "+emp.id) 
 console.log("员工姓名: "+emp.name) 
 console.log("员工邮箱: "+emp.email)
```

### 1. String 方法

下表列出了 String 对象支持的方法：

| 序号 | 方法 & 描述 | 实例 |
|---|---|---|
| 1. | charAt() 返回在指定位置的字符。 | 示例 1 |
| 2. | charCodeAt() 返回在指定的位置的字符的 Unicode 编码。 | 示例 2 |
| 3. | concat() 连接两个或更多字符串，并返回新的字符串。 | 示例 3 |
| 4. | indexOf() 返回某个指定的字符串值在字符串中首次出现的位置。 | 示例 4 |
| 5. | lastIndexOf() 从后向前搜索字符串，并从起始位置（0）开始计算返回字符串最后出现的位置。 | 示例 5 |
| 6. | localeCompare() 用本地特定的顺序来比较两个字符串。 | 示例 6 |
| 7. | **match()** 查找找到一个或多个正则表达式的匹配。 | 示例 7 |
| 8. | replace() 替换与正则表达式匹配的子串 | 示例 8 |
| 9. | search() 检索与正则表达式相匹配的值 | 示例 9 |
| 10. | slice() 提取字符串的片断，并在新的字符串中返回被提取的部分。 | 11. |
| split() 把字符串分割为子字符串数组。 | 示例 10 | 12. |
| substr() 从起始索引号提取字符串中指定数目的字符。 | 13. | substring() 提取字符串中两个指定的索引号之间的字符。 |
| 示例 11 | 14. | toLocaleLowerCase() 根据主机的语言环境把字符串转换为小写，只有几种语言（如土耳其语）具有地方特有的大小写映射。 |
| 示例 12 | 15. | toLocaleUpperCase() 据主机的语言环境把字符串转换为大写，只有几种语言（如土耳其语）具有地方特有的大小写映射。 |
| 示例 13 | 16. | toLowerCase() 把字符串转换为小写。 |
| 示例 14 | 17. | toString() 返回字符串。 |
| 示例 15 | 18. | toUpperCase() 把字符串转换为大写。 |
| 示例 16 | 19. | valueOf() 返回指定字符串对象的原始值。 |
| 示例 17 |  |  |

**示例 1**

```typescript
var str = new String("RUNOOB"); 
console.log("str.charAt(0) 为:" + str.charAt(0)); // R

console.log("str.charAt(1) 为:" + str.charAt(1)); // U 

console.log("str.charAt(2) 为:" + str.charAt(2)); // N 

console.log("str.charAt(3) 为:" + str.charAt(3)); // O 

console.log("str.charAt(4) 为:" + str.charAt(4)); // O 

console.log("str.charAt(5) 为:" + str.charAt(5)); // B
```

**示例 2**

```typescript
var str = new String("RUNOOB"); 
console.log("str.charCodeAt(0) 为:" + str.charCodeAt(0)); // 82

console.log("str.charCodeAt(1) 为:" + str.charCodeAt(1)); // 85 

console.log("str.charCodeAt(2) 为:" + str.charCodeAt(2)); // 78 

console.log("str.charCodeAt(3) 为:" + str.charCodeAt(3)); // 79 

console.log("str.charCodeAt(4) 为:" + str.charCodeAt(4)); // 79

console.log("str.charCodeAt(5) 为:" + str.charCodeAt(5)); // 66
```

**示例 3**

```typescript
var str1 = new String( "RUNOOB" ); 
var str2 = new String( "GOOGLE" ); 
var str3 = str1.concat( str2 ); 
console.log("str1 + str2 : "+str3) // RUNOOBGOOGLE
```

**示例 4**

```typescript
var str1 = new String( "RUNOOB" ); 

var index = str1.indexOf( "OO" ); 
console.log("查找的字符串位置 :" + index );  // 3
```

**示例 5**

```typescript
var str1 = new String( "This is string one and again string" ); 
var index = str1.lastIndexOf( "string" );
console.log("lastIndexOf 查找到的最后字符串位置 :" + index ); // 29

    
index = str1.lastIndexOf( "one" ); 
console.log("lastIndexOf 查找到的最后字符串位置 :" + index ); // 15
```

**示例 6**

```typescript
var str1 = new String( "This is beautiful string" );
  
var index = str1.localeCompare( "This is beautiful string");  

console.log("localeCompare first :" + index );  // 0
```

**示例 7**

```typescript
var; 
var n=str.match(/ain/g);  // ain,ain,ain
```

**示例 8**

```typescript
var re = /(\w+)\s(\w+)/; 
var str = "zara ali"; 
var newstr = str.replace(re, "$2, $1"); 
console.log(newstr); // ali, zara
```

**示例 9**

```typescript
var re = /apples/gi; 
var str = "Apples are round, and apples are juicy.";
if (str.search(re) == -1 ) { 
   console.log("Does not contain Apples" ); 
} else { 
   console.log("Contains Apples" ); 
} 
```

**示例 10**

```typescript
var str = "Apples are round, and apples are juicy."; 
var splitted = str.split(" ", 3); 
console.log(splitted)  // [ 'Apples', 'are', 'round,' ]
```

**示例 11**

```typescript
var str = "RUNOOB GOOGLE TAOBAO FACEBOOK"; 
console.log("(1,2): "    + str.substring(1,2));   // U

console.log("(0,10): "   + str.substring(0, 10)); // RUNOOB GOO

console.log("(5): "      + str.substring(5));     // B GOOGLE TAOBAO FACEBOOK
```

**示例 12**

```typescript
var str = "Runoob Google"; 
console.log(str.toLocaleLowerCase( ));  // runoob google
```

**示例 13**

```typescript
var str = "Runoob Google"; 
console.log(str.toLocaleUpperCase( ));  // RUNOOB GOOGLE
```

**示例 14**

```typescript
var str = "Runoob Google"; 
console.log(str.toLowerCase( ));  // runoob google
```

**示例 15**

```typescript
var str = "Runoob"; 
console.log(str.toString( )); // Runoob
```

**示例 16**

```typescript
var str = "Runoob Google"; 
console.log(str.toUpperCase( ));  // RUNOOB GOOGLE
```

**示例 17**

```typescript
var str = new String("Runoob"); 
console.log(str.valueOf( ));  // Runoob
```

### 2. String 对象的使用建议

在 TypeScript 中，使用 String 对象通常是不必要的，直接使用 string 字面量会更高效且符合 TypeScript 的最佳实践：

- **性能**：`String` 对象是一个引用类型，会占用更多内存，且每次创建一个新对象性能开销更大。
- **类型安全**：TypeScript 更鼓励使用 `string` 字面量类型，保持代码的简洁和一致性。

如果确实需要使用 String 对象的方法，可以通过 valueOf() 方法将对象转为原始字符串，然后继续处理。

通常情况下，TypeScript 推荐直接使用 string 字面量类型，以简化代码，提高性能，避免不必要的类型转换和复杂性。
