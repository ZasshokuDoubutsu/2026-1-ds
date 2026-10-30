#ifndef __BSP_SOFTTIMER_HPP__
#define __BSP_SOFTTIMER_HPP__

#include "Vasl_Core.hpp"
#include "bsp_inst.hpp"

extern Vasl::ST::TIM g_test_stimer;
extern Vasl::TypePT PT_softtimer;

Vasl::err_t bsp_softtimer_init(void);
Vasl::err_t bsp_softtimer_test_start(void);

#endif ///< __BSP_SOFTTIMER_HPP__
