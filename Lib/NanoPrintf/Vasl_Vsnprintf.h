#ifndef __VASL_VSNPRINTF_H__
#define __VASL_VSNPRINTF_H__

#include "./NanoPrintf/nanoprintf.h"

extern int npf_pprintf(npf_putc pc,
                void * NPF_RESTRICT pc_ctx,
                char const * NPF_RESTRICT format,
                ...);
                
extern int npf_snprintf(char * NPF_RESTRICT buffer,
                 size_t bufsz,
                 const char * NPF_RESTRICT format,
                 ...);
                
extern int npf_vsnprintf(char * NPF_RESTRICT buffer,
                 size_t bufsz,
                 const char * NPF_RESTRICT format,
                 va_list val);
                 
///< PPRINTF需自定义输出策略
#define VASL_PPRINTF(pc, pc_ctx, format, ...) \
    npf_pprintf(pc, pc_ctx, format, __VA_ARGS__)
///< 格式化输出到指定缓冲区
#define VASL_SNPRINTF(buffer, bufsz, format, ...) \
    npf_snprintf(buffer, bufsz, format, __VA_ARGS__)
///< 格式化输出到指定缓冲区 可变参数版本
#define VASL_VSNPRINTF(buffer, bufsz, format, ...) \
    npf_vsnprintf(buffer, bufsz, format, __VA_ARGS__)

#endif ///< __VASL_VSNPRINTF_H__
