#ifndef SIMD_STRCMP_H
#define SIMD_STRCMP_H

#include <immintrin.h>
#include <cstdint>
#include <tuple>

#ifdef _MSC_VER
// Define a replacement for __builtin_ctz using MSVC's _BitScanForward
static inline int __builtin_ctz(unsigned int x) noexcept {
    unsigned long index;
    _BitScanForward(&index, x);
    return (int)index;
}
#endif

template <class SimdType>
struct simd {
    SimdType data;

    inline operator SimdType() const noexcept { return data; }

    inline simd<SimdType> operator|(const simd<SimdType>& other) const noexcept {
        return {_mm256_or_si256(data, other.data)};
    }

    inline simd<SimdType> operator+(const simd<SimdType>& other) const noexcept {
        simd<SimdType> result;
        result.data = _mm256_add_epi8(data, other.data);
        return result;
    }

    inline simd<SimdType> operator==(const simd<SimdType>& other) const noexcept {
        simd<SimdType> result;
        result.data = _mm256_cmpeq_epi8(data, other.data);
        return result;
    }

    inline simd<SimdType> operator!=(const simd<SimdType>& other) const noexcept {
        simd<SimdType> result;
        result.data = _mm256_xor_si256(data, other.data);
        return result;
    }

    inline simd<SimdType> operator-(const simd<SimdType>& other) const noexcept {
        simd<SimdType> result;
        result.data = _mm256_sub_epi8(data, other.data);
        return result;
    }

    inline int mask() const noexcept { return _mm256_movemask_epi8(data); }

    static inline simd<SimdType> setzero() noexcept {
        simd<SimdType> result;
        result.data = _mm256_setzero_si256();
        return result;
    }

    static inline simd<SimdType> load(const void* ptr) noexcept {
        return {_mm256_loadu_si256((const SimdType*)ptr)};
    }

    static inline void store(void* ptr, const SimdType& value) noexcept {
        _mm256_storeu_si256((SimdType*)ptr, value);
    }
};

template <class SimdType = __m256i>
static inline SimdType operator|(const simd<SimdType>& a, const SimdType& b) noexcept {
    return _mm256_or_si256(a.data, b);
}

struct ChunkenizerIterator {
    const void* ptrA;
    const void* ptrB;

    ChunkenizerIterator(const void* ptrA, const void* ptrB) noexcept
        : ptrA(ptrA), ptrB(ptrB) {}

    inline void operator++() noexcept {
        ptrA = (const char*)ptrA + 32;
        ptrB = (const char*)ptrB + 32;
    }

    inline bool operator!=(const ChunkenizerIterator& other) const noexcept {
        return ptrA != other.ptrA;
    }

    inline std::tuple<simd<__m256i>, simd<__m256i>> operator*() const noexcept {
        return {simd<__m256i>::load(ptrA), simd<__m256i>::load(ptrB)};
    }
};

struct Chunkenizer {
    const void* ptrA;
    const void* ptrB;

    Chunkenizer(const void* ptrA, const void* ptrB) noexcept
        : ptrA(ptrA), ptrB(ptrB) {}

    inline ChunkenizerIterator begin() const noexcept { return {ptrA, ptrB}; }

    inline ChunkenizerIterator end() const noexcept { return {nullptr, nullptr}; }
};

int simd_strcmp_chunkenizer(const char* p1, const char* p2) noexcept;

int simd_strcmp_sub(const char* p1, const char* p2) noexcept;

#endif // SIMD_STRCMP_H

