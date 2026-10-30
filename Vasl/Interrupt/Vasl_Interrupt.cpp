#include "Interrupt/Vasl_Interrupt.hpp"

static void empty_handler(void) {}
static void empty_error_handler(const char* err_msg) { Vasl::IrqDisable(); (void)err_msg; VASL_INFLOOP {}}
static void empty_warn_handler(const char* warn_msg) { (void)warn_msg; }
void (* vasl_systick_it_callback_funcptr)(void) = empty_handler;
void (* vasl_error_callback_funcptr)(const char* err_msg) = empty_error_handler;
void (* vasl_warn_callback_funcptr)(const char* warn_msg) = empty_warn_handler;

void Vasl::Register::SysTick_IT_Callback(void (* handler)(void)) {
    if(nullptr == handler) { return; }
    vasl_systick_it_callback_funcptr = handler;
}

void Vasl::Register::Error_Handler(void (* handler)(const char* err_msg)) {
    if(nullptr == handler) { return; }
    vasl_error_callback_funcptr = handler;
}

void Vasl::Register::Warn_Handler(void (* handler)(const char* warn_msg)) {
    if(nullptr == handler) { return; }
    vasl_warn_callback_funcptr = handler;
}

void Vasl::Error(const char* err_msg) {
    vasl_error_callback_funcptr(err_msg);
}

void Vasl::Warn(const char* warn_msg) {
    vasl_warn_callback_funcptr(warn_msg);
}
