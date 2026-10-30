#include "bsp_tjchmi.hpp"

///< 用于存储TJC HMI命令的字符串
Vasl::str::static_string<128> g_tjc_hmi_str {};
///< 当前显示的页面、餐桌索引
size_t g_tjchmi_page {0U}, g_tjchmi_page_table {0U};
///< 订单有无Flag 对应n桌
extern bool g_has_tjchmi_order[6];
///< 订单有无Flag 对应n桌 TJC对应字符串
const char* g_has_tjchmi_order_str[6] {"bt0.val", "bt1.val", "bt2.val", "bt3.val", "bt4.val", "bt5.val"};
///< 订单数量 对应n道菜 TJC对应字符串
const char* g_tjchmi_order_str[5] {"n0.val", "n1.val", "n3.val", "n2.val", "n4.val"};
///< 主订单数量 对应n道菜 TJC对应字符串
const char* g_tjchmi_main_order_str[5] {"n0.val", "n4.val", "n1.val", "n2.val", "n3.val"};
Vasl::TypePT g_tjchmi_show_page {};

void bsp_tjchmi_format_cmd(const char *cmd, ...) {
    g_tjc_hmi_str.clear();
    va_list args;
    va_start(args, cmd);
    vsnprintf((char*)g_tjc_hmi_str.data(), sizeof g_tjc_hmi_str.data(), cmd, args);
    va_end(args);
	g_tjc_hmi_str += "\xff\xff\xff";
}

void bsp_tjchmi_format_string(char* name, char* showdata) {
    g_tjc_hmi_str.clear();
	g_tjc_hmi_str += name;
	g_tjc_hmi_str += "=" VASL_DQM;
	g_tjc_hmi_str += showdata;
	g_tjc_hmi_str += VASL_DQM "\xff\xff\xff";
    Vasl::log().time_a().msg_a("TJC") << "Send String: " << g_tjc_hmi_str.length();
}

void bsp_tjchmi_send_int(char* name, int num) {
    g_tjc_hmi_str.clear();
    g_tjc_hmi_str += name;
    g_tjc_hmi_str += "=";
    g_tjc_hmi_str.from_int(num);
	g_tjc_hmi_str += "\xff\xff\xff";
}

void bsp_tjchmi_send_float(char* name, float num) {
    g_tjc_hmi_str.clear();
	auto scaled_num = int(num * 100); ///< 将浮点数缩放为整数
    g_tjc_hmi_str += name;
    g_tjc_hmi_str += "=";
    g_tjc_hmi_str.from_int(scaled_num);
	g_tjc_hmi_str += "\xff\xff\xff";
}

void bsp_tjchmi_send_cmd() {
    TJCHMI_Uart() << g_tjc_hmi_str.data();
}

int bsp_tjchmi_show_page (Vasl::TypePT* pt) {
    extern MQTT_SubRcvParam_u g_tables[];

    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(200);
        if (0U == g_tjchmi_page) {
            ///< 首页 显示6个餐桌订单有无状态
            for (size_t i = 0U; i < 6U; ++i) {
                bsp_tjchmi_send_int(const_cast<char*>(g_has_tjchmi_order_str[i]), g_has_tjchmi_order[i] ? 1 : 0);
                TJCHMI_Uart() << g_tjc_hmi_str.data();
                Vasl::delay_ms(1);
            }
        }
        else if (1U == g_tjchmi_page) {
            ///< 订单详情页 显示单桌订单详情
            if (1 > g_tjchmi_page_table) { VASL_PT_RESTART; }
            for (size_t i = 0U; i < 5U; ++i) {
                bsp_tjchmi_send_int(
                    const_cast<char*>(g_tjchmi_order_str[i]), 
                    g_tables[g_tjchmi_page_table - 1].order[i]);
                TJCHMI_Uart() << g_tjc_hmi_str.data();
                Vasl::delay_ms(1);
            }
        }
        else if (3U == g_tjchmi_page) {
            ///< 总订单详情页/出餐页
            for (size_t i = 0U; i < 5U; ++i) {
                bsp_tjchmi_send_int(
                    const_cast<char*>(g_tjchmi_main_order_str[i]), 
                    g_main_order.order[i]);
                TJCHMI_Uart() << g_tjc_hmi_str.data();
                Vasl::delay_ms(1);
            }
        }
    VASL_PT_TASK_END
}
