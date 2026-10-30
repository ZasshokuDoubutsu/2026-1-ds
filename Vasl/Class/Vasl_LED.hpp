#ifndef __VASL_LED_HPP__
#define __VASL_LED_HPP__

#include "Vasl_Type.hpp"
#include "Vasl_Layer.hpp"

namespace Vasl {
    class TypeLED {
    private:
        IOType      io;
        LevelType   activeLevel;
        bool        isInitialized;
    public:
        TypeLED(void) : io(BSP_IO_PORT_FF_PIN_FF), activeLevel(VASL_LAYER_PIN_RESET_LEVEL), isInitialized(false) {}

        TypeLED(IOType io_, LevelType level = VASL_LAYER_PIN_RESET_LEVEL) : 
        io(io_), activeLevel(level), isInitialized(false) {
            if (io_ != BSP_IO_PORT_FF_PIN_FF) {
                this->isInitialized = true;
            }
        }

        ~TypeLED(void) {}

        void set(IOType io_, LevelType level = VASL_LAYER_PIN_RESET_LEVEL) {
            this->io = io_;
            this->activeLevel = level;
            if (this->io != BSP_IO_PORT_FF_PIN_FF) {
                this->isInitialized = true;
            }
        }

        void on(void) {
            if (!this->isInitialized || this->io == BSP_IO_PORT_FF_PIN_FF) return;
            Vasl::Layer::GPIO_WritePin(this->io, this->activeLevel);
        }

        void off(void) {
            if (!this->isInitialized || this->io == BSP_IO_PORT_FF_PIN_FF) return;
            Vasl::Layer::GPIO_WritePin(this->io, 
                this->activeLevel == VASL_LAYER_PIN_SET_LEVEL ? VASL_LAYER_PIN_RESET_LEVEL : VASL_LAYER_PIN_SET_LEVEL);
        }

        void toggle(void) {
            if (!this->isInitialized || this->io == BSP_IO_PORT_FF_PIN_FF) return;
            Vasl::Layer::GPIO_TogglePin(this->io);
        }

        bool status(void) {
            if (!this->isInitialized || this->io == BSP_IO_PORT_FF_PIN_FF) return false;
            LevelType currentState = Vasl::Layer::GPIO_ReadPin(this->io);
            return (currentState == this->activeLevel);
        }

        IOType getIO() const { return this->io; }
        LevelType getActiveLevel() const { return this->activeLevel; }
    };
}

#endif ///< __VASL_LED_HPP__
