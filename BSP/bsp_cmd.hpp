#ifndef __BSP_CMD_HPP__
#define __BSP_CMD_HPP__

#include "Vasl_Core.hpp"
#include "bsp_inst.hpp"

constexpr size_t BUFFER_SIZE {128U};
constexpr int CMD_COUNT_MAX {4};
constexpr int TJCHMI_CMD_COUNT_MAX {3};

extern std::pair<Vasl::err_t, Vasl::array<uint8_t, BUFFER_SIZE>> g_cmd_buffer, g_tjchmi_cmd_buffer;
extern Vasl::TypePT PT_cmd_process, PT_tjchmi_cmd_process;
Vasl::err_t bsp_cmd_start(void); /* NLCCMD初始化程序 */
Vasl::err_t bsp_tjchmi_cmd_start(void); /* TJCHMI_CMD初始化程序 */
int bsp_cmd_process(Vasl::TypePT* pt); /* NLCCMD处理程序 */
int bsp_tjchmi_cmd_process(Vasl::TypePT* pt); /* TJCHMI_CMD处理程序 */

#endif  ///< __BSP_CMD_HPP__
