#ifndef __VASL_RTDATAPACKET_HPP__
#define __VASL_RTDATAPACKET_HPP__

#include <array>
#include <utility>
#include "Struct/Vasl_Queue.hpp"
#include "Struct/Vasl_RingBuf.hpp"

/** ================================================================================ *//**
 *  @brief      串口接收缓冲结构模板
 *  @note       简陋版本，仅用于自带DMA+IDLE的串口接收
 * -------------------------------------------------------------------------------- */
namespace Vasl {
    template<uint32_t size>
    class TypeUartRcv {
    private:
        volatile size_t rxSize;                  ///< 串口接收数据大小
        volatile err_t rxFrameReadyFlag : 1;     ///< 接收一帧数据完成标志
        volatile err_t rxDoubleBufUsedFlag : 1;  ///< 双缓冲区全部使用标志
        volatile uint8_t                : 6;     ///< 保留位
        Vasl::array<uint8_t, size> rxBuf;  ///< 当前正在接收的缓冲区
    public:
        Vasl::array<uint8_t, size> rxData; ///< 已接收完成的缓冲区 供主程序处理

        TypeUartRcv() {reset();}

        void reset() {
            rxSize = 0;
            rxFrameReadyFlag = Vasl::NOT;
            rxBuf.fill(0);
            rxData.fill(0);
        }

        void init(void) {
            reset();
        }

        void swap(void) {
            std::swap(rxBuf, rxData);
        }

        size_t getRxSize(void) {
            return rxSize;
        }

        Vasl::err_t getRxFrameReadyFlag(void) {
            return rxFrameReadyFlag;
        }

        Vasl::err_t getRxDoubleBufUsedFlag(void) {
            return rxDoubleBufUsedFlag;
        }

        uint8_t* getRxBufPtr(void) {
            return rxBuf.data();
        }

        size_t getRxBufSize(void) {
            return size;
        }

        void restartRx(void) {
            rxFrameReadyFlag = Vasl::NOT;
            rxSize = 0;
            std::fill(rxData.begin(), rxData.end(), 0);
        }

        void setRxSize(size_t _size) {
            rxSize = _size;
        }

        void setRxFrameReadyFlag(Vasl::err_t flag) {
            rxFrameReadyFlag = flag;
        }

        void setRxDoubleBufUsedFlag(Vasl::err_t flag) {
            rxDoubleBufUsedFlag = flag;
        }

        void itHandler(void) {
            if (this->getRxFrameReadyFlag == Vasl::OK) {
                this->setRxDoubleBufUsedFlag(Vasl::OK);
                return; ///< 上次数据还未处理完，暂不交换，每次使用时检查该标志，若为OK则需再次处理第二缓冲区
            }
            this->setRxFrameReadyFlag(Vasl::OK);
            this->swap();
        }

        void itHandler(size_t _size) {
            this->setRxSize(_size);
            if (this->getRxFrameReadyFlag() == Vasl::OK) {
                this->setRxDoubleBufUsedFlag(Vasl::OK);
                return; ///< 上次数据还未处理完，暂不交换，每次使用时检查该标志，若为OK则需再次处理第二缓冲区
            }
            this->setRxFrameReadyFlag(Vasl::OK);
            this->swap();
        }
    };
}

/** ================================================================================ *//**
 *  @brief      串口接收包模板
 *  @note       支持中断接收和超时检测，使用Vasl::array双缓冲区交换
 * -------------------------------------------------------------------------------- */
namespace Vasl {
    template<uint32_t size>
    class TypeUartRcvDataPacket {
    private:
        // UsartType* 				usartPort;
        // IRQn_Type 				irqNum;
        Vasl::array<uint8_t, size> rxBuf;   // 当前正在接收的缓冲区
        volatile uint32_t 	    rxWriteIndex;   	// 当前接收位置
        volatile uint32_t 	    rxLastTime;     	// 最后接收时间
        volatile Vasl::err_t    rxFrameReadyFlag : 1;// 帧就绪标志
        volatile Vasl::err_t    rxFlowFlag 		 : 1;// 接收溢出标志
        volatile uint32_t		rxSize;             // 本次接收数据大小
    public:
        Vasl::array<uint8_t, size> rxData;  // 已接收完成的缓冲区 供主程序处理

        TypeUartRcvDataPacket() {reset();}
        // TypeUartRcvDataPacket(UsartType* usart) : usartPort(usart) {reset();}

        // 重置所有状态
        void reset() {
            rxWriteIndex = 0;
            rxLastTime = 0;
            rxFrameReadyFlag = Vasl::NOT;
            rxFlowFlag = Vasl::NOT;
            rxSize = 0;
            rxBuf.fill(0);
            rxData.fill(0);
        }

        void init(void) {
        // void init(UsartType* usart) {
            // usartPort = usart;
            reset();
        }

        // 接收中断服务程序
        void rxIsrHandler(uint8_t data, uint32_t currTick) {
            if(rxWriteIndex >= size) {
                rxWriteIndex = 0;	// 缓冲区溢出，重置
                rxFlowFlag = Vasl::OK;
            }
            rxBuf[rxWriteIndex++] = data;
            rxLastTime = currTick;  // 需要实现获取当前tick的函数
        }

        // 超时检测和缓冲区交换
        Vasl::err_t checkTimeout_SwapBuf(uint32_t currTick, uint32_t timeout = 8) {
            if(currTick - rxLastTime < timeout) {
                return Vasl::NOT;
            }

            if(rxWriteIndex > 0) {	// 交换数据
                auto* nonVolatileBuf = const_cast<Vasl::array<uint8_t, size>*>(&rxBuf);
                auto* nonVolatileData = const_cast<Vasl::array<uint8_t, size>*>(&rxData);
                std::swap(*nonVolatileBuf, *nonVolatileData);
                rxFrameReadyFlag = Vasl::OK;
                if(Vasl::OK == rxFlowFlag || size == rxWriteIndex) {
                    rxSize = -1;
                } else {
                    rxSize = rxWriteIndex;
                }
                rxWriteIndex = 0;  	// 重置写指针
            }

            rxLastTime = currTick;
            return Vasl::OK;
        }

        // 缓冲区交换
        Vasl::err_t swapBuf() {
            if(rxWriteIndex > 0) {	// 交换数据
                auto* nonVolatileBuf = const_cast<Vasl::array<uint8_t, size>*>(&rxBuf);
                auto* nonVolatileData = const_cast<Vasl::array<uint8_t, size>*>(&rxData);
                std::swap(*nonVolatileBuf, *nonVolatileData);
                rxFrameReadyFlag = Vasl::OK;
                rxWriteIndex = 0;  	// 重置写指针
            }
            return Vasl::OK;
        }

        // 标记缓冲已使用
        Vasl::err_t clearRxData() {
            auto& nonVolatileData = const_cast<Vasl::array<uint8_t, size>&>(rxData);
            std::fill(nonVolatileData.begin(), nonVolatileData.begin() + rxSize, 0);
            rxFrameReadyFlag = Vasl::NOT;
            rxFlowFlag = Vasl::NOT;
            rxSize = 0;
            return Vasl::OK;
        }

        // 获取接收数据帧标志位
        Vasl::err_t getRxFrameReadyFlag() {
            return this->rxFrameReadyFlag;
        }

        // 获取接收字节数
        size_t getRxSize() {
            return this->rxSize;
        }

        void setRxSize(size_t _size) {
            this->rxSize = _size;
        }

        void setRxFrameReadyFlag(Vasl::err_t flag) {
            this->rxFrameReadyFlag = flag;
        }
    };
}

namespace Vasl {
    template<size_t SIZE>
    class TypeUartRcvDataPacketRing {
    private:
        TypeRingBufU8<SIZE>     rxRingBuf;              ///< 接收数据环形缓冲区
        volatile uint32_t 	    lastRxTime;     	    ///< 最后接收时间
        volatile Vasl::err_t    rxFrameReadyFlag : 1;   ///< 帧就绪标志
        volatile Vasl::err_t    rxFlowFlag 		 : 1;   ///< 接收溢出标志
        volatile Vasl::err_t    rxStartFlag      : 1;   ///< 接收开始标志
        uint8_t                                  : 5;   ///< keep
        volatile uint32_t		rxSize;                 ///< 本次接收数据大小
    public:
        TypeUartRcvDataPacketRing() { init(); }

        void init(void) {
            lastRxTime = 0;
            clear();
        }

        void clear(void) {
            rxFrameReadyFlag = Vasl::NOT;
            rxFlowFlag = Vasl::NOT;
            rxSize = 0;
            rxStartFlag = Vasl::NOT;
            rxRingBuf.clear();
        }

        void setLastRxTime(uint32_t _time) { this->lastRxTime = _time; }

        void setRxFrameReadyFlag(Vasl::err_t flag) { this->rxFrameReadyFlag = flag; }

        void setRxFlowFlag(Vasl::err_t flag) { this->rxFlowFlag = flag; }

        void setRxSize(uint32_t _size) { this->rxSize = _size; }

        void setRxStartFlag(Vasl::err_t flag) { this->rxStartFlag = flag; }

        Vasl::err_t getRxFrameReadyFlag(void) { return this->rxFrameReadyFlag; }
        Vasl::err_t getRxFrameReadyFlag(void) const { return this->rxFrameReadyFlag; }

        Vasl::err_t getRxFlowFlag(void) { return this->rxFlowFlag; }

        uint32_t getRxSize(void) { return this->rxSize; }

        uint32_t getSpace(void) { return SIZE - this->rxSize; }

        uint32_t getLastRxTime(void) { return this->lastRxTime; }

        Vasl::err_t getRxStartFlag(void) { return this->rxStartFlag; }

        Vasl::err_t pop(uint8_t* data) { return rxRingBuf.pop(data); }

        uint8_t pop(void) { return rxRingBuf.pop(); }

        uint8_t* pop_ptr(void) { return rxRingBuf.pop_ptr(); }
        
        Vasl::err_t push(uint8_t data) { return rxRingBuf.push(data); }

        Vasl::err_t isrHandler(uint8_t data_, uint32_t currTick_) {
            this->lastRxTime = currTick_;
            this->rxStartFlag = Vasl::OK;
            return rxRingBuf.push(data_);
        }

        Vasl::err_t checkTimeout_CheckFrameReady(uint32_t currTick_, uint32_t timeout_ = 8) {
            if(currTick_ - lastRxTime < timeout_) { return Vasl::NOT; }

            this->rxStartFlag = Vasl::NOT;
            this->rxFrameReadyFlag = Vasl::OK;
            this->lastRxTime = currTick_;
            refreshRxSize();
            return Vasl::OK;
        }

        size_t pop_all(uint8_t* p_data, size_t max_buf_space_) {
            if(this->rxRingBuf.is_empty()) { return 0; }
            refreshRxSize();
            if(max_buf_space_ < this->rxSize) { return 0; }

            return this->rxRingBuf.pop(p_data, this->rxSize);
        }

        uint8_t* pop_all(void) {
            if(this->rxRingBuf.is_empty()) { return nullptr; }
            refreshRxSize();
            return this->rxRingBuf.pop(this->rxSize);
        }

        void refreshRxSize(void) {
            this->rxSize = this->rxRingBuf.get_used_space();
        }

        // const char* find_str(const char* pattern) const {
        //     return rxRingBuf.find_str(pattern);
        // }

        // const char* find_str_after(const char* pattern) const {
        //     return rxRingBuf.find_str_after(pattern);
        // }
    };
}

/** ================================================================================ *//**
 *  @brief      std::pair 详解（C++11/14/17 兼容）
 *
 *  std::pair 是 C++ 标准库中最简单的"容器"，它只是一个"两个数据的组合"。
 *  类似于 C 语言中的"结构体"，但更轻量、自动支持比较操作。
 *
 *  对比 C 语言结构体：
 *    C:     struct KeyValue { char key[16]; uint8_t value; };
 *    C++:   std::pair<Vasl::array<uint8_t,16>, uint8_t>
 *
 * -------------------------------------------------------------------------------- */

/** std::pair 常用方法说明（C++11/14/17）：
 *
 * 1. 什么是 pair？
 *    - pair = 两个数据打包成一个对象
 *    - 第一个元素叫 .first，第二个叫 .second
 *    - 可以存放任意两种不同类型的组合
 *
 * 2. 创建 pair 的方法（C++11/14/17）：
 *    ┌─────────────────────────────────────────────────────────────────────────────┐
 *    │ 方法                          │ 示例代码                                    │
 *    ├─────────────────────────────────────────────────────────────────────────────┤
 *    │ 直接构造                      │ std::pair<int, const char*> p(10, "hello"); │
 *    │ 使用 make_pair（推荐）         │ auto p = std::make_pair(10, "hello");       │
 *    │ C++11 花括号初始化             │ auto p = std::pair<int, const char*>{10, "hello"}; │
 *    │ C++11 类型推导（auto）         │ auto p = std::make_pair(10, "hello");       │
 *    │ C++17 结构化绑定（解构）        │ auto [key, value] = p;  // p.first, p.second │
 *    └─────────────────────────────────────────────────────────────────────────────┘
 *
 * 3. 访问元素：
 *    - p.first      // 访问第一个元素（类似结构体 .key）
 *    - p.second     // 访问第二个元素（类似结构体 .value）
 *    - std::get<0>(p)  // 模板方式访问第0个元素（first）
 *    - std::get<1>(p)  // 模板方式访问第1个元素（second）
 *
 * 4. 比较操作：
 *    - p1 == p2     // 两对都相等才返回 true
 *    - p1 != p2     // 任一不等返回 true
 *    - p1 < p2      // 字典序比较：先比 first，再比 second
 *    - p1 <= p2     // 小于等于
 *    - p1 > p2      // 大于
 *    - p1 >= p2     // 大于等于
 *
 *    比较规则（字典序）：
 *      ("a", 1) < ("b", 0)  // true（"a" < "b"）
 *      ("a", 2) < ("a", 1)  // true（first 相等，2 > 1？不，first 相等时比 second）
 *      ("a", 2) < ("a", 3)  // true（first 相等，2 < 3）
 *
 * 5. 修改元素：
 *    - p.first = 20;        // 修改第一个元素
 *    - p.second = "world";  // 修改第二个元素
 *    - std::get<0>(p) = 20; // 模板方式修改
 *
 * 6. 交换操作：
 *    - p1.swap(p2)          // 交换两个 pair 的内容
 *    - std::swap(p1, p2)    // 同上，功能相同
 *
 * 7. 与 TypeDataPair 结合使用：
 *    TypeDataPair 实际上就是：std::pair<Vasl::array<uint8_t, size>, dataType>
 *
 *    使用示例：
 *      // 创建 TypeDataPair
 *      TypeDataPair<16, float> data1;
 *      data1.first  = {'t','e','m','p'};  // 键名（16字节数组）
 *      data1.second = 25.5f;               // 值
 *
 *      // 使用 make_pair 创建（需手动指定模板参数）
 *      auto data2 = std::make_pair<Vasl::array<uint8_t,16>, float>(
 *          {'h','u','m','i'}, 24.0f);
 *
 * 8. 常见用途：
 *    - 作为函数返回值（返回多个值）
 *    - std::map/std::unordered_map 的键值对
 *    - 临时组合两个相关数据
 *    - 算法返回值（如 std::minmax）
 *
 * 9. C++17 结构化绑定（强烈推荐！）：
 *    auto [key, value] = pair_obj;
 *    // 等价于：
 *    //   auto& key = pair_obj.first;
 *    //   auto& value = pair_obj.second;
 *
 *    遍历 map 时使用：
 *      for (const auto& [key, value] : myMap) { }
 *
 * 10. 注意事项：
 *     - pair 没有 .size() 方法（固定只有两个元素）
 *     - pair 的比较要求两个元素的类型都支持比较操作符
 *     - C++20 才有 pair 的 tuple_size 支持
 *     - 结构化绑定需要 C++17
 */

/** ================================================================================ *//**
 *  @brief      std::tuple 简介（pair 的扩展，可包含多个元素）
 *
 *  tuple 是 pair 的扩展，可以包含任意数量的元素（3个、4个...N个）。
 *  C++11 起支持，需要包含 <tuple>
 *
 *  示例：
 *    std::tuple<int, float, const char*> t(10, 3.14f, "hello");
 *    int a = std::get<0>(t);      // 获取第0个元素
 *    auto [x, y, z] = t;          // C++17 结构化绑定
 *
 * -------------------------------------------------------------------------------- */


#endif ///< __VASL_RTDATAPACKET_HPP__
