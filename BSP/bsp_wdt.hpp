#ifndef __BSP_WDT_HPP__
#define __BSP_WDT_HPP__

#include "Vasl_Core.hpp"

extern Vasl::TypePT PT_wdt;
extern fsp_err_t R_WDT_Refresh (wdt_ctrl_t * const p_ctrl);
extern fsp_err_t R_WDT_Open (wdt_ctrl_t * const p_ctrl, wdt_cfg_t const * const p_cfg);

Vasl::err_t WDT_Init(void);
Vasl::err_t WDT_Feed(void);
namespace Vasl {
    namespace wdt {
        inline Vasl::err_t init(void) { return WDT_Init(); }
        inline Vasl::err_t feed(void) { return WDT_Feed(); }
    }
    namespace Register {
        void WDT_Timeout(void (* callback)(void));
    }
}    

#endif ///< __BSP_WDT_HPP__