#ifndef __OLED_V3_HPP__
#define __OLED_V3_HPP__

extern "C" {
    #include "hal_data.h"
}
#include "./OLED/SSD1306_S/oled_font.h"
#include "Vasl_Layer.hpp"

// 字体大小定义
#define OLED_FONT_6X8           6   /* 6x8字体 */
#define OLED_FONT_7X12          7   /* 7x12字体 */
#define OLED_FONT_8X16          8   /* 8x16字体 */

// 屏幕参数
#define OLED_PAGES              8   /* 8页 */
#define OLED_LIST               128 /* 128列 */
#define OLED_WIDTH              128 /* 128列 */
#define OLED_HEIGHT             64  /* 64行 */

#define OLED_UNDER_SHOW         (1)

namespace Disp {
    class OLEDv3 {
    private:
        uint8_t     address;
        IOType      sck_port;
        IOType      sda_port;
        uint8_t     oled_screen_size;   ///< 0: 128x32, 1: 128x64
        
        // 显示缓冲区 (每个对象独立一份)
        uint8_t oled_display_buffer[OLED_PAGES][OLED_LIST];
        
        void Start(void);
        void Stop(void);
        void SendByte(uint8_t Byte);
        void WriteCommand(uint8_t Command);
        void WriteData(uint8_t Data);
        void SetCursor(uint8_t Y, uint8_t X);
        
    public:
        // 构造函数，使用默认I2C引脚
        OLEDv3();
        
        // 构造函数，自定义I2C引脚
        OLEDv3(IOType sck_port, IOType sda_port, uint8_t address = 0x78, uint8_t oled_screen_size = 1);
        
        // 初始化OLED
        void Init(void);
        
        // 初始化OLED，自定义I2C引脚
        void Init(IOType sck_port, IOType sda_port, uint8_t address = 0x78, uint8_t oled_screen_size = 1);
        
        // 清屏
        void Clear(void);
        
        // 显示一个字符
        void ShowChar(uint8_t x, uint8_t y, char Char, uint8_t font_size = OLED_FONT_8X16);
        
        // 显示字符串
        void ShowString(uint8_t x, uint8_t y, char *String, uint8_t font_size = OLED_FONT_8X16);
        
        // 显示图像
        void ShowImage(uint8_t x, uint8_t y, const uint8_t *image, uint8_t width, uint8_t height);
        
        // 绘制点
        void DrawPoint(uint8_t x, uint8_t y);
        
        // 绘制点（带颜色）
        void DrawPointColor(uint8_t x, uint8_t y, uint8_t color);  // color: 0=黑, 1=白

        // 绘制直线
        void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
        
        // 绘制矩形
        void DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t fill = 0);
        
        // 更新整个屏幕显示
        void UpdateAll(void);
        
        // 更新指定区域显示
        void UpdateArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
        
        // 清除指定区域
        void ClearArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
        
        // 反转整个屏幕显示
        void ReverseAll(void);
        
        // 反转指定区域显示
        void ReverseArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height);
        
        // 显示中文字符串
        void ShowFontString(uint8_t x, uint8_t y, const uint8_t *str, uint8_t font_size);
        
        // 显示设置信息
        void ShowSetting(uint8_t x, uint8_t y, const uint8_t *str, uint8_t font_size);
        
        // 格式化输出
        void Printf(uint8_t x, uint8_t y, uint8_t font_size, const char *fmt, ...);

        // 获取显示缓冲区指针
        uint8_t (*GetDisplayBuffer())[OLED_LIST] { return oled_display_buffer; }
    };
}

#endif /* __OLED_V3_HPP__ */
