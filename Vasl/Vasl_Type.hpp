#ifndef __VASL_TYPE_HPP__
#define __VASL_TYPE_HPP__

#include "Vasl_Layer.hpp"
#include <algorithm>

namespace Vasl {
    template <typename T>
    inline T clamp(T x, T min_val, T max_val);
}

/** ================================================================================ *//**
 *  @brief      定义Vasl::ENUMs类型
 * -------------------------------------------------------------------------------- */
namespace Vasl {
    enum switch_state_t : uint8_t {
        CLOSE = 0,      ///< 关闭
        OPEN = 1        ///< 打开
    };
    enum light_state_t : uint8_t {
        LIGHTOFF = 0,   ///< 灭
        LIGHTON = 1     ///< 亮
    };
    enum press_state_t : uint8_t {
        DOWN = 0,       ///< 按下
        UP = 1          ///< 松开
    };
    enum level_t : uint8_t {
        LOW = VASL_LAYER_PIN_RESET_LEVEL,   ///< 低电平
        HIGH = VASL_LAYER_PIN_SET_LEVEL     ///< 高电平
    };
    enum done_state_t : uint8_t {
        NO = 0,         ///< 未完成
        YES = 1,        ///< 完成
    };
    enum err_t : uint8_t {
        OK  = 0,        ///< 成功
        NOT = 1,        ///< 失败/错误
    };
}

/** ================================================================================ *//**
 *  @brief      定义普通类型
 * -------------------------------------------------------------------------------- */
namespace Vasl {
    using it_func_t = void (* volatile)(void);
    using void_func_t = void (*)(void);
}

namespace Vasl {
    ///< 单个Flag包
    struct flag_t {
    private:
    public:
        Vasl::err_t flags;
        void set(Vasl::err_t flag) { flags = flag; }
        void set(Vasl::err_t flag) volatile { flags = flag; }
        void set_ok() { flags = Vasl::OK; }
        void set_ok() volatile { flags = Vasl::OK; }
        void set_not() { flags = Vasl::NOT; }
        void set_not() volatile { flags = Vasl::NOT; }
        Vasl::err_t get() const { return flags; }
        Vasl::err_t get() const volatile { return flags; }
        void toggle() { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void toggle() volatile { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void operator=(Vasl::err_t flag) { flags = flag; }
        void operator=(Vasl::err_t flag) volatile { flags = flag; }
        void operator!() { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void operator!() volatile { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void operator~() { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void operator~() volatile { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        operator bool() const { return flags == Vasl::OK; }
        operator bool() const volatile { return flags == Vasl::OK; }
        bool operator==(Vasl::err_t flag) const { return flags == flag; }
        bool operator==(Vasl::err_t flag) volatile { return flags == flag; }
    };
    class TypeFlag {
    private:
        Vasl::err_t flags;
    public:
        TypeFlag() : flags(Vasl::NOT) {}
        TypeFlag(Vasl::err_t f) : flags(f) {}
        ~TypeFlag() {}
        void set(Vasl::err_t flag) { flags = flag; }
        void set(Vasl::err_t flag) volatile { flags = flag; }
        void set_ok() { flags = Vasl::OK; }
        void set_ok() volatile { flags = Vasl::OK; }
        void set_not() { flags = Vasl::NOT; }
        void set_not() volatile { flags = Vasl::NOT; }
        Vasl::err_t get() const { return flags; }
        Vasl::err_t get() const volatile { return flags; }
        void toggle() { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void toggle() volatile { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void operator=(Vasl::err_t flag) { flags = flag; }
        void operator=(Vasl::err_t flag) volatile { flags = flag; }
        void operator!() { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void operator!() volatile { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void operator~() { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        void operator~() volatile { flags = (flags == Vasl::OK) ? Vasl::NOT : Vasl::OK; }
        operator bool() const { return flags == Vasl::OK; }
        operator bool() const volatile { return flags == Vasl::OK; }
    };
    template<typename T = bool, T VarOK = true>
    struct ret_t {
        T _ret = VarOK;

        void operator<<(T r) { if (r != VarOK) { _ret = !VarOK; } }
        void operator=(T r) { _ret = r; }
        T is_ok() const { return _ret == VarOK; }
        T is_not() const { return _ret != VarOK; }
        T is_not_ok() const { return _ret != VarOK; }
    };
}

namespace Vasl {
    template<typename T = size_t>
    struct usage_t {    ///< 请勿手动初始化/构造
        T       total   {0};
        T       free    {0};
        flag_t  is_free {Vasl::NOT};

        void isr(void) {
            total++;
            if(is_free.get() == Vasl::OK) { free++; }
        }
        void task(void) {is_free.set_not();}
        void idle(void) {is_free.set_ok();}
        void reset(void) {
            total = 0;
            free = 0;
        }
        float get_usage(float min_val = 0.0f, float max_val = 99.9f) const {
            if (total == 0) { return 0.0f; }
            return Vasl::clamp((float)(total - free) / (float)total * 100.0f, min_val, max_val);
        }
        float get_usage_and_reset(void) {
            float usage = get_usage();
            reset();
            return usage;
        }
    };
}

#endif ///< __VASL_TYPE_HPP__
