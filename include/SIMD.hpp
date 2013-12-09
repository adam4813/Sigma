#ifndef SIMD_HPP_INCLUDED
#define SIMD_HPP_INCLUDED

#include "components/BitArray.h"

// include for cpuid intrinsic
#if defined(__GNUG__)
#include <x86intrin.h>
#include "cpuid.h"

// redefine cpu_count to check that cpuid exists
static __inline int
__get_cpuid_count (unsigned int __level, unsigned int __count,
           unsigned int *__eax, unsigned int *__ebx,
           unsigned int *__ecx, unsigned int *__edx)
{
    unsigned int __ext = __level & 0x80000000;
    if (__get_cpuid_max (__ext, 0) < __level)
        return 0;

    __cpuid_count (__level, __count, *__eax, *__ebx, *__ecx, *__edx);
    return 1;
}

static inline bool
has_PREFETCH() {
    unsigned int eax = 0, ebx = 0, ecx = 0, edx = 0;
    if (__get_cpuid(0x80000001, &eax, &ebx, &ecx, &edx)) {
        return (ecx & (1 << 8)) || false;
    }
    return false;
}

template <class V>
// A deletor for aligned pointers
struct AlignedPointerDelete {
    void operator()(V* value) {
        _mm_free(value);
    }
} ;


#elif defined(_MSC_VER)
#include <intrin.h>
static inline bool
has_SSE2() {
    int CPUInfo[4] = {-1};
    __cpuid(CPUInfo, 0x1);
    return (CPUInfo[3] & (1 << 26)) || false;
}

static inline bool
has_PREFETCH() {
    int CPUInfo[4] = {-1};
    __cpuid(CPUInfo, 0x80000001);
    return (CPUInfo[2] & 0x100) || false;
}

template <class V>
// A deletor for aligned pointers
struct AlignedPointerDelete {
    void operator()(V* value) {
        _aligned_free(value);
    }
} ;
#else
template <class V>
// A deletor for pointers
struct AlignedPointerDelete {
    void operator()(V* value) {
        free(value);
    }
} ;

#endif


#define ROUND_DOWN(x, s) ((x) & ~((s)-1)) // rounds down x to a multiple of s (i.e. ROUND_DOWN(5, 4) becomes 4)
#define PREFETCH_T0(addr,nrOfBytesAhead) _mm_prefetch(((char *)(addr))+nrOfBytesAhead,_MM_HINT_T0)
#define PREFETCHW(addr,nrOfBytesAhead) _m_prefetchw(((char *)(addr))+nrOfBytesAhead)

    template<class T>
    inline T * AlignedMalloc(size_t s) {
#ifdef GCC
        T* p = static_cast<T*>(_mm_malloc(s, 16));
#elif defined VISUAL_STUDIO
        T* p = static_cast<T*>(_aligned_malloc(s, 16));
#else   // TODO : generic aligned malloc
        T* p = static_cast<T*>(malloc(s));
#endif
        return p;
    };

template<class Operation>
inline static void transform_n_sse2(size_t length, Operation op)
{
  size_t n;
  size_t end = ROUND_DOWN(length, op.BLOCK_SIZE);
  op.initialize();
  // execute op on array elements block-wise
  for(n = 0; n < end; n += op.BLOCK_SIZE)
    op.vector_op_sse2(n);
  // execute the remaining array elements one by one
  for(; n < length; n++) {
        op.scalar_op(n);
  }
}

template<class Operation>
inline static void transform_n_sse2(size_t length, Sigma::BitArray& bitmap, Operation op)
{
  size_t n;
  size_t end = ROUND_DOWN(length, op.BLOCK_SIZE);
  op.initialize();
  // execute op on array elements block-wise
  auto i = bitmap.iterator_word();
  for(n = i++; n < end; n = i++) {
    op.vector_op_sse2(n);
  }
  // execute the remaining array elements one by one
  for(n = end; n < length; n++) {
        op.scalar_op(n);
  }
}

template<class Operation>
inline static void transform_n_scalar(size_t length, Operation op)
{
  size_t n;
  // execute the array elements one by one
  for(n = 0; n < length; n++) op.scalar_op(n);
}

template<class Operation>
inline static void transform_n_scalar(size_t length, const Sigma::BitArray& bitmap, Operation op)
{
  size_t n;
  // execute the array elements one by one
  for(n = 0; n < length; n++) {
        if (bitmap[n]) {
            op.scalar_op(n);
        }
  }
}
#endif // SIMD_HPP_INCLUDED
