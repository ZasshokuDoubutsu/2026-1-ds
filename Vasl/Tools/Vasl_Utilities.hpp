
    
//     /* 字符串复制 */
//     inline void String_Copy(char* dest, const char* src, size_t max_len) {
//         if (max_len > 0) {
//             std::strncpy(dest, src, max_len - 1);
//             dest[max_len - 1] = '\0';
//         }
//     }
    
//     inline void String_CopyUnsafe(char* dest, const char* src) {
//         std::strcpy(dest, src);
//     }
    
//     /* 字符串连接 */
//     inline void String_Concat(char* dest, const char* src, size_t max_len) {
//         size_t len = std::strlen(dest);
//         if (len < max_len - 1) {
            // std::strncat(dest, src, max_len - len - 1);
//         }
//     }
    
//     inline void String_ConcatUnsafe(char* dest, const char* src) {
//         std::strcat(dest, src);
//     }
    
//     /* 字符串查找 */
//     inline char* String_Find(const char* str, const char* substr) {
//         return const_cast<char*>(std::strstr(str, substr));
//     }
    
//     inline bool String_StartsWith(const char* str, const char* prefix) {
//         return std::strncmp(str, prefix, std::strlen(prefix)) == 0;
//     }
    
//     inline bool String_EndsWith(const char* str, const char* suffix) {
//         size_t str_len = std::strlen(str);
//         size_t suffix_len = std::strlen(suffix);
//         return (str_len >= suffix_len) && 
//                (std::strcmp(str + str_len - suffix_len, suffix) == 0);
//     }
    
//     /* 串口数据解析相关内联函数 */
//     inline bool String_IsDigit(char c) {
//         return (c >= '0' && c <= '9');
//     }
    
//     inline bool String_IsHexDigit(char c) {
//         return ((c >= '0' && c <= '9') || 
//                 (c >= 'a' && c <= 'f') || 
//                 (c >= 'A' && c <= 'F'));
//     }
    
//     inline char String_ToLower(char c) {
//         return ((c >= 'A' && c <= 'Z') ? (c + 32) : c);
//     }

//     /* 内存拷贝 */
//     inline void VarCopy(void* dest, const void* src, size_t size) {
//         std::memcpy(dest, src, size);
//     }
    
//     template<typename T>
//     inline void VarCopyStruct(T* dest, const T& src) {
//         std::memcpy(dest, &src, sizeof(T)); // 拷贝整个结构体/变量
//     }
    
//     /* 内存设置（常用于清零或初始化） */
//     inline void SetMem(void* dest, int value, size_t size) {
//         std::memset(dest, value, size);
//     }
    
//     inline void ZeroMem(void* dest, size_t size) {
//         std::memset(dest, 0, size); // 清零内存块
//     }
    
//     template<typename T>
//     inline void ZeroStruct(T* dest) {
//         std::memset(dest, 0, sizeof(*dest)); // 清零结构体/变量
//     }
    
//     /* 内存移动（适用于源和目标内存区域可能重叠的情况） */
//     inline void MoveMem(void* dest, const void* src, size_t size) {
//         std::memmove(dest, src, size);
//     }
    
//     /* 内存比较 */
//     inline int CmpMem(const void* buf1, const void* buf2, size_t size) {
//         return std::memcmp(buf1, buf2, size);
//     }
// }
// #pragma endregion /* 字符与内存相关 */

// NOTE("C版本")
// #if VASL_LOCK
// #include "./SEGGER/InLibVasl_SEGGER4C.hpp"
// __asm void HardFault_Handler(void) {
//     IMPORT HardFault_Handler_Vasl
//     TST    lr, #4
//     ITT    EQ
//     MRSEQ  r0, MSP
//     MRSNE  r0, PSP
//     B      HardFault_Handler_Vasl
// }

// void HardFault_Handler_Vasl(uint32_t *hardfault_args) {
//         uint32_t stacked_r0  = hardfault_args[0];
//         uint32_t stacked_r1  = hardfault_args[1];
//         uint32_t stacked_r2  = hardfault_args[2];
//         uint32_t stacked_r3  = hardfault_args[3];
//         uint32_t stacked_r12 = hardfault_args[4];
//         uint32_t stacked_lr  = hardfault_args[5];
//         uint32_t stacked_pc  = hardfault_args[6];
//         uint32_t stacked_psr = hardfault_args[7];
//         Vasl_errlog("\r\n\t->HardFault!\r\nR0 = 0x%08X\r\nR1 = 0x%08X\r\nR2 = 0x%08X\r\nR3 = 0x%08X\r\nR12= 0x%08X\r\nLR = 0x%08X\r\nPC = 0x%08X\r\nPSR= 0x%08X\r\n",
//             stacked_r0, stacked_r1, stacked_r2, stacked_r3,
//             stacked_r12, stacked_lr, stacked_pc, stacked_psr);
// }
// #endif /* HLOCK */


// #endif
