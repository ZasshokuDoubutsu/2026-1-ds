#include "bsp_esp8266.hpp"

const char* __MQTT_SUBRCV_PARM[] {
    __MQTT_SUBRCV_PARM1_RICE,
    __MQTT_SUBRCV_PARM2_NOODLE,
    __MQTT_SUBRCV_PARM3_COLA,
    __MQTT_SUBRCV_PARM4_ORANGE,
    __MQTT_SUBRCV_PARM5_JUICE,
};
const char* __MQTT_SUBRCV_PARM_NAME_CH[] {
    __MQTT_SUBRCV_PARM1_NAME_CH_RICE,
    __MQTT_SUBRCV_PARM2_NAME_CH_NOODLE,
    __MQTT_SUBRCV_PARM3_NAME_CH_COLA,
    __MQTT_SUBRCV_PARM4_NAME_CH_ORANGE,
    __MQTT_SUBRCV_PARM5_NAME_CH_JUICE,
};

Vasl::flag_t g_esp_auto_task_flag[] {{Vasl::NOT}};
Vasl::flag_t g_tables_flag[] {Vasl::NOT, Vasl::NOT};
MQTT_SubRcvParam_u g_tables[6] {};
OrderParam_u g_main_order {};
Vasl::TypePT PT_esp8266_rcv_analysis {}, PT_esp8266_auto_task {};
Vasl::flag_t g_esp8266_rcv {Vasl::NOT};
Vasl::str::static_string<256> g_esp8266_analysis_str {};
bool g_has_tjchmi_order[6] {false};
Vasl::TypeQueue<uint8_t, TABLE_COUNT> g_order_queue {};
bool g_is_wifi_ok {false};

/** ================================================================================ *//**
 *  @brief      出餐队列函数实现
 * -------------------------------------------------------------------------------- */
bool table_is_in_queue(uint8_t table_id) {
    size_t avail = g_order_queue.available();
    for (size_t i = 0; i < avail; ++i) {
        if (g_order_queue.peek(i) == table_id) {
            return true;
        }
    }
    return false;
}

void table_add_to_queue(uint8_t table_id) {
    if (!table_is_in_queue(table_id)) {
        g_order_queue.push(table_id);
    }
}

void table_remove_from_queue(uint8_t table_id) {
    Vasl::TypeQueue<uint8_t, TABLE_COUNT> new_queue;
    size_t avail = g_order_queue.available();
    for (size_t i = 0; i < avail; ++i) {
        uint8_t t = g_order_queue.peek(i);
        if (t != table_id) {
            new_queue.push(t);
        }
    }
    g_order_queue = new_queue;
}

bool table_order_is_empty(uint8_t table_id) {
    for (int i = 0; i < int(ORDER_PARAM_COUNT); ++i) {
        if (g_tables[table_id].order[i] > 0) {
            return false;
        }
    }
    return true;
}

Vasl::err_t bsp_esp8266_init(void) {
    if (Vasl::err_t::OK != bsp_uart2_init()) { return Vasl::err_t::NOT; }
    return Vasl::err_t::OK;
}

int bsp_esp8266_rcv_analysis(Vasl::TypePT* pt) {
    static const char* p {nullptr};
    static uint8_t table_id {0U};
    const auto analysis_capacity = g_esp8266_analysis_str.capacity();

    VASL_PT_TASK_BEGIN
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == uart2_get_ringbuf().getRxFrameReadyFlag());
        {
        auto& ringbuf = uart2_get_ringbuf();
        ///< 拷贝并查找订阅接收参数
        for (auto len = ringbuf.getRxSize(); len-->0; ) {
            g_esp8266_analysis_str += ringbuf.pop();
        }
        ringbuf.clear();
        p = Vasl::str::find_str(
            g_esp8266_analysis_str.data(), 
            analysis_capacity, 
            __MQTT_SUBRCV_PREFIX);
        if (!p) { g_esp8266_rcv.set_ok(); VASL_PT_RESTART; }
        }
    VASL_PT_YIELD;
        ///< 查找餐桌ID
        p = Vasl::str::find_str_after(
            g_esp8266_analysis_str.data(), 
            analysis_capacity, 
            __MQTT_SUBRCV_PARM0_TABLE);
        if (!p) { g_esp8266_rcv.set_ok(); VASL_PT_RESTART; }
        table_id = static_cast<uint8_t>(Vasl::str::atou(p, ','));
        if (table_id) { table_id -= 1; }
        else { g_esp8266_rcv.set_ok(); VASL_PT_RESTART; }
        g_has_tjchmi_order[table_id] = true;
        table_add_to_queue(table_id);
        VASL_LOG_DBG(for (auto i {0U}; i < 6U; ++i) {
            Vasl::log().time_a().inf_a("DBG") 
                << "g_has_tjchmi_order[" << i << "] = " << g_has_tjchmi_order[i] << VASL_ENDL;
        })
    VASL_PT_YIELD;
        ///< 转换餐桌对应订单
        Vasl::log().time_a().inf_a() << "| 收到新订单 |  > 餐桌号: " << table_id + 1 << VASL_NEWL " 订单详情: " VASL_ENDL_R;
        for (int i = ORDER_PARAM_COUNT; i-->0; ) {
            p = Vasl::str::find_str_after(
                g_esp8266_analysis_str.data(), 
                analysis_capacity, 
                __MQTT_SUBRCV_PARM[ORDER_PARAM_COUNT - i - 1]);
            if (!p) { g_esp8266_rcv.set_ok(); VASL_PT_RESTART; }
            g_main_order.order[ORDER_PARAM_COUNT - i - 1]
                += g_tables[table_id].order[ORDER_PARAM_COUNT - i - 1] 
                = static_cast<uint8_t>(Vasl::str::atou(p, ','));
            Vasl::log() << VASL_T 
                << __MQTT_SUBRCV_PARM_NAME_CH[ORDER_PARAM_COUNT - i - 1] 
                << " x" << g_tables[table_id].order[ORDER_PARAM_COUNT - i - 1] << VASL_ENDL;
        }
        p = nullptr;
        g_esp8266_rcv.set_ok();
    VASL_PT_YIELD;
        ///< 输出总订单
        Vasl::log().time_a().inf_a() << "| 当前总订单 |" << VASL_ENDL;
        for (int i = ORDER_PARAM_COUNT; i-->0; ) {
            Vasl::log() << VASL_T 
                << __MQTT_SUBRCV_PARM_NAME_CH[ORDER_PARAM_COUNT - i - 1] 
                << " x" << g_main_order.order[ORDER_PARAM_COUNT - i - 1] << VASL_ENDL;
        }
        g_tables_flag[table_id].set_ok();
    VASL_PT_TASK_END
}

int bsp_esp8266_auto_task(Vasl::TypePT* pt) {
    constexpr auto _DELAY {3000U};

    VASL_PT_TASK_BEGIN
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == g_esp_auto_task_flag[0].get());
        g_esp_auto_task_flag[0].set_not();
        ESP8266_Uart() << "AT+CWMODE=1" VASL_ENDL;
        Vasl::log().time_a().inf_a() << "AutoTask [1/6] > 设置Station模式" << VASL_ENDL;
    VASL_PT_DELAY(_DELAY);
        ESP8266_Uart() << "AT+CWJAP=\"" __WIFI_SSID_1 "\",\"" __WIFI_PASSWORD_1 "\"" VASL_ENDL;
        Vasl::log().time_a().inf_a() << "AutoTask [2/6] > 连接到WiFi 1" << VASL_ENDL;
    VASL_PT_DELAY(_DELAY + 1000);
        ESP8266_Uart() << "AT+MQTTCLEAN=0" VASL_ENDL;
        Vasl::log().time_a().inf_a() << "AutoTask [3/6] > 清除MQTT连接" << VASL_ENDL;
    VASL_PT_DELAY(_DELAY);
        ESP8266_Uart() << 
            "AT+MQTTUSERCFG=0,1,\"" 
            __ONENET_DEVICE_ID_2 "\",\"" 
            __ONENET_PRODUCT_ID_2 "\",\"" 
            __ONENET_TOKEN_2 "\",0,0,\"\"" VASL_ENDL;
        Vasl::log().time_a().inf_a() << "AutoTask [4/6] > 配置用户MQTT信息" << VASL_ENDL;
    VASL_PT_DELAY(_DELAY);
        ESP8266_Uart() << "AT+MQTTCONN=0,\"218.201.45.2\",1883,1" VASL_ENDL;
        Vasl::log().time_a().inf_a() << "AutoTask [5/6] > 连接到MQTT服务器" << VASL_ENDL;
    VASL_PT_DELAY(_DELAY);
        ESP8266_Uart() << "AT+MQTTSUB=0,\"$sys/" << ONENET_PRODUCT_ID_2 << "/" << ONENET_DEVICE_ID_2 << "/thing/property/set\",0" VASL_ENDL;
        Vasl::log().time_a().inf_a() << "AutoTask [6/6] > 订阅主题 2" << VASL_ENDL;
    VASL_PT_DELAY(_DELAY - 1500);
        Vasl::log().time_a().suc_a() << "AutoTask 完成！" << VASL_ENDL_R;
        g_is_wifi_ok = true;
    VASL_PT_TASK_END
}
