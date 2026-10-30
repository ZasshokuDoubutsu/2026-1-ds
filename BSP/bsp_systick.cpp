#include "bsp_systick.hpp"
#include "Interrupt/Vasl_Interrupt.hpp"

volatile size_t vg_ms_ticks {0U};

size_t Vasl::get_ms_ticks(void) {
    return vg_ms_ticks;
}

Vasl::err_t bsp_systick_init(void) {
    SysTick_Config(SystemCoreClock / 1000);
    return Vasl::OK;
}

extern "C" void SysTick_Handler(void);

extern "C" void SysTick_Handler(void) {
    vg_ms_ticks++;
    [[likely]]
    if(nullptr != vasl_systick_it_callback_funcptr) {
        vasl_systick_it_callback_funcptr();
    }
}
