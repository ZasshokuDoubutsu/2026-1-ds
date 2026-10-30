#include "bsp_oled.hpp"

Vasl::TypePT PT_oled_loading_ico {}, PT_oled_cpu_temperature {}, PT_oled_cpu_usage {}, PT_oled_warn_handler {};
Disp::OLEDv3 g_oled {BSP_IO_PORT_00_PIN_01, BSP_IO_PORT_04_PIN_14, 0x78, 0};
Disp::OLEDv3 g_oled96 {BSP_IO_PORT_00_PIN_10, BSP_IO_PORT_05_PIN_13, 0x78, 1};
volatile Vasl::flag_t g_oled_warndisplay_flag {Vasl::NOT};
auto g_usage {0.f};

///< Y: 行号 X: 字体高度(FONT 后者)
constexpr size_t Y1_X8 {0U};
constexpr size_t Y2_X8 {8U};
constexpr size_t Y3_X12 {16U};

Vasl::err_t bsp_oled_init(void) {
    g_oled.Init();
    g_oled.Clear();
    g_oled.ShowString(0, Y1_X8, (char*)"Vasl v26.3pre", OLED_FONT_6X8);
    g_oled.UpdateAll();
    g_oled96.Init();
    g_oled96.Clear();
    g_oled96.UpdateAll();
    return Vasl::OK;
}

Vasl::err_t bsp_oled_after_all_init_ok(void) {
    g_oled.ShowString(14 * 6, Y1_X8, (char*)"Init ok", OLED_FONT_6X8);
    g_oled.UpdateArea(14 * 6, Y1_X8, VASL_C_STR_LEN("Init ok") * 6, 8);
    g_oled.ShowString(0, Y2_X8, (char*)"T:", OLED_FONT_6X8);
    g_oled.UpdateArea(0, Y2_X8, VASL_C_STR_LEN("T:") * 6, 8);
    g_oled.ShowString(0, Y3_X12, (char*)"@->", OLED_FONT_7X12);
    g_oled.UpdateArea(0, Y3_X12, VASL_C_STR_LEN("@->") * 7, 12);
    return Vasl::OK;
}

Vasl::err_t bsp_oled_error_handler(void) {
    g_oled.ShowString(14 * 6, Y1_X8, (char*)"Error! ", OLED_FONT_6X8);
    g_oled.UpdateArea(14 * 6, Y1_X8, VASL_C_STR_LEN("Error! ") * 6, 8);
    return Vasl::OK;
}

Vasl::err_t bsp_oled_warn_handler(const char* warn_msg) {
    constexpr auto WARN_CODE_LEN {4U};
    constexpr auto WARN_CODE_STR_LEN {WARN_CODE_LEN + 2U};
    char code_str[WARN_CODE_LEN + 1] {0}, warn_code_str[WARN_CODE_STR_LEN + 1] {0};
    Vasl::str::copy(code_str, warn_msg, WARN_CODE_LEN);
    Vasl::str::format(warn_code_str, WARN_CODE_STR_LEN + 1, "C:%s", code_str);
    g_oled_warndisplay_flag.set(Vasl::OK);
    g_oled.ShowString(14 * 6, Y2_X8, (char*)warn_code_str, OLED_FONT_6X8);
    g_oled.UpdateArea(14 * 6, Y2_X8, WARN_CODE_STR_LEN * 6, 8);
    return Vasl::OK;
}

int bsp_oled_loading (Vasl::TypePT* pt) {
    static const char load_ioc[] {'|', '/', '-', '\\'};
    static uint8_t load_index {0U};
    static char buf[] {0};

    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(200);
        buf[0] = load_ioc[load_index];
        Vasl::period<>(load_index, 4);
        g_oled.ShowChar(26, Y3_X12, buf[0], OLED_FONT_7X12);
        g_oled.UpdateArea(26, Y3_X12, 7, 12);
    VASL_PT_TASK_END
}

int bsp_oled_cpu_temperature (Vasl::TypePT* pt) {
    extern Vasl::TypePTSem SEM_adc0_conv_ok;
    extern float g_cpu_temperature;
    constexpr size_t BUF_SIZE {12};
    static char buf[BUF_SIZE] {0};

    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(1000);
    VASL_PT_SEM_WAIT(&SEM_adc0_conv_ok);
        const auto len = static_cast<uint8_t>(Vasl::str::format(buf, BUF_SIZE - 1, "%4.1fC", g_cpu_temperature));
        g_oled.ShowString(12, Y2_X8, (char*)buf, OLED_FONT_6X8);
        g_oled.UpdateArea(12, Y2_X8, len * 6, 8);
    VASL_PT_TASK_END
}

int bsp_oled_cpu_usage (Vasl::TypePT* pt) {
    extern bool g_is_wifi_ok;
    extern Vasl::usage_t<> CPUPerf;
    constexpr size_t BUF_SIZE {7};
    static char buf[BUF_SIZE] {0};

    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(1000);
        g_usage = CPUPerf.get_usage_and_reset();
        if (g_usage >= 80.f) { 
            Vasl::log().time_a().warn_a() << "cpu_usage: " << g_usage << "%" << VASL_ENDL_R; 
        }
        const auto len = static_cast<uint8_t>(Vasl::str::format(buf, BUF_SIZE - 1, "%4.1f%%", g_usage));
        g_oled.ClearArea(6 * 8, Y2_X8, len * 6, 8);
        g_oled.ShowString(6 * 8, Y2_X8, (char*)buf, OLED_FONT_6X8);
        g_oled.UpdateArea(6 * 8, Y2_X8, len * 6, 8);
        if (g_is_wifi_ok) {
            g_oled.ShowString(26 + (7 * 3), Y3_X12, (char*)"WiFi-OK", OLED_FONT_7X12);
            g_oled.UpdateArea(26 + (7 * 3), Y3_X12, VASL_C_STR_LEN("WiFi-OK") * 7, 12);
        }
    VASL_PT_TASK_END
}

int bsp_oled_warn_handler (Vasl::TypePT* pt) {
    static bool s_warn_showd {false};

    VASL_PT_TASK_BEGIN
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == g_oled_warndisplay_flag.get());
    VASL_PT_PERIOD_TASK(500);
        if (s_warn_showd) {
            g_oled.ShowString(14 * 6, Y1_X8, (char*)"Warn!", OLED_FONT_6X8);
        } else {
            g_oled.ClearArea(14 * 6, Y1_X8, VASL_C_STR_LEN("Init ok") * 6, 8);
        }
        s_warn_showd = !s_warn_showd;
        g_oled.UpdateArea(14 * 6, Y1_X8, VASL_C_STR_LEN("Init ok") * 6, 8);
    VASL_PT_TASK_END
}
