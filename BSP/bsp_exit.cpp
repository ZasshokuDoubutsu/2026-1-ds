#include "bsp_exit.hpp"

Vasl::err_t bsp_ex_irq6_init(void) {
    // if (FSP_SUCCESS != Vasl::Layer::Irq_Open(&g_external_irq6)) { return Vasl::NOT; }
    // if (FSP_SUCCESS != Vasl::Layer::Irq_Enable(&g_external_irq6)) { return Vasl::NOT; }
    return Vasl::OK;
}

// extern "C" void ra_ex_irq66_callback(external_irq_callback_args_t * p_args) {
//     unused(p_args);
// }

