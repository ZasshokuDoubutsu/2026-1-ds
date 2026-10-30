#ifndef __BSP_INST_HPP__
#define __BSP_INST_HPP__

#include "Vasl_Core.hpp"

/** ================================================================================ *//**
*  @brief      SysTick 相关
* -------------------------------------------------------------------------------- */
namespace Vasl { size_t get_ms_ticks(void); }

/** ================================================================================ *//**
*  @brief      UART 相关
* -------------------------------------------------------------------------------- */
constexpr size_t __UART7_RINGBUF_SIZE {256};
constexpr size_t __UART7_PRINTER_SIZE {128};
constexpr size_t __UART2_RINGBUF_SIZE {256+64};
constexpr size_t __UART2_PRINTER_SIZE {256+64};
constexpr size_t __UART9_RINGBUF_SIZE {128};
constexpr size_t __UART9_PRINTER_SIZE {128};
constexpr size_t __UART4_RINGBUF_SIZE {128};
constexpr size_t __UART4_PRINTER_SIZE {128};

extern Vasl::str::static_string<256> g_esp8266_analysis_str;

extern std::pair<Vasl::TypeUartRcvDataPacketRing<__UART7_RINGBUF_SIZE>, 
    Vasl::TypePrinter<__UART7_PRINTER_SIZE>>        Uart7;
extern std::pair<Vasl::TypeUartRcvDataPacketRing<__UART2_RINGBUF_SIZE>, 
    Vasl::TypePrinter<__UART2_PRINTER_SIZE>>        Uart2;
extern std::pair<Vasl::TypeUartRcvDataPacketRing<__UART9_RINGBUF_SIZE>, 
    Vasl::TypePrinter<__UART9_PRINTER_SIZE>>        Uart9;
extern std::pair<Vasl::TypeUartRcvDataPacketRing<__UART4_RINGBUF_SIZE>, 
    Vasl::TypePrinter<__UART4_PRINTER_SIZE>>        Uart4;

namespace Vasl {
    inline Vasl::TypePrinter<__UART7_PRINTER_SIZE>& log(void) { return Uart7.second; }
}

inline Vasl::TypeUartRcvDataPacketRing<__UART7_RINGBUF_SIZE>&   
            uart7_get_ringbuf(void) { return Uart7.first; }
inline Vasl::TypePrinter<__UART7_PRINTER_SIZE>&             
            uart7_get_printer(void) { return Uart7.second; }
inline Vasl::TypeUartRcvDataPacketRing<__UART2_RINGBUF_SIZE>&   
            uart2_get_ringbuf(void) { return Uart2.first; }
inline Vasl::TypePrinter<__UART2_PRINTER_SIZE>&             
            uart2_get_printer(void) { return Uart2.second; }
inline Vasl::TypeUartRcvDataPacketRing<__UART9_RINGBUF_SIZE>&   
            uart9_get_ringbuf(void) { return Uart9.first; }
inline Vasl::TypePrinter<__UART9_PRINTER_SIZE>&             
            uart9_get_printer(void) { return Uart9.second; }
inline Vasl::TypeUartRcvDataPacketRing<__UART4_RINGBUF_SIZE>&   
            uart4_get_ringbuf(void) { return Uart4.first; }
inline Vasl::TypePrinter<__UART4_PRINTER_SIZE>&             
            uart4_get_printer(void) { return Uart4.second; }

/** ================================================================================ *//**
*  @brief      ESP8266 相关
* -------------------------------------------------------------------------------- */
constexpr size_t ORDER_PARAM_COUNT {5};
union MQTT_SubRcvParam_u {
    struct {
        uint8_t rice;
        uint8_t noodle;
        uint8_t cola;
        uint8_t orange;
        uint8_t juice;
    };
    uint8_t order[ORDER_PARAM_COUNT];
};
using OrderParam_u = MQTT_SubRcvParam_u;

extern OrderParam_u g_main_order;
extern Vasl::flag_t g_esp_auto_task_flag[];
extern Vasl::flag_t g_esp8266_rcv;
extern Vasl::str::static_string<256> g_esp8266_analysis_str;

/** ================================================================================ *//**
*  @brief      OLED 相关
* -------------------------------------------------------------------------------- */
extern Disp::OLEDv3                             g_oled;
extern Disp::OLEDv3                             g_oled96;
inline Disp::OLEDv3&                            oled_get(void) { return g_oled; }
inline Disp::OLEDv3&                            oled96_get(void) { return g_oled96; }

/** ================================================================================ *//**
*  @brief      LED 相关
* -------------------------------------------------------------------------------- */
extern Vasl::TypeLED                            g_obled;
inline Vasl::TypeLED&                           obled_get(void) { return g_obled; }

/** ================================================================================ *//**
*  @brief      DEFINES
* -------------------------------------------------------------------------------- */
#define CMD_Uart()                              uart7_get_printer()
#define ESP8266_Uart()                          uart2_get_printer()
#define TJCHMI_Uart()                           uart9_get_printer()
#define BLE_Uart()                              uart4_get_printer()
#define OLED()                                  oled_get()

#endif ///< __BSP_INST_HPP__
