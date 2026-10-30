#include "bsp_button.hpp"
#include "bsp_systick.hpp"
#include "bsp_display.hpp"

constexpr static size_t KEY_REPEAT_INTERVAL {100U};
constexpr static size_t KEY_LONG_PRESS      {1000U};
extern Vasl::err_t bsp_softtimer_test_start(void);

Vasl::TypeSKey  g_obkey {};
Vasl::bsp::SKey_Event_q<EVENT_QUEUE_SIZE> g_keys_queue {};
Vasl::TypePT    PT_button_tick_handler {}, PT_button_process {};

Vasl::err_t bsp_button_init(void) {
    g_obkey.Init(Vasl::LOW);
    return Vasl::OK;
}

Vasl::err_t bsp_button_register(void) {
    g_obkey.RegisterEventCallback([]() {
        VASL_PASS()
    }, SKeyDef::Event::SHORT);
    g_obkey.RegisterEventCallback([]() {
        auto& oled = OLED();
        oled.ShowString(8 * 7, 16, (char*)"[W]>OK", OLED_FONT_7X12);
        oled.UpdateArea(8 * 7, 16, VASL_C_STR_LEN("[W]>OK"), 12);
        g_esp_auto_task_flag[0].set_ok();
    }, SKeyDef::Event::LONG);
    g_obkey.RegisterEventCallback([]() {
        VASL_PASS()
    }, SKeyDef::Event::REPEAT);
    return Vasl::OK;
}

int bsp_button_tick_handler (Vasl::TypePT* pt) {
    VASL_PT_TASK_BEGIN
        g_obkey.InterruptTask(BSP_IO_PORT_00_PIN_00, Vasl::get_ms_ticks(), 16, KEY_LONG_PRESS, KEY_REPEAT_INTERVAL);
        auto event = g_obkey.getEvent();
        if(event != SKeyDef::Event::NONE) {
            auto ret = g_keys_queue.push({&g_obkey, event});
            if(ret != Vasl::OK) { 
                Vasl::Warn("1002 SKey 队列入队失败 也许队列已满");
            }
        }
        VASL_PT_RESTART;
    VASL_PT_TASK_END
}

int bsp_button_process (Vasl::TypePT* pt) {
    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(5);
    VASL_PT_WAIT_UNTIL_OR_SKIP(g_keys_queue.is_not_empty());
        do {
            auto item = g_keys_queue.pop_ptr();
            if(item->first == nullptr) { break; }
            auto ret = item->first->Process(item->second);
            if(ret != Vasl::OK) {
                Vasl::Warn("1001 SKey 队列处理失败");
            }
        } while (g_keys_queue.is_not_empty());
    VASL_PT_TASK_END
}
