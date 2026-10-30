#ifndef __VASL_LAYER_HPP__
#define __VASL_LAYER_HPP__

/** ================================================================================ *//**
 *  @brief      平台相关获取
 * -------------------------------------------------------------------------------- */
#include "hal_data.h"
// #include "arm_math.h"
/** ================================================================================ *//**
 *  @brief      平台相关类型定义    ///< 未用到请手动注释
 * -------------------------------------------------------------------------------- */
#pragma region 平台相关类型定义
///< 端口引脚类型
using IOType        = bsp_io_port_pin_t;
using IOCtrlType    = ioport_ctrl_t;
using IOCfgType     = ioport_cfg_t;
///< 端口引脚电平类型
using LevelType     = bsp_io_level_t;
///< SCI UART 类型
#define __LAYER_USE_SCI_UART__      (UART)
#ifdef __LAYER_USE_SCI_UART__
    using UartType      = uart_instance_t;
    using UartCtrlType  = uart_ctrl_t;
    using UartCfgType   = uart_cfg_t;
#endif ///< __LAYER_USE_SCI_UART__
///< QSPI 类型
// #define __LAYER_USE_QSPI__          (QSPI)
#ifdef __LAYER_USE_QSPI__
    using QspiType      = spi_flash_instance_t;
    using QspiCtrlType  = spi_flash_ctrl_t;
    using QspiCfgType   = spi_flash_cfg_t;
#endif ///< __LAYER_USE_QSPI__
///< DMA 类型
// #define __LAYER_USE_DMAC__          (DMA)
#ifdef __LAYER_USE_DMAC__
    using DmacType      = transfer_instance_t;
    using DmacCtrlType  = transfer_ctrl_t;
    using DmacCfgType   = transfer_cfg_t;
#endif ///< __LAYER_USE_DMAC__
///< IIC 类型
// #define __LAYER_USE_I2C__           (I2C)
#ifdef __LAYER_USE_I2C__
    using I2cType       = i2c_master_instance_t;
    using I2cCtrlType   = i2c_master_ctrl_t;
    using I2cCfgType    = i2c_master_cfg_t;
#endif ///< __LAYER_USE_I2C__
///< IRQ 类型
// #define __LAYER_USE_IRQ__           (IRQ)
#ifdef __LAYER_USE_IRQ__
    using IrqType       = external_irq_instance_t;
    using IrqCtrlType   = external_irq_ctrl_t;
    using IrqCfgType    = external_irq_cfg_t;
#endif ///< __LAYER_USE_IRQ__
///< ADC 类型
#define __LAYER_USE_ADC__           (ADC)
#ifdef __LAYER_USE_ADC__
    using AdcType       = adc_instance_t;
    using AdcCtrlType   = adc_ctrl_t;
    using AdcCfgType    = adc_cfg_t;
    using AdcChannelCfgType = adc_channel_cfg_t;
    using AdcInfoType   = adc_info_t;
#endif ///< __LAYER_USE_ADC__
///< SPI 类型
// #define __LAYER_USE_SPI__           (SPI)
#ifdef __LAYER_USE_SPI__
    using SpiType       = spi_instance_t;
    using SpiCtrlType   = spi_ctrl_t;
    using SpiCfgType    = spi_cfg_t;
#endif ///< __LAYER_USE_SPI__
#pragma endregion 平台相关类型定义 /* ↑ */

/** ================================================================================ *//**
*  @brief      平台通用宏
* -------------------------------------------------------------------------------- */
#pragma region 平台通用宏
#ifndef __weak
    #define __weak      __attribute__((weak))
#endif /* __weak */
#ifndef unused
    #define unused(...)   (void)(__VA_ARGS__)
#endif /* unused */
#ifndef NULL
    #define NULL        ((void*)0)
#endif /* NULL */
#ifndef __O
	#define     __O     volatile             /*!< Defines 'write only' permissions */
#endif /* __O */
#ifndef __IO
	#define     __IO    volatile             /*!< Defines 'read / write' permissions */
#endif /* __IO */
#pragma endregion 平台通用宏 /* ↑ */

/** ================================================================================ *//**
 * @brief 定义平台相关函数
* -------------------------------------------------------------------------------- */
#pragma region 定义平台相关函数
namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      获取系统版本号
        *  @return     const char*: 系统版本号字符串
        * 
        * -------------------------------------------------------------------------------- */
        inline const char* Version(void) { return "Ver26.3preview"; }
        inline float VersionNumOnly(void) { return 26.3f; }
        /** ================================================================================ *//**
        *  @brief      软件重启
        *  @return     void
        * 
        * -------------------------------------------------------------------------------- */
        inline void SoftRestart(void) {
            __NVIC_SystemReset();
        }
        /** ================================================================================ *//**
        *  @brief      禁用中断
        *  @return     void
        * 
        * -------------------------------------------------------------------------------- */
        inline void IrqDisable(void) {
            __disable_irq();
        }
        /** ================================================================================ *//**
        *  @brief      禁用中断
        *  @return     void
        * 
        * -------------------------------------------------------------------------------- */
        inline void IrqEnable(void) {
            __enable_irq();
        }
        /** ================================================================================ *//**
        *  @brief      系统进入低功耗模式
        *  @note       仅在启用DWT时有效
        * 
        * -------------------------------------------------------------------------------- */
        inline void WFI(void) {
            __WFI();
        }
        inline void WFI(bool enable) {
            if (enable) { __WFI(); }
        }
        /** ================================================================================ *//**
        *  @brief      系统进入低功耗模式
        *  @note       仅在启用DWT时有效
        * 
        * -------------------------------------------------------------------------------- */
        inline void WFE(void) {
            __WFE();
        }
        /** ================================================================================ *//**
        *  @brief      忙一个机器周期
        * 
        * -------------------------------------------------------------------------------- */
        inline void NOP(void) {
            __NOP();
        }
    }
}
#pragma endregion 定义平台相关函数 /* ↑ */

#pragma region 常量和配置定义
/** ================================================================================ *//**
 * @brief 定义时间Tick相关函数的方式
* -------------------------------------------------------------------------------- */
#define __VASL_USE_RA_DELAY_US__    /** @brief 使用RA的延时函数 */
#define TIME_TICK_USE_DWT
// #define TIME_TICK_USE_SYSTICK

/** ================================================================================ *//**
 * @brief 定义GPIO PIN置位/复位时数值
* -------------------------------------------------------------------------------- */
#define VASL_LAYER_PIN_SET_LEVEL   (BSP_IO_LEVEL_HIGH)
#define VASL_LAYER_PIN_RESET_LEVEL (BSP_IO_LEVEL_LOW)

/** ================================================================================ *//**
 * @brief 定义时间Tick返回的魔术值
* -------------------------------------------------------------------------------- */
#define TIME_TICK_RETURN_MAGIC 666666

/** ================================================================================ *//**
 *  @brief 定义是否需要补偿延时
 *  @note  无法判断则无需修改
* -------------------------------------------------------------------------------- */
// #define NO_NEED_COMPEMSATION    /** @brief 定义时无需补偿 */

/** ================================================================================ *//**
 *  @brief      时间补偿
 * -------------------------------------------------------------------------------- */
#if !defined(NO_NEED_COMPEMSATION)
    #define DELAY_MS_COMPENSATION		(1)	/* 毫秒延时补偿,默认减1ms */
    #define DELAY_US_COMPENSATION		(0)	/* 微秒延时补偿,默认减0us */
    #define MS_COMPENSATION				(0)	/* 毫秒测量补偿,默认减0ms */
    #define US_COMPENSATION				(1)	/* 微秒测量补偿,默认减1us */
                                            /** @note 有几us误差正常 */
#endif /* !NO_NEED_COMPEMSATION */
#pragma endregion 常量和配置定义 /* ↑ */

#pragma region 定义时间相关函数
namespace Vasl {
    extern size_t get_ms_ticks(void);
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      毫秒延时函数
        *  @param[in]  ms 延时时长 ms
        * 
        * -------------------------------------------------------------------------------- */
        inline void delay_ms(uint32_t ms) {
        #if defined(NO_NEED_COMPEMSATION)
            R_BSP_SoftwareDelay(ms, BSP_DELAY_UNITS_MILLISECONDS);
        #else
            if (ms > DELAY_MS_COMPENSATION) {
                R_BSP_SoftwareDelay(ms - DELAY_MS_COMPENSATION, BSP_DELAY_UNITS_MILLISECONDS);
            } else {
                R_BSP_SoftwareDelay(ms, BSP_DELAY_UNITS_MILLISECONDS);   
            }
        }
        #endif /* NO_NEED_COMPEMSATION */
        /** ================================================================================ *//**
        *  @brief      毫秒Tick获取
        * 
        * -------------------------------------------------------------------------------- */
        inline uint32_t GetTickMs(void) {
            return get_ms_ticks();
        }
        /** ================================================================================ *//**
        *  @brief      DWT初始化函数
        * 
        * -------------------------------------------------------------------------------- */
        extern void DWT_Init(void);
        /** ================================================================================ *//**
        *  @brief      微秒延时
        *  @param[in]  us__ 延时时长 us
        * 
        * -------------------------------------------------------------------------------- */
        extern void delay_us(const uint32_t us__);
        /** ================================================================================ *//**
        *  @brief      获取当前周期计数
        *  @return     uint32_t: 当前运行周期计数
        *  @warning    无DWT时请自行实现
        * 
        * -------------------------------------------------------------------------------- */
        extern uint32_t GetCycleTick(void);
        /** ================================================================================ *//**
        *  @brief      获取微秒Tick函数
        *  @return     uint32_t: 当前微秒数
        * 
        * -------------------------------------------------------------------------------- */
        extern uint32_t GetTickUs(void);
    }
}
#pragma endregion 定义时间相关函数 /* ↑ */

#pragma region 定义GPIO相关函数
namespace Vasl {
    inline namespace Layer {
        #define __VASL_RA_GIOPORT_CTRL__ g_ioport.p_ctrl
        /** ================================================================================ *//**
        *  @brief      GPIO 写入Pin
        *  @param[in]  io: GPIO引脚IO
        *  @param[in]  state: GPIO引脚电平
        * 
        * -------------------------------------------------------------------------------- */
        inline void GPIO_WritePin(IOType io, LevelType state) {
            R_IOPORT_PinWrite(__VASL_RA_GIOPORT_CTRL__, io, state);
        }
        /** ================================================================================ *//**
        *  @brief      GPIO 置位Pin
        *  @param[in]  GPIOx: 通用输入输出端口
        *  @param[in]  io: GPIO引脚IO
        * 
        * -------------------------------------------------------------------------------- */
        inline void GPIO_SetPin(IOType io) {
            GPIO_WritePin(io, VASL_LAYER_PIN_SET_LEVEL);
        }
        /** ================================================================================ *//**
        *  @brief      GPIO 复位Pin
        *  @param[in]  io: GPIO引脚IO
        * 
        * -------------------------------------------------------------------------------- */
        inline void GPIO_ResetPin(IOType io) {
            GPIO_WritePin(io, VASL_LAYER_PIN_RESET_LEVEL);
        }
        /** ================================================================================ *//**
        *  @brief      GPIO 翻转Pin
        *  @param[in]  io: GPIO引脚IO
        * 
        * -------------------------------------------------------------------------------- */
        inline void GPIO_TogglePin(IOType io) {
            LevelType state = static_cast<LevelType>(R_BSP_PinRead(io));
            GPIO_WritePin(io, state == VASL_LAYER_PIN_SET_LEVEL ? VASL_LAYER_PIN_RESET_LEVEL : VASL_LAYER_PIN_SET_LEVEL);
        }
        /** ================================================================================ *//**
        *  @brief      GPIO 读取Pin
        *  @param[in]  io: GPIO引脚IO
        *  @return     GPIO_PinState: GPIO引脚电平
        * 
        * -------------------------------------------------------------------------------- */
        inline LevelType GPIO_ReadPin(IOType io) {
            return static_cast<LevelType>(R_BSP_PinRead(io));
        }
    }
}
#pragma endregion 定义GPIO相关函数 /* ↑ */
#pragma region 定义UART相关函数
#ifdef __LAYER_USE_SCI_UART__
namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      UART 打开/初始化
        *  @param[in]  ctrl: UART控制块指针
        *  @param[in]  cfg: UART配置指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t UART_Open(UartCtrlType* ctrl, const UartCfgType* cfg) {
            return R_SCI_UART_Open(ctrl, cfg);
        }
        /** ================================================================================ *//**
        *  @brief      UART 写入数据
        *  @param[in]  ctrl: UART控制块指针
        *  @param[in]  p_src: 数据指针
        *  @param[in]  bytes: 数据字节数
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t UART_Write(UartCtrlType* ctrl, uint8_t const* const p_src, uint32_t const bytes) {
            return R_SCI_UART_Write(ctrl, p_src, bytes);
        }
        /** ================================================================================ *//**
        *  @brief      UART 读取数据
        *  @param[in]  ctrl: UART控制块指针
        *  @param[in]  p_dest: 数据指针
        *  @param[in]  bytes: 数据字节数
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t UART_Read(UartCtrlType* ctrl, uint8_t* const p_dest, uint32_t const bytes) {
            return R_SCI_UART_Read(ctrl, p_dest, bytes);
        }
        /** ================================================================================ *//**
        *  @brief      UART 写入数据
        *  @param[in]  ctrl: UART控制块指针
        *  @param[in]  p_src: 数据指针
        *  @param[in]  bytes: 数据字节数
        *  @param[in]  ready_flag: 就绪标志指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t UART_Write(UartCtrlType* ctrl, uint8_t const* const p_src, uint32_t const bytes, bool* ready_flag) {
            while (*ready_flag != true) {}
            fsp_err_t ret = R_SCI_UART_Write(ctrl, p_src, bytes);
            if (FSP_SUCCESS == ret) {
                *ready_flag = false;
            }
            return ret;
        }
        /** ================================================================================ *//**
        *  @brief      UART 读取数据
        *  @param[in]  ctrl: UART控制块指针
        *  @param[in]  p_dest: 数据指针
        *  @param[in]  bytes: 数据字节数
        *  @param[in]  ready_flag: 就绪标志指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t UART_Read(UartCtrlType* ctrl, uint8_t* const p_dest, uint32_t const bytes, bool* ready_flag) {
            while (*ready_flag != true) {}
            fsp_err_t ret = R_SCI_UART_Read(ctrl, p_dest, bytes);
            if (FSP_SUCCESS == ret) {
                *ready_flag = false;
            }
            return ret;
        }
    }
}
#endif ///< __LAYER_USE_SCI_UART__
#pragma endregion 定义UART相关函数 /* ↑ */
#pragma region 定义QSPI相关函数
#ifdef __LAYER_USE_QSPI__
namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      QSPI 打开/初始化
        *  @param[in]  inst: QSPI实例指针
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t QSPI_Open(const QspiType* inst) { return R_QSPI_Open(inst->p_ctrl, inst->p_cfg); }
        inline fsp_err_t QSPI_Open(QspiType* inst) { return R_QSPI_Open(inst->p_ctrl, inst->p_cfg); }
        /** ================================================================================ *//**
        *  @brief      QSPI 直接写入数据
        *  @param[in]  inst: QSPI实例指针
        *  @param[in]  p_src: 数据指针 源数据/要发送的数据 地址
        *  @param[in]  bytes: 发送数据字节数
        *  @param[in]  read_after_write：发送数据结束后是否要立即读数据的标志位 true会立即读
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t QSPI_DirectWrite(
            const QspiType* inst, uint8_t const * p_src, uint32_t bytes, bool read_after_write) {
            return R_QSPI_DirectWrite(inst->p_ctrl, p_src, bytes, read_after_write);
        }
        inline fsp_err_t QSPI_DirectWrite(
            QspiType* inst, uint8_t const * p_src, uint32_t bytes, bool read_after_write) {
            return R_QSPI_DirectWrite(inst->p_ctrl, p_src, bytes, read_after_write);
        }
        /** ================================================================================ *//**
        *  @brief      QSPI 直接读取数据
        *  @param[in]  inst: QSPI实例指针
        *  @param[in]  p_dest: 数据指针 目的数据/要接收的数据 地址
        *  @param[in]  bytes: 读取数据字节数
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t QSPI_DirectRead(const QspiType* inst, uint8_t* p_dest, uint32_t bytes) {
            return R_QSPI_DirectRead(inst->p_ctrl, p_dest, bytes);
        }
        inline fsp_err_t QSPI_DirectRead(QspiType* inst, uint8_t* p_dest, uint32_t bytes) {
            return R_QSPI_DirectRead(inst->p_ctrl, p_dest, bytes);
        }
        /** ================================================================================ *//**
        *  @brief      QSPI 写入数据
        *  @param[in]  inst: QSPI实例指针
        *  @param[in]  p_src: 数据指针 源数据/要发送的数据 地址
        *  @param[in]  p_dest: 存储设备的目标地址
        *  @param[in]  byte_count: 数据字节数
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t QSPI_Write(const QspiType* inst, uint8_t*  p_src, uint8_t* p_dest, uint32_t byte_count) {
            return R_QSPI_Write(inst->p_ctrl, p_src, p_dest, byte_count);
        }
        inline fsp_err_t QSPI_Write(QspiType* inst, uint8_t*  p_src, uint8_t* p_dest, uint32_t byte_count) {
            return R_QSPI_Write(inst->p_ctrl, p_src, p_dest, byte_count);
        }
        /** ================================================================================ *//**
        *  @brief      QSPI 擦除扇区
        *  @param[in]  inst: QSPI实例指针
        *  @param[in]  p_device_address: 存储设备的目标地址
        *  @param[in]  byte_count: 要擦除的数据个数
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t QSPI_Erase(const QspiType* inst, uint8_t* p_device_address, uint32_t byte_count) {
            return R_QSPI_Erase(inst->p_ctrl, p_device_address, byte_count);
        }
        inline fsp_err_t QSPI_Erase(QspiType* inst, uint8_t* p_device_address, uint32_t byte_count) {
            return R_QSPI_Erase(inst->p_ctrl, p_device_address, byte_count);
        }
        /** ================================================================================ *//**
        *  @brief      QSPI 状态获取
        *  @warning    在写、擦除QSPI之前 需要判断上一个操作是否完成
        *  @note       状态获取时 会阻塞等待 直到状态获取完成
        *  @param[in]  inst: QSPI实例指针
        *  @param[in]  p_status: 状态指针 指向保存状态值的地址
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t QSPI_StatusGet(const QspiType* inst, spi_flash_status_t * const p_status) {
            return R_QSPI_StatusGet(inst->p_ctrl, p_status);
        }
        inline fsp_err_t QSPI_StatusGet(QspiType* inst, spi_flash_status_t * const p_status) {
            return R_QSPI_StatusGet(inst->p_ctrl, p_status);
        }
    }
}
#endif ///< __LAYER_USE_QSPI__
#pragma endregion 定义QSPI相关函数 /* ↑ */
#pragma region 定义DMAC相关函数
#ifdef __LAYER_USE_DMAC__
namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      DMAC 打开/初始化
        *  @param[in]  inst: DMAC实例指针
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t DMAC_Open(const DmacType* inst) { return R_DMAC_Open(inst->p_ctrl, inst->p_cfg); }
        inline fsp_err_t DMAC_Open(DmacType* inst) { return R_DMAC_Open(inst->p_ctrl, inst->p_cfg); }
        /** ================================================================================ *//**
        *  @brief      DMAC 关闭/注销
        *  @param[in]  inst: DMAC实例指针
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t DMAC_Close(const DmacType* inst) { return R_DMAC_Close(inst->p_ctrl); }
        inline fsp_err_t DMAC_Close(DmacType* inst) { return R_DMAC_Close(inst->p_ctrl); }
        /** ================================================================================ *//**
        *  @brief      复位DMAC传输的地址和数据个数
        *  @param[in]  inst: DMAC实例指针
        *  @param[in]  p_src: 数据指针 源数据/要发送的数据 地址
        *  @param[in]  p_dest: 存储设备的目标地址
        *  @param[in]  num_transfers: 数据字节数
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t DMAC_Reset(const DmacType* inst, void* p_src, void* p_dest, uint16_t num_transfers) {
            return R_DMAC_Reset(inst->p_ctrl, p_src, p_dest, num_transfers); 
        }
        inline fsp_err_t DMAC_Reset(DmacType* inst, void* p_src, void* p_dest, uint16_t num_transfers) {
            return R_DMAC_Reset(inst->p_ctrl, p_src, p_dest, num_transfers); 
        }
        /** ================================================================================ *//**
        *  @brief      DMAC 使能数据传输
        *  @param[in]  inst: DMAC实例指针
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t DMAC_Enable(const DmacType* inst) { return R_DMAC_Enable(inst->p_ctrl); }
        inline fsp_err_t DMAC_Enable(DmacType* inst) { return R_DMAC_Enable(inst->p_ctrl); }
        /** ================================================================================ *//**
        *  @brief      DMAC 失能链接触发
        *  @param[in]  inst: DMAC实例指针
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t DMAC_Disable(const DmacType* inst, transfer_start_mode_t mode) {
            return R_DMAC_SoftwareStart(inst->p_ctrl, mode);
        }
        inline fsp_err_t DMAC_Disable(DmacType* inst, transfer_start_mode_t mode) {
            return R_DMAC_SoftwareStart(inst->p_ctrl, mode);
        }
        /** ================================================================================ *//**
        *  @brief      DMAC 软件触发
        *  @param[in]  inst: DMAC实例指针
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t DMAC_SoftwareStart(const DmacType* inst, transfer_start_mode_t mode) {
            return R_DMAC_SoftwareStart(inst->p_ctrl, mode);
        }
        inline fsp_err_t DMAC_SoftwareStart(DmacType* inst, transfer_start_mode_t mode) {
            return R_DMAC_SoftwareStart(inst->p_ctrl, mode);
        }
        /** ================================================================================ *//**
        *  @brief      DMAC 停止软件触发
        *  @param[in]  inst: DMAC实例指针
        *  @return     fsp_err_t: FSP错误码
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t DMAC_SoftwareStop(const DmacType* inst) { return R_DMAC_SoftwareStop(inst->p_ctrl); }
        inline fsp_err_t DMAC_SoftwareStop(DmacType* inst) { return R_DMAC_SoftwareStop(inst->p_ctrl); }
        /** ================================================================================ *//**
        *  @brief      DMAC 设置传输长度
        *  @param[in]  inst: DMAC实例指针
        *  @param[in]  len: 数据字节数
        *  @return     void
        * 
        * -------------------------------------------------------------------------------- */
        inline void DMAC_SetTransferLength(const DmacType* inst, size_t len) {
            inst->p_cfg->p_info->length = static_cast<uint16_t>(len);
        }
        /** ================================================================================ *//**
        *  @brief      DMAC 设置传输源地址和目标地址
        *  @param[in]  inst: DMAC实例指针
        *  @param[in]  p_src: 数据指针 源数据/要发送的数据 地址
        *  @param[in]  p_dest: 存储设备的目标地址
        *  @return     void
        * 
        * -------------------------------------------------------------------------------- */
        inline void DMAC_SetTransferAddress(const DmacType* inst, uint8_t* p_src, uint8_t* p_dest) {
            inst->p_cfg->p_info->p_src = p_src;
            inst->p_cfg->p_info->p_dest = p_dest;
        }
    }
}
#endif ///< __LAYER_USE_DMAC__
#pragma endregion 定义DMAC相关函数 /* ↑ */
#pragma region 定义SCI I2C相关函数
#ifdef __LAYER_USE_SCI_I2C__
namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      SCI IIC 打开/初始化
        *  @param[in]  inst: IIC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SCI_I2C_Open(const I2cType* inst) {
            return R_SCI_I2C_Open(inst->p_ctrl, inst->p_cfg);
        }
        inline fsp_err_t SCI_I2C_Open(I2cType* inst) {
            return R_SCI_I2C_Open(inst->p_ctrl, inst->p_cfg);
        }
        /** ================================================================================ *//**
        *  @brief      SCI IIC 关闭/注销
        *  @param[in]  inst: IIC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SCI_I2C_Close(const I2cType* inst) { return R_SCI_I2C_Close(inst->p_ctrl); }
        inline fsp_err_t SCI_I2C_Close(I2cType* inst) { return R_SCI_I2C_Close(inst->p_ctrl); }
        /** ================================================================================ *//**
        *  @brief      IIC 设置从机地址
        *  @param[in]  inst: IIC实例指针
        *  @param[in]  slave: 从机地址
        *  @param[in]  addr_mode: 地址模式
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SCI_I2C_SlaveAddressSet(const I2cType* inst, uint32_t  const slave, i2c_master_addr_mode_t addr_mode) {
            return R_SCI_I2C_SlaveAddressSet(inst->p_ctrl, slave, addr_mode);
        }
        inline fsp_err_t SCI_I2C_SlaveAddressSet(I2cType* inst, uint32_t const slave, i2c_master_addr_mode_t addr_mode) {
            return R_SCI_I2C_SlaveAddressSet(inst->p_ctrl, slave, addr_mode);
        }
        /** ================================================================================ *//**
        *  @brief      SCI IIC 发送指定长度的数据
        *  @param[in]  inst: IIC实例指针
        *  @param[in]  p_src: 指向发送数据的起始地址
        *  @param[in]  bytes: 发送bytes个字节的数据
        *  @param[in]  restart: 是否发送重启信号 如果为true则发送重启信号，否则发送停止信号
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SCI_I2C_Write(const I2cType* inst, uint8_t* p_src, uint32_t bytes, bool restart) {
            return R_SCI_I2C_Write(inst->p_ctrl, p_src, bytes, restart);
        }
        inline fsp_err_t SCI_I2C_Write(I2cType* inst, uint8_t* p_src, uint32_t bytes, bool restart) {
            return R_SCI_I2C_Write(inst->p_ctrl, p_src, bytes, restart);
        }
        /** ================================================================================ *//**
        *  @brief      SCI IIC 接收指定长度的数据
        *  @param[in]  inst: IIC实例指针
        *  @param[in]  p_dest: 指向接收数据的起始地址
        *  @param[in]  bytes: 接收bytes个字节的数据
        *  @param[in]  restart: 是否发送重启信号 如果为true接收完一帧发送ACK，否则发送NACK
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SCI_I2C_Read(const I2cType* inst, uint8_t* p_dest, uint32_t bytes, bool restart) {
            return R_SCI_I2C_Read(inst->p_ctrl, p_dest, bytes, restart);
        }
        inline fsp_err_t SCI_I2C_Read(I2cType* inst, uint8_t* p_dest, uint32_t bytes, bool restart) {
            return R_SCI_I2C_Read(inst->p_ctrl, p_dest, bytes, restart);
        }
        /** ================================================================================ *//**
        *  @brief      SCI IIC 取消当前传输
        *  @param[in]  inst: IIC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SCI_I2C_Abort(const I2cType* inst) {
            return R_SCI_I2C_Abort(inst->p_ctrl);
        }
        inline fsp_err_t SCI_I2C_Abort(I2cType* inst) {
            return R_SCI_I2C_Abort(inst->p_ctrl);
        }
    }
}
#endif ///< __LAYER_USE_SCI_I2C__
#pragma endregion 定义SCI I2C相关函数 /* ↑ */
#pragma region 定义I2C相关函数
#ifdef __LAYER_USE_I2C__
namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      IIC 打开/初始化
        *  @param[in]  inst: IIC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t I2C_Open(const I2cType* inst) {
            return R_IIC_MASTER_Open(inst->p_ctrl, inst->p_cfg);
        }
        inline fsp_err_t I2C_Open(I2cType* inst) {
            return R_IIC_MASTER_Open(inst->p_ctrl, inst->p_cfg);
        }
        /** ================================================================================ *//**
        *  @brief      IIC 关闭/注销
        *  @param[in]  inst: IIC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t I2C_Close(const I2cType* inst) { return R_IIC_MASTER_Close(inst->p_ctrl); }
        inline fsp_err_t I2C_Close(I2cType* inst) { return R_IIC_MASTER_Close(inst->p_ctrl); }
        /** ================================================================================ *//**
        *  @brief      IIC 设置从机地址
        *  @param[in]  inst: IIC实例指针
        *  @param[in]  slave: 从机地址
        *  @param[in]  addr_mode: 地址模式
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t I2C_SlaveAddressSet(const I2cType* inst, uint32_t  const slave, i2c_master_addr_mode_t addr_mode) {
            return R_IIC_MASTER_SlaveAddressSet(inst->p_ctrl, slave, addr_mode);
        }
        inline fsp_err_t I2C_SlaveAddressSet(I2cType* inst, uint32_t const slave, i2c_master_addr_mode_t addr_mode) {
            return R_IIC_MASTER_SlaveAddressSet(inst->p_ctrl, slave, addr_mode);
        }
        /** ================================================================================ *//**
        *  @brief      IIC 发送指定长度的数据
        *  @param[in]  inst: IIC实例指针
        *  @param[in]  p_src: 指向发送数据的起始地址
        *  @param[in]  bytes: 发送bytes个字节的数据
        *  @param[in]  restart: 是否发送重启信号 如果为true则发送重启信号，否则发送停止信号
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t I2C_Write(const I2cType* inst, uint8_t* p_src, uint32_t bytes, bool restart) {
            return R_IIC_MASTER_Write(inst->p_ctrl, p_src, bytes, restart);
        }
        inline fsp_err_t I2C_Write(I2cType* inst, uint8_t* p_src, uint32_t bytes, bool restart) {
            return R_IIC_MASTER_Write(inst->p_ctrl, p_src, bytes, restart);
        }
        /** ================================================================================ *//**
        *  @brief      IIC 接收指定长度的数据
        *  @param[in]  inst: IIC实例指针
        *  @param[in]  p_dest: 指向接收数据的起始地址
        *  @param[in]  bytes: 接收bytes个字节的数据
        *  @param[in]  restart: 是否发送重启信号 如果为true接收完一帧发送ACK，否则发送NACK
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t I2C_Read(const I2cType* inst, uint8_t* p_dest, uint32_t bytes, bool restart) {
            return R_IIC_MASTER_Read(inst->p_ctrl, p_dest, bytes, restart);
        }
        inline fsp_err_t I2C_Read(I2cType* inst, uint8_t* p_dest, uint32_t bytes, bool restart) {
            return R_IIC_MASTER_Read(inst->p_ctrl, p_dest, bytes, restart);
        }
        /** ================================================================================ *//**
        *  @brief      IIC 取消当前传输
        *  @param[in]  inst: IIC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t I2C_Abort(const I2cType* inst) {
            return R_IIC_MASTER_Abort(inst->p_ctrl);
        }
        inline fsp_err_t I2C_Abort(I2cType* inst) {
            return R_IIC_MASTER_Abort(inst->p_ctrl);
        }
    }
}
#endif ///< __LAYER_USE_I2C__
#pragma endregion 定义I2C相关函数 /* ↑ */
#pragma region 定义IRQ相关函数
#ifdef __LAYER_USE_IRQ__
namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      IRQ 打开/初始化
        *  @param[in]  inst: IRQ实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t Irq_Open(const IrqType* inst) {
            return R_ICU_ExternalIrqOpen(inst->p_ctrl, inst->p_cfg);
        }
        inline fsp_err_t Irq_Open(IrqType* inst) {
            return R_ICU_ExternalIrqOpen(inst->p_ctrl, inst->p_cfg);
        }
        /** ================================================================================ *//**
        *  @brief      IRQ 使能
        *  @param[in]  inst: IRQ实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t Irq_Enable(const IrqType* inst) {
            return R_ICU_ExternalIrqEnable(inst->p_ctrl);
        }
        inline fsp_err_t Irq_Enable(IrqType* inst) {
            return R_ICU_ExternalIrqEnable(inst->p_ctrl);
        }
        /** ================================================================================ *//**
        *  @brief      IRQ 禁用
        *  @param[in]  inst: IRQ实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t Irq_Disable(const IrqType* inst) {
            return R_ICU_ExternalIrqDisable(inst->p_ctrl);
        }
        inline fsp_err_t Irq_Disable(IrqType* inst) {
            return R_ICU_ExternalIrqDisable(inst->p_ctrl);
        }
        /** ================================================================================ *//**
        *  @brief      IRQ 关闭/注销
        *  @param[in]  inst: IRQ实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t Irq_Close(const IrqType* inst) {
            return R_ICU_ExternalIrqClose(inst->p_ctrl);
        }
        inline fsp_err_t Irq_Close(IrqType* inst) {
            return R_ICU_ExternalIrqClose(inst->p_ctrl);
        }
    }
}
#endif ///< __LAYER_USE_IRQ__
#pragma endregion 定义IRQ相关函数 /* ↑ */
#pragma region 定义ADC相关函数
#ifdef __LAYER_USE_ADC__
namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      ADC 打开/初始化
        *  @param[in]  inst: ADC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t ADC_Open(const AdcType* inst) {
            return R_ADC_Open(inst->p_ctrl, inst->p_cfg);
        }
        inline fsp_err_t ADC_Open(AdcType* inst) {
            return R_ADC_Open(inst->p_ctrl, inst->p_cfg);
        }
        /** ================================================================================ *//**
        *  @brief      ADC 扫描配置
        *  @param[in]  inst: ADC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t ADC_ScanCfg(const AdcType* inst) {
            return R_ADC_ScanCfg(inst->p_ctrl, inst->p_channel_cfg);
        }
        inline fsp_err_t ADC_ScanCfg(AdcType* inst) {
            return R_ADC_ScanCfg(inst->p_ctrl, inst->p_channel_cfg);
        }
        /** ================================================================================ *//**
        *  @brief      ADC 关闭/注销
        *  @param[in]  inst: ADC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t ADC_Close(const AdcType* inst) {
            return R_ADC_Close(inst->p_ctrl);
        }
        inline fsp_err_t ADC_Close(AdcType* inst) {
            return R_ADC_Close(inst->p_ctrl);
        }
        /** ================================================================================ *//**
        *  @brief      ADC 扫描开始
        *  @param[in]  inst: ADC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t ADC_ScanStart(const AdcType* inst) {
            return R_ADC_ScanStart(inst->p_ctrl);
        }
        inline fsp_err_t ADC_ScanStart(AdcType* inst) {
            return R_ADC_ScanStart(inst->p_ctrl);
        }
        /** ================================================================================ *//**
        *  @brief      ADC 扫描停止
        *  @param[in]  inst: ADC实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t ADC_ScanStop(const AdcType* inst) {
            return R_ADC_ScanStop(inst->p_ctrl);
        }
        inline fsp_err_t ADC_ScanStop(AdcType* inst) {
            return R_ADC_ScanStop(inst->p_ctrl);
        }
        /** ================================================================================ *//**
        *  @brief      ADC 读取转换结果
        *  @param[in]  inst: ADC实例指针
        *  @param[in]  channel: ADC通道
        *  @param[out] p_data: 转换结果指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t ADC_Read(const AdcType* inst, adc_channel_t channel, uint16_t* p_data) {
            return R_ADC_Read(inst->p_ctrl, channel, p_data);
        }
        inline fsp_err_t ADC_Read(AdcType* inst, adc_channel_t channel, uint16_t* p_data) {
            return R_ADC_Read(inst->p_ctrl, channel, p_data);
        }
        /** ================================================================================ *//**
        *  @brief      ADC 获取通道信息
        *  @param[in]  inst: ADC实例指针
        *  @param[out] p_info: 通道信息指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t ADC_InfoGet(const AdcType* inst, AdcInfoType* p_info) {
            return R_ADC_InfoGet(inst->p_ctrl, p_info);
        }
        inline fsp_err_t ADC_InfoGet(AdcType* inst, AdcInfoType* p_info) {
            return R_ADC_InfoGet(inst->p_ctrl, p_info);
        }
    }
}
#endif ///< __LAYER_USE_ADC__
#pragma endregion 定义ADC相关函数 /* ↑ */
#pragma region 定义SPI相关函数
#ifdef __LAYER_USE_SPI__
namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      SPI 打开/初始化
        *  @param[in]  inst: SPI实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SPI_Open(const SpiType* inst) {
            return R_SPI_Open(inst->p_ctrl, inst->p_cfg);
        }
        inline fsp_err_t SPI_Open(SpiType* inst) {
            return R_SPI_Open(inst->p_ctrl, inst->p_cfg);
        }
        /** ================================================================================ *//**
        *  @brief      SPI 读取数据
        *  @param[in]  inst: SPI实例指针
        *  @param[out] p_dest: 接收数据指针
        *  @param[in]  length: 读取数据长度
        *  @param[in]  bit_width: 数据位宽
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SPI_Read(const SpiType* inst, void* p_dest, uint32_t length, spi_bit_width_t bit_width) {
            return R_SPI_Read(inst->p_ctrl, p_dest, length, bit_width);
        }
        inline fsp_err_t SPI_Read(SpiType* inst, void* p_dest, uint32_t length, spi_bit_width_t bit_width) {
            return R_SPI_Read(inst->p_ctrl, p_dest, length, bit_width);
        }
        /** ================================================================================ *//**
        *  @brief      SPI 写入数据
        *  @param[in]  inst: SPI实例指针
        *  @param[in]  p_src: 发送数据指针
        *  @param[in]  length: 发送数据长度
        *  @param[in]  bit_width: 数据位宽
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SPI_Write(const SpiType* inst, void* p_src, uint32_t length, spi_bit_width_t bit_width) {
            return R_SPI_Write(inst->p_ctrl, p_src, length, bit_width);
        }
        inline fsp_err_t SPI_Write(SpiType* inst, void* p_src, uint32_t length, spi_bit_width_t bit_width) {
            return R_SPI_Write(inst->p_ctrl, p_src, length, bit_width);
        }
        /** ================================================================================ *//**
        *  @brief      SPI 写入并读取数据
        *  @param[in]  inst: SPI实例指针
        *  @param[in]  p_src: 发送数据指针
        *  @param[out] p_dest: 接收数据指针
        *  @param[in]  length: 数据长度
        *  @param[in]  bit_width: 数据位宽
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SPI_WriteRead(
            const SpiType* inst, void* p_src, void* p_dest, uint32_t length, spi_bit_width_t bit_width) {
            return R_SPI_WriteRead(inst->p_ctrl, p_src, p_dest, length, bit_width);
        }
        inline fsp_err_t SPI_WriteRead(
            SpiType* inst, void* p_src, void* p_dest, uint32_t length, spi_bit_width_t bit_width) {
            return R_SPI_WriteRead(inst->p_ctrl, p_src, p_dest, length, bit_width);
        }
        /** ================================================================================ *//**
        *  @brief      SPI 关闭
        *  @param[in]  inst: SPI实例指针
        * 
        * -------------------------------------------------------------------------------- */
        inline fsp_err_t SPI_Close(const SpiType* inst) {
            return R_SPI_Close(inst->p_ctrl);
        }
        inline fsp_err_t SPI_Close(SpiType* inst) {
            return R_SPI_Close(inst->p_ctrl);
        }
    }
}
#endif ///< __LAYER_USE_SPI__
#pragma endregion 定义SPI相关函数 /* ↑ */

namespace Vasl {
    inline namespace Layer {
        /** ================================================================================ *//**
        *  @brief      VaslP ARM CMSIS 余弦函数
        *  @param[in]  x 输入值
        *  @return     float: 余弦值
        * 
        * -------------------------------------------------------------------------------- */
        inline float ArmCosF32(float x) {
            #include "math.h"
            return cosf(x);
        }
    }
}

#endif ///< __VASL_LAYER_HPP__
