#ifndef __VASL_STRXX_HPP__
#define __VASL_STRXX_HPP__

#include <stddef.h>
#include <stdint.h>
#include <algorithm>
#include <cstring>

#include "../Struct/Vasl_Array.hpp"

namespace Vasl {
    namespace str {
        size_t len(const char* str) noexcept;
        size_t len(char* str) noexcept;
        size_t len(const char* str, size_t max_len) noexcept;
        size_t len(char* str, size_t max_len) noexcept;

        const char* find_char(const char* str, char c) noexcept;
        const char* find_char(const char* str, size_t max_len, char c) noexcept;
        const char* find_char_after(const char* str, char c) noexcept;
        const char* find_char_after(const char* str, size_t max_len, char c) noexcept;

        const char* find_str(const char* str, const char* pattern) noexcept;
        const char* find_str(const char* str, size_t max_len, const char* pattern) noexcept;
        const char* find_str_after(const char* str, const char* pattern) noexcept;
        const char* find_str_after(const char* str, size_t max_len, const char* pattern) noexcept;

        int atoi(const char* str) noexcept;
        int atoi(const char* str, size_t max_len) noexcept;
        int atoi(const char* str, char end_char) noexcept;
        unsigned int atou(const char* str) noexcept;
        unsigned int atou(const char* str, size_t max_len) noexcept;
        unsigned int atou(const char* str, char end_char) noexcept;

        int itostr(char* buf, size_t bufsz, int value) noexcept;
        int utostr(char* buf, size_t bufsz, unsigned int value) noexcept;
        int format(char* buf, size_t bufsz, const char* format, ...) noexcept;

        bool cmp(const char* str, const char* pattern);
        bool cmp(const char* str, size_t max_len, const char* pattern) noexcept;

        void copy(const char* dest, const char* str, int len) noexcept;
    }
}

namespace Vasl {
    namespace str {
        // 前向声明
        template<size_t N, typename T>
        class static_string;

        // 非成员函数声明
        template<size_t N, typename T = uint8_t>
        static_string<N, T> operator+(const static_string<N, T>& lhs, const static_string<N, T>& rhs);

        // ========================================================================
        // static_string 类定义
        // ========================================================================
        template<size_t N, typename T = uint8_t>
        class static_string {
        private:
            Vasl::array<T, N> data_;
            size_t length_;

        public:
            // ----------------------------------------------------------------
            // 构造函数
            // ----------------------------------------------------------------
            static_string() : data_(), length_(0) {
                // 确保空字符终止
                data_[0] = 0;
            }

            static_string(const T* str) : data_(), length_(0) {
                set(str);
            }

            static_string(const char* str) : data_(), length_(0) {
                if (!str) { return; }
                while (length_ < N && str[length_]) {
                    data_[length_] = static_cast<T>(str[length_]);
                    length_++;
                }
                data_[length_] = 0;  // 确保空字符终止
            }

            // ----------------------------------------------------------------
            // 基础访问方法
            // ----------------------------------------------------------------
            [[nodiscard]] size_t length() const noexcept { return length_; }
            [[nodiscard]] size_t size() const noexcept { return length_; }
            [[nodiscard]] size_t capacity() const noexcept { return N; }
            [[nodiscard]] bool empty() const noexcept { return length_ == 0; }

            // 访问元素（无边界检查）
            // [[nodiscard]] T& operator[](size_t idx) { return data_[idx]; }
            // [[nodiscard]] const T& operator[](size_t idx) const { return data_[idx]; }
            [[nodiscard]] const char& operator[](size_t idx) const { return reinterpret_cast<const char&>(data_[idx]); }

            // [[nodiscard]] T* data() { return data_.data(); }
            // [[nodiscard]] const T* data() const { return data_.data(); }
            [[nodiscard]] const char* data() const { return reinterpret_cast<const char*>(data_.data()); }

            // ----------------------------------------------------------------
            // 赋值操作
            // ----------------------------------------------------------------
            void clear() {
                length_ = 0;
                data_[0] = 0;
            }

            void set(const char* str) {
                clear();
                if (!str) { return; }
                while (length_ < N && str[length_]) {
                    data_[length_] = static_cast<T>(str[length_]);
                    length_++;
                }
                data_[length_] = 0;
            }

            void set(const T* str) {
                clear();
                if (!str) { return; }
                while (length_ < N && str[length_]) {
                    data_[length_] = str[length_];
                    length_++;
                }
                data_[length_] = 0;
            }

            // 赋值运算符
            static_string& operator=(const static_string& other) {
                if (this != &other) {
                    length_ = std::min(other.length_, N);
                    for (size_t i = 0; i < length_; ++i) {
                        data_[i] = other.data_[i];
                    }
                    data_[length_] = 0;
                }
                return *this;
            }

            static_string& operator=(const char* str) {
                set(str);
                return *this;
            }

            static_string& operator=(const T* str) {
                set(str);
                return *this;
            }

            // ----------------------------------------------------------------
            // 比较操作
            // ----------------------------------------------------------------
            // 与 static_string 比较
            [[nodiscard]] bool operator==(const static_string& other) const {
                if (length_ != other.length_) return false;
                return std::memcmp(data_.data(), other.data_.data(), length_) == 0;
            }

            [[nodiscard]] bool operator!=(const static_string& other) const {
                return !(*this == other);
            }

            [[nodiscard]] bool operator<(const static_string& other) const {
                size_t min_len = std::min(length_, other.length_);
                int cmp = std::memcmp(data_.data(), other.data_.data(), min_len);
                if (cmp != 0) return cmp < 0;
                return length_ < other.length_;
            }

            [[nodiscard]] bool operator>(const static_string& other) const {
                return other < *this;
            }

            [[nodiscard]] bool operator<=(const static_string& other) const {
                return !(other < *this);
            }

            [[nodiscard]] bool operator>=(const static_string& other) const {
                return !(*this < other);
            }

            // 与 const char* 比较
            [[nodiscard]] bool operator==(const char* other) const {
                if (!other) return length_ == 0;
                size_t other_len = 0;
                while (other[other_len]) other_len++;
                if (length_ != other_len) return false;
                return std::memcmp(data_.data(), other, length_) == 0;
            }

            [[nodiscard]] bool operator!=(const char* other) const {
                return !(*this == other);
            }

            [[nodiscard]] bool operator<(const char* other) const {
                if (!other) return false;
                size_t min_len = std::min(length_, strlen(other));
                int cmp = std::memcmp(data_.data(), other, min_len);
                if (cmp != 0) return cmp < 0;
                return length_ < strlen(other);
            }

            [[nodiscard]] bool operator>(const char* other) const {
                if (!other) return length_ > 0;
                size_t other_len = strlen(other);
                size_t min_len = std::min(length_, other_len);
                int cmp = std::memcmp(data_.data(), other, min_len);
                if (cmp != 0) return cmp > 0;
                return length_ > other_len;
            }

            [[nodiscard]] bool operator<=(const char* other) const {
                return !(*this > other);
            }

            [[nodiscard]] bool operator>=(const char* other) const {
                return !(*this < other);
            }

            // ----------------------------------------------------------------
            // 追加操作
            // ----------------------------------------------------------------
            bool append(const char* str) {
                if (!str) return true;
                size_t i = 0;
                while (str[i] && length_ + i < N) {
                    data_[length_ + i] = static_cast<T>(str[i]);
                    i++;
                }
                length_ += i;
                data_[length_] = 0;
                return !str[i];  // 返回 true 表示完全复制
            }

            bool append(const T* str) {
                if (!str) return true;
                size_t i = 0;
                while (str[i] && length_ + i < N) {
                    data_[length_ + i] = str[i];
                    i++;
                }
                length_ += i;
                data_[length_] = 0;
                return !str[i];
            }

            bool append(const static_string& other) {
                return append(other.data_.data());
            }

            // += 运算符
            static_string& operator+=(const char* str) {
                append(str);
                return *this;
            }

            static_string& operator+=(const T* str) {
                append(str);
                return *this;
            }

            static_string& operator+=(const static_string& other) {
                append(other);
                return *this;
            }

            static_string& operator+=(char c) {
                if (length_ < N) {
                    data_[length_] = static_cast<T>(c);
                    length_++;
                    data_[length_] = 0;
                }
                return *this;
            }

            // ----------------------------------------------------------------
            // 拼接操作 (operator+)
            // ----------------------------------------------------------------
            friend static_string<N, T> operator+<N, T>(const static_string<N, T>& lhs, const static_string<N, T>& rhs);

            // ----------------------------------------------------------------
            // 查找操作
            // ----------------------------------------------------------------
            // 查找字符 - 返回位置，未找到返回 length_
            [[nodiscard]] size_t find(char c, size_t pos = 0) const {
                for (size_t i = pos; i < length_; ++i) {
                    if (data_[i] == static_cast<T>(c)) return i;
                }
                return length_;
            }

            // 查找子串 - 返回位置，未找到返回 length_
            [[nodiscard]] size_t find(const char* pattern, size_t pos = 0) const {
                if (!pattern || !*pattern) return pos;
                size_t pattern_len = 0;
                while (pattern[pattern_len]) pattern_len++;
                if (pattern_len > length_) return length_;

                for (size_t i = pos; i <= length_ - pattern_len; ++i) {
                    bool match = true;
                    for (size_t j = 0; j < pattern_len; ++j) {
                        if (data_[i + j] != static_cast<T>(pattern[j])) {
                            match = false;
                            break;
                        }
                    }
                    if (match) return i;
                }
                return length_;
            }

            // 查找 static_string 子串
            [[nodiscard]] size_t find(const static_string& pattern, size_t pos = 0) const {
                return find(reinterpret_cast<const char*>(pattern.data_.data()), pos);
            }

            // 检查是否包含子串/字符
            [[nodiscard]] bool contains(char c) const {
                return find(c) < length_;
            }

            [[nodiscard]] bool contains(const char* pattern) const {
                return find(pattern) < length_;
            }

            [[nodiscard]] bool contains(const static_string& pattern) const {
                return find(pattern) < length_;
            }

            // 检查是否以指定字符串开头/结尾
            [[nodiscard]] bool starts_with(const char* prefix) const {
                if (!prefix) return true;
                size_t prefix_len = 0;
                while (prefix[prefix_len]) prefix_len++;
                if (prefix_len > length_) return false;
                return std::memcmp(data_.data(), prefix, prefix_len) == 0;
            }

            [[nodiscard]] bool starts_with(const static_string& prefix) const {
                return starts_with(reinterpret_cast<const char*>(prefix.data_.data()));
            }

            [[nodiscard]] bool ends_with(const char* suffix) const {
                if (!suffix) return true;
                size_t suffix_len = 0;
                while (suffix[suffix_len]) suffix_len++;
                if (suffix_len > length_) return false;
                return std::memcmp(data_.data() + length_ - suffix_len, suffix, suffix_len) == 0;
            }

            [[nodiscard]] bool ends_with(const static_string& suffix) const {
                return ends_with(reinterpret_cast<const char*>(suffix.data_.data()));
            }

            // ----------------------------------------------------------------
            // 数值转换
            // ----------------------------------------------------------------
            // 字符串转数值 - 使用现有的 Vasl::str::atoi/atou
            [[nodiscard]] int to_int() const {
                return Vasl::str::atoi(reinterpret_cast<const char*>(data_.data()));
            }

            [[nodiscard]] unsigned int to_uint() const {
                return Vasl::str::atou(reinterpret_cast<const char*>(data_.data()));
            }

            // 数值转字符串 - 使用现有的 Vasl::str::itostr/utostr
            bool from_int(int value) {
                char buf[32];
                Vasl::str::itostr(buf, sizeof(buf), value);
                *this += buf;
                return true;
            }

            bool from_uint(unsigned int value) {
                char buf[32];
                Vasl::str::utostr(buf, sizeof(buf), value);
                *this += buf;
                return true;
            }

            // ----------------------------------------------------------------
            // 擦除操作
            // ----------------------------------------------------------------
            // 删除从 pos 开始的 len 个字符
            bool erase(size_t pos, size_t len = 1) {
                if (pos >= length_) return false;
                if (len == 0) return true;

                size_t remove_end = std::min(pos + len, length_);
                size_t move_count = length_ - remove_end;

                // 移动后面的字符
                for (size_t i = 0; i < move_count; ++i) {
                    data_[pos + i] = data_[remove_end + i];
                }
                length_ -= (remove_end - pos);
                data_[length_] = 0;
                return true;
            }

            // 删除指定位置的字符
            bool remove_at(size_t pos) {
                return erase(pos, 1);
            }

            // 删除第一个匹配字符
            bool remove(char c) {
                size_t pos = find(c);
                if (pos < length_) {
                    return erase(pos, 1);
                }
                return false;
            }

            // 删除所有匹配字符
            size_t remove_all(char c) {
                size_t count = 0;
                size_t i = 0;
                while (i < length_) {
                    if (data_[i] == static_cast<T>(c)) {
                        erase(i, 1);
                        count++;
                    } else {
                        i++;
                    }
                }
                return count;
            }

            // ----------------------------------------------------------------
            // 替换操作 (简化版)
            // ----------------------------------------------------------------
            // 替换第一个匹配
            bool replace(char old_c, char new_c) {
                size_t pos = find(old_c);
                if (pos < length_) {
                    data_[pos] = static_cast<T>(new_c);
                    return true;
                }
                return false;
            }

            // 替换所有匹配
            size_t replace_all(char old_c, char new_c) {
                size_t count = 0;
                for (size_t i = 0; i < length_; ++i) {
                    if (data_[i] == static_cast<T>(old_c)) {
                        data_[i] = static_cast<T>(new_c);
                        count++;
                    }
                }
                return count;
            }

            // ----------------------------------------------------------------
            // 子串操作
            // ----------------------------------------------------------------
            // 提取子串 (注意：返回 static_string 需要额外的模板参数)
            // 这里提供一个简化版本：复制子串到目标
            bool substr(static_string& dest, size_t pos, size_t len) const {
                if (pos >= length_) {
                    dest.clear();
                    return false;
                }
                size_t copy_len = std::min(len, length_ - pos);
                dest.clear();
                for (size_t i = 0; i < copy_len; ++i) {
                    dest.data_[i] = data_[pos + i];
                }
                dest.length_ = copy_len;
                dest.data_[copy_len] = 0;
                return true;
            }

            // ----------------------------------------------------------------
            // JSON解析 - 基础版（简单key:value查找）
            // ----------------------------------------------------------------
            // 查找 "key":value 中的value部分
            bool get_json_value(const char* key, static_string& value) const {
                if (!key) return false;

                // 查找 key 的位置（带引号）
                size_t key_pos = find(key);
                if (key_pos >= length_) return false;

                // 检查key前面是否有引号
                size_t start_pos = key_pos;
                if (key_pos > 0 && data_[key_pos - 1] == static_cast<T>('"')) {
                    start_pos--;
                }

                // 从key位置往后找 ":"
                size_t colon_pos = start_pos + std::strlen(key);
                if (colon_pos >= length_) return false;

                while (colon_pos < length_ && data_[colon_pos] != static_cast<T>(':')) {
                    colon_pos++;
                    if (colon_pos >= length_) return false;
                }

                // 跳过冒号和可能的空格
                colon_pos++;
                while (colon_pos < length_ && data_[colon_pos] == static_cast<T>(' ')) {
                    colon_pos++;
                }

                if (colon_pos >= length_) return false;

                // 判断value类型
                if (data_[colon_pos] == static_cast<T>('"')) {
                    // 字符串类型：找到下一个引号
                    colon_pos++;  // 跳过开始的引号
                    size_t end_pos = colon_pos;
                    while (end_pos < length_ && data_[end_pos] != static_cast<T>('"')) {
                        end_pos++;
                    }
                    if (end_pos >= length_) return false;
                    substr(value, colon_pos, end_pos - colon_pos);
                } else if (data_[colon_pos] == static_cast<T>('t') || data_[colon_pos] == static_cast<T>('f')) {
                    // 布尔类型：true 或 false
                    size_t end_pos = colon_pos;
                    while (end_pos < length_ &&
                           (data_[end_pos] == static_cast<T>('t') ||
                            data_[end_pos] == static_cast<T>('r') ||
                            data_[end_pos] == static_cast<T>('u') ||
                            data_[end_pos] == static_cast<T>('e') ||
                            data_[end_pos] == static_cast<T>('f') ||
                            data_[end_pos] == static_cast<T>('a') ||
                            data_[end_pos] == static_cast<T>('l') ||
                            data_[end_pos] == static_cast<T>('s'))) {
                        end_pos++;
                    }
                    substr(value, colon_pos, end_pos - colon_pos);
                } else {
                    // 数字类型：读取直到逗号或结束符
                    size_t end_pos = colon_pos;
                    while (end_pos < length_ &&
                           data_[end_pos] != static_cast<T>(',') &&
                           data_[end_pos] != static_cast<T>('}') &&
                           data_[end_pos] != static_cast<T>(']')) {
                        end_pos++;
                    }
                    substr(value, colon_pos, end_pos - colon_pos);
                }

                return true;
            }

            // 便捷方法：获取整数值
            bool get_json_int(const char* key, int& value) const {
                static_string<32> str_value;
                if (!get_json_value(key, str_value)) return false;
                value = str_value.to_int();
                return true;
            }

            // 便捷方法：获取无符号整数值
            bool get_json_uint(const char* key, unsigned int& value) const {
                static_string<32> str_value;
                if (!get_json_value(key, str_value)) return false;
                value = str_value.to_uint();
                return true;
            }

            // 便捷方法：获取布尔值
            bool get_json_bool(const char* key, bool& value) const {
                static_string<8> str_value;
                if (!get_json_value(key, str_value)) return false;
                value = (str_value == "true");
                return true;
            }

            // 便捷方法：获取字符串值
            bool get_json_str(const char* key, static_string& value) const {
                return get_json_value(key, value);
            }
        };

        // ========================================================================
        // 非成员函数实现
        // ========================================================================

        // operator+ 实现
        template<size_t N, typename T>
        static_string<N, T> operator+(const static_string<N, T>& lhs, const static_string<N, T>& rhs) {
            static_string<N, T> result;
            result.append(lhs);
            result.append(rhs);
            return result;
        }

        // const char* 与 static_string 比较
        template<size_t N, typename T>
        [[nodiscard]] bool operator==(const char* lhs, const static_string<N, T>& rhs) {
            return rhs == lhs;
        }

        template<size_t N, typename T>
        [[nodiscard]] bool operator!=(const char* lhs, const static_string<N, T>& rhs) {
            return rhs != lhs;
        }
    }
}

#endif ///< __VASL_STRXX_HPP__
