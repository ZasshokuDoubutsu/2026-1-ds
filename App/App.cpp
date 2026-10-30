#include "App.hpp"
#include "bsp_systick.hpp"
#include "bsp_uart.hpp"

/** ================================================================================ *//**
*  @brief      main.c默认非用户使用初始化函数
* 
* -------------------------------------------------------------------------------- */
void Vasl::system_init(void) {
    bsp_systick_init();
    Vasl::Layer::DWT_Init();
    bsp_uart7_init();
}

extern "C" {
    // /** ================================================================================ *//**
    // *  @brief      ERRORlog函数 临时方案
    // *  @note       用于C程序打印错误信息
    // * 
    // * -------------------------------------------------------------------------------- */
    // void __vasl_errlog__(const char * sFormat, ...) {
    //     // 格式化字符串到缓冲区
    //     va_list args;
    //     va_start(args, sFormat);
        
    //     char buffer[160];
    //     int len = npf_vsnprintf(buffer, sizeof(buffer), sFormat, args);
        
    //     // 无论缓冲区是否足够，都打印已有内容
    //     if (len > 0) {
    //         // 如果缓冲区不够，确保最后有\n
    //         if ((size_t)len >= sizeof(buffer)) {
    //             // 检查最后一个字符是否已经是\n
    //             if (len >= 1 && buffer[len-1] != '\n') {
    //                 // 确保缓冲区至少有1个字节的空间添加\n
    //                 if ((size_t)len < sizeof(buffer) - 1) {
    //                     buffer[len] = '\n';
    //                     buffer[len+1] = '\0';
    //                     len += 1;
    //                 }
    //             }
    //         }
    //         Vasl::errlog("%s", buffer);
    //     }
    //     va_end(args);
    // }
} /** extern "C" */
