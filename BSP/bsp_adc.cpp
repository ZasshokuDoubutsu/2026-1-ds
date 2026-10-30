#include "bsp_adc.hpp"

///< 任务控制块
Vasl::TypePT    PT_adc0_cputemp {}, PT_adc0_cputemp_conv {};
///< 一些变量
volatile Vasl::flag_t  g_adc0_scan_ok_f {Vasl::NOT};
uint16_t        g_adc0_buf_raw {0U};
float           g_adc0_temperature_v {0.f}, g_cpu_temperature {0.f};
AdcInfoType     g_adc_info {};
float           g_adc_calibration {0.f};
Vasl::TypePTSem SEM_adc0_read_ok {}, SEM_adc0_conv_ok {};

///< 公式：T = (Vs - V1) / slope + T1
///< 对于RA系列MCU，斜率通常为4mV/°C = 0.004 V/°C
constexpr float SLOPE {0.004f};  ///< 4.0f / 1000.0f
constexpr float T1 {127.0f};     ///< 基准温度点

Vasl::err_t bsp_adc_init(void) {
    if (FSP_SUCCESS != Vasl::Layer::ADC_Open(&g_adc0)) { return Vasl::NOT; }
    if (FSP_SUCCESS != Vasl::Layer::ADC_ScanCfg(&g_adc0)) { return Vasl::NOT; }
    if (FSP_SUCCESS != Vasl::Layer::ADC_InfoGet(&g_adc0, &g_adc_info)) { return Vasl::NOT; }
    g_adc_calibration = (VASL_VOLTAGE * (float)g_adc_info.calibration_data) / (float)VASL_BIT_WIDTH(12);     
    return Vasl::OK;
}

/* ============ 任务 读取CPU温度 ============ */
int bsp_adc_read_cputemperature (Vasl::TypePT* pt) {
    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(1000);
        if (FSP_SUCCESS != Vasl::Layer::ADC_ScanStart(&g_adc0)) { VASL_PT_RESTART; }
    VASL_PT_WAIT_UNTIL_OR_SKIP(g_adc0_scan_ok_f.get() == Vasl::OK);
        g_adc0_scan_ok_f.set(Vasl::NOT);
        if (FSP_SUCCESS != Vasl::ADC_Read(&g_adc0, ADC_CHANNEL_TEMPERATURE, &g_adc0_buf_raw)) { 
            g_adc0_buf_raw = 2333;
            VASL_PT_RESTART;
        }
        Vasl::CO::SEM_Post(&SEM_adc0_read_ok);
    VASL_PT_TASK_END
}

/* ============ 任务 转换温度 ============ */
int bsp_adc_convert_cputemperature (Vasl::TypePT* pt) {
    VASL_PT_TASK_BEGIN
    VASL_PT_SEM_WAIT(&SEM_adc0_read_ok);
        g_adc0_temperature_v = g_adc0_buf_raw * VASL_VOLTAGE / (float)VASL_BIT_WIDTH(12);
        g_cpu_temperature = (g_adc0_temperature_v - g_adc_calibration) / SLOPE + T1;
        Vasl::CO::SEM_Post(&SEM_adc0_conv_ok);
    VASL_PT_TASK_END
}

extern "C" void ra_adc_callback(adc_callback_args_t * p_args) {
    switch (p_args->channel) {
        default: {
            g_adc0_scan_ok_f.set_ok();
            break;
        }
    }
}
