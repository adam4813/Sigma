#ifndef WORLDPOSITION_SIMD_GCC_HPP_INCLUDED
#define WORLDPOSITION_SIMD_GCC_HPP_INCLUDED

#include "SIMD.hpp"
#include <utility>
#include <cmath>
#include "components/ArrayPool.hpp"

namespace Sigma {
    typedef double coordinate_type;
    typedef std::vector<coordinate_type, AlignedVectorAllocator<coordinate_type>> aligned_vector_type;
    typedef std::shared_ptr<float> coordinate_array;

    /** \brief kernel to build a bitmap
     */
    class build_bitmap_distance_view_kernel {
    public:
        enum {BLOCK_SIZE = 8};

        build_bitmap_distance_view_kernel(const coordinate_type aa, const coordinate_type * const xx,
                                        const coordinate_type distance_view, BitArray& bitmap) :
            a(aa), x(xx), distance(distance_view), bitmap(bitmap) {}

        inline void initialize() {
            asm volatile ("shufpd $0,   %%xmm0,     %%xmm0\n"
                          "shufpd $0,   %[max],     %[max]\n"
                          "shufpd $0,   %[mask],    %[mask]\n"
                          "movapd       %[max],     %%xmm8\n"
                          "movapd       %[mask],    %%xmm9\n"
                          "mov          %[src],     %%r10\n"
                          "mov          %[bitmap],  %%r13\n"
                          : : "Yz"(a), [max]"x"(distance), [mask]"x"(DABS_MASK), [src]"r"(x), [bitmap]"r"(bm)\
                          : "xmm8", "xmm9", "r10", "r11", "r13");
        }

        inline void scalar_op (const size_t i) const {
            // update vectorbit
            bitmap[i] &= (fabs(a + x[i]) < distance);
        }

        __attribute__((aligned(64), always_inline))
        inline void vector_op_sse2 (const size_t i) const {
            // load 16 data elements at a time
            asm volatile (
                        "prefetcht0 448(%%r10,%[index],8)\n"
                        "prefetcht0 512(%%r10,%[index],8)\n"
                        // load 8 elements from src array in registers
                        "movapd  48(%%r10,%[index],8), %%xmm1\n"
                        // add translation parameter
                        "addpd %%xmm0, %%xmm1\n"
                        // so on for 7 elements
                        "movapd  32(%%r10,%[index],8), %%xmm2\n"
                        "addpd %%xmm0, %%xmm2\n"
                        "movapd  16(%%r10,%[index],8), %%xmm3\n"
                        "addpd %%xmm0, %%xmm3\n"
                        "movapd  0(%%r10,%[index],8), %%xmm4\n"
                        "addpd %%xmm0, %%xmm4\n"
                        // calculate absolute value
                        "andpd          %%xmm9,     %%xmm1\n"
                        "andpd          %%xmm9,     %%xmm2\n"
                        "andpd          %%xmm9,     %%xmm3\n"
                        "andpd          %%xmm9,     %%xmm4\n"
                        // compare with FAR value
                        "cmppd $1,      %%xmm8,     %%xmm1\n"
                        "cmppd $1,      %%xmm8,     %%xmm2\n"
                        "cmppd $1,      %%xmm8,     %%xmm3\n"
                        "cmppd $1,      %%xmm8,     %%xmm4\n"
                        "movmskpd       %%xmm1,     %%r9\n"
                        "mov            %%r9,       %%r12\n"
                        "shl            $2,         %%r12\n"
                        "movmskpd       %%xmm2,     %%r9\n"
                        "or             %%r9,       %%r12\n"
                        "shl            $2,         %%r12\n"
                        "movmskpd       %%xmm3,     %%r9\n"
                        "or             %%r9,       %%r12\n"
                        "shl            $2,         %%r12\n"
                        "movmskpd       %%xmm4,     %%r9\n"
                        "or             %%r9,       %%r12\n"
                        "andb           %%r12b,     (%%r13)\n"
                        "add            $1,         %%r13\n"
                        :: [index]"r"(i) : "memory", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4", "%r9", "%r10", "%r12", "r13", "cc");
        }

    private:
        const coordinate_type a;
        const coordinate_type * const x;
        const coordinate_type distance;
        BitArray& bitmap;
        const unsigned short* bm = bitmap.data();
    };

    /** \brief kernel to perform y = (float) (x + a) for gcc
     */
    class dapx_kernel {
    public:
        enum {BLOCK_SIZE = 16};

        dapx_kernel(const coordinate_type aa, const coordinate_type * const xx, float *rr) : a(aa), x(xx), r(rr) {}

        inline void initialize() {
            asm volatile ("shufpd $0,   %%xmm0,     %%xmm0\n"
                          "mov          %[dst],     %%r11\n"
                          "mov          %[src],     %%r10\n"
                          : : [a]"Yz"(a), [dst]"r"(r), [src]"r"(x) : "r10", "r11");
        }

        inline void scalar_op (const size_t i) const {
            r[i] = (float) (a + x[i]);
        }

        __attribute__((aligned(64), always_inline))
        inline void vector_op_sse2 (const size_t i) const {
            // load 16 data elements at a time
            asm volatile (
                        "prefetcht0 640(%%r10,%[index],8)\n"
                        "prefetcht0 704(%%r10,%[index],8)\n"
                        "prefetchw 512(%%r11,%[index],4)\n"
                        // load 8 elements from src array in registers
                        "movapd  (%%r10,%[index],8), %%xmm1\n"
                        // add translation parameter
                        "addpd %%xmm0, %%xmm1\n"
                        // convert to float
                        "cvtpd2ps %%xmm1,    %%xmm1\n"
                        // so on for 7 elements
                        "movapd  16(%%r10,%[index],8), %%xmm2\n"
                        "addpd %%xmm0, %%xmm2\n"
                        "cvtpd2ps %%xmm2,    %%xmm2\n"
                        "movapd  32(%%r10,%[index],8), %%xmm3\n"
                        "addpd %%xmm0, %%xmm3\n"
                        "cvtpd2ps %%xmm3,    %%xmm3\n"
                        "movapd  48(%%r10,%[index],8), %%xmm4\n"
                        "addpd %%xmm0, %%xmm4\n"
                        "cvtpd2ps %%xmm4,    %%xmm4\n"
                        // pack result in xmm1 and xmm3
                        "movlhps %%xmm2, %%xmm1\n"
                        "movlhps %%xmm4, %%xmm3\n"

                        // again for 8 elements
                        "movapd  64(%%r10,%[index],8), %%xmm2\n"
                        "addpd %%xmm0, %%xmm2\n"
                        "cvtpd2ps %%xmm2,    %%xmm2\n"
                        "movapd  80(%%r10,%[index],8), %%xmm4\n"
                        "addpd %%xmm0, %%xmm4\n"
                        "cvtpd2ps %%xmm4,    %%xmm4\n"
                        "movapd  96(%%r10,%[index],8), %%xmm5\n"
                        "addpd %%xmm0, %%xmm5\n"
                        "cvtpd2ps %%xmm5,    %%xmm5\n"
                        "movapd  112(%%r10,%[index],8), %%xmm7\n"
                        "addpd %%xmm0, %%xmm7\n"
                        "cvtpd2ps %%xmm7,    %%xmm7\n"
                        "movlhps %%xmm4, %%xmm2\n"
                        "movlhps %%xmm7, %%xmm5\n"

                        // store the result in dst array
                        "movaps %%xmm1, (%%r11,%[index],4)\n"
                        "movaps %%xmm3, 16(%%r11,%[index],4)\n"
                        "movaps %%xmm2, 32(%%r11,%[index],4)\n"
                        "movaps %%xmm5, 48(%%r11,%[index],4)\n"
                        : : [index]"r"(i) : "memory", "xmm0", "xmm1", "xmm2", "xmm3", "xmm4",\
                                                        "xmm5", "xmm6", "xmm7", "%r10", "%r11", "cc");
        }

    private:
        const coordinate_type a;
        const coordinate_type * const x;
        float * const r;
    };

    struct Translate {

        __attribute__((target("sse2")))
        Translate() : functor(&Translate::RelativeTo_sse2), functor_with_distance_view(InViewPositions_sse2) {};

        __attribute__((target("default")))
        Translate() : functor(&Translate::RelativeTo_default), functor_with_distance_view(InViewPositions_default) {};


        virtual ~Translate() {};

        coordinate_array RelativeTo(const coordinate_type x, const aligned_vector_type& v, BitArray& bitmap) {
            return (this->*functor)(x, v, bitmap);
        }

        void InViewPositions(const coordinate_type x, const aligned_vector_type& v, const coordinate_type distance, BitArray& bitmap) const {
            return functor_with_distance_view(x, v, distance, bitmap);
        }

        void IncreaseLength() { arraypool.IncreaseLength(); };

        void DecreaseLength() { arraypool.DecreaseLength(); };

        __attribute__((hot))
        coordinate_array RelativeTo_sse2(const coordinate_type x, const aligned_vector_type& v, BitArray& bitmap) {
            size_t length = v.size();
            auto shared_dst = arraypool.GetArray(length);
            float* dst = shared_dst.get();
            transform_n_sse2(length, bitmap, dapx_kernel(-x, v.data(), dst));
            return shared_dst;
        };

        __attribute__((hot))
        static void InViewPositions_sse2(const coordinate_type x,
                                                              const aligned_vector_type& v,
                                                              const coordinate_type distance,
                                                              BitArray& bitmap) {
            size_t length = v.size();
            transform_n_sse2(length, build_bitmap_distance_view_kernel(-x, v.data(), distance, bitmap));
        };

        coordinate_array RelativeTo_default(const coordinate_type x, const aligned_vector_type& v, BitArray& bitmap) {
            size_t length = v.size();
            auto shared_dst = arraypool.GetArray(length);
            float* dst = shared_dst.get();
            transform_n_scalar(length, bitmap, dapx_kernel(-x, v.data(), dst));
            return shared_dst;
        };

        static void InViewPositions_default(const coordinate_type x, const aligned_vector_type& v,
                                                                     const coordinate_type distance,
                                                                     BitArray& bitmap) {
            size_t length = v.size();
            transform_n_scalar(length, build_bitmap_distance_view_kernel(-x, v.data(), distance, bitmap));
        };

    private:
        coordinate_array (Translate::*functor)(const coordinate_type, const aligned_vector_type&, BitArray&);
        const std::function<void(const coordinate_type, const aligned_vector_type&, const coordinate_type, BitArray&)> functor_with_distance_view;
        // A pool of allocated array
        ArrayPool<float> arraypool;
    };
}
#endif // WORLDPOSITION_SIMD_GCC_HPP_INCLUDED
