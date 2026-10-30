#include "Vasl_Strxx.hpp"

size_t Vasl::str::len(const char* str) noexcept {
    size_t len = 0;
    while (str[len] != '\0') { len++; }
    return len;
}

size_t Vasl::str::len(char* str) noexcept {
    return Vasl::str::len(static_cast<const char*>(str));
}

size_t Vasl::str::len(const char* str, size_t max_len) noexcept {
    size_t len = 0;
    while (str[len] != '\0' && len < max_len) { len++; }
    return len;
}

size_t Vasl::str::len(char* str, size_t max_len) noexcept {
    return Vasl::str::len(static_cast<const char*>(str), max_len);
}

// ==== find_char: 查找单个字符 ====
const char* Vasl::str::find_char(const char* str, char c) noexcept {
    while (*str) {
        if (*str == c) return str;
        str++;
    }
    return nullptr;
}

const char* Vasl::str::find_char(const char* str, size_t max_len, char c) noexcept {
    size_t i = 0;
    while (str[i] && i < max_len) {
        if (str[i] == c) return str + i;
        i++;
    }
    return nullptr;
}

// ==== find_str: 查找子串 ====
const char* Vasl::str::find_str(const char* str, const char* pattern) noexcept {
    if (!str || !pattern || !*pattern) return str;

    while (*str) {
        const char* s = str;
        const char* p = pattern;

        while (*p && *s == *p) {
            s++;
            p++;
        }

        if (*p == '\0') return str;  // 完全匹配
        str++;
    }
    return nullptr;
}

const char* Vasl::str::find_str(const char* str, size_t max_len, const char* pattern) noexcept {
    if (!str || !pattern || !*pattern) return str;

    size_t s = 0;
    while (str[s] && s < max_len) {
        size_t p = 0;
        while (pattern[p] && (s + p < max_len) && str[s + p] == pattern[p]) {
            p++;
        }

        if (pattern[p] == '\0') return str + s;  // 完全匹配
        s++;
    }
    return nullptr;
}

// ==== find_char_after: 查找字符并返回其后位置 ====
const char* Vasl::str::find_char_after(const char* str, char c) noexcept {
    const char* p = find_char(str, c);
    return p ? (p + 1) : nullptr;
}

const char* Vasl::str::find_char_after(const char* str, size_t max_len, char c) noexcept {
    const char* p = find_char(str, max_len, c);
    return p ? (p + 1) : nullptr;
}

// ==== find_str_after: 查找子串并返回其后位置 ====
const char* Vasl::str::find_str_after(const char* str, const char* pattern) noexcept {
    const char* p = find_str(str, pattern);
    return p ? (p + len(pattern)) : nullptr;
}

const char* Vasl::str::find_str_after(const char* str, size_t max_len, const char* pattern) noexcept {
    const char* p = find_str(str, max_len, pattern);
    return p ? (p + len(pattern)) : nullptr;
}

// ==== 字符串转数值 ====
#include <stdlib.h>

int Vasl::str::atoi(const char* str) noexcept {
    return static_cast<int>(strtol(str, nullptr, 10));
}

int Vasl::str::atoi(const char* str, size_t max_len) noexcept {
    if (!str || max_len == 0) return 0;

    // 临时截取字符串
    char temp_buf[32] = {0};
    size_t copy_len = (max_len < 31) ? max_len : 31;
    for (size_t i = 0; i < copy_len; ++i) {
        if (str[i] == '\0') break;
        temp_buf[i] = str[i];
    }
    return static_cast<int>(strtol(temp_buf, nullptr, 10));
}

unsigned int Vasl::str::atou(const char* str) noexcept {
    return static_cast<unsigned int>(strtoul(str, nullptr, 10));
}

unsigned int Vasl::str::atou(const char* str, size_t max_len) noexcept {
    if (!str || max_len == 0) return 0;

    // 临时截取字符串
    char temp_buf[32] = {0};
    size_t copy_len = (max_len < 31) ? max_len : 31;
    for (size_t i = 0; i < copy_len; ++i) {
        if (str[i] == '\0') break;
        temp_buf[i] = str[i];
    }
    return static_cast<unsigned int>(strtoul(temp_buf, nullptr, 10));
}

int Vasl::str::atoi(const char* str, char end_char) noexcept {
    if (!str) return 0;

    // 复制直到遇到结束字符或字符串结束
    char temp_buf[32] = {0};
    size_t i = 0;
    while (i < 31 && str[i] != '\0' && str[i] != end_char) {
        temp_buf[i] = str[i];
        i++;
    }
    return static_cast<int>(strtol(temp_buf, nullptr, 10));
}

unsigned int Vasl::str::atou(const char* str, char end_char) noexcept {
    if (!str) return 0;

    // 复制直到遇到结束字符或字符串结束
    char temp_buf[32] = {0};
    size_t i = 0;
    while (i < 31 && str[i] != '\0' && str[i] != end_char) {
        temp_buf[i] = str[i];
        i++;
    }
    return static_cast<unsigned int>(strtoul(temp_buf, nullptr, 10));
}

// ==== 数值转字符串 ====
#include "./NanoPrintf/Vasl_Vsnprintf.h"

int Vasl::str::itostr(char* buf, size_t bufsz, int value) noexcept {
    return VASL_SNPRINTF(buf, bufsz, "%d", value);
}

int Vasl::str::utostr(char* buf, size_t bufsz, unsigned int value) noexcept {
    return VASL_SNPRINTF(buf, bufsz, "%u", value);
}

// ==== 格式化字符串 ====
int Vasl::str::format(char* buf, size_t bufsz, const char* format, ...) noexcept {
    va_list args;
    va_start(args, format);
    int len = VASL_VSNPRINTF(buf, bufsz, format, args);
    va_end(args);
    return len;
}

bool Vasl::str::cmp(const char* str, const char* pattern) {
    return std::strcmp(str, pattern) == 0;
}

bool Vasl::str::cmp(const char* str, size_t max_len, const char* pattern) noexcept {
    return std::strncmp(str, pattern, max_len) == 0;
}

void Vasl::str::copy(const char* dest, const char* str, int len) noexcept {
    memcpy(const_cast<char*>(dest), str, len);
    *(const_cast<char*>(dest) + len) = '\0';
}
