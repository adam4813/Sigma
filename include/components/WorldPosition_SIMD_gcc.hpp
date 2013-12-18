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
                                        const coordinate_type distance_view, BitArray<unsigned short>& bitmap) :
            a(aa), x(xx), distance(distance_view), bitmap(bitmap) {}

        inline void scalar_op (const size_t i) const {
            // update vectorbit
            bitmap[i] &= (fabs(a + x[i]) < distance);
        }

        __attribute__((aligned(64), always_inline))
        inline void vector_op_sse2 (const size_t i) const {
            // load 16 data elements at a time
            asm volatile (
                        "shufpd $0,   %%xmm0,     %%xmm0\n"
                        "shufpd $0,   %[max],     %[max]\n"
                        "shufpd $0,   %[mask],    %[mask]\n"
                        "jmp          1f\n"
                    "0:\n"
                        "prefetcht0 448(%[src],%[cnt],8)\n"
                        "prefetcht0 512(%[src],%[cnt],8)\n"
                        // load 8 elements from src array in registers
                        "movapd  48(%[src],%[cnt],8), %%xmm1\n"
                        // add translation parameter
                        "addpd %%xmm0, %%xmm1\n"
                        // so on for 7 elements
                        "movapd  32(%[src],%[cnt],8), %%xmm2\n"
                        "addpd %%xmm0, %%xmm2\n"
                        "movapd  16(%[src],%[cnt],8), %%xmm3\n"
                        "addpd %%xmm0, %%xmm3\n"
                        "movapd  0(%[src],%[cnt],8), %%xmm4\n"
                        "addpd %%xmm0, %%xmm4\n"
                        // calculate absolute value
                        "andpd          %[mask],     %%xmm1\n"
                        "andpd          %[mask],     %%xmm2\n"
                        "andpd          %[mask],     %%xmm3\n"
                        "andpd          %[mask],     %%xmm4\n"
                        // compare with FAR value
                        "cmppd $1,      %[max],     %%xmm1\n"
                        "cmppd $1,      %[max],     %%xmm2\n"
                        "cmppd $1,      %[max],     %%xmm3\n"
                        "cmppd $1,      %[max],     %%xmm4\n"
                        "movmskpd       %%xmm1,     %%ebx\n"
                        "mov            %%ebx,      %%eax\n"
                        "shl            $2,         %%eax\n"
                        "movmskpd       %%xmm2,     %%ebx\n"
                        "or             %%ebx,      %%eax\n"
                        "shl            $2,         %%eax\n"
                        "movmskpd       %%xmm3,     %%ebx\n"
                        "or             %%ebx,      %%eax\n"
                        "shl            $2,         %%eax\n"
                        "movmskpd       %%xmm4,     %%ebx\n"
                        "or             %%ebx,      %%eax\n"
                        "andb           %%al,       (%[bitmap])\n"
                        "add            $1,         %[bitmap]\n"
                        "add            $8,         %[cnt]\n"
                    "1:\n"
                        "cmp            %[cnt],      %[length]\n"
                        "ja             0b\n"
                        :: [length]"d"(i), "Yz"(a), [max]"x"(distance), [mask]"x"(DABS_MASK),\
                           [src]"S"(x), [bitmap]"D"(bm), [cnt]"c"(0L)\
                        : "memory", "xmm1", "xmm2", "xmm3", "xmm4", "%ebx", "%eax", "cc");
        }

    private:
        const coordinate_type a;
        const coordinate_type * const x;
        const coordinate_type distance;
        BitArray<unsigned short>& bitmap;
        const unsigned short* bm = bitmap.data();
    };

    /** \brief kernel to perform y = (float) (x + a) for gcc
     */
    class dapx_kernel {
    public:
        enum {BLOCK_SIZE = 16};

        dapx_kernel(const coordinate_type aa, const coordinate_type * const xx, float *rr)
            : a(aa), x(xx), r(rr) {}

        inline void scalar_op (const size_t i) const {
            r[i] = (float) (a + x[i]);
        }

        __attribute__((aligned(64), always_inline))
        inline void vector_op_sse2 (const size_t i) const {
            // load 16 data elements at a time
            asm volatile (
                        "shufpd $0,   %%xmm0,     %%xmm0\n"
                        "jmp    1f\n"
                    "0:\n"
                        "prefetcht0 640(%[src],%[cnt],8)\n"
                        "prefetcht0 704(%[src],%[cnt],8)\n"
                        "prefetchw 512(%[dst],%[cnt],4)\n"
                        // load 8 elements from src array in registers
                        "movapd  (%[src],%[cnt],8), %%xmm1\n"
                        // add translation parameter
                        "addpd %%xmm0, %%xmm1\n"
                        // convert to float
                        "cvtpd2ps %%xmm1,    %%xmm1\n"
                        // so on for 7 elements
                        "movapd  16(%[src],%[cnt],8), %%xmm2\n"
                        "addpd %%xmm0, %%xmm2\n"
                        "cvtpd2ps %%xmm2,    %%xmm2\n"
                        "movapd  32(%[src],%[cnt],8), %%xmm3\n"
                        "addpd %%xmm0, %%xmm3\n"
                        "cvtpd2ps %%xmm3,    %%xmm3\n"
                        "movapd  48(%[src],%[cnt],8), %%xmm4\n"
                        "addpd %%xmm0, %%xmm4\n"
                        "cvtpd2ps %%xmm4,    %%xmm4\n"
                        // pack result in xmm1 and xmm3
                        "movlhps %%xmm2, %%xmm1\n"
                        "movlhps %%xmm4, %%xmm3\n"

                        // again for 8 elements
                        "movapd  64(%[src],%[cnt],8), %%xmm2\n"
                        "addpd %%xmm0, %%xmm2\n"
                        "cvtpd2ps %%xmm2,    %%xmm2\n"
                        "movapd  80(%[src],%[cnt],8), %%xmm4\n"
                        "addpd %%xmm0, %%xmm4\n"
                        "cvtpd2ps %%xmm4,    %%xmm4\n"
                        "movapd  96(%[src],%[cnt],8), %%xmm5\n"
                        "addpd %%xmm0, %%xmm5\n"
                        "cvtpd2ps %%xmm5,    %%xmm5\n"
                        "movapd  112(%[src],%[cnt],8), %%xmm7\n"
                        "addpd %%xmm0, %%xmm7\n"
                        "cvtpd2ps %%xmm7,    %%xmm7\n"
                        "movlhps %%xmm4, %%xmm2\n"
                        "movlhps %%xmm7, %%xmm5\n"

                        // store the result in dst array
                        "movaps %%xmm1, (%[dst],%[cnt],4)\n"
                        "movaps %%xmm3, 16(%[dst],%[cnt],4)\n"
                        "movaps %%xmm2, 32(%[dst],%[cnt],4)\n"
                        "movaps %%xmm5, 48(%[dst],%[cnt],4)\n"
                        "add            $0x10,      %[cnt]\n"
                    "1:\n"
                        "cmp            %[cnt],      %[length]\n"
                        "ja             0b\n"
                        : : [length]"d"(i), "Yz"(a), [dst]"r"(r), [src]"S"(x), [cnt]"c"(0L)\
                        : "memory", "xmm1", "xmm2", "xmm3", "xmm4", "xmm5", "xmm6", "xmm7", "cc", "%ax");
        }

    private:
        const coordinate_type a;
        const coordinate_type * const x;
        float * const r;
    };

    struct Translate {

        __attribute__((target("sse2")))
        Translate() : functor(&Translate::RelativeTo_sse2), functor_with_distance_view(&Translate::InViewPositions_sse2) {};

        __attribute__((target("default")))
        Translate() : functor(&Translate::RelativeTo_default), functor_with_distance_view(&Translate::InViewPositions_default) {};


        virtual ~Translate() {};

        coordinate_array RelativeTo(const coordinate_type x, const aligned_vector_type& v) {
            return (this->*functor)(x, v);
        }

        void InViewPositions(const coordinate_type x, const aligned_vector_type& v, const coordinate_type distance, BitArray<unsigned short>& bitmap) const {
            return (this->*functor_with_distance_view)(x, v, distance, bitmap);
        }

        void IncreaseLength() { arraypool.IncreaseLength(); };

        void DecreaseLength() { arraypool.DecreaseLength(); };

        __attribute__((hot))
        coordinate_array RelativeTo_sse2(const coordinate_type x, const aligned_vector_type& v) {
            size_t length = v.size();
            auto shared_dst = arraypool.GetArray(length);
            float* dst = shared_dst.get();
            transform_n_sse2(length, dapx_kernel(-x, v.data(), dst));
            return shared_dst;
        };

        __attribute__((hot))
        void InViewPositions_sse2(const coordinate_type x,
                                                              const aligned_vector_type& v,
                                                              const coordinate_type distance,
                                                              BitArray<unsigned short>& bitmap) const {
            size_t length = v.size();
            transform_n_sse2(length, build_bitmap_distance_view_kernel(-x, v.data(), distance, bitmap));
        };

        coordinate_array RelativeTo_default(const coordinate_type x, const aligned_vector_type& v) {
            size_t length = v.size();
            auto shared_dst = arraypool.GetArray(length);
            float* dst = shared_dst.get();
            transform_n_scalar(length, dapx_kernel(-x, v.data(), dst));
            return shared_dst;
        };

        void InViewPositions_default(const coordinate_type x, const aligned_vector_type& v,
                                                                     const coordinate_type distance,
                                                                     BitArray<unsigned short>& bitmap) const {
            size_t length = v.size();
            transform_n_scalar(length, build_bitmap_distance_view_kernel(-x, v.data(), distance, bitmap));
        };

    private:
        coordinate_array (Translate::*functor)(const coordinate_type, const aligned_vector_type&);
        void (Translate::*functor_with_distance_view)(const coordinate_type, const aligned_vector_type&, const coordinate_type, BitArray<unsigned short>&) const;
        // A pool of allocated array
        ArrayPool<float> arraypool;
    };
}
#endif // WORLDPOSITION_SIMD_GCC_HPP_INCLUDED
