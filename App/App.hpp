#ifndef __APP_HPP__
#define __APP_HPP__ 

#include "Vasl_Core.hpp"
#include "App_Core.hpp"

///< Vasl程序声明

    ///< C接口 Start
    #ifdef __cplusplus
        extern "C" {
    #endif

    ///< C接口 End
    #ifdef __cplusplus
        } /** extern "C" */
    #endif
    
namespace Vasl {
    extern void App_Init(void);      /** App初始化 */
    extern void App_InfLoop(void);   /** App超级循环 */
    void system_init(void);
}
#define VASL_MAIN           Vasl::system_init();\
                            Vasl::App_Init();\
                            for(;;) { Vasl::App_InfLoop(); }
#define VASL_SYSTICK_IRQ    vasl_systick_it_callback_funcptr();
#define VASL_HARDFAULT_IRQ    \
        do { \
            __disable_irq();    \
            Vasl::errlog(" HardFault!!!\r\n\t-> by StopLog\r\n"); \
            for(;;) {}  \
        } while(0);

#endif /* __APP_HPP__  */
