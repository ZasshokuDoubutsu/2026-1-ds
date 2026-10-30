#ifndef __VASL_EASY_MENU_HPP__
#define __VASL_EASY_MENU_HPP__

extern "C" {
    #include "Easy_Menu.h"
}

namespace Vasl {
    namespace EM {
        inline void init(
                void (*Display_Char)(unsigned short int x, unsigned short int y, char ch, unsigned char reverse_flag), 
                void (*Display_Char_Line)(unsigned short int x, unsigned char line, char ch, unsigned char reverse_flag),
                void (*Display_Chinese_Char)(unsigned short int x, unsigned short int y, char* ch, unsigned char reverse_flag), 
                void (*Display_Chinese_Char_Line)(unsigned short int x, unsigned char line, char* ch, unsigned char reverse_flag)) {
            Easy_Menu_Init(Display_Char, Display_Char_Line, Display_Chinese_Char, Display_Chinese_Char_Line);
        }
        inline void process(unsigned int Easy_Menu_Tick) { Easy_Menu_Display(Easy_Menu_Tick); }
        inline void input(Easy_Menu_Input_TYPE user_input) { Easy_Menu_Input(user_input); }
    }
}

#endif ///< __VASL_EASY_MENU_HPP__