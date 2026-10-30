#include "bsp_softtimer.hpp"
#include "Vasl_Layer.hpp"

Vasl::ST::TIM g_test_stimer {};
Vasl::TypePT PT_softtimer {};

Vasl::err_t bsp_softtimer_init(void) {
    auto ret = Vasl::ST::Install([]() {
        return (uint32_t)Vasl::get_ms_ticks();
    }); ///< 注册时基
    return ret;
}

Vasl::err_t bsp_softtimer_test_start(void) {
    Vasl::log().time_a().inf_a() << "TestSoftTimer 启动" << VASL_ENDL_R;
    return Vasl::ST::Start(&g_test_stimer, 1000, [](Vasl::ST::TIM* timer, void* userData) {
        unused(timer);
        Vasl::log().time_a().inf_a() << "定时时间到" VASL_ENDL;
        Vasl::log() << VASL_T "-> UD: " << static_cast<const char*>(userData) << VASL_ENDL_R;
    }, static_cast<void*>(const_cast<char*>("TestSoftTimer")));
}
