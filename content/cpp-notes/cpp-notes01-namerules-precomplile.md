---
title: "C++Notes01 : NameRule and PreCompile"
weight: 1
description: "Variable-Names Class-and-Function-Names Selective-Compilation"
date: 2018-05-22
tags: ["C++"]
featureimage: "covers/cpp-notes01-namerules-precomplile.svg"
---
## 一、常见命名法

### 1.1 匈牙利命名法

　　基本原则是:变量名＝属性＋类型＋对象描述,其中每一对象的名称都要求有明确含义,可以取对象名字全称或名字的一部分.命名要基于容易记忆容易理解的原则.保证名字的连贯性是非常重要的.

### 1.2 Camel命名法

　　即骆驼式命名法,原因是采用该命名法的名称看起来就像骆驼的驼峰一样高低起伏.Camel命名法有两种形式:混合使用大小写字母和单词之间加下划线,例如runFast和run_fast都属于Camel命名法.

### 1.3 Pascal命名法

　　与Camel命名法类似,不过Pascal命名法的首字母为大写字母.

## 二、变量的命名规则

　　要求用“匈牙利法则”.即开头字母用变量的类型,其余部分用变量的英文意思或其英文意思的缩写,尽量避免用中文的拼音,要求单词的第一个字母应大写.即:


<center>变量名=变量类型+变量的英文意思(或缩写)</center>

### 2.1 普通变量

|    key type    | Prefix |             Sample             |
| :---: | :---: | :---: |
|     short      |   n    |              nCnt              |
| unsigned short |   w    |              wCnt              |
|      int       |   n    |              nCnt              |
|  unsigned int  |   u    |              uCnt              |
|      char      |   c    |             cName              |
|  char[](字符串)|   sz   | szName[]------>sz(string zero) |
|      long      |   l    |              lCnt              |
| unsigned long  |   ul   |             ulCnt              |
|     float      |   f    |             fValue             |
|      bool      |   b    |            bEnable             |
|     double     |   d    |             dValue             |
|     string     |  str   |            strName             |
|    pointer     |   p    |  pnCnt---->p(pointer),n(int)   |
|     array      |   a    |   anCnt[5]---a(array),n(int)   |

### 2.2 全局变量

　　以g_开头,本人不喜下划线,所以以g开头.如gnCnt

### 2.3 静态变量

　　以s_开头,本人以s开头.如snCnt.类的静态成员变量msCnt.

### 2.4 成员变量

　　以m_开头,本人以m开头,如mnCnt.

### 2.5 枚举类型/typedef

　　要求用枚举变量或其缩写做前缀.并且要求用大写.

例如:

```cpp
enum EMDAYS{
    EMDAYS_MONDAY;
    EMDAYS_TUESDAY;
};
```

### 2.6 结构体/联合体/typedef

　　要求定义的类型用大写,并要加上前缀,其内部变量的命名规则与变量命名规则一致.

**结构体一般用S开头**

```cpp
struct SPOINT{
    int nX;
    int nY;
};
```

**联合体一般用U开头**

```cpp
union UPOINT{
    long lX;
    long lY;
};
```

### 2.7 常量

　　常量名由类型前缀＋全大写字母组成,单词间通过下划线来界定,如:cDELIMITER, nMAX_BUFFER ....

**类型前缀的定义与变量命名规则中的相同**

### 2.8 宏

　　全大写字母组成,单词间通过下划线来界定,本人习惯以DEF开头如:DEF_ERROR_UNKNOWN

## 三、类/类成员函数的命名

### 3.1 类的命名

　　类的名称都要以大写字母“C”开头,后跟一个或多个单词.为便于界定,每个单词的首字母要大写.类的命名推荐用"名词"或"形容词＋名词"的形式,例如:"CAnalyzer", "CFastVector" ....

### 3.2 成员函数的命名

　　保护成员函数的开头应当加上一个下划线“_”以示区别,例如:"_SetState()" ....

### 3.3 私有成员函数

　　类似地,私有成员函数的开头应当加上两个下划线“__”,例如:"__DestroyImp()" ....

### 3.4 虚函数

　　虚函数习惯以“Do”开头,如:"DoRefresh()", "_DoEncryption()" ....

### 3.5 回调和事件处理函数

　　回调和事件处理函数习惯以单词“On”开头.例如:"_OnTimer()", "OnExit()" ....

## 四、命名Sample

```cpp
#ifndef _CSAMPLE_H_
#define _CSAMPLE_H_
#define SAMPLE_FAMILYNAME_LEN 10
int gnChildNum = 0 ;
const char *gpcFATHER_NAME = "Wang";
const int nMAX_XXX = 5 ;
class CSample
{
public:
    CSample();
    ~CSample();
    void    setName(String strName);
    vritual int doSetWeight();
protected:
    int    _setFamiyName(const char *pcFamilyName);
    int    _getFamilyName();
private:
    int    __setFirstName();
    int    __getFirstName();
private:
    typedef struct{
        int nIncome;
        int nStock;
    }SProperty;
    typedef enum{
        EMDAYSPLAN_MEETING,
        EMDAYSPLAY_APOINTMENT,
    }EMDAYSPLANS;
private:
    short    mnFamilyNameLen;
    int      mnFirstNameLen;
    char     mcSex;
    char     mszFamilyName[ SAMPLE_FAMILYNAME_LEN ];
    EMDAYSPLANS   meDaysPlans;
    SProperty     mSProperty;
    char          *mpszFamilyMember[ 3 ];
};
#endif
```

## 五、预处理命令

### 5.1 宏

```cpp
#define NAME //宏定义
#undef NAME  //取消宏定义
```

### 5.2 条件编译

```cpp
//style1
#ifdef MacroID
...
#else
...
#endif

//style2
#if expression
...
#elif expression
...
#endif

//style3头文件卫士,防止头文件被多次引用
#ifndef MacroID
#define MacroID
...file content
#endif
```

**Sample01**

```cpp
//表达式选择编译
#define GAME_SELECTOR DLG223

const char* command_args[] = {
#if GAME_SELECTOR == DLG223
    "DLG223",
#elif GAME_SELECTOR == DLG234
    "DLG234",
#elif GAME_SELECTOR == DLG229
    "dlg229_tv",
#elif GAME_SELECTOR == GINIPIANO_GAME1
    "Game1_Stage",
    "0",
    "0"
#elif GAME_SELECTOR == PREVENT_JAMSESSION
    "JamSession",
#elif GAME_SELECTOR == GINIPIANO_FIREWORK
    "Firework",
#endif

//宏定义选择编译1
#define DLG234

#if defined(DLG143)
    SingStage stage;
#elif defined(DLG223)
    G223Stage stage;
#elif defined(DLG234)
    G234Stage stage;
#elif defined(DLG127)
    SpaceStage stage;
#endif
};

//宏定义选择编译2
#if defined(  TARGET_ANDROID ) || defined( TARGET_PC )
    mbReturnValue = VFALSE;
#else
    meState = GS_EXIT;
#endif

//宏定义选择编译3
#ifdef TARGET_ANDROID
...
#endif
```

### 5.3 文件选择包含

根据宏定义来确定包含的文件名,如

```cpp
//us
#include "g00/BUILDEROBJ_LIST.H"

//uk
#include "g03/BUILDEROBJ_LIST.H"

//fr
#include "g05/BUILDEROBJ_LIST.H"
```

实现方法如下:

1. location.h定义数字标识符

   ```cpp
   //location.h
   #ifndef __LOCALIZATION_H
   #define __LOCALIZATION_H

   #define LOC_USeng 00
   #define LOC_GBeng 03
   #define LOC_FRfre 05

   #endif
   ```

2. GameMacroDef.h进行字符链接

   ```cpp
   //GameMacroDef.h
   #ifndef _GAME_MACRO_DEF_H
   #define _GAME_MACRO_DEF_H

   #include "Include/localization.h"

   //====identifier Concat and Convert MacroDefine Begin====

   #define $macroString(str)         #str
   #define $macroString2(aa,bb)      $macroString(aa##bb)
   #define $macroString3(aa,bb,cc)   $macroString(aa##bb##cc)
   #define $macroConcat(aa,bb)       aa##bb

   #define MacroSTR_1(aa)            $macroString(aa)
   #define MacroSTR_2(aa,bb)         $macroString2(aa,bb)
   #define MacroSTR_3(aa,bb,cc)      $macroString3(aa,bb,cc)
   #define MacroCONCAT(aa,bb)        $macroConcat(aa,bb)
   //====identifier Concat and Convert MacroDefine End====

   #define LOCALIZATION LOC_USeng

   #define MacroHeaderFileStr(typePrefix,fileName) MacroSTR_2(typePrefix ,LOCALIZATION/fileName )
   ...

   #endif
   ```

   `#`和`##`

   * `#`的主要作用是将宏参数不经扩展地转换成字符串常量.

   * `##`将两个参数链接起来,数值依然是数值

      ```cpp
     #define F abc
     #define B def
     #define FB(arg) #arg
     #define FB1(arg) FB(arg)

     FB(F B) // "F B"
     FB1(F B) //"abc def"
     ```

     凡是宏定义里有用#或##的地方宏参数是不会再展开,需要多加一层中间转换宏.

     $macroString2(aa,bb) //将为 "gLOCALIZATION/BUILDEROBJ_LIST.H”

3. TheApp.h里include

   ```cpp
   ...
   #include MacroHeaderFileStr( g , BUILDEROBJ_LIST.H  )
   #include MacroHeaderFileStr( g , RESOURCE.H  )
   #include MacroHeaderFileStr( s , _SENTENCE_RES.h  )
   ...
   //效果#include “g00/BUILDEROBJ_LIST.H”
   //#include “g00/RESOURCE.H”
   //改变LOCALIZATION的宏定义即可变为g03/g05
   ```



