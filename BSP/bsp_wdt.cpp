#include "bsp_wdt.hpp"

static Vasl::void_func_t __wdt_timeout_cb__ {nullptr};
Vasl::TypePT PT_wdt {0};

void Vasl::Register::WDT_Timeout(void (* callback)(void)) {
    if (callback == nullptr) { return; }
    __wdt_timeout_cb__ = callback;
}

/*初始化看门狗并启动计数器*/
Vasl::err_t WDT_Init(void) {
   ///< 如果使用J-Link调试器进行调试的话需要加上这一句话
   R_DEBUG->DBGSTOPCR_b.DBGSTOP_WDT = 0;

   ///< 初始化看门狗（WDT）模块
   if (FSP_SUCCESS != R_WDT_Open(&g_wdt0_ctrl, &g_wdt0_cfg)) { return Vasl::err_t::NOT; }

   /* 刷新看门狗计数器，在这里的作用是初次启动寄存模式下的看门狗计数器
    * 要注意，除非是在刷新允许的范围内，否则自启动模式下不应该使用该函数*/
   if (FSP_SUCCESS != R_WDT_Refresh(&g_wdt0_ctrl)) { return Vasl::err_t::NOT; }
   return Vasl::err_t::OK;
}

/*喂狗*/
Vasl::err_t WDT_Feed(void) {
   /* 喂狗 刷新递减计数器的值 */
   if (FSP_SUCCESS != R_WDT_Refresh(&g_wdt0_ctrl)) { return Vasl::err_t::NOT; }
   return Vasl::err_t::OK;
}

/* 当看门狗NMI发生时中断回调 */
extern "C" void ra_wdt_timeout_callback (wdt_callback_args_t * p_args) {
   (void) p_args;

   if (__wdt_timeout_cb__ != nullptr) { __wdt_timeout_cb__(); }

   /* 通过软件复位MCU*/
   Vasl::Layer::SoftRestart();
}
