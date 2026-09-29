---
title: "Doxgen 注释规范"
weight: 9
description: "Doxygen 风格的 C/C++ 注释规范：头文件、函数、行注释、枚举/结构体、模块定义及关键字说明"
date: 2026-09-28
tags: ["software-config"]
featureimage: "covers/Doxgen注释规范.svg"
---

# 一、头文件注释

```cpp
/** 
* @brief 摘要
* @file 文件名
* @author 作者
* @version 版本号
* @date 你啥时候搞的
* @note 注解
* @since 自从
*/
```

示例

```cpp
/** @file  main.c
* @brief       项目主函数文件
* @details  主要包括协议应用栈程序框架，main函数入口
* @author      wanghuan  any question please send mail to 371463817@qq.com
* @date        2018-8-17
* @version     V1.0
* @copyright   Copyright (c) 2018-2020  江苏亨通光网科技有限公司
**********************************************************************************
* @attention
* 硬件平台: nRF52832_QFAA \n
* SDK版本：nRF5_SDK_15.0.0
* @par 修改日志:
* <table>
* <tr><th>Date        <th>Version  <th>Author    <th>Description
* <tr><td>2018/08/17  <td>1.0      <td>wanghuan  <td>创建初始版本
* </table>
*
**********************************************************************************
*/
```

# 二、函数注释

```cpp
/**
* @brief 这里写这个函数是干什么用的
* @param i1[in] 输入参数1
* @param i2[in] 输入参数2
* @param o3[out] 输出参数1
* @return 返回值解释一下
* @warning 警告: 例如: 参数不能为空
* @note 注解
* @see 相当于是请参考xxoo函数之类的
*/
```

示例:

```cpp
/**
* @brief        can send the message
* @param[in]    canx : The Number of CAN
* @param[in]    id : the can id
* @param[in]    p : the data will be sent
* @param[in]    size : the data size
* @param[in]    is_check_send_time : is need check out the time out
* @note         Notice that the size of the size is smaller than the size of the buffer.
* @return
*   +1 Send successfully \n
*   -1 input parameter error \n
*   -2 canx initialize error \n
*   -3 canx time out error \n
* @par Sample
* @code
*   u8 p[8] = {0};
*   res_ res = 0;
*   res = can_send_msg(CAN1,1,p,0x11,8,1);
* @endcode
*/
extern s32 can_send_msg(const CAN_TypeDef * canx,
            const u32 id,
            const u8 *p,
            const u8 size,
            const u8 is_check_send_time);
```

# 三、行注释

```cpp
/**< 在这里写你要加的东西 */
///< 在这里写你要加的东西
```

示例

```cpp
#define ADDRESS (0x20000000) ///< address

/**  学生结构体 */
struct Student {
    char name;///< 学生姓名
    int number;///< 学生学号
} Str_Student;
```

# 四、枚举/结构体注释

```cpp
/** @enum NB_msg_types_t
* @brief 定义驱动上报应用消息类型
*/

/** @struct ME3617_info_t
* @brief ME3617信息结构体 \n
* 定义存储ME3617的信息
*/

typedef struct 结构体名字
{
    成员1, ///< 简要说明文字 */ 如果不加<，则会认为是成员2的注释
    成员2, ///< 简要说明文字
    成员3, ///< 简要说明文字
}结构体别名；
```

# 五、组/模块定义

```cpp
/**
 * @defgroup 定义log的级别
 * @{
 */
#define A_LEVEL 0 ///< A_LEVEL
#define B_LEVEL 1 ///< B_LEVEL
#define C_LEVEL 2 ///< C_LEVEL
#define D_LEVEL 3 ///< D_LEVEL
/**
 * @}
 */

/** @defgroup bsp_me3616 Bsp me3616 driver module.
* @{
* @ingroup bsp_drivers
* @brief 使用该驱动之前，先进行驱动句柄的实例注册. \n
* ME3616驱动支持云平台Onenet和OceanConnect \n
* 当使能GPS驱动使能时，支持GPS操作
*/

/** @} bsp_me3616*/

/** @name 协议栈用全局参数
* @brief 蓝牙5协议栈参数配置（广播、连接、安全等）相关宏定义，协议栈各模块句柄等全局参数
* @{
*/

/** @} 协议栈用全局参数 */
```

# 六、常用关键字列表

| 关键字         | 说明                                                             |
| ----------- | -------------------------------------------------------------- |
| @author     | 作者的信息                                                          |
| @brief      | 用于class 或 function 的简易说明。<br> eg：@brief 本函数负责打印错误信息串。          |
| @bug        | 被标记的代码会在 Bug 列表中出现。                                            |
| @class      | 类名。                                                            |
| @date       | 日期。                                                            |
| @file       | 文件名，可以默认为空，DoxyGen 会自己加。                                       |
| @param      | 主要用于函数说明中，后面接参数的名字，然后再接关于该参数的说明。                               |
| @return     | 描述该函数的返回值情况。<br> eg: @return 本函数返回执行结果，若成功则返回 TRUE，否则返回 FLASE。 |
| @retval     | 描述返回值类型。<br> eg:  @retval NULL 空字符串。<br>@retval !NULL 非空字符串。   |
| @note       | 注解。                                                            |
| @attention  | 注意。                                                            |
| @name       | 分组名。                                                           |
| @warning    | 警告信息。                                                          |
| @enum       | 引用了某个枚举，Doxygen 会在该枚举处产生一个链接。 <br> eg：@enum CTest::MyEnum      |
| @var        | 引用了某个变量，Doxygen 会在该枚举处产生一个链接。<br> eg：@var CTest::m\_FileKey。   |
| @class      | 引用某个类，格式：@class \[] \[] <br> eg:@class CTest "inc/class.h"     |
| @exception  | 可能产生的异常描述。<br> eg: @exception 本函数执行可能会产生超出范围的异常。               |
| @todo       | 对将要做的事情进行注释。                                                   |
| @see        | see also 字段。                                                   |
| @relates    | 通常用做把非成员函数的注释文档包含在类的说明文档中                                      |
| @since      | 从哪个版本后开始有这个函数的。                                                |
| @code       | 在注释中开始说明一段代码，直到 @endcode 命令。                                   |
| @endcode    | 在注释中代码段的结束。                                                    |
| @remarks    | 备注。                                                            |
| @pre        | 用来说明代码项的前提条件。                                                  |
| @post       | 用来说明代码项之后的使用条件。                                                |
| @deprecated | 这个函数可能会在将来的版本中取消。                                              |
| @defgroup   | 模块名。                                                           |
| @{          | 模块开始。                                                          |
| @}          | 模块结束。                                                          |
| @version    | 版本号。                                                           |
| @fn         | 声明一个函数。                                                        |
| @par        | 开始一个段落，段落名称描述由你自己指定，比如可以写一段示例代码。<br> - 一级项目符号 <br> -# 二级项目符号   |

# 七、综合示例

```cpp
//============================================================================
//  Copyright (c) 2011 - 2012,  VTech Electronics Ltd.
//  All rights reserved.
//
//File ID.  : 
//Filename  : EGameObject.h
//Brief     : This file was generated by FormatFileDes
//
//
//Version   : 1.0
//Model     : VPad
//Author    : xs_ho
//Create    : Feb-16-2011 (MDY)
//Finish    : 
//
//Keywords  : 
//$Id: EGameObject.h 445 2013-07-18 09:32:11Z xs_ho $
//Signature : [VTECH FILE DESCRIPTION]
//============================================================================

#ifndef __EGAMEOBJECT_H
#define __EGAMEOBJECT_H

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "ETypeDef.h"
#include "EMessage.h"

//#define DEBUG_EGAMEOBJECT
#ifdef  NDEBUG
#undef  DEBUG_EGAMEOBJECT
#endif

/**
	@class EGameObject
	@brief Extended GameObject class
	@note You need to inherit this class to any of your own game object class
*/
class EGameObject : public GameObject
{
	friend class EGameStage;
	friend class EMessage;
	friend class ETimer;
public:
	EGameObject();
	~EGameObject();

	/**
	@brief tell pop-up which puppets can not be paused
	@param [out] puppets vector to hold all of non-paused puppet pointer
	@return void
	*/
	virtual void getExcludedPuppets(Vector& puppets) {;}

protected:
	/**
	@brief function for game stage to execute before the instance simulation take place, it will be called once per frame.

	You can execute such as motion sensor analyse by override it.
	@param [in] time game engine run time
	@return VFALSE will cause game stage exit
	@see GameStageBase class in game engine
	@note If have no special require, you should return VTRUE when override. See GameStageBase in game engine for more information.
	*/
	virtual VBOOL preSimulate(VUINT32 time) {(void)time; return VTRUE;}

	/**
	@brief function for game stage to execute before render take place, it will be called once per frame.
	@param [in] time game engine run time
	@return VFALSE will cause game stage exit
	@see GameStageBase class in game engine
	*/
	virtual VBOOL preRender(VUINT32 time) {(void)time; return VTRUE;}

	/**
	@brief add game object into GE
	@return game object ID from GE
	@see GameObjectManager.addObject() for the detail.
	*/
	VINT32 addObject();

	/**
	@brief see GameObjectManager.removeObject() for the detail.
	*/
	void removeObject();

	/**
	@brief change the permition of being paused
	@param [in] can_be_paused specify if this game object can be paused when need.
	@return void
	*/
	void enablePaused(VBOOL can_be_paused) {mCanBePaused = can_be_paused;}

	/**
	@brief function for game stage to execute when pause required
	@return void
	*/
	virtual void pause() {return;}

	/**
	@brief function for game stage to execute when resume required
	@return void
	*/
	virtual void resume() {return;}

	/**
	@brief check if the game object has been paused
	@return VTRUE if puased, VFALSE else
	*/
	VBOOL isPaused() {return mPaused;}

	/**
	@brief Declear the message map. The EGameObject Wizard tools will generate information involved map
	*/
	DECLARE_EMESSAGE_MAP();

private:
	VBOOL mCanBePaused;
	VBOOL mPaused;
	VUINT32 mRegisterRef;
	Vector mPausedMsg;

	/**
	@brief check if the game object can be paused
	@return VTRUE if can be puased, VFALSE else
	*/
	VBOOL canBePaused() {return mCanBePaused;}

	/**
	@brief save message if the message arrives during game object is paused
	@return void
	@remark called by EMessage
	*/
	void savePausedMessage(VUINT32 msg, VUINT32 param1, VUINT32 param2);

	/**
	@brief excute messages which are held during game object is paused
	@return void
	@remark called by EGameStage
	*/
	void resumePausedMessage();
};
/** @} */ // end of EGameObject

#endif//__EGAMEOBJECT_H

```
