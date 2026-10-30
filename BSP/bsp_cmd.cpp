#include "bsp_cmd.hpp"
#include "bsp_esp8266.hpp"

/** ================================================================================ *//**
 *  @brief      NLC命令处理程序声明
 * -------------------------------------------------------------------------------- */
static void nlc_handle_help(int argc, char** argv);
static void nlc_handle_vasl(int argc, char** argv);
static void nlc_handle_at(int argc, char** argv);
static void nlc_handle_em(int argc, char** argv);

static void nlc_handle_car_done(int argc, char** argv);
static void nlc_handle_order(int argc, char** argv);
static void nlc_handle_page(int argc, char** argv);
// static void nlc_handle_tjcdbg(int argc, char** argv);

/** ================================================================================ *//**
 *  @brief      变量声明定义
 * -------------------------------------------------------------------------------- */
std::pair<Vasl::err_t, Vasl::array<uint8_t, BUFFER_SIZE>> g_cmd_buffer {Vasl::NOT, {}};
std::pair<Vasl::err_t, Vasl::array<uint8_t, BUFFER_SIZE>> g_tjchmi_cmd_buffer {Vasl::NOT, {}};
Vasl::TypePT PT_cmd_process {}, PT_tjchmi_cmd_process {};

/** ================================================================================ *//**
 *  @brief      命令表定义
 * -------------------------------------------------------------------------------- */
static volatile Vasl::NLC_CommandDefinition s_SysCmdTable[CMD_COUNT_MAX] = {
	{"/help", nlc_handle_help},
	{"/vasl", nlc_handle_vasl},
	{"/at", nlc_handle_at},
	{"/em", nlc_handle_em},
};
static volatile Vasl::NLC_CommandDefinition s_TjchmiCmdTable[TJCHMI_CMD_COUNT_MAX] = {
	{"/car_done", nlc_handle_car_done},
	{"/order", nlc_handle_order},
	{"/page", nlc_handle_page},
	// {"/tjcdbg", nlc_handle_tjcdbg},
};

/** ================================================================================ *//**
 *  @brief      NLCCMD进程处理
 * -------------------------------------------------------------------------------- */
Vasl::err_t bsp_cmd_start(void) { /* NLCCMD初始化程序 */
    g_cmd_buffer.first = Vasl::NOT;
    g_cmd_buffer.second.fill(0);
    return Vasl::OK;
}

Vasl::err_t bsp_tjchmi_cmd_start(void) { /* TJCHMI_CMD初始化程序 */
    g_tjchmi_cmd_buffer.first = Vasl::NOT;
    g_tjchmi_cmd_buffer.second.fill(0);
    return Vasl::OK;
}

int bsp_cmd_process(Vasl::TypePT* pt) { /* NLCCMD处理程序 */
    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(20);
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == g_cmd_buffer.first);
        Vasl::NLC_ProcessCore(
            (const char*)(g_cmd_buffer.second.data()), 
            const_cast<Vasl::NLC_CommandDefinition*>(s_SysCmdTable), 
            CMD_COUNT_MAX);
        bsp_cmd_start();
    VASL_PT_TASK_END
}

int bsp_tjchmi_cmd_process(Vasl::TypePT* pt) { /* TJCHMI_CMD处理程序 */
    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(20);
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == g_tjchmi_cmd_buffer.first);
        Vasl::NLC_ProcessCore(
            (const char*)(g_tjchmi_cmd_buffer.second.data()), 
            const_cast<Vasl::NLC_CommandDefinition*>(s_TjchmiCmdTable), 
            TJCHMI_CMD_COUNT_MAX);
        bsp_tjchmi_cmd_start();
    VASL_PT_TASK_END
}

/** ================================================================================ *//**
 *  @brief      NLC命令集
 * -------------------------------------------------------------------------------- */
static void nlc_handle_help(int argc, char** argv) {    /* ---------- help 命令处理 ---------- */
    (void)argc; (void)argv;
    Vasl::log().time_a().inf_a() << "* |NLC CMD Help|" VASL_ENDL;
    for (int i = 0; i < CMD_COUNT_MAX - 1; i++) {
        Vasl::log() << "->  " << s_SysCmdTable[i + 1].prefix << VASL_ENDL;
    }
    return;
}

static void nlc_handle_vasl(int argc, char** argv) {    /* ---------- vasl 命令处理 ---------- */
    (void)argc; (void)argv;
    if(argc < 1) {
        Vasl::log().time_a().inf_a() << VASL_NEWL "->  Vasl 32-bit MCU library" VASL_ENDL;
        Vasl::log() << "->  /vasl * : ver | rst | ..." VASL_ENDL;
        return;
    }

    if(VASL_CMD(argv, 2, "ver")) {   /* 命令1-1 ver */
        Vasl::log().time_a().inf_a() << "\n->  Vasl " << Vasl::Layer::Version() << VASL_ENDL;
        return;
    }
    else if(VASL_CMD(argv, 2, "rst")) {   /* 命令1-1 rst */
        Vasl::log().time_a().inf_a() << VASL_NEWL "->  即将在1s内重启..." VASL_ENDL;
        Vasl::delay_ms(800U);
        Vasl::Layer::SoftRestart();
        return;
    }
    else {  /* 未知命令/参数错误 */
        Vasl::log().time_a().inf_a() << "未知参数2: " << argv[0] << VASL_ENDL;
        return;
    }
}

#define UNDEF_C_STR "## undef "
static void nlc_handle_at(int argc, char** argv) {    /* ---------- at 命令处理 ---------- */
    (void)argc; (void)argv;
    if(argc < 1) {
        Vasl::log().time_a().inf_a() <<
            VASL_NEWL "->  at 命令是用于发送对应指令到ESP8266WiFi模块"
            VASL_NEWL "->  /at * : "
                "to | "
                "help(h) | "
                "mode | "
                "auto | "
                "wifi | "
                "mqtt_cfg(mcfg) | "
                "mqtt_conn(mcn) | "
                "mqtt_sub(msub) | "
                "mqtt_clean(mclean) | "
                "..." VASL_ENDL;
        return;
    }

    if(VASL_CMD(argv, 2, "to")) {
        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /at to AT 吧！" VASL_ENDL; } 
        else { ESP8266_Uart() << argv[1] << VASL_ENDL; }
        return;
    }
    else if(VASL_CMD(argv, 2, "help") || VASL_CMD(argv, 2, "h")) {
        if (argc < 2) {
            Vasl::log().time_a().inf_a() << "试试: /at help mqtt 吧！" VASL_ENDL; }
        else if(VASL_CMD(argv, 3, "mqtt")) {
            Vasl::log().time_a().inf_a() 
                << "MQTT连接步骤：" VASL_ENDL
                << "~0 </at wifi 1> ESP8266连接网络；" VASL_ENDL
                << "~1 </at mqtt_cfg 2> 设置MQTT用户配置；" VASL_ENDL
                << "~2 </at mqtt_conn onenet> 连接OneNet MQTT服务器；" VASL_ENDL
                << "~3 </at mqtt_sub 2> 订阅系统命令Topic；" VASL_ENDL
                << "~4 </at mqtt_clean> 断开并清理MQTT连接；" VASL_ENDL
                << "..." VASL_ENDL
                << VASL_ENDL;
        }
        return;
    }
    else if(VASL_CMD(argv, 2, "mode")) {
        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /at mode sta 吧！" VASL_ENDL; }
        else if(Vasl::str::cmp(argv[1], "sta")) {
            ESP8266_Uart() << "AT+CWMODE=1" VASL_ENDL;
        }
        else if(Vasl::str::cmp(argv[1], "ap")) {
            ESP8266_Uart() << "AT+CWMODE=0" VASL_ENDL;
        } 
        else {
            Vasl::log().time_a().inf_a() << "未知参数3: " << argv[1] << VASL_ENDL;
        }
        return;
    }
    else if(VASL_CMD(argv, 2, "auto")) {
        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /at auto sub_onenet 吧！" VASL_ENDL; }
        else if(Vasl::str::cmp(argv[1], "sub_onenet")) {
            g_esp_auto_task_flag[0].set_ok();
        }
        else {
            Vasl::log().time_a().inf_a() << "未知参数3: " << argv[1] << VASL_ENDL;
        }
        return;
    }
    else if(VASL_CMD(argv, 2, "wifi")) {
        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /at wifi 2 吧！" VASL_ENDL; }
        else if(Vasl::str::cmp(argv[1], "1")) {
            ESP8266_Uart() << "AT+CWJAP=\"" __WIFI_SSID_1 "\",\"" __WIFI_PASSWORD_1 "\"" VASL_ENDL;
        } 
        else if(Vasl::str::cmp(argv[1], "2")) {
            ESP8266_Uart() << "AT+CWJAP=\"" __WIFI_SSID_2 "\",\"" __WIFI_PASSWORD_2 "\"" VASL_ENDL;
        } 
        else {
            Vasl::log().time_a().inf_a() << "未知参数3: " << argv[1] << VASL_ENDL;
        }
        return;
    }
    else if (VASL_CMD(argv, 2, "mqtt_cfg") || VASL_CMD(argv, 2, "mcfg")) {
        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /at mqtt_cfg 2 吧！" VASL_ENDL; }
        // else if(Vasl::str::cmp(argv[1], "1")) {
        //     ESP8266_Uart() << 
        //         "AT+MQTTUSERCFG=0,1,\"" __ONENET_DEVICE_ID_1 "\",\"" __ONENET_PRODUCT_ID_1 "\",\"" __ONENET_TOKEN_1 "\",0,0,\"\""
        //         VASL_ENDL;
        // } 
        else if(Vasl::str::cmp(argv[1], "2")) {
            ESP8266_Uart() << 
                "AT+MQTTUSERCFG=0,1,\"" __ONENET_DEVICE_ID_2 "\",\"" __ONENET_PRODUCT_ID_2 "\",\"" __ONENET_TOKEN_2 "\",0,0,\"\""
                VASL_ENDL;
        } 
        else {
            Vasl::log().time_a().inf_a() << "未知参数3: " << argv[1] << VASL_ENDL;
        }
        return;
    }
    else if(VASL_CMD(argv, 2, "mqtt_conn") || VASL_CMD(argv, 2, "mcn")) {
        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /at mqtt_conn onenet 吧！" VASL_ENDL; }
        else if(Vasl::str::cmp(argv[1], "onenet")) {
            ESP8266_Uart() << "AT+MQTTCONN=0,\"218.201.45.2\",1883,1" VASL_ENDL;
        } 
        else {
            Vasl::log().time_a().inf_a() << "未知参数3: " << argv[1] << VASL_ENDL;
        }
        return;
    }
    else if(VASL_CMD(argv, 2, "mqtt_sub") || VASL_CMD(argv, 2, "msub")) {
        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /at mqtt_sub 2 吧！" VASL_ENDL; }
        // else if(Vasl::str::cmp(argv[1], "1")) {
        //     ESP8266_Uart() << "AT+MQTTSUB=0,\"$sys/" << ONENET_PRODUCT_ID_1 << "/" << ONENET_DEVICE_ID_1 << "/thing/property/set\",0" VASL_ENDL;
        // } 
        else if(Vasl::str::cmp(argv[1], "2")) {
            ESP8266_Uart() << "AT+MQTTSUB=0,\"$sys/" << ONENET_PRODUCT_ID_2 << "/" << ONENET_DEVICE_ID_2 << "/thing/property/set\",0" VASL_ENDL;
        } 
        else {
            Vasl::log().time_a().inf_a() << "未知参数3: " << argv[1] << VASL_ENDL;
        }
        return;
    }
    else if(VASL_CMD(argv, 2, "mqtt_clean") || VASL_CMD(argv, 2, "mclean")) {
        ESP8266_Uart() << "AT+MQTTCLEAN=0" VASL_ENDL;
        return;
    }
    else {  /* 未知命令/参数错误 */
        Vasl::log().time_a().inf_a() << "未知参数2: " << argv[0] << VASL_ENDL;
        return;
    }
}

static void nlc_handle_em(int argc, char** argv) {    /* ---------- em 命令处理 ---------- */
    (void)argc; (void)argv;
    if(argc < 1) {
        Vasl::log().time_a().inf_a() 
            << VASL_NEWL "->  em 命令是用于控制EasyMenu菜单"
            VASL_NEWL "->  /em * : input(ip) | ..." VASL_ENDL;
        return;
    }

    if(VASL_CMD(argv, 2, "input") || VASL_CMD(argv, 2, "ip")) {   /* 命令1-1 input */
        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /em input enter 吧！" VASL_ENDL; } 
        else if(Vasl::str::cmp(argv[1], "enter") 
            || Vasl::str::cmp(argv[1], "right") 
            || Vasl::str::cmp(argv[1], "r")) {
            Vasl::EM::input(EASY_MENU_RIGHT);
        }
        else if(Vasl::str::cmp(argv[1], "back")
            || Vasl::str::cmp(argv[1], "left") 
            || Vasl::str::cmp(argv[1], "l")) {
            Vasl::EM::input(EASY_MENU_LEFT);
        }
        else if(Vasl::str::cmp(argv[1], "up")
            || Vasl::str::cmp(argv[1], "u")) {
            Vasl::EM::input(EASY_MENU_UP);
        }
        else if(Vasl::str::cmp(argv[1], "down")
            || Vasl::str::cmp(argv[1], "d")) {
            Vasl::EM::input(EASY_MENU_DOWN);
        }
        return;
    }
    else {  /* 未知命令/参数错误 */
        Vasl::log().time_a().inf_a() << "未知参数2: " << argv[0] << VASL_ENDL;
        return;
    }
}

static void nlc_handle_order(int argc, char** argv) {    /* ---------- order 命令处理 ---------- */
    (void)argc; (void)argv;
    extern Vasl::TypeQueue<uint8_t, TABLE_COUNT> g_order_queue;
    extern bool g_has_tjchmi_order[6];

    if(argc < 1) {
        Vasl::log().time_a().inf_a()
            << VASL_NEWL "->  order 命令是用于控制订单"
            VASL_NEWL "->  /order * : sub(s) | ..." VASL_ENDL;
        return;
    }

    if(VASL_CMD(argv, 2, "sub") || VASL_CMD(argv, 2, "s")) {   /* 命令1-1 sub */
        constexpr const auto _index {1};
        auto update = [](char** argv_) {
            if (Vasl::str::cmp(argv_[_index], "r")) {
                return __RICE_INDEX__;
            }
            else if (Vasl::str::cmp(argv_[_index], "n")) {
                return __NOODLE_INDEX__;
            }
            else if (Vasl::str::cmp(argv_[_index], "c")) {
                return __COLA_INDEX__;
            }
            else if (Vasl::str::cmp(argv_[_index], "o")) {
                return __ORANGE_INDEX__;
            }
            else if (Vasl::str::cmp(argv_[_index], "j")) {
                return __JUICE_INDEX__;
            }
            else { return -1; }
        };

        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /order sub r吧！" VASL_ENDL; }
        else {
            auto const index = update(argv);
            if (index < 0) {
                Vasl::log().time_a().err_a("NLC") << "未知参数3: " << argv[_index] << VASL_ENDL
                    << "请使用 r/n/c/o/j 中的一个" << VASL_ENDL;
                return;
            }

            ///< 按FIFO顺序找到有该菜的桌
            const auto avail = g_order_queue.available();
            for (size_t i = 0; i < avail; ++i) {
                uint8_t table_id = g_order_queue.peek(i);
                if (g_tables[table_id].order[index] > 0) {
                    g_tables[table_id].order[index] -= 1;
                    VASL_LOG_DBG(Vasl::log().time_a().inf_a("ORDER")
                        << "桌号" << table_id + 1 << "出餐: index=" << index << VASL_ENDL;)
                    ///< 检查该桌所有菜品是否都出完
                    if (table_order_is_empty(table_id)) {
                        VASL_LOG_DBG(Vasl::log().time_a().suc_a("ORDER")
                            << "桌号" << table_id + 1 << "所有菜品出餐完成！" << VASL_ENDL;)
                        g_tables_flag[table_id].set_not();
                        g_has_tjchmi_order[table_id] = false;
                        table_remove_from_queue(table_id);
                        ///< 通知STM32小车
                        BLE_Uart() << "/order done t" << table_id + 1 << VASL_ENDL;
                    }
                    break;
                }
            }

            if (g_main_order.order[index] > 0) {
                g_main_order.order[index] -= 1;
            }
        }
        return;
    }
    else {  /* 未知命令/参数错误 */
        Vasl::log().time_a().inf_a() << "未知参数2: " << argv[0] << VASL_ENDL;
        return;
    }
}

static void nlc_handle_car_done(int argc, char** argv) {    /* ---------- car_done 命令处理 ---------- */
    (void)argc; (void)argv;
    if(argc < 1) {
        Vasl::log().time_a().inf_a() 
            << VASL_NEWL "->  order 命令是用于控制订单"
            VASL_NEWL "->  /order * : sub(s) | ..." VASL_ENDL;
        return;
    }

    if(VASL_CMD(argv, 2, "sub") || VASL_CMD(argv, 2, "s")) {   /* 命令1-1 sub */
        auto update = [](char** argv_) {
            if (Vasl::str::cmp(argv_[2], "r")) {
                return __RICE_INDEX__;
            }
            else if (Vasl::str::cmp(argv_[2], "n")) {
                return __NOODLE_INDEX__;
            }
            else if (Vasl::str::cmp(argv_[2], "c")) {
                return __COLA_INDEX__;
            }
            else if (Vasl::str::cmp(argv_[2], "o")) {
                return __ORANGE_INDEX__;
            }
            else if (Vasl::str::cmp(argv_[2], "j")) {
                return __JUICE_INDEX__;
            }
            else { return -1;  }
        };
        if (argc < 2) { Vasl::log().time_a().inf_a() << "试试: /order sub 1 r吧！" VASL_ENDL; } 
        else if(Vasl::str::cmp(argv[1], "1")) { 
            auto const index = update(argv);
            if (index < 0) {
                Vasl::log().time_a().err_a("NLC") << "未知参数3: " << argv[2] << VASL_ENDL
                    << "请使用 r/n/c/o/j 中的一个" << VASL_ENDL;
                return;
            }
            g_tables[0].order[index] -= 1;  ///< 因为参数少 所以直接硬编码  若table参数可选值较多 应对argv[1]使用Vasl::str::atou
            g_main_order.order[index] -= 1;
        }
        else if(Vasl::str::cmp(argv[1], "2")) {
            auto const index = update(argv);
            g_tables[1].order[index] -= 1;
            g_main_order.order[index] -= 1;
        }
        return;
    }
    else {  /* 未知命令/参数错误 */
        Vasl::log().time_a().inf_a() << "未知参数2: " << argv[0] << VASL_ENDL;
        return;
    }
}

static void nlc_handle_page(int argc, char** argv) {    /* ---------- page 命令处理 ---------- */
    (void)argc; (void)argv;
    extern size_t g_tjchmi_page, g_tjchmi_page_table;

    if(argc < 1) {
        Vasl::log().time_a().inf_a() 
            << VASL_NEWL "->  page 命令是用于标记TJC LCD当前页面" VASL_ENDL;
        return;
    }

    if(VASL_CMD(argv, 2, "0")) {
        g_tjchmi_page = 0U;
        g_tjchmi_page_table = 0U;
    }
    else if(VASL_CMD(argv, 2, "3")) {
        g_tjchmi_page = 3U;
        g_tjchmi_page_table = 0U;
    }
    else if(VASL_CMD(argv, 2, "t1")) {
        g_tjchmi_page = 1U;
        g_tjchmi_page_table = 1U;
    }
    else if(VASL_CMD(argv, 2, "t2")) {
        g_tjchmi_page = 1U;
        g_tjchmi_page_table = 2U;
    }
    else if(VASL_CMD(argv, 2, "t3")) {
        g_tjchmi_page = 1U;
        g_tjchmi_page_table = 3U;
    }
    else if(VASL_CMD(argv, 2, "t4")) {
        g_tjchmi_page = 1U;
        g_tjchmi_page_table = 4U;
    }
    else if(VASL_CMD(argv, 2, "t5")) {
        g_tjchmi_page = 1U;
        g_tjchmi_page_table = 5U;
    }
    else if(VASL_CMD(argv, 2, "t6")) {
        g_tjchmi_page = 1U;
        g_tjchmi_page_table = 6U;
    }
    else {  /* 未知命令/参数错误 */
        Vasl::log().time_a().inf_a() << "未知参数2: " << argv[0] << VASL_ENDL;
        return;
    }
}


