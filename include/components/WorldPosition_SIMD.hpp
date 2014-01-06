#ifndef WORLDPOSITION_SIMD_HPP_INCLUDED
#define WORLDPOSITION_SIMD_HPP_INCLUDED

#include "SIMD.hpp"
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
		a(aa), x(xx), distance(distance_view), bitmap(bitmap), bm(const_cast<unsigned short*>(bitmap.data())) {
            a_v = _mm_set1_pd(aa);
            distance_v = _mm_set1_pd(distance);
            mask = _mm_set1_pd(DABS_MASK);
        }

        inline void scalar_op (const size_t i) const {
            // update vectorbit
            bitmap[i] &= (fabs(a + x[i]) < distance);
        }

        inline void vector_op_sse2 (const size_t length) const {
            for (auto i = 0; i < length; i += BLOCK_SIZE) {
                // load 16 data elements at a time
                const coordinate_type* const t = x + i;
                PREFETCH_T0(t + 56, 0);
                PREFETCH_T0(t + 64, 0);

                // load 8 elements from src array in registers
                __m128d X1 = _mm_load_pd(t + 6);
                // add translation parameter
                X1 = _mm_add_pd(X1, a_v);
                __m128d X2 = _mm_load_pd(t + 4);
                X2 = _mm_add_pd(X2, a_v);
                __m128d X3 = _mm_load_pd(t + 2);
                X3 = _mm_add_pd(X3, a_v);
                __m128d X4 = _mm_load_pd(t);
                X4 = _mm_add_pd(X4, a_v);

                // compute absolute value
                X1 = _mm_and_pd(X1, mask);
                X2 = _mm_and_pd(X2, mask);
                X3 = _mm_and_pd(X3, mask);
                X4 = _mm_and_pd(X4, mask);

                // compare with distance view value
                __m128d result1 = _mm_cmplt_pd(X1, distance_v);
                __m128d result2 = _mm_cmplt_pd(X2, distance_v);
                __m128d result3 = _mm_cmplt_pd(X3, distance_v);
                __m128d result4 = _mm_cmplt_pd(X4, distance_v);

                // extract comparison result and pack it
                unsigned char cmp = _mm_movemask_pd(result1);
                unsigned char last = cmp;
                last = last << 2;
                cmp = _mm_movemask_pd(result2);
                last |= cmp;
                last = last << 2;
                cmp = _mm_movemask_pd(result3);
                last |= cmp;
                last = last << 2;
                cmp = _mm_movemask_pd(result4);
                last |= cmp;

                // write in bitmap
                *(bm + (i >> 4)) &= last;
            }
        }

    private:
        __m128d a_v;
        const coordinate_type a;
        const coordinate_type * const x;
        __m128d distance_v;
        const coordinate_type distance;
        __m128d mask;
        BitArray<unsigned short>& bitmap;
        unsigned short* bm;
    };

    /** \brief kernel to perform y = (float) (x + a) using intrinsics
     */
    class dapx_kernel {
    public:
        enum {BLOCK_SIZE = 16};

        dapx_kernel(const coordinate_type aa, const coordinate_type * const xx, float *rr)
            : a(aa), x(xx), r(rr) {
            a_v = _mm_set1_pd(aa);
        }

        inline void scalar_op (const size_t i) const {
            r[i] = (float) (a + x[i]);
        }

        inline void vector_op_sse2 (const size_t length) const {
            for (auto i = 0; i < length; i += BLOCK_SIZE) {
                // load 16 data elements at a time
                const coordinate_type* const t = x + i;
                PREFETCH_T0(t + 80, 0);
                PREFETCH_T0(t + 88, 0);
                float* const u = r + i;
    //            PREFETCHW(u + 64, 0);
                __m128d X1 = _mm_load_pd(t);
                X1 = _mm_add_pd(X1, a_v);
                __m128 result1 = _mm_cvtpd_ps(X1);
                __m128d X2 = _mm_load_pd(t + 2);
                __m128 result2 = _mm_cvtpd_ps(_mm_add_pd(X2, a_v));
                __m128d X3 = _mm_load_pd(t + 4);
                __m128 result3 = _mm_cvtpd_ps(_mm_add_pd(X3, a_v));
                __m128d X4 = _mm_load_pd(t + 6);
                __m128 result4 = _mm_cvtpd_ps(_mm_add_pd(X4, a_v));
                __m128 resultA = _mm_movelh_ps(result1, result2);
                __m128 resultB = _mm_movelh_ps(result3, result4);
                X2 = _mm_load_pd(t + 8);
                result1 = _mm_cvtpd_ps(_mm_add_pd(X2, a_v));
                X4 = _mm_load_pd(t + 10);
                result2 = _mm_cvtpd_ps(_mm_add_pd(X4, a_v));
                __m128d X5 = _mm_load_pd(t + 12);
                result3 = _mm_cvtpd_ps(_mm_add_pd(X5, a_v));
                __m128d X7 = _mm_load_pd(t + 14);
                result4 = _mm_cvtpd_ps(_mm_add_pd(X7, a_v));
                __m128 resultC = _mm_movelh_ps(result1, result2);
                __m128 resultD = _mm_movelh_ps(result3, result4);
                _mm_store_ps(u, resultA);
                _mm_store_ps(u + 4, resultB);
                _mm_store_ps(u + 8, resultC);
                _mm_store_ps(u + 12, resultD);
            }
        }

    private:
        __m128d a_v;
        const coordinate_type a;
        const coordinate_type * const x;
        float * const r;
    };

    struct Translate {

        virtual ~Translate() {};

        void IncreaseLength() { arraypool.IncreaseLength(); };

        void DecreaseLength() { arraypool.DecreaseLength(); };

        coordinate_array RelativeTo(const coordinate_type x, const aligned_vector_type& v) {
            size_t length = v.size();
            auto shared_dst = arraypool.GetArray(length);
            float* dst = shared_dst.get();
            if (has_SSE2()) {
                transform_n_sse2(length, dapx_kernel(-x, v.data(), dst));
            }
            else {
                transform_n_scalar(length, dapx_kernel(-x, v.data(), dst));
            }
            return shared_dst;
        };

        static void InViewPositions(const coordinate_type x,  const aligned_vector_type& v,
                                                              const coordinate_type distance,
                                                              BitArray<unsigned short>& bitmap) {
            size_t length = v.size();
            if (has_SSE2()) {
                transform_n_sse2(length, build_bitmap_distance_view_kernel(-x, v.data(), distance, bitmap));
            }
            else {
                transform_n_scalar(length, build_bitmap_distance_view_kernel(-x, v.data(), distance, bitmap));
            }
        };

    private:
        // A pool of allocated array
        ArrayPool<float> arraypool;
    };
}
#endif // WORLDPOSITION_SIMD_HPP_INCLUDED
