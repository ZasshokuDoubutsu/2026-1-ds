#include "Vasl_Layer.hpp"

/** ================================================================================ *//**
*  @brief      DWT初始化函数
* 
* -------------------------------------------------------------------------------- */
void Vasl::Layer::DWT_Init(void) {
#if defined(TIME_TICK_USE_DWT)
    /* 确保 DWT 计数器可用 */
    if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    }
    /* 启用周期计数器 */
    if (!(DWT->CTRL & DWT_CTRL_CYCCNTENA_Msk)) {
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
    }
    /* 重置周期计数器 */
    DWT->CYCCNT = 0;
#endif /* defined  */
}

/** ================================================================================ *//**
*  @brief      获取微秒Tick函数
*  @return     uint32_t: 当前微秒数
* 
* -------------------------------------------------------------------------------- */
uint32_t Vasl::Layer::GetTickUs(void) {
#if defined(TIME_TICK_USE_DWT)
    // 计算每个微秒的周期数
    uint32_t cyclesPerUs = SystemCoreClock / 1000000;
    // 返回当前微秒计数
    return DWT->CYCCNT / cyclesPerUs;
#elif defined(TIME_TICK_USE_SYSTICK)
    static uint32_t us_per_tick = 0;
    if (us_per_tick == 0) {
        /* 预计算：每个计数对应的微秒数 × 1000 避免浮点数 */
        us_per_tick = 1000000 / (SystemCoreClock / 1000);
    }
    uint32_t ms = Vasl::GetTickMs();
    uint32_t val = SysTick->VAL;
    return ms * 1000 + ((SysTick->LOAD - val) * us_per_tick) / 1000;
#else
    return TIME_TICK_RETURN_MAGIC;
#endif /* TIME_TICK_USE_DWT || TIME_TICK_USE_SYSTICK */
}

/** ================================================================================ *//**
*  @brief      获取当前周期计数
*  @return     uint32_t: 当前运行周期计数
*  @warning    无DWT时请自行实现
* 
* -------------------------------------------------------------------------------- */
uint32_t Vasl::Layer::GetCycleTick(void) {
#if defined(TIME_TICK_USE_DWT)
    return DWT->CYCCNT;
#elif defined(TIME_TICK_USE_SYSTICK)
    static uint32_t last_ms = 0;
    static uint32_t systick_overflows = 0;
    uint32_t current_val = SysTick->VAL & 0xFFFFFF;
    uint32_t current_ms = Vasl::GetTickMs();
    /* 通过毫秒差计算溢出次数（每1ms经历LOAD次计数） */
    uint32_t ms_delta = current_ms - last_ms;
    systick_overflows += ms_delta;
    last_ms = current_ms;
    /* 计算总周期数：溢出次数 × LOAD + (LOAD - 当前值) */
    uint32_t load_val = SysTick->LOAD & 0xFFFFFF;
    return (systick_overflows * load_val) + (load_val - current_val);
#else
    return TIME_TICK_RETURN_MAGIC;
#endif /* TIME_TICK_USE_DWT || TIME_TICK_USE_SYSTICK */
}

/** ================================================================================ *//**
*  @brief      微秒延时
*  @param[in]  us__ 延时时长 us
* 
* -------------------------------------------------------------------------------- */
void Vasl::Layer::delay_us(const uint32_t us) {
#ifdef __VASL_USE_RA_DELAY_US__
    R_BSP_SoftwareDelay(us, BSP_DELAY_UNITS_MICROSECONDS);
#else
#if defined(TIME_TICK_USE_DWT)
    uint32_t start = GetCycleTick(); // 起始周期计数
    uint32_t cycles = us * (SystemCoreClock / 1000000); // 计算需要的周期数
    // 等待达到目标周期数 处理计数器回绕
    while ((GetCycleTick() - start) < (cycles - DELAY_US_COMPENSATION)) {/* 空循环 */}
#elif defined(TIME_TICK_USE_SYSTICK)
    uint32_t start = Vasl::GetTickUs();
    while(Vasl::GetTickUs() - start < (us - DELAY_US_COMPENSATION)) {/* 空循环 */}
#else
    return;
#endif /* TIME_TICK_USE_DWT || TIME_TICK_USE_SYSTICK */
#endif
}
