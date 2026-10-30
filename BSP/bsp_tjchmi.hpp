#ifndef __BSP_TJCHMI_HPP__
#define __BSP_TJCHMI_HPP__

#include "Vasl_Core.hpp"
#include "bsp_inst.hpp"
#include "bsp_uart.hpp"

extern size_t g_tjchmi_page, g_tjchmi_page_table;
extern Vasl::TypePT g_tjchmi_show_page;

void bsp_tjchmi_format_cmd(const char *cmd, ...);
void bsp_tjchmi_format_string(char* name, char* showdata);
void bsp_tjchmi_send_int(char* name, int num);
void bsp_tjchmi_send_float(char* name, float num);
void bsp_tjchmi_send_cmd(void);

int bsp_tjchmi_show_page (Vasl::TypePT* pt);

#endif ///< __BSP_TJCHMI_HPP__
