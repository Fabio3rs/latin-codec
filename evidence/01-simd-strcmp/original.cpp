#include <immintrin.h>
#include <iostream>
#include <string>
#include <string_view>


#ifdef _MSC_VER
// Define a replacement for __builtin_ctz using MSVC's _BitScanForward
static inline int __builtin_ctz(unsigned int x) {
    unsigned long index;
    _BitScanForward(&index, x);
    return (int)index;
}
#endif

template <class SimdType> struct simd {
    SimdType data;

    inline operator SimdType() const { return data; }

    inline simd<SimdType> operator|(const simd<SimdType> &other) const {
        return {_mm256_or_si256(data, other.data)};
    }

    inline simd<SimdType> operator+(const simd<SimdType> &other) const {
        simd<SimdType> result;
        result.data = _mm256_add_epi8(data, other.data);
        return result;
    }

    inline simd<SimdType> operator==(const simd<SimdType> &other) const {
        simd<SimdType> result;
        result.data = _mm256_cmpeq_epi8(data, other.data);
        return result;
    }

    inline simd<SimdType> operator!=(const simd<SimdType> &other) const {
        simd<SimdType> result;
        result.data = _mm256_xor_si256(data, other.data);
        return result;
    }

    inline simd<SimdType> operator-(const simd<SimdType> &other) const {
        simd<SimdType> result;
        result.data = _mm256_sub_epi8(data, other.data);
        return result;
    }

    inline int mask() const { return _mm256_movemask_epi8(data); }

    static inline simd<SimdType> setzero() {
        simd<SimdType> result;
        result.data = _mm256_setzero_si256();
        return result;
    }

    static inline simd<SimdType> load(const void *ptr) {
        return {_mm256_loadu_si256((const SimdType *)ptr)};
    }

    static inline void store(void *ptr, const SimdType &value) {
        _mm256_storeu_si256((SimdType *)ptr, value);
    }
};

template <class SimdType = __m256i>
static inline SimdType operator|(const simd<SimdType> &a, const SimdType &b) {
    return _mm256_or_si256(a.data, b);
}

struct ChunkenizerIterator {
    const void *ptrA;
    const void *ptrB;

    ChunkenizerIterator(const void *ptrA, const void *ptrB)
        : ptrA(ptrA), ptrB(ptrB) {}

    inline void operator++() {
        ptrA = (const char *)ptrA + 32;
        ptrB = (const char *)ptrB + 32;
    }

    inline bool operator!=(const ChunkenizerIterator &other) const {
        return ptrA != other.ptrA;
    }

    inline std::tuple<simd<__m256i>, simd<__m256i>> operator*() const {
        return {simd<__m256i>::load(ptrA), simd<__m256i>::load(ptrB)};
    }
};

struct Chunkenizer {
    const void *ptrA;
    const void *ptrB;

    Chunkenizer(const void *ptrA, const void *ptrB) : ptrA(ptrA), ptrB(ptrB) {}

    inline ChunkenizerIterator begin() const { return {ptrA, ptrB}; }

    inline ChunkenizerIterator end() const { return {nullptr, nullptr}; }
};

int simd_strcmp_chunkenizer(const char *p1, const char *p2) {
    const unsigned char *s1 = (const unsigned char *)p1;
    const unsigned char *s2 = (const unsigned char *)p2;

    auto vzero = simd<__m256i>::setzero();

    for (auto [v1, v2] : Chunkenizer(s1, s2)) {
        // Compute differences
        auto diff = v1 - v2;

        // Check for null terminators in either string
        auto v1_null = v1 == vzero;
        auto v2_null = v2 == vzero;

        // Check if any byte is non-zero (mismatch or null)
        int mask = (diff | (v1_null | v2_null)).mask();
        if (mask) {
            int index = __builtin_ctz(mask); // Find first mismatch or null
            return s1[index] - s2[index];
        }
    }

    return 0;
}


int simd_strcmp_sub(const char *p1, const char *p2) {
    const unsigned char *s1 = (const unsigned char *)p1;
    const unsigned char *s2 = (const unsigned char *)p2;

    __m256i vzero = _mm256_setzero_si256();

    while (1) {
        // Load 32 bytes from each string
        __m256i v1 = _mm256_loadu_si256((const __m256i *)s1);
        __m256i v2 = _mm256_loadu_si256((const __m256i *)s2);

        // Compute differences
        __m256i diff = _mm256_sub_epi8(v1, v2);

        // Check for null terminators in either string
        __m256i v1_null = _mm256_cmpeq_epi8(v1, vzero);
        __m256i v2_null = _mm256_cmpeq_epi8(v2, vzero);

        // Combine masks: null terminator or difference
        __m256i diff_or_null =
            _mm256_or_si256(diff, _mm256_or_si256(v1_null, v2_null));

        // Check if any byte is non-zero (mismatch or null)
        int mask = _mm256_movemask_epi8(diff_or_null);
        if (mask) {
            int index = __builtin_ctz(mask); // Find first mismatch or null
            return s1[index] - s2[index];
        }

        // Advance by 32 bytes
        s1 += 32;
        s2 += 32;
    }
}


int main() {
    std::string s1 = "Olá mundo!çççããããão";
    std::string s2 = "Olá mundo!çççããããão";
    int result = simd_strcmp_chunkenizer(s1.c_str(), s2.c_str());
    std::cout << "Result of simd_strcmp_chunkenizer: " << result << std::endl;
}

