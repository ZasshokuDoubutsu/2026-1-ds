#include "Vasl_NLC.hpp"

#include <string.h>
#include <ctype.h>
#include "Vasl_Layer.hpp"
#include "Vasl_Macro.h"
#include "Strxx/Vasl_Strxx.hpp"

// ================== 内部函数声明 ==================
static int sNLC_ParseArgsSafe(char* args, char* argv[], int max_argc);

// ================== 内部函数实现 ==================

/** ================================================================================ *//**
 *  @brief 安全地解析命令行参数
 *  @param args 参数字符串
 *  @param argv 参数指针数组
 *  @param max_argc 最大参数数量
 *  @return 实际参数数量
 * 
 * -------------------------------------------------------------------------------- */
static int sNLC_ParseArgsSafe(char* args, char* argv[], int max_argc) {
    int argc = 0;
    char* start = args;
    
    while (*start && argc < max_argc) {
        // 跳过空格
        while (*start && isspace((unsigned int)*start)) start++;
        if (!*start) break;
        
        argv[argc++] = start;  // 记录参数起始位置
        
        // 找到参数结束
        while (*start && !isspace((unsigned int)*start)) start++;
        
        if (*start) {
            // 临时修改字符串结束参数
            *start = '\0';
            start++;
        }
    }
    return argc;
}

// ================== 核心函数实现 ==================

/** ================================================================================ *//**
 *  @brief 自然语言命令核心处理函数
 *  @param input 输入命令字符串
 *  @param commands 命令定义数组
 *  @param cmd_count 命令数量
 * 
 * -------------------------------------------------------------------------------- */
void Vasl::NLC_ProcessCore(const char* input, const Vasl::NLC_CommandDefinition commands[], int cmd_count, void(*not_found_handler)(void)) {
    char command[MAX_CMD_LEN] = {0};
    char args[MAX_ARGS_LEN] = {0};
    char* argv[MAX_ARGC] = {0};     // 参数指针数组
    int argc = 0;                   // 参数计数
    
    // 计算输入长度
    size_t input_len = Vasl::str::len(input, VASL_U32MAX);

    /* 跳过开头的空格 */
    size_t start_index = 0;
    while (start_index < input_len && isspace((unsigned int)input[start_index]))
        start_index++;

    /* 提取命令部分 到第一个空格或结束 */
    size_t cmd_index = 0;
    size_t i = start_index;
    while (i < input_len && !isspace((unsigned int)input[i]) && cmd_index < MAX_CMD_LEN - 1) {
        command[cmd_index++] = (char)tolower((unsigned int)input[i]);
        i++;
    }
    command[cmd_index] = '\0';

    /* 提取参数部分 剩余内容 - 优化版本 */
    size_t args_index = 0;
    while (i < input_len && args_index < MAX_ARGS_LEN - 1) {
        // 跳过连续空格，只保留一个空格分隔
        if (!isspace((unsigned int)input[i]) || (args_index > 0 && !isspace((unsigned int)args[args_index-1]))) {
            args[args_index++] = input[i];
        }
        i++;
    }
    args[args_index] = '\0';
    
    /* 修剪参数末尾空格 */
    while (args_index > 0 && isspace((unsigned int)args[args_index - 1])) {
        args_index--;
        args[args_index] = '\0';
    }
    
    /* 使用自定义的线程安全参数分割函数 */
    if (args_index > 0) {
        argc = sNLC_ParseArgsSafe(args, argv, MAX_ARGC);
    }
    
    /* 查找匹配的命令 */
    int found = 0;
    for (int idx = 0; idx < cmd_count; idx++) {
        if (strcmp(command, commands[idx].prefix) == 0) {
            commands[idx].handler(argc, argv);
            found = 1;
            break;
        }
    }
    
    if (!found && not_found_handler) {
        not_found_handler();
    }
}
