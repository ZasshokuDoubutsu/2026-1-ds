#include "bsp_uart.hpp"

namespace Vasl {
    extern size_t get_ms_ticks(void);
}
/// @brief  UART7 主Serial T:613 R:614
std::pair<Vasl::TypeUartRcvDataPacketRing<__UART7_RINGBUF_SIZE>, Vasl::TypePrinter<__UART7_PRINTER_SIZE>>
    Uart7 {{}, Vasl::TypePrinter<__UART7_PRINTER_SIZE>{Vasl::Interface::Printer::Uart{&g_uart7}}};
/// @brief  UART2 ESP8266 T:302 R:301
std::pair<Vasl::TypeUartRcvDataPacketRing<__UART2_RINGBUF_SIZE>, Vasl::TypePrinter<__UART2_PRINTER_SIZE>> 
    Uart2 {{}, Vasl::TypePrinter<__UART2_PRINTER_SIZE>{Vasl::Interface::Printer::Uart{&g_uart2}}};
/// @brief  UART9 TJCLCD T:109 R:110
std::pair<Vasl::TypeUartRcvDataPacketRing<__UART9_RINGBUF_SIZE>, Vasl::TypePrinter<__UART9_PRINTER_SIZE>> 
    Uart9 {{}, Vasl::TypePrinter<__UART9_PRINTER_SIZE>{Vasl::Interface::Printer::Uart{&g_uart9}}};
/// @brief  UART4 BLE T:512 R:511
std::pair<Vasl::TypeUartRcvDataPacketRing<__UART4_RINGBUF_SIZE>, Vasl::TypePrinter<__UART4_PRINTER_SIZE>> 
    Uart4 {{}, Vasl::TypePrinter<__UART4_PRINTER_SIZE>{Vasl::Interface::Printer::Uart{&g_uart4}}};

Vasl::TypePT PT_uart7_checktimeout {}, PT_uart7_process {};
Vasl::TypePT PT_uart2_checktimeout {}, PT_uart2_process {};
Vasl::TypePT PT_uart9_checktimeout {}, PT_uart9_process {};
Vasl::TypePT PT_uart4_checktimeout {}, PT_uart4_process {};

extern Vasl::err_t bsp_softtimer_uart7_start(void);

extern "C" void ra_uart7_it_callback(uart_callback_args_t * p_args) {
    switch (p_args->channel) {
        default: break;
        case 7: {
            switch (p_args->event) {
                default: break;
                case UART_EVENT_RX_CHAR: {
                    Uart7.first.isrHandler((uint8_t)p_args->data, Vasl::get_ms_ticks());
                    break;
                }
                case UART_EVENT_TX_COMPLETE: {
                    Uart7.second.isrHandler_TransmitOver();
                    break;
                }
                case UART_EVENT_RX_COMPLETE: {
                    break;
                }
            }
            break;
        }
    }
}

extern "C" void ra_uart2_it_callback(uart_callback_args_t * p_args) {
    switch (p_args->channel) {
        default: break;
        case 2: {
            switch (p_args->event) {
                default: break;
                case UART_EVENT_RX_CHAR: {
                    Uart2.first.isrHandler((uint8_t)p_args->data, Vasl::get_ms_ticks());
                    break;
                }
                case UART_EVENT_TX_COMPLETE: {
                    Uart2.second.isrHandler_TransmitOver();
                    break;
                }
                case UART_EVENT_RX_COMPLETE: {
                    break;
                }
            }
            break;
        }
    }
}

extern "C" void ra_uart9_it_callback(uart_callback_args_t * p_args) {
    switch (p_args->channel) {
        default: break;
        case 9: {
            switch (p_args->event) {
                default: break;
                case UART_EVENT_RX_CHAR: {
                    Uart9.first.isrHandler((uint8_t)p_args->data, Vasl::get_ms_ticks());
                    break;
                }
                case UART_EVENT_TX_COMPLETE: {
                    Uart9.second.isrHandler_TransmitOver();
                    break;
                }
                case UART_EVENT_RX_COMPLETE: {
                    break;
                }
            }
            break;
        }
    }
}

extern "C" void ra_uart4_it_callback(uart_callback_args_t * p_args) {
    switch (p_args->channel) {
        default: break;
        case 4: {
            switch (p_args->event) {
                default: break;
                case UART_EVENT_RX_CHAR: {
                    Uart4.first.isrHandler((uint8_t)p_args->data, Vasl::get_ms_ticks());
                    break;
                }
                case UART_EVENT_TX_COMPLETE: {
                    Uart4.second.isrHandler_TransmitOver();
                    break;
                }
                case UART_EVENT_RX_COMPLETE: {
                    break;
                }
            }
            break;
        }
    }
}

Vasl::err_t bsp_uart7_init(void) {
    Uart7.first.init();
    Vasl::Layer::UART_Open(&g_uart7_ctrl, &g_uart7_cfg);
    return Vasl::OK;
}

Vasl::err_t bsp_uart2_init(void) {
    Uart2.first.init();
    if (FSP_SUCCESS != Vasl::Layer::UART_Open(&g_uart2_ctrl, &g_uart2_cfg)) { return Vasl::NOT; }
    return Vasl::OK;
}

Vasl::err_t bsp_uart9_init(void) {
    Uart9.first.init();
    if (FSP_SUCCESS != Vasl::Layer::UART_Open(&g_uart9_ctrl, &g_uart9_cfg)) { return Vasl::NOT; }
    return Vasl::OK;
}

Vasl::err_t bsp_uart4_init(void) {
    Uart4.first.init();
    if (FSP_SUCCESS != Vasl::Layer::UART_Open(&g_uart4_ctrl, &g_uart4_cfg)) { return Vasl::NOT; }
    return Vasl::OK;
}

auto bsp_uart7_checktimeout (Vasl::TypePT* pt) -> int {
    VASL_PT_TASK_BEGIN
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == Uart7.first.getRxStartFlag());
        Uart7.first.checkTimeout_CheckFrameReady(Vasl::get_ms_ticks());
    VASL_PT_TASK_END
}

int bsp_uart2_checktimeout (Vasl::TypePT* pt) {
    VASL_PT_TASK_BEGIN
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == Uart2.first.getRxStartFlag());
        Uart2.first.checkTimeout_CheckFrameReady(Vasl::get_ms_ticks());
    VASL_PT_TASK_END
}

int bsp_uart9_checktimeout (Vasl::TypePT* pt) {
    VASL_PT_TASK_BEGIN
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == Uart9.first.getRxStartFlag());
        Uart9.first.checkTimeout_CheckFrameReady(Vasl::get_ms_ticks());
    VASL_PT_TASK_END
}

int bsp_uart4_checktimeout (Vasl::TypePT* pt) {
    VASL_PT_TASK_BEGIN
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == Uart4.first.getRxStartFlag());
        Uart4.first.checkTimeout_CheckFrameReady(Vasl::get_ms_ticks());
    VASL_PT_TASK_END
}

int bsp_uart7_process (Vasl::TypePT* pt) {
    constexpr size_t BUFFER_SIZE {128U};
    extern std::pair<Vasl::err_t, Vasl::array<uint8_t, BUFFER_SIZE>> g_cmd_buffer;

    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(5);
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == Uart7.first.getRxFrameReadyFlag());
        const auto rx_size = Uart7.first.getRxSize();
        for (auto ret = rx_size; ret-- > 0; ) {
            g_cmd_buffer.second[rx_size - ret - 1] = Uart7.first.pop();
        }
        g_cmd_buffer.first = Vasl::OK;
        Uart7.first.clear();
    VASL_PT_TASK_END
}

int bsp_uart2_process (Vasl::TypePT* pt) {
    extern Vasl::flag_t g_esp8266_rcv;

    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(5);
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == g_esp8266_rcv.get());
        g_esp8266_rcv.set_not();
        ///< 回显接收数据 通过Serial7
        Vasl::log().time_a().inf_a() << "ESP8266 say: " << g_esp8266_analysis_str.data();
        g_esp8266_analysis_str.clear();
    VASL_PT_TASK_END
}

int bsp_uart9_process (Vasl::TypePT* pt) {
    constexpr size_t BUFFER_SIZE {128U};
    extern std::pair<Vasl::err_t, Vasl::array<uint8_t, BUFFER_SIZE>> g_tjchmi_cmd_buffer;

    VASL_PT_TASK_BEGIN
    VASL_PT_PERIOD_TASK(5);
    VASL_PT_WAIT_UNTIL_OR_SKIP(Vasl::OK == Uart9.first.getRxFrameReadyFlag());
        const auto rx_size = Uart9.first.getRxSize();
        for (auto ret = rx_size; ret-- > 0; ) {
            g_tjchmi_cmd_buffer.second[rx_size - ret - 1] = Uart9.first.pop();
        }
        g_tjchmi_cmd_buffer.first = Vasl::OK;
        Uart9.first.clear();
    VASL_PT_TASK_END
}
