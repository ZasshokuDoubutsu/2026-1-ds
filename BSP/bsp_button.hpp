#ifndef __BSP_BUTTON_HPP__
#define __BSP_BUTTON_HPP__

#include "Vasl_Core.hpp"
#include "bsp_inst.hpp"

namespace Vasl {
    namespace bsp {
        template<size_t EVENT_QUEUE_SIZE>
        using SKey_Event_q = TypeQueue<std::pair<Vasl::TypeSKey*, SKeyDef::Event>, EVENT_QUEUE_SIZE>;
    }
}

extern Vasl::TypeSKey g_obkey;
constexpr static size_t EVENT_QUEUE_SIZE = 3;
extern Vasl::bsp::SKey_Event_q<EVENT_QUEUE_SIZE> g_keys_queue;
extern Vasl::TypePT PT_button_tick_handler, PT_button_process;

Vasl::err_t bsp_button_init(void);
Vasl::err_t bsp_button_register(void);
int bsp_button_tick_handler (Vasl::TypePT* pt);
int bsp_button_process (Vasl::TypePT* pt);

#endif ///< __BSP_BUTTON_HPP__
