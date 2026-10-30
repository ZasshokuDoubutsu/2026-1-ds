/**
 * @file    Vasl_Array.hpp
 * @brief   std::array wrapper with volatile support
 * @author  Vasl
 * @note    Inherits std::array, adds volatile support, at() returns pair
 */

#ifndef __VASL_ARRAY_HPP__
#define __VASL_ARRAY_HPP__

#include <array>
#include <cstddef>
#include <utility>

namespace Vasl {

/**
 * @brief  std::array wrapper with volatile support
 * @tparam T   Element type (can include volatile)
 * @tparam N   Number of elements
 * @note  Inherits std::array, provides volatile access through partial specialization
 */
template<class T, std::size_t N>
struct array : public std::array<T, N> {
    using base_type = std::array<T, N>;
    using base_type::base_type;
    
    // Default constructor - performs zero initialization
    array() : base_type{} {}
    
    // Single-value constructor - fills array with given value
    // Allows initialization like: Vasl::array<uint8_t, 256> arr{0};
    array(const T& value) : base_type{} {
        this->fill(value);
    }
    
    // Inherits all std::array methods, including at() which returns reference
};

// ============================================================================
// Partial specialization for volatile T - provides volatile element access
// ============================================================================

template<class T, std::size_t N>
struct array<volatile T, N> : public std::array<T, N> {
    using base_type = std::array<T, N>;
    using base_type::base_type;
    
    // Default constructor - performs zero initialization
    array() : base_type{} {}
    
    // Single-value constructor - fills array with given value
    // Allows initialization like: Vasl::array<volatile uint8_t, 256> arr{0};
    array(const T& value) : base_type{} {
        this->fill(value);
    }
    
    // =========================================================================
    // Volatile element access methods
    // =========================================================================
    
    /**
     * @brief Access element at position - volatile
     */
    [[nodiscard]] volatile T& operator[](std::size_t idx) volatile noexcept
    { return base_type::operator[](idx); }

    /**
     * @brief Access element at position - const volatile
     */
    [[nodiscard]] const volatile T& operator[](std::size_t idx) const volatile noexcept
    { return base_type::operator[](idx); }

    /**
     * @brief Get data pointer - volatile
     */
    [[nodiscard]] volatile T* data() volatile noexcept
    { return base_type::data(); }

    /**
     * @brief Get data pointer - const volatile
     */
    [[nodiscard]] const volatile T* data() const volatile noexcept
    { return base_type::data(); }

    /**
     * @brief Access element with bounds checking - volatile
     * @note Returns volatile reference for hardware register access
     */
    [[nodiscard]] volatile T& at(std::size_t idx) volatile noexcept
    {
        if (idx < N) return base_type::operator[](idx);
        return base_type::operator[](0);
    }

    /**
     * @brief Access element with bounds checking - const volatile
     */
    [[nodiscard]] const volatile T& at(std::size_t idx) const volatile noexcept
    {
        if (idx < N) return base_type::operator[](idx);
        return base_type::operator[](0);
    }

    /**
     * @brief Get first element - volatile
     */
    [[nodiscard]] volatile T& front() volatile noexcept
    { return base_type::front(); }

    /**
     * @brief Get first element - const volatile
     */
    [[nodiscard]] const volatile T& front() const volatile noexcept
    { return base_type::front(); }

    /**
     * @brief Get last element - volatile
     */
    [[nodiscard]] volatile T& back() volatile noexcept
    { return base_type::back(); }

    /**
     * @brief Get last element - const volatile
     */
    [[nodiscard]] const volatile T& back() const volatile noexcept
    { return base_type::back(); }

    /**
     * @brief Fill with value - volatile
     */
    void fill(const volatile T& val) volatile noexcept
    { 
        for (std::size_t i = 0; i < N; ++i) {
            base_type::operator[](i) = val;
        }
    }
}; // 第119行结束第一个volatile特化

// ============================================================================
// Non-member functions
// ============================================================================

// swap
template<class T, std::size_t N>
constexpr void swap(array<T, N>& lhs, array<T, N>& rhs) noexcept
{ lhs.swap(rhs); }

// get<> for tuple-like access
template<std::size_t I, class T, std::size_t N>
[[nodiscard]] constexpr T& get(array<T, N>& arr) noexcept
{ return std::get<I>(arr); }

template<std::size_t I, class T, std::size_t N>
[[nodiscard]] constexpr const T& get(const array<T, N>& arr) noexcept
{ return std::get<I>(arr); }

} // namespace Vasl

#endif // __VASL_ARRAY_HPP__
