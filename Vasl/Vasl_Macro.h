#ifndef __VASL_MACRO_H__
#define __VASL_MACRO_H__

/** ================================================================================ *//**
 *  @brief      定义通用开关宏
 * -------------------------------------------------------------------------------- */
#define VASL_OK		(0)	            ///< 使用宏
#define VASL_NOT	(1 - VASL_OK)	///< 不使用宏
#define VASL_OPEN	(0)	            ///< 使用宏
#define VASL_CLOSE	(1 - VASL_OPEN)	///< 未使用宏
#define VASL_LOCK	h_is_locked__	///< 锁定宏

/** ================================================================================ *//**
 *  @brief      定义通用功能宏 GNU
 * -------------------------------------------------------------------------------- */
///< 函数属性宏：强制保留函数在符号表中
#define VASL_FN_USED                __attribute__((used))
///< 函数属性宏：强制内联
#define VASL_FN_ALWAYS_INLINE       __attribute__((always_inline))
///< 函数属性宏：强制不使用
#define VASL_FN_UNAVAILABLE(MSG)    __attribute__((unavailable(MSG)))
#define VASL_FN_NOT_USED            VASL_FN_UNAVAILABLE("This function has been removed")
#define VASL_FN_REMOVED             VASL_FN_NOT_USED
#define VASL_FN_NOT_USE(MSG)        VASL_FN_UNAVAILABLE(MSG)
#define VASL_FN_REMOVE(MSG)         VASL_FN_NOT_USED(MSG)
///< 函数属性宏：建议不使用
#define VASL_FN_OLDED               __attribute__((deprecated("This function is old")))
#define VASL_FN_OLD(MSG)            __attribute__((deprecated(MSG)))
///< 函数属性宏：无返回值
#define VASL_FN_NORET               __attribute__((noreturn))
///< 函数属性宏：主函数
#define VASL_FN_MAIN                __attribute__((main))
///< 函数属性宏：构造函数
#define VASL_FN_CONSTRUCTOR         __attribute__((constructor))
///< 函数属性宏：析构函数
#define VASL_FN_DESTRUCTOR          __attribute__((destructor))
///< 函数属性宏：无参数
#define VASL_FN_NOARGS              __attribute__((noargs))
#define VASL_FN_NO_PARAMS           __attribute__((noargs))
///< 函数属性宏：别名
#define VASL_FN_ALIAS(ORIG_NAME)    __attribute__((alias(ORIG_NAME)))
///< 函数属性宏：热函数
#define VASL_FN_HOT                 __attribute__((hot))
///< 函数属性宏：冷函数
#define VASL_FN_COLD                __attribute__((cold))
///< 变量属性宏：对齐
#define VASL_VAR_ALIGN(ALIGN_BYTE)  __attribute__((aligned(ALIGN_BYTE)))
///< 变量属性宏：打包
#define VASL_VAR_PACKED             __attribute__((packed))

///< 无限循环宏
#define VASL_INFLOOP        for(;;)
///< 条件暂停宏
#define VASL_STOP(IS_STOP)  while(!IS_STOP) {}
///< 循环条件宏
#define VASL_LOOP(IS_LOOP)  while(IS_LOOP)

///< VASL_PASS占位宏
#define VASL_PASS(MSG, ...)
///< VASL_NOTE占位宏
#define VASL_NOTE(MSG, ...)
///< VASL_NEXT占位宏
#define VASL_NEXT(MSG, ...)
///< VASL_CODE占位宏
#define VASL_CODE(MSG, ...)
///< VASL_NOCODE占位宏
#define VASL_NOCODE(MSG, ...)

///< 字符串化宏 *这坨SHIT似乎并没有什么用
#define VASL_TO_STRING(STR) #STR

///> const char 字符串长度
#define VASL_C_STR_LEN(STR) (sizeof STR - 1)
#define VASL_C_STR_LEN_UNSAFE(STR) (sizeof STR)

///< 换行符宏
#define VASL_NEWL "\r\n"
#define VASL_ENDL "\r\n"
#define VASL_RN   "\r\n"

///< 制表符宏
#define VASL_T2     "  "
#define VASL_T4     "    "
#define VASL_T8     "        "
#define VASL_T      VASL_T4
#define VASL_TAB    VASL_T

///< 转义符宏
#define VASL_DOUBLE_QUOTE_MARK "\""
#define VASL_DQM VASL_DOUBLE_QUOTE_MARK
#define VASL_SINGLE_QUOTE_MARK "'"
#define VASL_SQM VASL_SINGLE_QUOTE_MARK

///< 手动日志前缀 - 无颜色
#define VASL_NOR "[LOG] "
#define VASL_SUC "[LOG] "
#define VASL_INF "[INFO] "
#define VASL_WAR "[WARNING] "
#define VASL_WRN "[WARNING] "
#define VASL_ERR "[ERROR] "

///< 手动日志前缀 - 带ANSI颜色代码
#define VASL_NOR_A "\033[102;30m[LOG]\033[0m \033[32m"       ///< 亮绿色背景，黑色字体
#define VASL_SUC_A "\033[102;30m[LOG]\033[0m \033[32m"       ///< 亮绿色背景，黑色字体
#define VASL_INF_A "\033[104;30m[INFO]\033[0m \033[34m"      ///< 亮蓝色背景，黑色字体
#define VASL_WAR_A "\033[103;30m[WARNING]\033[0m \033[33m"   ///< 亮黄色背景，黑色字体
#define VASL_WRN_A "\033[103;30m[WARNING]\033[0m \033[33m"   ///< 亮黄色背景，黑色字体
#define VASL_ERR_A "\033[101;30m[ERROR]\033[0m \033[31m"     ///< 亮红色背景，黑色字体
#define VASL_NOR_ANSI "\033[102;30m[LOG]\033[0m \033[32m"       ///< 亮绿色背景，黑色字体
#define VASL_SUC_ANSI "\033[102;30m[LOG]\033[0m \033[32m"       ///< 亮绿色背景，黑色字体
#define VASL_INF_ANSI "\033[104;30m[INFO]\033[0m \033[34m"      ///< 亮蓝色背景，黑色字体
#define VASL_WAR_ANSI "\033[103;30m[WARNING]\033[0m \033[33m"   ///< 亮黄色背景，黑色字体
#define VASL_WRN_ANSI "\033[103;30m[WARNING]\033[0m \033[33m"   ///< 亮黄色背景，黑色字体
#define VASL_ERR_ANSI "\033[101;30m[ERROR]\033[0m \033[31m"     ///< 亮红色背景，黑色字体

#define VASL_ANSI_RST "\033[0m"
#define VASL_ANSI_R "\033[0m"

#define VASL_ENDL_A VASL_ENDL VASL_ANSI_R
#define VASL_ENDL_R VASL_ENDL VASL_ANSI_R

///< 注意：宏无法调试
///< 功能：循环8位次数宏
#define VASL_LOOP_TIMES_8(cntvar, TIMES, CODE) do { \
        for(uint8_t cntvar = 0U; cntvar < (TIMES); cntvar++) { CODE } \
    } while(0);
///< 注意：宏无法调试
///< 功能：循环16位次数宏
#define VASL_LOOP_TIMES_16(cntvar, TIMES, CODE) do { \
        for(uint16_t cntvar = 0U; cntvar < (TIMES); cntvar++) { CODE } \
    } while(0);
///< 注意：宏无法调试
///< 功能：循环32位次数宏
#define VASL_LOOP_TIMES_32(cntvar, TIMES, CODE) do { \
        for(uint32_t cntvar = 0U; cntvar < (TIMES); cntvar++) { CODE } \
    } while(0);

///< 注意：宏无法调试
///< 单次执行宏：仅执行一次 注意定义域和代码填写区域
#define VASL_ONCE(CODE) \
                    do { static bool vasl_once_flag_##__COUNTER__ = 0; \
                    for(; !vasl_once_flag_##__COUNTER__; vasl_once_flag_##__COUNTER__ = 1) { CODE } } while(0);

///< 仅仅为了好看
#define VASL_DO     do
#define VASL_ENDDO  while(0);

///< 调试宏
#ifndef VASL_LOG_DBG
    #define VASL_LOG_DBG(x)  x
#endif

/** ================================================================================ *//**
 *  @brief      定义常量宏
 * -------------------------------------------------------------------------------- */
///< NaN 宏
#define VASL_NAN         0.0F / 0.0F
///< 无穷大宏
#define VASL_INFINITY    1.0F / 0.0F
///< 8位无符号整数最大值
#define VASL_U8MAX       255U
///< 16位无符号整数最大值
#define VASL_U16MAX      65535U
///< 32位无符号整数最大
#define VASL_U32MAX      4294967295U
///< 8位无符号整数最小值
#define VASL_U8MIN       0U
///< 16位无符号整数最小值
#define VASL_U16MIN      0U
///< 32位无符号整数最小值
#define VASL_U32MIN      0U
///< 8位有符号整数最大值
#define VASL_I8MAX       127
///< 16位有符号整数最大值
#define VASL_I16MAX      32767
///< 32位有符号整数最大值
#define VASL_I32MAX      2147483647
///< 8位有符号整数最小值
#define VASL_I8MIN       -128
///< 16位有符号整数最小值
#define VASL_I16MIN      -32768
///< 32位有符号整数最小值
#define VASL_I32MIN      -2147483648

///< 任意位宽
#define VASL_BIT_WIDTH(bits)    (1U << (bits))
///< 6位宽
#define VASL_BIT_WIDTH6         VASL_BIT_WIDTH(6)
///< 8位位宽
#define VASL_BIT_WIDTH8         VASL_BIT_WIDTH(8)
///< 10位宽
#define VASL_BIT_WIDTH10        VASL_BIT_WIDTH(10)
///< 12位宽
#define VASL_BIT_WIDTH12        VASL_BIT_WIDTH(12)
///< 14位宽
#define VASL_BIT_WIDTH14        VASL_BIT_WIDTH(14)
///< 16位宽
#define VASL_BIT_WIDTH16        VASL_BIT_WIDTH(16)
///< 18位宽
#define VASL_BIT_WIDTH18        VASL_BIT_WIDTH(18)
///< 20位宽
#define VASL_BIT_WIDTH20        VASL_BIT_WIDTH(20)
///< 22位宽
#define VASL_BIT_WIDTH22        VASL_BIT_WIDTH(22)
///< 24位宽
#define VASL_BIT_WIDTH24        VASL_BIT_WIDTH(24)

#define VASL_INTERRUPT          volatile

#define VASL_VOLTAGE            3.3f

#endif ///< VASL_MACRO_H__
