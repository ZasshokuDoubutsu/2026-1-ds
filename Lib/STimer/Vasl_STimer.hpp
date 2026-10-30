#ifndef __VASL_STIMER_HPP__
#define __VASL_STIMER_HPP__

#include "STimer/MultiTimer.h"
#include "Vasl_Type.hpp"

namespace Vasl {
    namespace ST {
        ///< 计时器对象
        using TIM = MultiTimer;
        ///< 安装系统时间基接口
        Vasl::err_t Install(PlatformTicksFunction_t fn_ptr);
        ///< 设置定时、超时回调函数、用户数据指针，并启动当前定时器
        Vasl::err_t Start(
            TIM* timer, uint32_t timing, MultiTimerCallback_t callback, void* userData);
        ///< 停止定时器
        Vasl::err_t Stop(TIM* timer);
        ///< 后台处理定时器事件
        Vasl::err_t Process(void);
    }
}

#endif ///< __VASL_STIMER_HPP__
