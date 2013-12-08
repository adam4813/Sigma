#include "components/BitArray.h"

namespace Sigma {
    // default constructor
    iterator16::iterator16(const std::weak_ptr<BitArray>& ba) : bit_array(ba), current(-16), size(ba.expired() ? 0 : ba.lock()->size()) {}
    // constructor with index
    iterator16::iterator16(const std::weak_ptr<BitArray>& ba, const size_t s) : bit_array(ba), current(ROUND_DOWN(s, 16)-16), size(ba.expired() ? 0 : ba.lock()->size()) {}

    //TODO : make it inline
    const size_t iterator16::operator++(int j) {
        if (bit_array.expired()) {
            return size;
        }
        auto ptr = bit_array.lock();
        int s = size -33;
        auto i = (this->current >> 4);
        while (this->current < s) {
            this->current += 16;
            i ++;
            if (ptr->bitarray[i] != 0) {
                return this->current;
            }
        }
        return size;
    }
}
