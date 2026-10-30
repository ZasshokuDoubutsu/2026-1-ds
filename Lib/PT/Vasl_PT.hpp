#ifndef __VASL_PT_HPP__
#define __VASL_PT_HPP__

#include "pt.h"
#include "pt-sem.h"

#define __PT_GET_MS_TICK__          Vasl::get_ms_ticks()
#define __MACRO_VAR_NAME__          __COUNTER__

///< PT任务开始标志
#define VASL_PT_BEGIN               PT_BEGIN(pt)
///< PT任务结束标志
#define VASL_PT_END                 PT_END(pt)
///< PT让出CPU
#define VASL_PT_YIELD               PT_YIELD(pt)
///< PT让出CPU 并判断为真时继续执行
#define VASL_PT_YIELD_UNTIL(cond)   PT_YIELD_UNTIL(pt, cond)
///< PT彻底结束
#define VASL_PT_EXIT                PT_EXIT(pt)
///< PT下次重新启动
#define VASL_PT_RESTART             PT_RESTART(pt)
///< PT等待直到条件成立
#define VASL_PT_WAIT_UNTIL_OR_SKIP(cond)    PT_WAIT_UNTIL(pt, cond)
#define VASL_PT_WAIT_UNTIL(cond)    PT_WAIT_UNTIL(pt, cond)

///< PT信号量等待
#define VASL_PT_SEM_WAIT(s)         PT_SEM_WAIT(pt, s)
///< PT信号量发送
#define VASL_PT_SEM_POST(s)         PT_SEM_SIGNAL(pt, s)
#define VASL_PT_SEM_SIGNAL(s)       PT_SEM_SIGNAL(pt, s)

///< PT任务结构
#define VASL_PT_TASK_BEGIN          VASL_PT_BEGIN; for(;;) {
#define VASL_PT_TASK_END            } VASL_PT_END;

///< PT周期任务
#define VASL_PT_PERIOD_TASK(PERIOD) \
    do {static size_t last_var##__MACRO_VAR_NAME__ = 0; \
        VASL_PT_WAIT_UNTIL_OR_SKIP(__PT_GET_MS_TICK__ - last_var##__MACRO_VAR_NAME__ >= PERIOD); \
        last_var##__MACRO_VAR_NAME__ = __PT_GET_MS_TICK__; } while (0)
///< PT任务延时
#define VASL_PT_DELAY(MS)   \
    do {static size_t last_var##__MACRO_VAR_NAME__ = 0; \
        last_var##__MACRO_VAR_NAME__ = __PT_GET_MS_TICK__; \
        VASL_PT_WAIT_UNTIL_OR_SKIP(__PT_GET_MS_TICK__ - last_var##__MACRO_VAR_NAME__ >= MS); } while (0)

namespace Vasl {
    /** @note Vasl Ver26.3preview起自动初始化 */
    using TypePT = struct vasl_pt {
        lc_t lc {0};
    };
    /** @note Vasl Ver26.3preview起自动初始化 */
    using TypePTSem = struct vasl_pt_sem {
        unsigned int count {0};
    };
}

namespace Vasl {
    namespace CO {
        struct TypeTaskEntry {
            int (*func)(TypePT*);   ///< 任务函数
            TypePT* pt;             ///< PT状态指针
        };

        template<size_t MAX_TASKS = 3, bool ENABLE_WFI = true, typename T = uint8_t>
        class Scheduler {
            TypeTaskEntry tasks_[MAX_TASKS];  ///< 静态数组，无堆
            T task_count_ = 0;
            
        public:
            Scheduler() = default;  ///< 默认构造，无需参数
            
            ///< 添加任务
            bool add(int (*f)(TypePT*), TypePT* pt) {
                if (task_count_ >= MAX_TASKS) return false;
                tasks_[task_count_] = {f, pt};
                task_count_++;
                return true;
            }
            
            ///< 运行一轮：执行所有任务
            /// @return true=有任务在工作（不休眠），false=所有任务都在等待（可休眠）
            bool ProcessOnce() {
                if constexpr (ENABLE_WFI) {
                    bool any_working = false;
                    for (T i = 0; i < task_count_; i++) {
                        int status = tasks_[i].func(tasks_[i].pt);
                        if (status == PT_YIELDED || status == PT_EXITED) {
                            any_working = true;
                        }
                    }
                    return any_working;
                } else {
                    for (T i = 0; i < task_count_; i++) {
                        tasks_[i].func(tasks_[i].pt);
                    }
                    return true;  // 禁用WFI，永远返回true（不休眠）
                }
            }

            T GetTaskCount() const { return task_count_; }
        };
        
        inline void SEM_Post(TypePTSem* sem, size_t count = 1) {
            sem->count += count;
        }
    }
}

#endif ///< __VASL_PT_HPP__
