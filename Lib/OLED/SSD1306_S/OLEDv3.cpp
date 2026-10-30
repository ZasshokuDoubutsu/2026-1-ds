#include "./OLED/SSD1306_S/OLEDv3.hpp"
#include "string.h"
#include "stdarg.h"
#include "stdio.h"
#include "./NanoPrintf/nanoprintf.h"

namespace Disp {
    // 默认构造函数，使用默认I2C引脚
    OLEDv3::OLEDv3() : address(0x78), 
                      sck_port(BSP_IO_PORT_FF_PIN_FF),
                      sda_port(BSP_IO_PORT_FF_PIN_FF) {
    }
    
    // 自定义引脚构造函数
    OLEDv3::OLEDv3(IOType _sck_port, IOType _sda_port, uint8_t _address, uint8_t _oled_screen_size) 
        : address(_address), sck_port(_sck_port), sda_port(_sda_port), oled_screen_size(_oled_screen_size) {
    }
    
    /**
      * @brief  I2C开始
      * @param  无
      * @retval 无
      */
    void OLEDv3::Start(void) {
        Vasl::Layer::GPIO_WritePin(sda_port, BSP_IO_LEVEL_HIGH);
        Vasl::Layer::GPIO_WritePin(sck_port, BSP_IO_LEVEL_HIGH);
        Vasl::Layer::GPIO_WritePin(sda_port, BSP_IO_LEVEL_LOW);
        Vasl::Layer::GPIO_WritePin(sck_port, BSP_IO_LEVEL_LOW);
    }

    /**
      * @brief  I2C停止
      * @param  无
      * @retval 无
      */
    void OLEDv3::Stop(void) {
        Vasl::Layer::GPIO_WritePin(sda_port, BSP_IO_LEVEL_LOW);
        Vasl::Layer::GPIO_WritePin(sck_port, BSP_IO_LEVEL_HIGH);
        Vasl::Layer::GPIO_WritePin(sda_port, BSP_IO_LEVEL_HIGH);
    }

    /**
      * @brief  I2C发送一个字节
      * @param  Byte 要发送的一个字节
      * @retval 无
      */
    void OLEDv3::SendByte(uint8_t Byte) {
        uint8_t i;
        for (i = 0; i < 8; i++) {
            Vasl::Layer::GPIO_WritePin(sda_port, (LevelType)(Byte & (0x80 >> i)));
            Vasl::Layer::GPIO_WritePin(sck_port, BSP_IO_LEVEL_HIGH);
            Vasl::Layer::GPIO_WritePin(sck_port, BSP_IO_LEVEL_LOW);
        }
        
        // 等待从机应答
        Vasl::Layer::GPIO_WritePin(sda_port, BSP_IO_LEVEL_HIGH); // 释放SDA线
        Vasl::Layer::GPIO_WritePin(sck_port, BSP_IO_LEVEL_HIGH); // 拉高SCL
        
        // 添加短暂延时，确保信号稳定
        for (i = 0; i < 10; i++);
        
        // 检查应答信号（可选，这里不检查，但保持时序正确）
        Vasl::Layer::GPIO_WritePin(sck_port, BSP_IO_LEVEL_LOW); // 拉低SCL
        
        // 添加短暂延时，确保信号稳定
        for (i = 0; i < 10; i++);
    }

    /**
      * @brief  OLED写命令
      * @param  Command 要写入的命令
      * @retval 无
      */
    void OLEDv3::WriteCommand(uint8_t Command)
    {
        Start();
        SendByte(address);      //从机地址
        SendByte(0x00);         //写命令
        SendByte(Command);
        Stop();
    }

    /**
      * @brief  OLED写数据
      * @param  Data 要写入的数据
      * @retval 无
      */
    void OLEDv3::WriteData(uint8_t Data)
    {
        Start();
        SendByte(address);      //从机地址
        SendByte(0x40);         //写数据
        SendByte(Data);
        Stop();
    }

    /**
      * @brief  OLED设置光标位置
      * @param  Y 以左上角为原点，向下方向的坐标，范围：0~7
      * @param  X 以左上角为原点，向右方向的坐标，范围：0~127
      * @retval 无
      */
    void OLEDv3::SetCursor(uint8_t Y, uint8_t X)
    {
        // 确保坐标在有效范围内
        if (Y > 7) Y = 7;
        if (X > 127) X = 127;
        
        WriteCommand(0xB0 | Y);                  //设置Y位置（页地址）
        WriteCommand(0x10 | ((X & 0xF0) >> 4));    //设置X位置高4位
        WriteCommand(0x00 | (X & 0x0F));            //设置X位置低4位
    }

    /**
      * @brief  OLED初始化
      * @param  无
      * @retval 无
      */
    void OLEDv3::Init(void) {
        // // 初始化GPIO
        // GPIO_InitTypeDef GPIO_InitStructure;

        // GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
        // GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        // GPIO_InitStructure.Pin = sck_pin;
        // HAL_GPIO_Init(sck_port, &GPIO_InitStructure);

        // GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
        // GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
        // GPIO_InitStructure.Pin = sda_pin;
        // HAL_GPIO_Init(sda_port, &GPIO_InitStructure);

        // Vasl::Layer::GPIO_WritePin(sck_port, BSP_IO_LEVEL_HIGH);
        // Vasl::Layer::GPIO_WritePin(sda_port, BSP_IO_LEVEL_HIGH);

        // 上电延时
        for (int i = 0; i < 1000; i++) {
            for (int j = 0; j < 1000; j++);
        }

        WriteCommand(0xAE); //关闭显示

        WriteCommand(0xD5); //设置显示时钟分频比/振荡器频率
        WriteCommand(0x80);

        WriteCommand(0xA8); //设置多路复用率
        WriteCommand(oled_screen_size == 0 ? 0x1F : 0x3F);

        WriteCommand(0xD3); //设置显示偏移
        WriteCommand(0x00);

        WriteCommand(0x40); //设置显示开始行
#if OLED_UNDER_SHOW == 0
        WriteCommand(0xA1); //设置左右方向，0xA1正常 0xA0左右反置

        WriteCommand(0xC8); //设置上下方向，0xC8正常 0xC0上下反置
#else
        WriteCommand(0xA0); //设置左右方向，0xA1正常 0xA0左右反置

        WriteCommand(0xC0); //设置上下方向，0xC8正常 0xC0上下反置

#endif
        WriteCommand(0xDA); //设置COM引脚硬件配置
        WriteCommand(oled_screen_size == 0 ? 0x02 : 0x12);

        WriteCommand(0x81); //设置对比度控制
        WriteCommand(0xCF);

        WriteCommand(0xD9); //设置预充电周期
        WriteCommand(0xF1);

        WriteCommand(0xDB); //设置VCOMH取消选择级别
        WriteCommand(0x30);

        WriteCommand(0xA4); //设置整个显示打开/关闭

        WriteCommand(0xA6); //设置正常/倒转显示

        WriteCommand(0x8D); //设置充电泵
        WriteCommand(0x14);

        WriteCommand(0xAF); //开启显示

        Clear();            //OLED清屏
    }

    /**
      * @brief  OLED初始化，自定义I2C引脚
      * @param  sck_port SCK引脚端口
      * @param  sck_pin SCK引脚编号
      * @param  sda_port SDA引脚端口
      * @param  sda_pin SDA引脚编号
      * @param  address OLED设备地址，默认为0x78
      * @retval 无
      */
    void OLEDv3::Init(IOType sck_port_, IOType sda_port_, uint8_t address_, uint8_t oled_screen_size_) {
        // 更新成员变量
        this->address = address_;
        this->sck_port = sck_port_;
        this->sda_port = sda_port_;
        this->oled_screen_size = oled_screen_size_;
        
        // 调用原始初始化函数
        Init();
    }

    /**
      * @brief  OLED清屏
      * @param  无
      * @retval 无
      */
    void OLEDv3::Clear(void)
    {
        uint8_t i, j;
        for (j = 0; j < 8; j++)
        {
            SetCursor(j, 0);
            for(i = 0; i < 128; i++)
            {
                WriteData(0x00);
                oled_display_buffer[j][i] = 0x00;
            }
        }
    }

    /**
      * @brief  OLED显示一个字符
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  Char 要显示的一个字符，范围：ASCII可见字符
      * @param  font_size 字体大小
      * @retval 无
      */
    void OLEDv3::ShowChar(uint8_t x, uint8_t y, char Char, uint8_t font_size) {
        const uint8_t* font_data = NULL;
        uint8_t font_width = 0;
        uint8_t font_height = 0;
        
        // 根据字体大小选择字体数组
        if (font_size == OLED_FONT_6X8) {
            font_data = oled_font_6x8[Char - ' '];
            font_width = 6;
            font_height = 8;
        } else if (font_size == OLED_FONT_7X12) {
            font_data = oled_font_7x12[Char - ' '];
            font_width = 7;
            font_height = 12;
        } else if (font_size == OLED_FONT_8X16) {
            font_data = oled_font_8x16[Char - ' '];
            font_width = 8;
            font_height = 16;
        } else {
            // 默认为8x16字体
            font_data = oled_font_8x16[Char - ' '];
            font_width = 8;
            font_height = 16;
        }
        
        // 显示字符
        ShowImage(x, y, font_data, font_width, font_height);
    }

    /**
      * @brief  OLED显示字符串
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  String 要显示的字符串，范围：ASCII可见字符
      * @param  font_size 字体大小
      * @retval 无
      */
    void OLEDv3::ShowString(uint8_t x, uint8_t y, char *String, uint8_t font_size) {
        uint8_t current_x = x;
        uint8_t font_width = 0;
        
        // 根据字体大小确定字符宽度
        if (font_size == OLED_FONT_6X8) {
            font_width = 6;
        } else if (font_size == OLED_FONT_7X12) {
            font_width = 7;
        } else if (font_size == OLED_FONT_8X16) {
            font_width = 8;
        } else {
            font_width = 8; // 默认字体宽度
        }
        
        while (*String != '\0') {
            // 检查是否超出显示范围
            if (current_x > 128 - font_width) break;
            
            ShowChar(current_x, y, *String, font_size);
            current_x += font_width;
            String++;
        }
    }

    /**
      * @brief  OLED显示图像
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  image 图像数据指针
      * @param  width 图像宽度
      * @param  height 图像高度
      * @retval 无
      */
    void OLEDv3::ShowImage(uint8_t x, uint8_t y, const uint8_t *image, uint8_t width, uint8_t height) {
        uint8_t i, j;
        uint8_t page, shift;

        ClearArea(x, y, width, height);
        for (i = 0; i < (height - 1) / 8 + 1; i++) {
            for (j = 0; j < width; j++) {
                if (x + j < OLED_WIDTH) {
                    page = y / 8;
                    shift = y % 8;

                    if (page + i < OLED_PAGES) {
                        oled_display_buffer[page + i][x + j] |= image[i * width + j] << (shift);
                    }

                    if (page + i + 1 < OLED_PAGES && shift > 0) {
                        oled_display_buffer[page + i + 1][x + j] |= image[i * width + j] >> (8 - shift);
                    }
                }
            }
        }
    }

    /**
      * @brief  OLED绘制点
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @retval 无
      */
    void OLEDv3::DrawPoint(uint8_t x, uint8_t y) {
        if (x < OLED_WIDTH && y < OLED_HEIGHT) {
            oled_display_buffer[y / 8][x] |= (0x01 << (y % 8));
        }
    }

    // /**
    //   * @brief  OLED绘制点（带颜色）
    //   * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
    //   * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
    //   * @param  color 颜色：0=黑，1=白
    //   * @retval 无
    //   */
    // void OLEDv3::DrawPointColor(uint8_t x, uint8_t y, uint8_t color) {
    //     if (x < OLED_WIDTH && y < OLED_HEIGHT) {
    //         if (color) {
    //             // 画白点 - 设置位
    //             oled_display_buffer[y / 8][x] |= (0x01 << (y % 8));
    //         } else {
    //             // 画黑点 - 清除位
    //             oled_display_buffer[y / 8][x] &= ~(0x01 << (y % 8));
    //         }
    //     }
    // }

    /**
      * @brief  OLED绘制直线
      * @param  x1 起点x坐标，范围：0~127
      * @param  y1 起点y坐标，范围：0~63
      * @param  x2 终点x坐标，范围：0~127
      * @param  y2 终点y坐标，范围：0~63
      * @retval 无
      */
    void OLEDv3::DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
        // 水平线
        if (y1 == y2) {
            uint8_t start = (x1 < x2) ? x1 : x2;
            uint8_t end = (x1 < x2) ? x2 : x1;
            for (uint8_t x = start; x <= end; x++) {
                DrawPoint(x, y1);
            }
            return;
        }

        // 垂直线
        if (x1 == x2) {
            uint8_t start = (y1 < y2) ? y1 : y2;
            uint8_t end = (y1 < y2) ? y2 : y1;
            for (uint8_t y = start; y <= end; y++) {
                DrawPoint(x1, y);
            }
            return;
        }

        // 斜线 - 使用Bresenham算法
        int8_t dx = static_cast<int8_t>(x2) - static_cast<int8_t>(x1);
        int8_t dy = static_cast<int8_t>(y2) - static_cast<int8_t>(y1);
        int8_t sx = (dx >= 0) ? 1 : -1;
        int8_t sy = (dy >= 0) ? 1 : -1;
        dx = (dx >= 0) ? dx : -dx;
        dy = (dy >= 0) ? dy : -dy;

        int16_t err = dx - dy;
        uint8_t x = x1;
        uint8_t y = y1;

        while (1) {
            DrawPoint(x, y);
            if (x == x2 && y == y2) break;

            int16_t e2 = 2 * err;
            if (e2 > -dy) {
                err -= dy;
                x = static_cast<uint8_t>(static_cast<int16_t>(x) + sx);
            }
            if (e2 < dx) {
                err += dx;
                y = static_cast<uint8_t>(static_cast<int16_t>(y) + sy);
            }
        }
    }

    /**
      * @brief  OLED绘制矩形
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  width 矩形宽度
      * @param  height 矩形高度
      * @param  fill 是否填充 0-不填充 1-填充
      * @retval 无
      */
    void OLEDv3::DrawRectangle(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t fill) {
        uint8_t i, j;
        if (fill) {
            for (i = x; i < x + width && i < OLED_WIDTH; i++) {
                for (j = y; j < y + height && j < OLED_HEIGHT; j++) {
                    DrawPoint(i, j);
                }
            }
        } else {
            for (i = x; i < x + width && i < OLED_WIDTH; i++) {
                DrawPoint(i, y);
                if (y + height - 1 < OLED_HEIGHT) {
                    DrawPoint(i, y + height - static_cast<uint8_t>(1U));
                }
            }
            for (i = y; i < y + height && i < OLED_HEIGHT; i++) {
                DrawPoint(x, i);
                if (x + width - 1 < OLED_WIDTH) {
                    DrawPoint(x + width - static_cast<uint8_t>(1U), i);
                }
            }
        }
    }

    /**
      * @brief  OLED更新整个屏幕显示
      * @param  无
      * @retval 无
      */
    void OLEDv3::UpdateAll(void) {
        uint8_t i;
        for (i = 0; i < OLED_PAGES; i++) {
            SetCursor(i, 0);
            for (uint8_t j = 0; j < OLED_LIST; j++) {
                WriteData(oled_display_buffer[i][j]);
            }
        }
    }

    /**
      * @brief  OLED更新指定区域显示
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  width 区域宽度
      * @param  height 区域高度
      * @retval 无
      */
    void OLEDv3::UpdateArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
    {
        uint8_t i, j;
        uint8_t start_page = y / 8U;
        uint8_t end_page = (y + height - static_cast<uint8_t>(1U)) / static_cast<uint8_t>(8U);

        for (i = start_page; i <= end_page && i < OLED_PAGES; i++)
        {
            if (x < OLED_WIDTH)
            {
                SetCursor(i, x);
                for (j = 0; j < width && x + j < OLED_WIDTH; j++)
                {
                    WriteData(oled_display_buffer[i][x + j]);
                }
            }
        }
    }

    /**
      * @brief  OLED清除指定区域
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  width 区域宽度
      * @param  height 区域高度
      * @retval 无
      */
    void OLEDv3::ClearArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
        uint8_t i, j;

        for (i = y; i < y + height && i < OLED_HEIGHT; i++)
        {
            for (j = x; j < x + width && j < OLED_WIDTH; j++)
            {
                oled_display_buffer[i / 8][j] &= ~static_cast<uint8_t>(0x01 << (i % 8));
            }
        }
    }

    /**
      * @brief  OLED反转整个屏幕显示
      * @param  无
      * @retval 无
      */
    void OLEDv3::ReverseAll(void)
    {
        uint8_t i, j;
        for (i = 0; i < OLED_PAGES; i++)
        {
            for (j = 0; j < OLED_LIST; j++)
            {
                oled_display_buffer[i][j] ^= 0xFF;
            }
        }
    }

    /**
      * @brief  OLED反转指定区域显示
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  width 区域宽度
      * @param  height 区域高度
      * @retval 无
      */
    void OLEDv3::ReverseArea(uint8_t x, uint8_t y, uint8_t width, uint8_t height)
    {
        uint8_t i, j;

        for (i = y; i < y + height && i < OLED_HEIGHT; i++)
        {
            for (j = x; j < x + width && j < OLED_WIDTH; j++)
            {
                oled_display_buffer[i / 8][j] ^= ~((uint8_t)0x01 << (i % (uint8_t)8));
            }
        }
    }

    /**
      * @brief  OLED显示中文字符串
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  str 字符串指针
      * @param  font_size 字体大小
      * @retval 无
      */
    void OLEDv3::ShowFontString(uint8_t x, uint8_t y, const uint8_t *str, uint8_t font_size)
    {
        // 简化实现，实际应该查找GBK字库
        // 这里只是示例实现
        while (*str != '\0') {
            ShowChar(x, y, *str, font_size);
            x += font_size;
            str++;
        }
    }

    /**
      * @brief  OLED显示设置信息
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  str 字符串指针
      * @param  font_size 字体大小
      * @retval 无
      */
    void OLEDv3::ShowSetting(uint8_t x, uint8_t y, const uint8_t *str, uint8_t font_size)
    {
        // 简化实现，实际应该根据设置信息显示
        ShowString(x, y, (char*)str, font_size);
    }

    /**
      * @brief  OLED格式化输出
      * @param  x 以左上角为原点，向右方向的坐标，范围：0~127
      * @param  y 以左上角为原点，向下方向的坐标，范围：0~63
      * @param  font_size 字体大小
      * @param  fmt 格式化字符串
      * @retval 无
      */
    void OLEDv3::Printf(uint8_t x, uint8_t y, uint8_t font_size, const char *fmt, ...)
    {
        char str[64];
        va_list args;
        va_start(args, fmt);
        npf_vsnprintf(str, sizeof(str), fmt, args);
        va_end(args);
        ShowString(x, y, str, font_size);
    }
}
