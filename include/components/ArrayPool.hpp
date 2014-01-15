#ifndef ARRAYPOOL_HPP_INCLUDED
#define ARRAYPOOL_HPP_INCLUDED

#include <stdexcept>
#include <forward_list>
#include <memory>
#include <cassert>
#include "SIMD.hpp"

using std::shared_ptr;
using std::forward_list;

namespace Sigma {
    template<class T>
    class ArrayPool {
    public:
        ArrayPool() : length(0) {};

        virtual ~ArrayPool() {};

        const shared_ptr<T> GetArray(size_t length) {
            assert(length == length);
            for (auto& p : pool) {
                if (p.unique()) {
                    return p;
                }
            }
            return AllocateArrays(length);
        }

        void IncreaseLength() {
            length++;
            pool.clear();
        }

        void DecreaseLength() {
            if (length != 0) {
                length--;
                pool.clear();
            }
        }

    private:
        const shared_ptr<T> AllocateArrays(size_t length) {
            auto ptr = AlignedMalloc<T>(length * sizeof(T));
            if (ptr == NULL) {
                throw std::runtime_error("Error: unable to allocate memory");
            }
            auto ret = shared_ptr<T>(ptr, AlignedPointerDelete<T>());
            pool.push_front(ret);
            return ret;
        }

        size_t length;
        forward_list<shared_ptr<T>> pool;
    };
}

#endif // ARRAYPOOL_HPP_INCLUDED
