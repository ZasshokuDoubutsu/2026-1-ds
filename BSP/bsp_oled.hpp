#ifndef __BSP_OLED_HPP__
#define __BSP_OLED_HPP__

#include "Vasl_Core.hpp"
#include "bsp_inst.hpp"
#include "OLED/SSD1306_S/OLEDv3.hpp"

extern Vasl::TypePT PT_oled_loading_ico, PT_oled_cpu_temperature, PT_oled_cpu_usage, PT_oled_warn_handler;
extern Disp::OLEDv3 g_oled;
extern Disp::OLEDv3 g_oled96;
extern volatile Vasl::flag_t g_oled_warndisplay_flag;

Vasl::err_t bsp_oled_init(void);
Vasl::err_t bsp_oled_after_all_init_ok(void);
Vasl::err_t bsp_oled_error_handler(void);
Vasl::err_t bsp_oled_warn_handler(const char* warn_msg);
int bsp_oled_loading (Vasl::TypePT* pt);
int bsp_oled_cpu_temperature (Vasl::TypePT* pt);
int bsp_oled_cpu_usage (Vasl::TypePT* pt);
int bsp_oled_warn_handler (Vasl::TypePT* pt);

#endif ///< __BSP_OLED_HPP__
