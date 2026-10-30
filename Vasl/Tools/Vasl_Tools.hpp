#ifndef __VASL_TOOLS_HPP__
#define __VASL_TOOLS_HPP__

#include "Vasl_Layer.hpp"

namespace Vasl {
    extern size_t get_ms_ticks(void);
    namespace str {
        extern int format(char* buf, size_t bufsz, const char* format, ...) noexcept;
    }
    inline const char* get_timestamp(void) {
        static char timestamp[32] = {0};      ///< 静态缓冲区
        size_t ms = get_ms_ticks();
        size_t seconds = ms / 1000;
        size_t milliseconds = ms - (seconds * 1000);

        Vasl::str::format(timestamp, sizeof timestamp, "[%02lu:%03lu] ", seconds, milliseconds);
        return timestamp;
    }
    inline const char* get_timestamp_a(void) {
        static char timestamp[42] = {0};      ///< 静态缓冲区
        size_t ms = get_ms_ticks();
        size_t seconds = ms / 1000;
        size_t milliseconds = ms - (seconds * 1000);

        Vasl::str::format(timestamp, sizeof(timestamp), "\033[90m[%02lu:%03lu]\033[0m ", seconds, milliseconds);
        return timestamp;
    }
}

namespace Vasl {
    /** ================================================================================ *//**
     *  @brief      线性映射
     *  @note       输入值超出范围时，输出值会被限制在输出范围的边界上
     *  @param[in]  x       输入值
     *  @param[in]  in_min  输入范围最小值
     *  @param[in]  in_max  输入范围最大值
     *  @param[out] out_min 输出范围最小值
     *  @param[out] out_max 输出范围最大值
     *  @return     映射后的输出值
     * 
     * -------------------------------------------------------------------------------- */
    template <typename T>
    inline T map(T x, T in_min, T in_max, T out_min, T out_max) {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
    /** ================================================================================ *//**
     *  @brief      限制值在指定范围内
     *  @note       输入值超出范围时，输出值会被限制在指定范围内
     *  @param[in]  x       输入值
     *  @param[in]  min_val 最小值
     *  @param[in]  max_val 最大值
     *  @return     限制后的输出值
     * 
     * -------------------------------------------------------------------------------- */
    template <typename T>
    inline T clamp(T x, T min_val, T max_val) {
        return std::max(min_val, std::min(max_val, x));
    }
    /** ================================================================================ *//**
     *  @brief      周期限制值在指定范围内
     *  @note       输入值超出范围时，输出值会被限制在指定周期内
     *  @warning    首次调用后，变量为 0/var旧值+1/o，如要使首次调用为 0/var旧值，请将period函数后置调用
     *  @param[in]  x       输入值
     *  @param[in]  period  周期值
     *  @param[in]  o       增量值，默认值为1
     *  @param[in]  offset  偏移值，默认值为0
     *  @return     周期限制后的输出值
     * 
     * -------------------------------------------------------------------------------- */
    template <typename T, typename U = int8_t>
    inline T period(T& x, size_t period, U o = 1, T offset = 0) {
        x += o;
        if(x >= period) {x = offset;}
        return x;
    }
}

namespace Vasl {
    struct PerfMs {
        size_t start_ms_tick;
        
        void start(size_t curr_ms_tick = Vasl::GetTickMs()) {
            start_ms_tick = curr_ms_tick;
        }
        size_t over(size_t curr_ms_tick = Vasl::GetTickMs()) {
            return curr_ms_tick - start_ms_tick;
        }
    };
}

#define VASL_PERF_LOGER(CH, ...)  // Vasl::log(CH, __VA_ARGS__)

///< 毫秒性能检测宏 - 开始
#define VASL_PERF_MS(name)    \
    size_t perf_ms_start_##name = Vasl::Layer::GetTickMs();

///< 毫秒性能检测宏 - 结束
#define VASL_PERF_MS_END(name, label)                                      \
    size_t duration_##name = Vasl::Layer::GetTickMs() - perf_ms_start_##name; \
    if (duration_##name > MS_COMPENSATION) {duration_##name -= MS_COMPENSATION;} \
    VASL_PERF_LOGER("[PERF-ms] Task |%s| use %lu ms" VASL_ENDL_R, label, duration_##name);

///< 微秒性能检测宏 - 开始
#define VASL_PERF_US(name)                                        \
    size_t perf_us_start_##name = Vasl::Layer::GetTickUs();        \
    uint32_t perf_cycle_start_##name = Vasl::Layer::GetCycleTick();

///< 微秒性能检测宏 - 结束
#define VASL_PERF_US_END(name, label)                                                 \
    size_t perf_us_end_##name = Vasl::Layer::GetTickUs();                               \
    size_t perf_us_duration_##name = perf_us_end_##name - perf_us_start_##name;           \
    uint32_t perf_cycle_end_##name = Vasl::Layer::GetCycleTick();                       \
    uint32_t perf_cycle_elapsed_##name = perf_cycle_end_##name - perf_cycle_start_##name; \
    if (perf_cycle_end_##name < perf_cycle_start_##name) {                               \
        perf_cycle_elapsed_##name = (0xFFFFFFFF - perf_cycle_start_##name) + perf_cycle_end_##name; \
        perf_us_duration_##name = perf_cycle_elapsed_##name / (SystemCoreClock / 1000000); \
    }                                                                                   \
    if (perf_us_duration_##name > US_COMPENSATION) {                                     \
        perf_us_duration_##name -= US_COMPENSATION;                                      \
    }                                                                                   \
    VASL_PERF_LOGER("[PERF-us] Task |%s| use %lu us (%lu cycles)" VASL_ENDL_R, label, perf_us_duration_##name, perf_cycle_elapsed_##name);


#endif ///< __VASL_TOOLS_HPP__
