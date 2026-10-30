#ifndef __Vasl_NLC_HPP__
#define __Vasl_NLC_HPP__

#include <string.h>
#include "Vasl_Type.hpp"

#define VASL_CMD(__ARGV__, __ARG__, __CMD__)	strcmp(__ARGV__[__ARG__ - 2], __CMD__) == 0
///<        /at to AT ...: 
///<        /at是命令前缀 to是argv[0] AT是argv[1] ...是argv[2]等

// ================== NLC配置区 ==================
#define MAX_ARGC 		(4)	    // 最大命令参数数量
#define MAX_CMD_LEN		(100)    // 最大命令长度
#define MAX_ARGS_LEN	(64)	// 最大参数长度

// ================== 命令处理函数类型及命令定义结构 ==================
namespace Vasl {
    using NLC_CmdHandler = void(*)(int argc, char** argv);
    typedef struct {
        const char* prefix;			// 命令前缀
        Vasl::NLC_CmdHandler handler; 	// 处理函数
    } NLC_CommandDefinition;
}

// ================== 核心函数声明 ==================
namespace Vasl {
    /** ================================================================================ *//**
    *  @brief 自然语言命令核心处理函数
    *  @param input 输入命令字符串
    *  @param commands 命令定义数组
    *  @param cmd_count 命令数量
    * 
    * -------------------------------------------------------------------------------- */
    void NLC_ProcessCore(const char* input, const Vasl::NLC_CommandDefinition commands[], int cmd_count, void(*not_found_handler)(void) = nullptr);
}

#endif /* __Vasl_NLC_HPP__ */
