#include "App.hpp"
#include "hal_data.h"

/* 头文件 ----------------------------------------------------------------------------------------- */
#include "bsp_inst.hpp"
#include "bsp_button.hpp"
#include "bsp_display.hpp"
#include "bsp_uart.hpp"
#include "bsp_softtimer.hpp"
#include "bsp_oled.hpp"
#include "bsp_exit.hpp"
#include "bsp_adc.hpp"
#include "bsp_wdt.hpp"
#include "bsp_cmd.hpp"
#include "bsp_esp8266.hpp"
#include "bsp_easymenu.hpp"
#include "bsp_tjchmi.hpp"

/* 宏 -------------------------------------------------------------------------------------------- */
#define VASL_PERIOD_BINMOD(MS, CODE)  do { static_assert(((MS) & ((MS) - 1U)) == 0U,    \
            VASL_NEWL "->\t[ERROR] The parameter of VASL_PERIOD_BINMOD must be a power of 2!"   \
            VASL_NEWL "->\t[错误] VASL_PERIOD_BINMOD参数必须为2的幂！");    \
                if ((Vasl::get_ms_ticks() & ((MS) - 1U)) == 0U) { CODE } } while(0);

/* 代码 ------------------------------------------------------------------------------------------ */


/* 变量 ------------------------------------------------------------------------------------------ */
Vasl::usage_t<> CPUPerf {};
Vasl::CO::Scheduler<24> PTScheduler {};

/* 程序 ------------------------------------------------------------------------------------------ */

/** ================================================================================ *//**
 *  @brief      App程序初始化 
 *  @note       仅于main.c默认初始化函数之后执行一次
 * 
 * --------------------------------------------------------------------------------- */
void Vasl::App_Init(void) {
    Vasl::log().time_a().inf_a() << "|====== 程序初始化开始 ======|" VASL_ENDL_R;
    Vasl::log().time_a().inf_a() << VASL_ENDL 
        << VASL_T "> 编译日期: " __DATE__ VASL_ENDL
        << VASL_T "> 编译时间: " __TIME__ VASL_ENDL
        << VASL_T "> C++ 版本: " << __cplusplus << VASL_ENDL
        << VASL_T "> Vasl 版本: " << Vasl::Layer::Version() << VASL_ENDL
        << VASL_T "> 固件版本: " "Ver 1.1.7" << VASL_ENDL_R;
    Vasl::Register::Error_Handler([](const char* err_msg) {
        bsp_oled_error_handler();
        g_obled.off();
        Vasl::log().time_a().err_a("Vasl::Error") << "错误信息: " << err_msg << VASL_ENDL_R;
        Vasl::IrqDisable();
        for(;;) {}
    }); ///< 注册错误处理函数
    Vasl::Register::Warn_Handler([](const char* warn_msg) {
        bsp_oled_warn_handler(warn_msg);
        Vasl::log().time_a().warn_a("Vasl::Warn") << "警告信息: " << warn_msg << VASL_ENDL_R;
    }); ///< 注册警告处理函数
    Vasl::Register::WDT_Timeout([](void) {
        ///< 注意 看门狗超时会导致中断关闭 Printer 等依赖中断
        Vasl::log().time_a().err_a("Vasl::WDT") << "看门狗超时" << VASL_ENDL_R;
    }); ///< 注册看门狗超时处理函数
    Vasl::Register::SysTick_IT_Callback([](void) {
        CPUPerf.isr();
        VASL_PERIOD_BINMOD(4, {
            VASL_PASS(/* 4ms period code */)
        });
    }); ///< 注册SysTick中断处理函数
    ///< 初始化BSP任务
        bsp_button_init();
        bsp_button_register();
        bsp_oled_init();
        bsp_cmd_start();
        bsp_tjchmi_cmd_start();
        bsp_easymenu_init();
        g_esp_auto_task_flag[0].set_ok();
        if (Vasl::NOT == bsp_esp8266_init()) { Vasl::Error("26030100 UART2/ESP8266初始化失败"); }
        if (Vasl::NOT == bsp_uart9_init()) { Vasl::Error("26030200 UART9初始化失败"); }
        if (Vasl::NOT == bsp_uart4_init()) { Vasl::Error("26030201 UART4初始化失败"); }
        if (Vasl::NOT == bsp_softtimer_init()) { Vasl::Error("26030300 软件定时器初始化失败"); }
        if (Vasl::NOT == bsp_softtimer_test_start()) { Vasl::Error("26030301 软件定时器测试任务启动失败"); }
        if (Vasl::NOT == bsp_adc_init()) { Vasl::Error("26030400 ADC初始化失败"); }
    ///< 添加任务到调度器
        Vasl::ret_t<> ret {};
        ret << PTScheduler.add(bsp_button_tick_handler, &PT_button_tick_handler);
        ret << PTScheduler.add(bsp_button_process, &PT_button_process);
        ret << PTScheduler.add(bsp_uart7_checktimeout, &PT_uart7_checktimeout);
        ret << PTScheduler.add(bsp_uart2_checktimeout, &PT_uart2_checktimeout);
        ret << PTScheduler.add(bsp_uart9_checktimeout, &PT_uart9_checktimeout);
        ret << PTScheduler.add(bsp_uart4_checktimeout, &PT_uart4_checktimeout);
        ret << PTScheduler.add(bsp_uart7_process, &PT_uart7_process);
        ret << PTScheduler.add(bsp_cmd_process, &PT_cmd_process);
        ret << PTScheduler.add(bsp_tjchmi_cmd_process, &PT_tjchmi_cmd_process);
        ret << PTScheduler.add(bsp_esp8266_rcv_analysis, &PT_esp8266_rcv_analysis);
        ret << PTScheduler.add(bsp_esp8266_auto_task, &PT_esp8266_auto_task);
        ret << PTScheduler.add(bsp_uart2_process, &PT_uart2_process);
        ret << PTScheduler.add(bsp_uart9_process, &PT_uart9_process);
        ret << PTScheduler.add(bsp_tjchmi_show_page, &g_tjchmi_show_page);
        ret << PTScheduler.add(bsp_oled_loading, &PT_oled_loading_ico);
        ret << PTScheduler.add(bsp_oled_warn_handler, &PT_oled_warn_handler);
        ret << PTScheduler.add(bsp_oled_cpu_usage, &PT_oled_cpu_usage);
        ret << PTScheduler.add(bsp_adc_read_cputemperature, &PT_adc0_cputemp);
        ret << PTScheduler.add(bsp_adc_convert_cputemperature, &PT_adc0_cputemp_conv);
        ret << PTScheduler.add(bsp_oled_cpu_temperature, &PT_oled_cpu_temperature);
        ret << PTScheduler.add([](Vasl::TypePT* pt) -> int {
            VASL_PT_TASK_BEGIN
            VASL_PT_PERIOD_TASK(5);
                Vasl::EM::process(Vasl::get_ms_ticks());
            VASL_PT_TASK_END
        }, &PT_menu);
        ret << PTScheduler.add([](Vasl::TypePT* pt) -> int {
            VASL_PT_TASK_BEGIN
            VASL_PT_PERIOD_TASK(4);
                Vasl::wdt::feed();      ///< 喂狗
            VASL_PT_TASK_END
        }, &PT_wdt);
        ret << PTScheduler.add([](Vasl::TypePT* pt) -> int {
            VASL_PT_TASK_BEGIN
                Vasl::ST::Process();    ///< 软件定时器后台
            VASL_PT_RESTART;
            VASL_PT_TASK_END
        }, &PT_softtimer);
        if (ret.is_not_ok()) { Vasl::Error("26060000 PTScheduler 任务添加失败"); }
        Vasl::log().time_a().inf_a() << "PTScheduler 任务数量: " << PTScheduler.GetTaskCount() << VASL_ENDL_R;
    if (Vasl::NOT == wdt::init()) { Vasl::Error("26030500 看门狗初始化失败"); }
    Vasl::log().time_a().suc_a() << "程序初始化完成" << VASL_ENDL_R;
}

/** ================================================================================ *//**
 *  @brief      App程序循环
 *  @note       主循环中默认调用此函数 
 * 
 * -------------------------------------------------------------------------------- */
void Vasl::App_InfLoop(void) {
    VASL_ONCE({
        bsp_oled_after_all_init_ok();
        Vasl::log().time_a().inf_a() << "程序循环开始" << VASL_ENDL_R;
    })

    CPUPerf.task();     ///< CPU检测 - 任务
    auto ret = PTScheduler.ProcessOnce(); ///< 主调度器
    CPUPerf.idle();     ///< CPU检测 - 空闲
	Vasl::WFI(!ret);  ///< 空闲休眠
}

/* 函数 -------------------------------------------------------------------------------------*/
