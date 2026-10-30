#ifndef __BSP_UART_HPP__
#define __BSP_UART_HPP__

#include "Vasl_Core.hpp"
#include "bsp_inst.hpp"
#include "Class/Vasl_Printer.hpp"

extern Vasl::TypePT    PT_uart7_checktimeout, PT_uart7_process;
extern Vasl::TypePT    PT_uart2_checktimeout, PT_uart2_process;
extern Vasl::TypePT    PT_uart9_checktimeout, PT_uart9_process;
extern Vasl::TypePT    PT_uart4_checktimeout, PT_uart4_process;

Vasl::err_t bsp_uart7_init(void);
Vasl::err_t bsp_uart2_init(void);
Vasl::err_t bsp_uart9_init(void);
Vasl::err_t bsp_uart4_init(void);
auto bsp_uart7_checktimeout (Vasl::TypePT* pt) -> int;
int bsp_uart2_checktimeout (Vasl::TypePT* pt);
int bsp_uart9_checktimeout (Vasl::TypePT* pt);
int bsp_uart4_checktimeout (Vasl::TypePT* pt);
int bsp_uart7_process (Vasl::TypePT* pt);
int bsp_uart2_process (Vasl::TypePT* pt);
int bsp_uart9_process (Vasl::TypePT* pt);

#endif ///< __BSP_UART_HPP__
