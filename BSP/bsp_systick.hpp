#ifndef __BSP_SYSTICK_HPP__
#define __BSP_SYSTICK_HPP__

#include "Vasl_Type.hpp"
#include "etl/atomic.h"
extern "C" {
    #include "hal_data.h"
    void SysTick_Handler(void);
}

Vasl::err_t bsp_systick_init(void);

namespace Vasl {
    size_t get_ms_ticks(void);
}

extern volatile size_t vg_ms_ticks;

#endif ///< __BSP_SYSTICK_HPP__
