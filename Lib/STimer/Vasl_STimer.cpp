#include "STimer/Vasl_STimer.hpp"
#include "STimer/MultiTimer.h"

Vasl::err_t Vasl::ST::Install(PlatformTicksFunction_t fn_ptr) {
    auto ret = multiTimerInstall(fn_ptr);
    return static_cast<Vasl::err_t>((-1) * ret);
}

Vasl::err_t Vasl::ST::Start(TIM* timer, uint32_t timing, MultiTimerCallback_t callback, void* userData) {
    auto ret = multiTimerStart(static_cast<MultiTimer*>(timer), timing, callback, userData);
    return static_cast<Vasl::err_t>((-1) * ret);
}

Vasl::err_t Vasl::ST::Stop(TIM* timer) {
    multiTimerStop(static_cast<MultiTimer*>(timer));
    return Vasl::OK;
}

Vasl::err_t Vasl::ST::Process(void) {
    auto ret = multiTimerYield();
    return static_cast<Vasl::err_t>((-1) * ret);
}
