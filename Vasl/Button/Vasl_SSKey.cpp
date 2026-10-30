#include "Button/Vasl_SSKey.hpp"
#include "Vasl_Layer.hpp"
#include "Vasl_Type.hpp"

namespace Vasl {
    /** ================================================================================ *//**
    *   @brief      初始化按键方法
    *   @param[in]  this->pressLevel: 按键电平类型 SKeyDef::Type_PressLevel
    * 
    * -------------------------------------------------------------------------------- */
    void TypeSKey::Init(Vasl::level_t level_) {
        this->pressLevel = level_;
        this->currState  = SKeyDef::State::RELEASE;
        this->lastState  = SKeyDef::State::RELEASE;
        this->longFlag   = Vasl::NOT;
        this->event      = SKeyDef::Event::NONE;
        this->pressTime  = 0;
        this->repeatTime = 0;
    }

    /** ================================================================================ *//**
     *  @brief      注册按键事件回调函数方法
     *  @param[in]  callback_: 回调函数指针
     *  @param[in]  event_type: 按键事件类型 SKeyDef::Event
     * 
     * -------------------------------------------------------------------------------- */
    void TypeSKey::RegisterEventCallback(Vasl::void_func_t callback_, SKeyDef::Event event_type_) {
        if(nullptr == callback_) { return; }

        switch (event_type_) {
            default: break;
            case SKeyDef::Event::SHORT:
                this->callbackShort = callback_;
                break;
            case SKeyDef::Event::LONG:
                this->callbackLong = callback_;
                break;
            case SKeyDef::Event::REPEAT:
                this->callbackRepeat = callback_;
                break;
        }
    }

    /** ================================================================================ *//**
    *   @brief      按键处理方法
    *   @param[in]  msTime: 当前时间毫秒数
    *   @param[in]  debounceTime: 去抖时间毫秒数
    *   @param[in]  longPressTime: 长按时间毫秒数
    *   @param[in]  repeatTime: 重复触发时间毫秒数
    *   @param[out] this->event: 按键事件 SKeyDef::SKeyEvent
    *   @return     SKeyDef::SKeyEvent
    * 
    * -------------------------------------------------------------------------------- */
    SKeyDef::Event TypeSKey::Proc(size_t msTime_, size_t debounceTime_, size_t longPressTime_, size_t repeatTime_) {
        if(debounceTime_ >= longPressTime_ || debounceTime_ >= repeatTime_) {
            return SKeyDef::Event::NONE;
        }
        
        if(this->currState != this->lastState) {
            this->lastState = this->currState;
            if(SKeyDef::State::PRESS == this->currState) { /** 按下 */
                this->pressTime = msTime_;
                this->longFlag = Vasl::NOT;
            } else if((msTime_ - this->pressTime > debounceTime_) && (this->longFlag == Vasl::NOT)) {    /** 释放或短按 */
                    return SKeyDef::Event::SHORT;
            }
            return SKeyDef::Event::NONE;
        }

        if(SKeyDef::State::PRESS == this->currState && (msTime_ - this->pressTime) > longPressTime_) {
            if(Vasl::NOT == this->longFlag) {   /** 长按首次 */
                this->longFlag = Vasl::OK;
                this->repeatTime = msTime_;
                return SKeyDef::Event::LONG;
            } else if(msTime_ - this->repeatTime > repeatTime_) { /** 长按重复 */
                this->repeatTime = msTime_;
                return SKeyDef::Event::REPEAT;
            }
        }
        
        return SKeyDef::Event::NONE;
    }

    /** ================================================================================ *//**
     *  @brief      按键事件回调处理方法
     *  @note       处理按键事件，调用注册的回调函数
     *  @return     Vasl::err_t
     * 
     * -------------------------------------------------------------------------------- */
    Vasl::err_t TypeSKey::Process(SKeyDef::Event event_) {
        switch (event_) {
            default: break;
            case SKeyDef::Event::SHORT:
                if (this->callbackShort != nullptr) {
                    this->callbackShort();
                } else {
                    return Vasl::NOT;
                }
                break;
            case SKeyDef::Event::LONG:
                if (this->callbackLong != nullptr) {
                    this->callbackLong();
                } else {
                    return Vasl::NOT;
                }
                break;
            case SKeyDef::Event::REPEAT:
                if (this->callbackRepeat != nullptr) {
                    this->callbackRepeat();
                } else {
                    return Vasl::NOT;
                }
                break;
        }
        return Vasl::OK;
    }


    /** ================================================================================ *//**
     *  @brief      按键事件处理方法 重载函数调用运算符
     *  @note       处理按键事件，调用注册的回调函数
     *  @return     Vasl::err_t
     * 
     * -------------------------------------------------------------------------------- */
    Vasl::err_t TypeSKey::operator() (void) {
        if (this->refreshEventFlag == Vasl::NOT) {
            return Vasl::NOT;
        }
        this->refreshEventFlag = Vasl::NOT;
        return this->Process(this->event);
    }

    /** ================================================================================ *//**
     *  @brief      中断任务处理按键事件
     *  @note       扫描和判断事件，在中断中调用此函数
     *  @param[in]  port: GPIO端口
     *  @param[in]  pin: GPIO引脚
     *  @param[in]  msTime: 当前时间毫秒数
     *  @param[in]  debounceTime: 去抖时间毫秒数
     *  @param[in]  longPressTime: 长按时间毫秒数
     *  @param[in]  repeatTime: 重复触发时间毫秒数
     *  @return     Vasl::err_t
     * 
     * -------------------------------------------------------------------------------- */
    void TypeSKey::InterruptTask(
        IOType io_,
        size_t msTime_, 
        size_t debounceTime_, 
        size_t longPressTime_, 
        size_t repeatTime_) {
        TypeSKey::Scan(io_);
        this->event = TypeSKey::Proc(msTime_, debounceTime_, longPressTime_, repeatTime_);
        if (this->event != SKeyDef::Event::NONE) {
            this->refreshEventFlag = Vasl::OK;
        }
    }

} /** namespace Vasl */
