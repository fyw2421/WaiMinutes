---
title: "JavaNotes02 : 快速入门"
weight: 3
description: "开发环境搭建、第一个程序、程序结构、八种基本类型与类型转换、字符串、流程控制、数组操作与命令行参数，含完整可运行示例。"
featureimage: "covers/java-basics.svg"
date: 2026-10-13
tags: ["Java"]
---

# 一、搭建开发环境

## (一) 选版本与安装 JDK

Java 每半年发一个版本，但**只有其中一部分是 LTS（长期支持）**，其余版本的支持周期很短。学新特性可以尝鲜，生产环境应当只上 LTS。

| 版本 | 类型 | 说明 |
| --- | --- | --- |
| 25 | LTS | 当前最新 LTS，支持到 2031 年 |
| 21 | LTS | 虚拟线程正式版，生态已成熟 |
| 17 | LTS | 不少框架以它为兼容基线 |
| 11 | LTS | 老项目常见的基线版本 |
| 8 | LTS | 仅供维护已无法升级的老系统 |

**新项目直接选 25**；已有项目跟随它既有的基线版本，不要为了"追新"去升级——本教程涉及的新语法在 17 及以上都能用。

发行版推荐 [Eclipse Temurin](https://adoptium.net/temurin/releases/)（Adoptium）：免费、跨平台、LTS 版本齐全。也可用 Oracle 官方发行版，见 [dev.java/download](https://dev.java/download/)；JDK 17 起用于生产的许可条款需要留意。

安装时**要装 JDK，不是 JRE**：

- **JDK**（Java Development Kit）含 `javac` 编译器，开发与运行都需要
- **JRE**（Java Runtime Environment）只有虚拟机，不含 `javac`，**只能运行不能编译**

装完在终端验证，**两条命令都要有输出**：

```bash
java -version     # 运行时
javac -version    # 编译器
```

如果 `java` 正常但 `javac` 报"不是内部或外部命令"，说明 PATH 里少了 JDK 的 `bin` 目录——几乎所有"能运行不能编译"的怪问题都出在这里。

### 环境变量

| 变量 | 作用 |
| --- | --- |
| `JAVA_HOME` | 指向 JDK 安装的**根目录**（不是 `bin` 目录） |
| `PATH` | 需包含 `%JAVA_HOME%\bin`（Windows）或 `$JAVA_HOME/bin`（macOS / Linux） |

多数安装器会自动配好。需要手动设置时，Windows 上最常见的错误是把 `JAVA_HOME` 指到 bin 目录——后面拼路径就成了 `bin\bin`，命令依然找不到。

macOS / Linux 用包管理器更省事：

```bash
brew install --cask temurin@21        # macOS
sudo apt install openjdk-21-jdk       # Debian / Ubuntu
```

### 装了多个 JDK 时

`java -version` 显示的未必是你以为的那一个——它只按 PATH 的**先后顺序**取第一个。终端里临时切版本可以直接写完整路径：

```bash
"/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home/bin/java" -version
```

想长期管理多版本，用 [SDKMAN!](https://sdkman.io/)（macOS / Linux）或同类的版本管理器切换，避免反复改系统环境变量。

### 选一个 IDE

命令行适合"改一行、跑一下"的小程序，但代码超过几百行、开始有多个文件时，没有 IDE 会很痛苦：**没有补全、没有跳转、找不到引用、改错名字只能靠编译报错告诉你**。

| IDE | 适合 | 说明 |
| --- | --- | --- |
| **IntelliJ IDEA** | Java 主力开发 | 社区版功能已足够写 Java（静态检查、调试、重构都有），写 Spring/数据库功能更强 |
| **VS Code** | 轻量、习惯通用编辑器 | 靠扩展获得 Java 支持，配置略繁琐，但界面熟悉、启动快 |
| **Eclipse** | 维护老项目 | 存量项目常是它，现在的新项目不推荐 |

**IDEA 社区版不需要付费**就能完成本章及后续所有内容，不要一上来就买 Ultimate。

无论用哪个，**IDE 只是编辑器**——它背后调用的还是你装的那个 JDK 的 `javac`。IDE 能编译成功但命令行 `javac` 失败，通常说明**两者用的不是同一个 JDK**（IDE 自带或另配了一个）。这是新手最常见的"代码没错却跑不起来"，排查时先看 IDE 设置里的 JDK 路径。

### VS Code 快速上手

1. 安装 [Extension Pack for Java](https://code.visualstudio.com/docs/languages/java)（VS Code 文档里的 Java 一节，装齐了调试与格式化插件）
2. **指定 JDK**：按 `Ctrl+Shift+P` → 输入 `Java: Configure Java Runtime` → 选自己装的那个，**不要选 IDE 内置的**
3. 用 **File → Open Folder** 打开**项目根目录**（不是单个 `.java` 文件），否则包结构和依赖都识别不了
4. `Main.java` 上点 `Run` 旁的三角，或按 `F5` 调试

### IDEA 快速上手

1. 首次启动选 **New Project → JDK**（选自己装的那个，不是 "Download JDK"）
2. 项目结构默认就是 Maven/Gradle 标准布局：源码放 `src/main/java`，测试放 `src/test/java`——**不要自创目录**，否则后续 Maven 一节会接不上
3. 右键 `Main.java` → `Run 'Main.main()'`

本教程后续所有代码都可以先在命令行验证一遍，再搬进 IDE——**能独立跑起来的代码，出了问题才知道是 IDE 配置还是代码本身**。

## (二) 第一个程序

```java
public class Main {
    public static void main(String[] args) {
        System.out.println("Hello, Java!");
    }
}
```

```bash
javac Main.java    // 生成 Main.class
java Main          // 运行，注意不写 .class 后缀
```

`main` 方法的签名是**固定**的，由 JVM 反射调用：

```java
public static void main(String[] args)
```

四个部分写错任何一个都启动不了，且编译器不会给出足够友好的提示——所以遇到"找不到主类"时先回头核对签名。

## (三) jshell：不用建项目就能试代码

```bash
$ jshell
jshell> int x = 10;
jshell> x * 2
$2 ==> 20
jshell> System.out.println("hi")
hi
```

`jshell` 直接连接运行中的 JVM，适合验证语法、试 API、演示教学。退出用 `/exit`。

## (四) 命令行运行参数

```java
public class Main {
    public static void main(String[] args) {
        if (args.length < 2) {
            System.err.println("用法: java Main <名称> <年龄>");
            System.exit(1);
        }
        String name = args[0];
        int age = Integer.parseInt(args[1]);
        System.out.printf("%s 今年 %d 岁%n", name, age);
    }
}
```

```bash
java Main 张三 30
```

**外部输入一律要先校验**：`args[0]` 可能抛 `ArrayIndexOutOfBoundsException`（数组越界），`Integer.parseInt` 可能抛 `NumberFormatException`（非数字）。上面显式检查了长度，并对非零退出码作了处理。

# 二、程序结构

一个 `.java` 文件的组成：

```java
package com.example.demo;   // 1. 包声明，全小写，必须与目录结构一致

import java.util.List;     // 2. 导入，全小写

/** 3. 类声明，public 类必须与文件名同名 */
public class Main {

    // 4. 成员变量（字段）
    private int count = 0;
    private static final String PREFIX = "Main";   // 常量

    // 5. 构造方法：与类名同名、无返回类型
    public Main(int count) {
        this.count = count;      // this 指向当前对象
    }

    // 6. 实例方法
    public void run() {
        System.out.println(PREFIX + ":" + count);
    }

    // 7. 静态/实例初始化块（可选，按书写顺序执行）
    static { }
    { }
}
```

**一个文件可以有多个类，但只能有一个 `public` 类**。主类之外的辅助类可以不写 `public`，此时它们仅在本包内可见。

# 三、变量与数据类型

## (一) 八种基本类型

| 类别 | 类型 | 位宽 | 取值范围 | 默认值 |
| --- | --- | --- | --- | --- |
| 整数 | `byte` | 8 | -128 ~ 127 | 0 |
| 整数 | `short` | 16 | -32768 ~ 32767 | 0 |
| 整数 | `int` | 32 | 约 ±21亿 | 0 |
| 整数 | `long` | 64 | 约 ±922亿亿 | 0 |
| 浮点 | `float` | 32 | 约 7 位有效数字 | 0.0 |
| 浮点 | `double` | 64 | 约 15~16 位有效数字 | 0.0 |
| 字符 | `char` | 16 | 0 ~ 65535 | `'\u0000'` |
| 布尔 | `boolean` | — | true / false | false |

**不要凭直觉推算内存占用**，Java 的类型大小与 C 不同：

- `char` 占 **2 字节**（UTF-16 码元），C 语言里 `char` 占 1 字节
- `boolean` 大小由 JVM 实现决定，但**不能用它做数组下标**——大小不确定，不能取地址
- 数值字面量默认是 `int`，`long` 要加 `L` 后缀

```java
int  a = 0b1010;          // 二进制，十进制的 10
int  b = 0xFF;            // 十六进制，十进制的 255
int  c = 017;             // 八进制，十进制的 15
long d = 10_000_000L;     // 下划线仅提升可读性，不影响数值
float e = 1.5f;           // 不加 f 默认是 double
```

## (二) 成员变量有默认值，局部变量没有

```java
class Field {
    int a = 10;
    int b;              // 有默认值 0
    static int c = 20;
    static int d;       // 有默认值 0
}

class Local {
    void run() {
        int x = 5;
        // int y = x + 1;   // 编译错误：变量 y 未初始化
    }
}
```

这是**有意设计**：强制"声明即初始化"，杜绝"忘了赋值就当默认值用"这类隐蔽 bug。局部变量没有任何默认值，未赋值就编译不过。

## (三) 类型转换

```java
long big = 1234567890123L;
int small = (int) big;      // 截断：保留低 32 位，结果 1912276171

double d1 = 3 / 2;          // 1    ← 整数除法！
double d2 = 3 / 2.0;        // 1.5
double d3 = (double) 3 / 2; // 1.5
```

**整数除法是最容易踩的坑**：两个 `int` 相除时先做整数运算，结果再转 `double`，所以小数部分已经被截掉了。要得到小数结果，必须让**参与运算的一方**是浮点数。

隐式转换规则：`byte → short → int → long → float → double` 可以自动进行（范围越来越大的方向），反向必须显式强转。

```java
int i = 300;
byte b = (byte) i;    // 溢出，得到 44
```

## (四) final 与不可变

`final` 修饰局部变量表示只能赋值一次；修饰引用表示**引用不变，但对象内容可变**：

```java
final List<String> list = new ArrayList<>();
list.add("a");              // 允许：改的是对象内容
list.remove(0);             // 允许：同上
// list = new ArrayList<>();  // 编译错误：换引用
```

这就是"引用不可变"与"对象不可变"的区别。要真正不可变，需要类本身设计成不可变——所有字段 `final`、不暴露 `setter`、可变内容改为返回新对象。`String`、`Integer`、`LocalDate` 都是这样设计的。

## (五) var：类型推断

```java
var list = new ArrayList<String>();   // 推断为 ArrayList<String>
var count = 10;                       // 推断为 int
var map = new HashMap<String, Integer>();
```

`var` 只在**局部变量**上可用，字段、方法参数、返回值都不能用。推断规则是"右边能确定类型"，所以下面这些编译不过：

```java
// var x = null;              // 无法推断
// var k = {1, 2};            // 数组初始化器没有目标类型，无法推断（new int[]{1,2} 则可以）
// var f = () -> {};          // lambda 无法推断
```

注意 `var y = new int[]{1, 2};` 是**可以**编译的（推断为 `int[]`）；报错的是**没有 `new` 的裸数组初始化器**。

## (六) 字符串

`String` 是**不可变**的，任何"修改"都返回新对象：

```java
String s = "abc";
s = s + "def";                 // 生成新对象，s 指向新引用
s = s.replace("b", "B");       // 同样生成新对象
```

### 循环拼接的陷阱

编译器只对**单个表达式**内的 `+` 做优化，把 `s1 + s2 + s3` 编译成一次 `StringBuilder` 拼接。但循环里的累积属于多条语句，**不会**被优化：

```java
// 慢：每次迭代都创建新 String
String result = "";
for (int i = 0; i < 10000; i++) {
    result += i;                 // 实际是 result = new String(result + i)
}

// 快：复用同一个 StringBuilder
StringBuilder sb = new StringBuilder();
for (int i = 0; i < 10000; i++) {
    sb.append(i).append(',');
}
String fast = sb.toString();
```

### StringBuilder 与 StringJoiner

```java
StringBuilder sb = new StringBuilder(16);   // 预估容量，减少扩容
sb.append("a").append(1).append(true);

StringJoiner sj = new StringJoiner(", ", "[", "]");
sj.add("a"); sj.add("b");
String s = sj.toString();                    // [a, b]

String.join(",", "a", "b", "c");             // a,b,c
```

### 常用方法

```java
s.length();          s.isEmpty();        s.isBlank();      // Java 11+
s.charAt(0);         s.indexOf("a");     s.contains("ab");
s.substring(2, 5);   s.toUpperCase();    s.trim();          s.strip();
s.replace('a','b');  s.replaceAll("a+", "b");
s.split(",");        String.valueOf(123);  Integer.parseInt("123");
s.startsWith("ab");  s.equals(t);        s.equalsIgnoreCase(t);
```

注意 `==` 与 `equals` 的区别：`==` 比较**引用地址**，`equals` 比较**内容**。两个内容相同的字符串用 `==` 比较可能得到 `false`。

```java
String a = "hello";
String b = new String("hello");
System.out.println(a == b);        // false
System.out.println(a.equals(b));   // true
```

# 四、流程控制

## (一) 条件与 switch 表达式

```java
char grade;
if (score >= 90)       grade = 'A';
else if (score >= 60)  grade = 'B';
else                   grade = 'C';
```

Java 14 起支持 switch **表达式**，比传统的 `case ... break` 更安全（不会漏 break 导致贯穿）：

```java
String level = switch (score) {
    case 90, 95 -> "A";     // 逗号并列多个值
    case 60    -> "B";
    default    -> {          // 需要多条语句时用块
        String s = "C";
        yield s;             // 用 yield 返回值
    }
};
```

## (二) 循环

```java
for (int i = 0; i < n; i++) { }
while (cond) { }
do { } while (cond);

for (String name : names) { }   // 增强 for
```

增强 `for` **不能在循环体内直接增删集合**，会抛 `ConcurrentModificationException`。确实要边遍历边改，用迭代器：

```java
Iterator<String> it = list.iterator();
while (it.hasNext()) {
    if (it.next().isEmpty()) {
        it.remove();           // 用迭代器删除，合法
    }
}
// list.removeIf(s -> s.isEmpty());   // 更简洁的等价写法
```

## (三) break 与 continue

```java
outer:
for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
        if (j > 3) continue outer;   // 继续外层循环
        if (i > 5) break outer;      // 跳出外层循环
    }
}
```

标签只能贴在循环前，且只能配合 `break`/`continue` 使用。嵌套循环较深时它比层层 `if` 清晰，但**更好的做法通常是把内层循环抽成独立方法**——这样只需一个 `return`，没有标签。

# 五、数组操作

## (一) 声明与初始化

```java
int[] a = new int[5];          // 全 0
int[] b = {1, 2, 3};           // 字面量
int[][] c = new int[2][3];     // 二维，2 行 3 列
int[][] d = {{1,2},{3,4}};     // 二维字面量
```

数组长度**创建后不可变**，不能扩容。需要动态增长用 `ArrayList`。

```java
int[] e = new int[3];
// e[3] = 1;    // ArrayIndexOutOfBoundsException
```

## (二) 遍历

```java
for (int i = 0; i < a.length; i++) { System.out.print(a[i]); }
for (int v : a)                  { System.out.print(v); }
Arrays.stream(a).forEach(System.out::println);
```

## (三) 常用工具方法

```java
Arrays.sort(a);                        // 升序排序
Arrays.toString(a);                    // 输出 [1, 2, 3]
Arrays.asList(1, 2, 3);                // 转 List（视图，不是副本）
Arrays.copyOf(a, 10);                  // 扩容到 10，不足补 0
Arrays.copyOfRange(a, 1, 4);           // 拷贝 [1, 4)
Arrays.equals(a, b);                   // 比较内容
Arrays.fill(a, -1);                    // 全部填 -1
```

**`Arrays.asList` 返回的是固定长度的视图**，不是副本：

```java
List<Integer> fixed = Arrays.asList(1, 2, 3);
// fixed.add(4);        // UnsupportedOperationException
// fixed.set(0, 9);      // 允许（改的是原数组）

List<Integer> modifiable = new ArrayList<>(Arrays.asList(1, 2, 3));
modifiable.add(4);       // 允许
```

这个区别在实践中很容易踩：把 `Arrays.asList` 的结果当作普通 `List` 传入后续逻辑，对方一 `add` 就炸。要可变就显式包一层 `new ArrayList<>(...)`。

## (四) System.arraycopy

```java
System.arraycopy(src, srcPos, dest, destPos, length);
//             源数组，源起始，目标数组，目标起始，拷贝长度
```

参数顺序容易记错——**源在前、目标在后**，且位置参数在前、长度参数在最后。JDK 内部的 `Arrays.copyOf` 就是基于它实现的。

```java
int[] copy = new int[a.length];
System.arraycopy(a, 0, copy, 0, a.length);
// 等价于 copy = Arrays.copyOf(a, a.length);
```

它做的是**内存块复制**，不走 `clone()`/`equals` 那些钩子，因此比逐元素复制快得多——但对于基本类型数组，`Arrays.copyOf` 内部也是这么实现的。

# 六、参考资料

- [Eclipse Temurin 下载页](https://adoptium.net/temurin/releases/) — 免费跨平台的 JDK 发行版与各 LTS 版本
- [dev.java 下载页](https://dev.java/download/) — Oracle 官方 JDK 下载入口与许可说明
- [Java 官方教程：语言基础](https://docs.oracle.com/javase/tutorial/java/language_basics/index.html) — 基本类型、变量、运算符与流程控制的官方说明
- [Java 官方教程：入门](https://docs.oracle.com/javase/tutorial/java/nutsandbolts/index.html) — `main` 方法、类与对象、数组、流程控制
- [Java 语言规范 JLS（SE 22）](https://docs.oracle.com/javase/specs/jls/se22/html/index.html) — 数值转换规则（第 5.1 节）、`switch` 表达式语法（第 15.28 节）
- [Java SE 22 API 文档](https://docs.oracle.com/en/java/javase/22/docs/api/) — `String`、`StringBuilder`、`Arrays`、`StringJoiner` 的完整签名
- [dev.java](https://dev.java/learn/) — Oracle 的语言特性教程，record、switch 表达式等新语法说明
