---
title: "十四、循环"
weight: 14
description: "本章介绍 循环。"
date: 2026-09-28
tags: ["TypeScript"]
featureimage: "covers/typescript-tutorial.svg"
---

有的时候，我们可能需要多次执行同一块代码。一般情况下，语句是按顺序执行的：函数中的第一个语句先执行，接着是第二个语句，依此类推。

编程语言提供了更为复杂执行路径的多种控制结构。

循环语句允许我们多次执行一个语句或语句组，下面是大多数编程语言中循环语句的流程图：

循环的关键点是循环可能一次都不会执行。当条件为 false 时，会跳过循环主体，直接执行紧接着 while 循环的下一条语句。

## (一) 实例

编译以上代码得到如下 JavaScript 代码：

执行以上 JavaScript 代码，输出结果为：

```typescript
5 的阶乘为：120
```

## (二) do...while 循环

不像 **for** 和 **while** 循环，它们是在循环头部测试循环条件。**do...while** 循环是在循环的尾部检查它的条件。

### 1. 语法

语法格式如下所示：

```typescript
do
{
   statement(s);
}while( condition );
```

请注意，条件表达式出现在循环的尾部，所以循环中的 statement(s) 会在条件被测试之前至少执行一次。

如果条件为 true，控制流会跳转回上面的 do，然后重新执行循环中的 statement(s)。这个过程会不断重复，直到给定条件变为 false 为止。

### 2. 流程图

### 3. 实例

编译以上代码得到如下 JavaScript 代码：

执行以上 JavaScript 代码，输出结果为：

```typescript
10
9
8
7
6
5
4
3
2
1
0
```

## (三) break 语句

**break** 语句有以下两种用法：

- 当 **break** 语句出现在一个循环内时，循环会立即终止，且程序流将继续执行紧接着循环的下一条语句。
- 它可用于终止 **switch** 语句中的一个 case。

如果您使用的是嵌套循环（即一个循环内嵌套另一个循环），break 语句会停止执行最内层的循环，然后开始执行该块之后的下一行代码。

### 1. 语法

语法格式如下所示：

```typescript
break;
```

### 2. 流程图

### 3. 实例

编译以上代码得到如下 JavaScript 代码：

执行以上 JavaScript 代码，输出结果为：

```typescript
在 1~10 之间第一个被 5 整除的数为 : 5
```

## (四) continue 语句

**continue** 语句有点像 **break** 语句。但它不是强制终止，continue 会跳过当前循环中的代码，强迫开始下一次循环。

对于 **for** 循环，**continue** 语句执行后自增语句仍然会执行。对于 **while** 和 **do...while** 循环，**continue** 语句重新执行条件判断语句。

### 1. 语法

语法格式如下所示：

```typescript
continue;
```

### 2. 流程图

### 3. 实例

编译以上代码得到如下 JavaScript 代码：

执行以上 JavaScript 代码，输出结果为：

```typescript
0 ~20 之间的奇数个数为: 10
```

## (五) 无限循环

无限循环就是一直在运行不会停止的循环。 for 和 while 循环都可以创建无限循环。

for 创建无限循环语法格式：

```typescript
for(;;) { 
   // 语句

}
```

实例

```typescript
for(;;) { 
   console.log("这段代码会不停的执行") 
}
```

while 创建无限循环语法格式：

```typescript
while(true) { 
   // 语句

} 
```

实例

```typescript
while(true) { 
   console.log("这段代码会不停的执行") 
}
```
