#ifndef BITARRAY_H_INCLUDED
#define BITARRAY_H_INCLUDED

#include <vector>
#include <stdexcept>
#include <memory>
#include <iostream>
#include "components/SharedPointerMap.hpp"
//#include "systems/WorldObjectsSystem.h"

#define ROUND_DOWN(x, s) ((x) & ~((s)-1)) // rounds down x to a multiple of s (i.e. ROUND_DOWN(5, 4) becomes 4)

namespace Sigma {
    /** \brief A reference to simulate an lvalue
     */
    class reference {
    public:
        reference(unsigned short& c, const size_t offset) : c(c), offset(offset) {};

        reference(reference& r) : c(r.c) {
            c = r.c;
            offset = offset;
        }

        reference(reference&& r) : c(r.c) {
            offset = std::move(offset);
        }

        virtual ~reference() {};

        reference& operator=(bool b) {
            if (b) {
                c |= 1 << offset;
            }
            else {
                c &= ~ (1 << offset);
            }
            return *this;
        };

        reference& operator=(reference& r) {
            c = r.c;
            offset = offset;
            return *this;
        }

        reference& operator=(reference&& r) {
            c = std::move(r.c);
            offset = std::move(offset);
            return *this;
        }

        reference& operator|=(bool b) {
            if (b) {
                c |= 1 << offset;
            }
            return *this;
        };

        reference& operator&=(bool b) {
            if (! b) {
                c &= ~ (1 << offset);
            }
            return *this;
        };

        operator bool() const { return (c & (1 << offset)) != 0; };

    private:
        unsigned short& c;
        size_t offset;
    };

    class BitArray;

    /** \brief An iterator giving bunch of 16 elements
     */
    class iterator16 {
    public:
        // default constructor
        iterator16(const std::weak_ptr<BitArray>& ba);
        // constructor with index
        iterator16(const std::weak_ptr<BitArray>& ba, const size_t s);

        /** \brief Return 16 elements in which at least 1 is not null
         *
         * \param i int fake argument
         * \return const size_t the starting index of the 16 elements (multiple of 16)
         *
         */
        const size_t operator++(int i);

    private:
        int current;
        unsigned int size;
        std::weak_ptr<BitArray> bit_array;
    };

    /** \brief A bitset like boost::dynamic_bitset
     */
    class BitArray : public std::enable_shared_from_this<BitArray> {
        friend class Sigma::iterator16;

//        friend class Sigma::WorldObjectSystem;
    public:

        template<class ...Args>
        /** \brief Call the constructor of a BitArray
         *
         * \param args Args&&... arguments to forward
         * \return std::shared_ptr<BitArray> a shared_ptr on the BitArray
         *
         */
        static std::shared_ptr<BitArray> Create(Args&&... args) {
            return std::shared_ptr<BitArray>(new BitArray(std::forward<Args>(args)...));
        }

        // Default destructor
        virtual ~BitArray() {};
        // Copy constructor
        BitArray(BitArray& ba) {
            bitarray = ba.bitarray;
            def_value = ba.def_value;
        }
        // Move Constructor
        BitArray(BitArray&& ba) {
            bitarray = std::move(ba.bitarray);
            def_value = std::move(ba.def_value);
        }
        // Copy assignment
        BitArray& operator=(BitArray& ba) {
            bitarray = ba.bitarray;
            def_value = ba.def_value;
            return *this;
        }
        // Move assignment
        BitArray& operator=(BitArray&& ba) {
            bitarray = std::move(ba.bitarray);
            def_value = std::move(ba.def_value);
            return *this;
        }

        // Compound assignment operators
        // OR combination between 2 BitArrays
        BitArray& operator|=(const BitArray& ba) {
            if (bitarray.size() != ba.bitarray.size()) {
                throw std::length_error("BitArrays have different size !");
            }
            auto it = ba.bitarray.cbegin();
            for (auto &c : bitarray) {
                c |= *it++;
            }
            return *this;
        }

        // AND combination between 2 BitArrays
        BitArray& operator&=(const BitArray& ba) {
            if (bitarray.size() != ba.bitarray.size()) {
                throw new std::length_error("BitArrays have different size !");
            }
            auto it = ba.bitarray.cbegin();
            for (auto &c : bitarray) {
                c &= *it++;
            }
            return *this;
        }

        // XOR combination between 2 BitArrays
        BitArray& operator^=(const BitArray& ba) {
            if (bitarray.size() != ba.bitarray.size()) {
                throw new std::length_error("BitArrays have different size !");
            }
            auto it = ba.bitarray.cbegin();
            for (auto &c : bitarray) {
                c ^= *it++;
            }
            return *this;
        }

        // Bitwise logical operators
        // NOT operation
        BitArray& operator~() {
            for (auto &c : bitarray) {
                c = ~c;
            }
            return *this;
        }

        // OR combination between 2 BitArrays
        BitArray operator|(const BitArray& ba) {
            if (bitarray.size() != ba.bitarray.size()) {
                throw std::length_error("BitArrays have different size !");
            }
            BitArray result(size());
            auto itr = ba.bitarray.cbegin();
            auto itl = bitarray.cbegin();
            for (auto &c : result.bitarray) {
                c = *itl++ | *itr++;
            }
            return result;
        }

        // AND combination between 2 BitArrays
        BitArray operator&(const BitArray& ba) {
            if (bitarray.size() != ba.bitarray.size()) {
                throw new std::length_error("BitArrays have different size !");
            }
            BitArray result(size());
            auto itr = ba.bitarray.cbegin();
            auto itl = bitarray.cbegin();
            for (auto &c : result.bitarray) {
                c = *itl++ & *itr++;
            }
            return result;
        }

        // XOR combination between 2 BitArrays
        BitArray operator^(const BitArray& ba) {
            if (bitarray.size() != ba.bitarray.size()) {
                throw new std::length_error("BitArrays have different size !");
            }
            BitArray result(size());
            auto itr = ba.bitarray.cbegin();
            auto itl = bitarray.cbegin();
            for (auto &c : result.bitarray) {
                c = *itl++ ^ *itr++;
            }
            return result;
        }

        // Access to an element of the BitArray
        bool operator[](size_t idx) const {
            auto offset = idx >> 4;
            if (offset >= bitarray.size()) {
                throw new std::out_of_range("Index does not exist");
            }
            auto bit_id = idx % 16;
            return ((bitarray[offset] & (1 << bit_id)) != 0);
        }

        // left-side reference
        reference operator[](size_t idx) {
            auto offset = idx >> 4;
            if (offset >= bitarray.size()) {
                    bitarray.resize(offset + 1, def_value);
                    bsize = idx + 1;
            }
            auto bit_id = idx % 16;
            return reference(bitarray[offset], bit_id);
        }

        iterator16 iterator_word() {
            return iterator16(shared_from_this());
        }

        const size_t size() const {
            return bsize;
        }

        const unsigned short* data() const {
            return bitarray.data();
        }

    private:
        // Default constructor
        BitArray() : bsize(0), def_value(0) {};
        // Constructor with default value
        BitArray(const bool b) : bsize(0), def_value(b ? 0xFFFF : 0) {};
        // Constructor with initial size
        BitArray(const size_t s) : bsize(s), bitarray( (s >> 4) + 1), def_value(0) { bitarray.assign(bitarray.size(), this->def_value); };
        // Constructor with initial size and default value
        BitArray(const size_t s, const bool b) : bsize(s), bitarray( (s >> 4) + 1), def_value(b ? 0xFFFF : 0) {
            bitarray.assign(bitarray.size(), this->def_value);
        };

        std::vector<unsigned short> bitarray;
        size_t bsize;
        unsigned short def_value;
    };
}

#endif // BITARRAY_H_INCLUDED
