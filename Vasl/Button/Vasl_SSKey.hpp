#ifndef __VASL_SSKEY_HPP__
#define __VASL_SSKEY_HPP__

#include "Vasl_Layer.hpp"
#include "Vasl_Type.hpp"

namespace SKeyDef {
    enum class State : uint8_t {
        RELEASE = 0,    /** 按键释放 */
        PRESS   = 1     /** 按键按下 */
    };

    enum class Event : uint8_t {
        NONE   = 0,    /** 无事件 */
        SHORT  = 1,    /** 短按事件 */
        LONG   = 2,    /** 长按事件 */
        REPEAT = 3     /** 重复触发事件 */
    };
} /** namespace SKey */

namespace Vasl {
    extern size_t get_ms_ticks(void);
    class TypeSKey {
    private:
        Vasl::level_t       pressLevel  : 1;    /** <-  Vasl::LOW 
                                                        Vasl::HIGH*/
        SKeyDef::State      currState   : 1;    /** <-  SKeyDef::RELEASE 
                                                        SKeyDef::PRESS*/
        SKeyDef::State      lastState   : 1;    /** <-  SKeyDef::RELEASE 
                                                        SKeyDef::PRESS*/
        Vasl::err_t         longFlag    : 1;    /** <-  Vasl::OK 
                                                        Vasl::NOT*/
        SKeyDef::Event      event       : 3;    /** <-  SKeyDef::NONE 
                                                        SKeyDef::SHORT 
                                                        SKeyDef::LONG 
                                                        SKeyDef::REPEAT*/
        Vasl::err_t         refreshEventFlag : 1;   /** Vasl::LOW 
                                                        Vasl::HIGH*/
        Vasl::void_func_t   callbackShort;      /** 短按回调函数 */
        Vasl::void_func_t   callbackLong;       /** 长按回调函数 */
        Vasl::void_func_t   callbackRepeat;     /** 重复触发回调函数 */
        size_t              pressTime;          /** 按键按下时间 */
        size_t              repeatTime;         /** 重复触发时间 */
        
        // [[deprecated("it will be private in future versions")]]
        void Scan(IOType io_) {
            this->currState = (this->pressLevel == static_cast<Vasl::level_t>(Vasl::Layer::GPIO_ReadPin(io_)))
                            ? SKeyDef::State::PRESS : SKeyDef::State::RELEASE;
        }

        // [[deprecated("it will be private in future versions")]]
        SKeyDef::Event Proc(size_t msTime_ = Vasl::get_ms_ticks(), size_t debounceTime_ = 16, size_t longPressTime_ = 1000, size_t repeatTime_ = 200);
        
    public:
        TypeSKey() : 
            pressLevel(Vasl::LOW), 
            currState(SKeyDef::State::RELEASE), 
            lastState(SKeyDef::State::RELEASE), 
            longFlag(Vasl::NOT), 
            event(SKeyDef::Event::NONE), 
            pressTime(0), 
            repeatTime(0) {}
        
        TypeSKey(Vasl::level_t level_) : 
            pressLevel(level_), 
            currState(SKeyDef::State::RELEASE), 
            lastState(SKeyDef::State::RELEASE), 
            longFlag(Vasl::NOT), 
            event(SKeyDef::Event::NONE), 
            pressTime(0), 
            repeatTime(0) {}

        void Init(Vasl::level_t level_);

        void RegisterEventCallback(Vasl::void_func_t callback_, SKeyDef::Event event_type_);

        SKeyDef::Event getEvent(void) { return this->event; }

        void InterruptTask(
            IOType io_, 
            size_t msTime_ = Vasl::get_ms_ticks(),
            size_t debounceTime_ = 16, 
            size_t longPressTime_ = 1000, 
            size_t repeatTime_ = 200);

        Vasl::err_t Process(SKeyDef::Event event_);
        
        Vasl::err_t operator() (void);
    };

} /** namespace Vasl */

#endif ///< __VASL_SSKEY_HPP__
