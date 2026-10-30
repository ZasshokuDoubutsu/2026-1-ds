#ifndef __VASL_RINGBUF_HPP__
#define __VASL_RINGBUF_HPP__

#include "Vasl_Queue.hpp"

namespace Vasl {
    template <size_t N>
    using TypeRingBufU8 = TypeQueue<uint8_t, N>;
    template <size_t N>
    using TypeRingBufU16 = TypeQueue<uint16_t, N>;
    template <size_t N>
    using TypeRingBufU32 = TypeQueue<uint32_t, N>;
    template <size_t N>
    using TypeRingBufI8 = TypeQueue<int8_t, N>;
    template <size_t N>
    using TypeRingBufI16 = TypeQueue<int16_t, N>;
    template <size_t N>
    using TypeRingBufI32 = TypeQueue<int32_t, N>;
}

#endif ///< __VASL_RINGBUF_HPP__
