#ifndef __VASL_QUEUE_HPP__
#define __VASL_QUEUE_HPP__

#include "Vasl_Array.hpp"
#include "Vasl_Type.hpp"

namespace Vasl {
    /**
     * @tparam T 元素类型
     * @tparam N 队列容量
     */
    template <typename T, size_t N>
    class TypeQueue {
    private:
        Vasl::array<T, N> buf;
        size_t index_write;
        size_t index_read;
    public:
        TypeQueue() : index_write(0), index_read(0) {}
        
        constexpr size_t size() const { return N; }

        bool is_empty() const { return index_write == index_read; }

        bool is_not_empty() const { return index_write != index_read; }

        bool is_full() const {
            size_t next = index_write + 1;
            if (next == N) { next = 0; }
            return next == index_read;
        }

        size_t available() const {
            if (index_write >= index_read) { return (index_write - index_read); }
            return (N - index_read + index_write);
        }

        size_t get_used_space() const {
            return (index_write >= index_read) ? (index_write - index_read) : (N - index_read + index_write);
        }

        err_t push(const T& value) {
            if (is_full()) { return Vasl::NOT; }

            buf[index_write] = value;
            size_t next_write = index_write + 1;
            if (next_write == N) { next_write = 0; }
            index_write = next_write;
            return Vasl::OK;
        }

        err_t pop(T* p_value) {
            if (p_value == nullptr || is_empty()) { return Vasl::NOT; }

            *p_value = buf[index_read];
            if (N == ++index_read) { index_read = 0; }
            return Vasl::OK;
        }

        T pop(void) {
            if (is_empty()) { return 0; }

            T value = buf[index_read];
            if (N == ++index_read) { index_read = 0; }
            return value;
        }

        T* pop_ptr(void) {
            if (is_empty()) { return nullptr; }

            T* p_value = &buf[index_read];
            if (N == ++index_read) { index_read = 0; }
            return p_value;
        }

        size_t push(const T* data, size_t len) {
            size_t pushed = 0;
            for (size_t i = 0; i < len; ++i) {
                if (is_full()) break;
                buf[index_write] = data[i];
                size_t next_write = index_write + 1;
                if (next_write == N) { next_write = 0; }
                index_write = next_write;
                ++pushed;
            }
            return pushed;
        }

        size_t pop(T* data, size_t len) {
            if (data == nullptr) return 0;

            size_t popped = 0;
            for (size_t i = 0; i < len; ++i) {
                if (is_empty()) break;
                data[i] = buf[index_read];
                if (N == ++index_read) { index_read = 0; }
                ++popped;
            }
            return popped;
        }

        const T& peek(size_t offset = 0) const {
            size_t avail = available();
            if (offset >= avail) {
                // 返回 buf[0] 作为哨兵值（空队列时）
                return buf[0];
            }
            size_t idx = index_read + offset;
            if (idx >= N) idx -= N;
            return buf[idx];
        }

        const T& operator()(size_t offset = 0) const { return peek(offset); }

        T& operator[](size_t index) {
            size_t idx = index_read + index;
            if (idx >= N) idx -= N;
            return buf[idx];
        }

        void clear() { index_write = index_read = 0; }

        // 查找字符串，返回偏移量（未找到返回 N）
        size_t find_str(const char* pattern) const {
            if (pattern == nullptr || *pattern == 0) return N;

            size_t pattern_len = 0;
            while (pattern[pattern_len]) pattern_len++;

            size_t avail = get_used_space();
            if (pattern_len > avail) return N;

            for (size_t i = 0; i <= avail - pattern_len; ++i) {
                bool match = true;
                for (size_t j = 0; j < pattern_len; ++j) {
                    if (peek(i + j) != static_cast<uint8_t>(pattern[j])) {
                        match = false;
                        break;
                    }
                }
                if (match) return i;
            }

            return N;
        }

        // 查找字符串后，返回字符串后面的偏移量（未找到返回 N）
        size_t find_str_after(const char* pattern) const {
            size_t pos = find_str(pattern);
            if (pos == N) return N;

            size_t pattern_len = 0;
            while (pattern[pattern_len]) pattern_len++;

            return pos + pattern_len;
        }

        size_t get_index_read() const { return index_read; }

        size_t get_index_write() const { return index_write; }
    };
}

#endif ///< __VASL_QUEUE_HPP__
