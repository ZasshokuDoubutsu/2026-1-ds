#ifndef __BSP_ESP8266_HPP__
#define __BSP_ESP8266_HPP__

#include "Vasl_Core.hpp"
#include "bsp_inst.hpp"

/** ================================================================================ *//**
 *  @brief      WiFi 配置参数
 * -------------------------------------------------------------------------------- */
#define __WIFI_SSID_1           "your_wifi_ssid"
#define __WIFI_PASSWORD_1       "your_wifi_password"
#define __WIFI_SSID_2           ""
#define __WIFI_PASSWORD_2       ""

constexpr const char WIFI_SSID_1[] = __WIFI_SSID_1;
constexpr const char WIFI_PASSWORD_1[] = __WIFI_PASSWORD_1;
constexpr const char WIFI_SSID_2[] = __WIFI_SSID_2;
constexpr const char WIFI_PASSWORD_2[] = __WIFI_PASSWORD_2;

/** ================================================================================ *//**
 *  @brief      MQTT 配置参数
 * -------------------------------------------------------------------------------- */
#define __TO_MQTT_CFG_USER_MSG "AT+MQTTUSERCFG="

/** ================================================================================ *//**
 *  @brief      Onenet 配置参数
 * -------------------------------------------------------------------------------- */
#define __ONENET_PRODUCT_ID_2   "your_product_id"
#define __ONENET_DEVICE_ID_2    "your_device_name"
#define __ONENET_TOKEN_2        "your_onenet_token"

constexpr const char ONENET_PRODUCT_ID_2[] = __ONENET_PRODUCT_ID_2;
constexpr const char ONENET_DEVICE_ID_2[] = __ONENET_DEVICE_ID_2;
constexpr const char ONENET_TOKEN_2[] = __ONENET_TOKEN_2;

extern Vasl::err_t bsp_uart2_init(void);
extern const char* __MQTT_SUBRCV_PARM[];
extern Vasl::flag_t g_esp_auto_task_flag[];
extern Vasl::flag_t g_tables_flag[];
extern MQTT_SubRcvParam_u g_tables[];
extern Vasl::TypePT PT_esp8266_rcv_analysis, PT_esp8266_auto_task;
extern Vasl::flag_t g_esp8266_rcv;
extern Vasl::str::static_string<256> g_esp8266_analysis_str;
extern bool g_has_tjchmi_order[6];

/** ================================================================================ *//**
 *  @brief      出餐队列管理 - FIFO原则
 * -------------------------------------------------------------------------------- */
constexpr size_t TABLE_COUNT {6U};
extern Vasl::TypeQueue<uint8_t, TABLE_COUNT> g_order_queue;

bool table_is_in_queue(uint8_t table_id);
void table_add_to_queue(uint8_t table_id);
void table_remove_from_queue(uint8_t table_id);
bool table_order_is_empty(uint8_t table_id);

Vasl::err_t bsp_esp8266_init(void);
int bsp_esp8266_rcv_analysis(Vasl::TypePT* pt);
int bsp_esp8266_auto_task(Vasl::TypePT* pt);

#define __MQTT_SUBRCV_PARM0_TABLE   "&t:"
#define __MQTT_SUBRCV_PARM1_RICE    "&r:"
#define __MQTT_SUBRCV_PARM2_NOODLE  "&n:"
#define __MQTT_SUBRCV_PARM3_COLA    "&c:"
#define __MQTT_SUBRCV_PARM4_ORANGE  "&o:"
#define __MQTT_SUBRCV_PARM5_JUICE   "&j:"
#define __MQTT_SUBRCV_PARM0_NAME_TABLE      "餐桌号"
#define __MQTT_SUBRCV_PARM1_NAME_CH_RICE    "米饭"
#define __MQTT_SUBRCV_PARM2_NAME_CH_NOODLE  "面条"
#define __MQTT_SUBRCV_PARM3_NAME_CH_COLA    "可乐"
#define __MQTT_SUBRCV_PARM4_NAME_CH_ORANGE  "橙汁"
#define __MQTT_SUBRCV_PARM5_NAME_CH_JUICE   "果汁"

#define __RICE_INDEX__      0
#define __NOODLE_INDEX__    1
#define __COLA_INDEX__      2
#define __ORANGE_INDEX__    3
#define __JUICE_INDEX__     4

#define __MQTT_SUBRCV_PREFIX    "+MQTTSUBRECV:"

#endif  ///< __BSP_ESP8266_HPP__
