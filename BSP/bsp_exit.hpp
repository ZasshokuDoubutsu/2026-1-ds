#ifndef __BSP_EXIT_HPP__
#define __BSP_EXIT_HPP__

extern "C" {
    #include "common_data.h"
    #include "hal_data.h"
}
#include "Vasl_Core.hpp"

Vasl::err_t bsp_ex_irq6_init(void);

#endif ///< __BSP_IT_HPP__
