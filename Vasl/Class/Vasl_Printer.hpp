///< 相较于Vasl_Logger的更轻量级的打印类
///< 在不支持ANSI的环境下，可搭配Vasl_Macro.h:__VASL_ERR__等宏使用此类

#ifndef __VASL_PRINTER_HPP__
#define __VASL_PRINTER_HPP__

#include "Vasl_Layer.hpp"
#include "Vasl_Macro.h"
#include "NanoPrintf/Vasl_Vsnprintf.h"
#include <string.h>
#include "Strxx/Vasl_Strxx.hpp"
#include "Struct/Vasl_Array.hpp"
#include "Tools/Vasl_Tools.hpp"

#define PRINTER_INTERFACE_UART      (Uart)
// #define PRINTER_INTERFACE_RTT       (RTT)

#ifdef PRINTER_INTERFACE_UART
namespace Vasl::Interface::Printer {
    struct Uart {
        UartType* uart;
        volatile Vasl::err_t tx_ok_flag;
        Uart(const UartType* u = nullptr) : uart(const_cast<UartType*>(u)), tx_ok_flag(Vasl::OK) {}
        bool write(const char* data, size_t len) {
            if (!uart) return false;
            tx_ok_flag = Vasl::NOT;
            return Vasl::Layer::UART_Write(uart->p_ctrl, (uint8_t*)data, len);
        }
    };
}
#endif ///< define PRINTER_INTERFACE_UART

#ifdef PRINTER_INTERFACE_RTT
namespace Vasl::Interface::Printer {
    struct RTT {
        uint8_t channel = 0;
        bool write(const char* data, size_t len) {
            SEGGER_RTT_Write(channel, data, len);
            return true;
        }
    };
}
#endif ///< define PRINTER_INTERFACE_RTT

namespace Vasl {
    template<size_t BufSize = 128, size_t timeout_tick = 250,
                typename Backend = Interface::Printer::Uart>
    class TypePrinter {
    private:
        Vasl::array<char, BufSize>  buffer_;
        Backend                     backend_;

        void wait_tx_ok() {
            auto start_tick = Vasl::get_ms_ticks();
            // constexpr auto _timeout {0x180U * timeout_tick * 1000U};
            // auto timeout {_timeout};
            // while ((backend_.tx_ok_flag == Vasl::NOT) 
            //     && (timeout--) 
            //     && (Vasl::get_ms_ticks() - start_tick >= timeout_tick)) {
            //         Vasl::Layer::NOP();
            // }
            while (backend_.tx_ok_flag == Vasl::NOT) {
                if (Vasl::get_ms_ticks() - start_tick >= timeout_tick) {
                    break;
                }
            }
        }

        void write_formatted(const char* fmt, ...) {
            wait_tx_ok();
            va_list args;
            va_start(args, fmt);
            VASL_VSNPRINTF(buffer_.data(), buffer_.size(), fmt, args);
            va_end(args);
            backend_.write(buffer_.data(), Vasl::str::len(buffer_.data(), buffer_.size()));
        }

        void write_raw(const char* str) {
            wait_tx_ok();
            backend_.write(str, Vasl::str::len(str));
        }

    public:
        TypePrinter() = default;
        explicit TypePrinter(const Backend& backend) : backend_(backend) {}
        explicit TypePrinter(Backend&& backend) : backend_(std::move(backend)) {}
        ~TypePrinter() = default;

        void isrHandler_TransmitOver(void) { backend_.tx_ok_flag = Vasl::OK; }

        void operator()(const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            VASL_VSNPRINTF(buffer_.data(), buffer_.size(), fmt, args);
            va_end(args);
            backend_.write(buffer_.data(), Vasl::str::len(buffer_.data(), buffer_.size()));
        }

        TypePrinter& operator<<(const char* str) { write_raw(str); return *this; }
        TypePrinter& operator<<(int value) { write_formatted("%d", value); return *this; }
        TypePrinter& operator<<(size_t value) { write_formatted("%u", value); return *this; }
        TypePrinter& operator<<(uint32_t value) { write_formatted("%u", value); return *this; }
        TypePrinter& operator<<(uint16_t value) { write_formatted("%u", value); return *this; }
        TypePrinter& operator<<(uint8_t value) { write_formatted("%u", value); return *this; }
        TypePrinter& operator<<(int32_t value) { write_formatted("%d", value); return *this; }
        TypePrinter& operator<<(int16_t value) { write_formatted("%d", value); return *this; }
        TypePrinter& operator<<(int8_t value) { write_formatted("%d", value); return *this; }
        TypePrinter& operator<<(float value) { write_formatted("%f", value); return *this; }
        TypePrinter& operator<<(double value) { write_formatted("%lf", value); return *this; }
        TypePrinter& operator<<(char value) { backend_.write(&value, 1); return *this; }
        TypePrinter& operator<<(bool value) { write_raw(value ? "true" : "false"); return *this; }
        TypePrinter& operator<<(err_t flag) { write_raw(flag == OK ? "Vasl::OK" : "Vasl::NOT"); return *this; }

        TypePrinter& time() { write_raw(Vasl::get_timestamp()); return *this; }
        TypePrinter& time_a() { write_raw(Vasl::get_timestamp_a()); return *this; }

        TypePrinter& inf(const char* str = "INFO") { write_formatted("[%s] ", str); return *this; }
        TypePrinter& err(const char* str = "ERROR") { write_formatted("[%s] ", str); return *this; }
        TypePrinter& warn(const char* str = "WARNING") { write_formatted("[%s] ", str); return *this; }
        TypePrinter& msg(const char* str = "LOG") { write_formatted("[%s] ", str); return *this; }
        TypePrinter& suc(const char* str = "LOG") { write_formatted("[%s] ", str); return *this; }

        TypePrinter& inf_a(const char* str = "INFO") { write_formatted("\033[104;30m[%s]\033[0m \033[34m", str); return *this; }
        TypePrinter& err_a(const char* str = "ERROR") { write_formatted("\033[101;30m[%s]\033[0m \033[31m", str); return *this; }
        TypePrinter& warn_a(const char* str = "WARNING") { write_formatted("\033[103;30m[%s]\033[0m \033[33m", str); return *this; }
        TypePrinter& msg_a(const char* str = "LOG") { write_formatted("\033[102;30m[%s]\033[0m \033[32m", str); return *this; }
        TypePrinter& suc_a(const char* str = "LOG") { write_formatted("\033[102;30m[%s]\033[0m \033[32m", str); return *this; }

        void set_backend(const Backend& backend) { backend_ = backend; }
        Backend& get_backend() { return backend_; }
        const Backend& get_backend() const { return backend_; }
        bool write(const char* data, size_t len) { return backend_.write(data, len); }
        bool write(const char* str) { return backend_.write(str, Vasl::str::len(str)); }
    };
}

#endif ///< __VASL_PRINTER_HPP__
