#include "bsp_easymenu.hpp"

Vasl::TypePT PT_menu {0};

Vasl::err_t bsp_easymenu_init(void) {
    Vasl::EM::init(nullptr, [](unsigned short int x, unsigned char line, char ch, unsigned char reverse_flag){
        (void)reverse_flag;
        auto& oled96 = oled96_get();
        oled96.ShowChar(static_cast<uint8_t>(x), line * CHAR_HEIGHT, ch, OLED_FONT_6X8);
        oled96.UpdateArea(static_cast<uint8_t>(x), line * CHAR_HEIGHT, 1 * CHAR_WIDTH, CHAR_HEIGHT);
    }, nullptr, nullptr);
    return Vasl::OK;
}
