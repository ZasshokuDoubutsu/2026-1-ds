# RA6M5 智能送餐车 · 厨房主控

基于 Renesas RA6M5（R7FA6M5BF2CBG）的送餐系统厨房主控固件。主控通过 ESP8266（UART2）接入 WiFi/MQTT，与点餐端通信；通过 OLED + 按键提供本地菜单与参数配置；通过 UART4 以透传方式与小车上的 STM32 通信，下发 NLC 风格的行车指令。

- MCU：R7FA6M5BF2CBG（Cortex-M33，1 MB ROM / 512 KB RAM）
- 工程生成：Renesas FSP 6.3.0 + RASC，CMake + Ninja + arm-none-eabi-gcc
- 语言：C / C++17

## 目录结构

| 路径 | 说明 |
|---|---|
| `App/` | 应用层：初始化、超级循环、任务调度 |
| `BSP/` | 板级驱动：UART、按键、OLED、ADC、看门狗、ESP8266、菜单、命令解析 |
| `Vasl/` | Vasl 32 位 MCU 库：日志、字符串、队列、环形缓冲、协程、NLC 自然语言命令、软定时器封装 |
| `Lib/` | 本项目保留的适配层与小库（见下方"依赖"） |
| `src/` | RASC 生成的入口与启动文件 |
| `ra_cfg/`、`ra_gen/` | RASC 生成的配置与 HAL 实例 |
| `cmake/`、`CMakeLists.txt`、`Config.cmake` | 构建脚本 |
| `configuration.xml` | RASC 工程配置，可用 FSP Smart Configurator 打开 |
| `Doc/` | 协议报文示例 |

## 外设分配

| 外设 | 用途 |
|---|---|
| SCI2 (UART2) | ESP8266 WiFi 模组，AT 指令 |
| SCI4 (UART4) | 与小车 STM32 的 BLE 透传链路 |
| SCI7 (UART7) | 调试串口，日志输出 |
| SCI9 (UART9) | 预留串口 |
| ADC0 | 温度传感器 |
| IWDT/WDT | 看门狗 |
| GPIO | 按键、OLED 软 I2C |

## 依赖

本仓库不包含以下第三方代码，需自行获取或由 RASC 生成：

- **Renesas FSP 6.3.0**：`ra/` 目录，由 RASC 依据 `configuration.xml` 生成
- **Embedded Template Library (ETL)**：`Lib/etl/`，MIT
- **nanoprintf**：`Lib/NanoPrintf/nanoprintf.c/h`，MIT
- **MultiTimer**：`Lib/STimer/MultiTimer.c/h`，MIT
- **protothreads**：`Lib/PT/pt.h` 等，BSD
- **EasyMenu**：`Lib/EasyMenu/Easy_Menu*.c/h`
- **GuiLite 示例**：原工程 `Doc/GUI-sample/` 未收录

保留在本仓库的 `Lib/` 内容为项目自研适配层：`Vasl_Vsnprintf.h`、`Vasl_PT.hpp`、`Vasl_STimer.*`、`Vasl_EasyMenu.hpp`、`Easy_Menu_User.*`、`User_Menu_Config.json`、`OLED/`、`syscalls/`。

## 构建

```sh
cmake -DARM_TOOLCHAIN_PATH="C:/arm-gnu-toolchain/bin" \
      -DCMAKE_TOOLCHAIN_FILE=cmake/gcc.cmake \
      -G Ninja -B build/Debug
cmake --build build/Debug
```

VS Code 中可设置环境变量 `ARM_GCC_TOOLCHAIN_PATH` 指向工具链 `bin` 目录，然后在状态栏执行构建；`Config.cmake` 中的 `RASC_EXE_PATH` 需改为本机 FSP 安装路径（也可用 `-DRASC_EXE_PATH=...` 或环境变量覆盖）。

> 本仓库为公开源码整理版，未包含 `ra/` 与上述第三方库，直接配置会因缺少头文件而失败；补齐依赖后按原工程方式构建。

## 配置

WiFi 与 OneNET 参数集中在 `BSP/bsp_esp8266.hpp`，仓库中为占位值，使用前替换为实际值：

```c
#define __WIFI_SSID_1           "your_wifi_ssid"
#define __WIFI_PASSWORD_1       "your_wifi_password"
#define __ONENET_PRODUCT_ID_2   "your_product_id"
#define __ONENET_DEVICE_ID_2    "your_device_name"
#define __ONENET_TOKEN_2        "your_onenet_token"
```

串口调试命令由 `BSP/bsp_cmd.cpp` 提供，如 `/at wifi 1`、`/at mqtt_conn onenet`、`/vasl ver`。

## 许可

代码仅用于学习与交流。第三方组件遵循其原始许可。
