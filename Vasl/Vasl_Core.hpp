#ifndef __VASL_CORE_HPP__
#define __VASL_CORE_HPP__

/** ================================================================================ *//**
*  @brief      头文件区
* -------------------------------------------------------------------------------- */
///< Vasl核心头文件
#include "Vasl_Layer.hpp"               ///< 平台兼容层
#include "Vasl_Macro.h"                 ///< 宏定义
#include "Vasl_Type.hpp"                ///< 自定类型定义
#include "Tools/Vasl_Tools.hpp"         ///< 实用工具函数
#include "Button/Vasl_SSKey.hpp"        ///< 超级简单按键类
#include "Struct/Vasl_RTDataPacket.hpp" ///< 收发数据包类
#include "Struct/Vasl_Queue.hpp"        ///< 队列类
#include "Struct/Vasl_RingBuf.hpp"      ///< 环形缓冲区类
#include "Struct/Vasl_Array.hpp"        ///< 数组容器
#include "Strxx/Vasl_Strxx.hpp"         ///< 字符串操作类
#include "NLC/Vasl_NLC.hpp"             ///< 自然语言命令核心
#include "Loger/Vasl_LogerMacro.hpp"    ///< 详细日志宏定义
#include "Interrupt/Vasl_Interrupt.hpp" ///< 中断处理

///< Vasl强依赖库头文件
#include "./NanoPrintf/Vasl_Vsnprintf.h"///< 格式化输出库

///< Cxx库头文件
#include <cstdarg>                      ///< 可变参数函数

///< 平台相关头文件
#include "hal_data.h"                   ///< 平台相关
// #include "arm_math.h"                   ///< ARM数学库

///< 可选Lib库头文件
#include "Class/Vasl_LED.hpp"           ///< LED类
#include "Class/Vasl_Printer.hpp"       ///< 打印类
#include "STimer/Vasl_STimer.hpp"       ///< 软件定时器
#include "OLED/SSD1306_S/OLEDv3.hpp"    ///< 软件OLED驱动库
#include "PT/Vasl_PT.hpp"               ///< 简易协程库
#include "EasyMenu/Vasl_EasyMenu.hpp"   ///< 简易菜单库
#include "etl/atomic.h"                 ///< etl原子操作库
// #include "etl/array_view.h"             ///< etl数组视图库

#endif /* __VASL_CORE_HPP__ */
