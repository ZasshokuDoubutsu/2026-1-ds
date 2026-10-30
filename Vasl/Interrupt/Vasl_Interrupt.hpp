#ifndef __VASL_INTERRUPT_HPP__
#define __VASL_INTERRUPT_HPP__

#include "Vasl_Macro.h"
#include "Vasl_Type.hpp"

#define VASL_ERROR_HANDLER(err_num) \
    vasl_error_callback_funcptr(err_num)

#define VASL_WARN_HANDLER(warn_num) \
    vasl_warn_callback_funcptr(warn_num)

constexpr size_t __WARN_MSG_SIZE__ = 128;

namespace Vasl {
    inline namespace Register {
        void SysTick_IT_Callback(void (* handler)(void));
        void Error_Handler(void (* handler)(const char* err_msg));
        void Warn_Handler(void (* handler)(const char* warn_msg));
    }
    void Error(const char* err_msg);
    void Warn(const char* warn_msg);
}

extern void (* vasl_systick_it_callback_funcptr)(void);

#endif ///< __VASL_INTERRUPT_HPP__
