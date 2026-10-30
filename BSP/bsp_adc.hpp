#ifndef __BSP_ADC_HPP__
#define __BSP_ADC_HPP__

#include "Vasl_Core.hpp"

Vasl::err_t bsp_adc_init(void);

extern Vasl::TypePT     PT_adc0_cputemp, PT_adc0_cputemp_conv;
extern volatile Vasl::flag_t   g_adc0_scan_ok_f;
extern uint16_t         g_adc0_buf_raw;
extern float            g_adc0_temperature_v, g_cpu_temperature;
extern AdcInfoType      g_adc_info;
extern float            g_adc_calibration;
extern Vasl::TypePTSem  SEM_adc0_read_ok, SEM_adc0_conv_ok;

int bsp_adc_read_cputemperature (Vasl::TypePT* pt);
int bsp_adc_convert_cputemperature (Vasl::TypePT* pt);

#endif ///< __BSP_ADC_HPP__
