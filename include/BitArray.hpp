#ifndef BITARRAY_H_INCLUDED
#define BITARRAY_H_INCLUDED

#include <vector>
#include <stdexcept>
#include <memory>
#include <iostream>
#include "SharedPointerMap.hpp"
// deactivated
//#include "AlignedVectorAllocator.hpp"

#if defined(_MSC_VER)
#include <intrin.h>
#endif

#define ROUND_DOWN(x, s) ((x) & ~((s)-1)) // rounds down x to a multiple of s (i.e. ROUND_DOWN(5, 4) becomes 4)

namespace Sigma {
	/** \brief A reference to simulate an lvalue
	 */
	template<class T>
	class reference {
	public:
		reference(T& c, const size_t offset) : c(c), offset(offset) {};

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
		T& c;
		size_t offset;
	};

	template<class T>
	class BitArrayIterator;

	/** \brief A bitset like boost::dynamic_bitset
	 */
	template<class T>
	class BitArray : public std::enable_shared_from_this<BitArray<T>> {
	public:
		/** \brief Call the constructor of a BitArray
		 *
		 * \param args are forwarded to constructor
		 * \return std::shared_ptr<BitArray> a shared_ptr on the BitArray
		 *
		 */
		static std::shared_ptr<BitArray<T>> Create() {
			return std::shared_ptr<BitArray<T>>(new BitArray<T>());
		}

		static std::shared_ptr<BitArray<T>> Create(const size_t s, const bool b) {
			return std::shared_ptr<BitArray<T>>(new BitArray<T>(s, b));
		}

		static std::shared_ptr<BitArray<T>> Create(const size_t s) {
			return std::shared_ptr<BitArray<T>>(new BitArray<T>(s));
		}

		static std::shared_ptr<BitArray<T>> Create(const bool b) {
			return std::shared_ptr<BitArray<T>>(new BitArray<T>(b));
		}

		// Default destructor
		virtual ~BitArray() {};
		// Copy constructor
		BitArray(BitArray& ba) : blocksize(sizeof(T) << 3) {
			bitarray = ba.bitarray;
			def_value = ba.def_value;
		}
		// Move Constructor
		BitArray(BitArray&& ba) : blocksize(sizeof(T) << 3) {
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
			auto offset = idx / blocksize;
			if (offset >= bitarray.size()) {
				throw new std::out_of_range("Index does not exist");
			}
			auto bit_id = idx % blocksize;
			return ((bitarray[offset] & (1 << bit_id)) != 0);
		}

		// left-side reference
		reference<T> operator[](size_t idx) {
			auto offset = idx / blocksize;
			if (offset >= bitarray.size()) {
					bitarray.resize(offset + 1, def_value);
					bsize = idx + 1;
			}
			auto bit_id = idx % blocksize;
			return reference<T>(bitarray[offset], bit_id);
		}

		const size_t size() const {
			return bsize;
		}

		const T* data() const {
			return bitarray.data();
		}

		std::unique_ptr<BitArrayIterator<T>> iterator() {
			return std::unique_ptr<BitArrayIterator<T>>(new BitArrayIterator<T>(this->shared_from_this()));
		};

		const size_t count() const {
			size_t sum = 0;
			auto length = size();
			#if defined(__GNUG__) || defined(_MSC_VER)
				size_t j = ROUND_DOWN(size(), 64);
				unsigned long long* start = (unsigned long long*) bitarray.data();
				auto end = start + (j >> 6);
				for (auto i = start; i < end; i++) {
					#if defined(__GNUG__)
					sum += __builtin_popcountll(*i);
					#elif defined(_MSC_VER)
					sum += __popcnt64(*i);
					#endif
				}
			#else
				size_t j = 0;
			#endif
			for (; j < length; j++) {
				if ((*this)[j]) {
					sum++;
				}
			}
			return sum;
		}

	private:
		// Default constructor
		BitArray() : bsize(0), def_value(0), blocksize(sizeof(T) << 3)  {};
		// Constructor with default value
		BitArray(const bool b) : bsize(0), def_value(b ? -1 : 0), blocksize(sizeof(T) << 3)  {};
		// Constructor with initial size
		BitArray(const size_t s) : bsize(s), def_value(0), blocksize(sizeof(T) << 3)  {
			bitarray = std::vector<T>((s / blocksize) + 1);
//			bitarray = std::vector<T, AlignedVectorAllocator<T>>((s / blocksize) + 1);
			bitarray.assign(bitarray.size(), this->def_value);
		};
		// Constructor with initial size and default value
		BitArray(const size_t s, const bool b) : bsize(s), def_value(b ? -1 : 0), blocksize(sizeof(T) << 3)  {
			bitarray = std::vector<T>((s / blocksize) + 1);
//			bitarray = std::vector<T, AlignedVectorAllocator<T>>((s / blocksize) + 1);
			bitarray.assign(bitarray.size(), this->def_value);
		};

		std::vector<T> bitarray;
//		std::vector<T, AlignedVectorAllocator<T>> bitarray;
		size_t bsize;
		T def_value;
		const unsigned int blocksize;
	};

#if defined(__GNUG__) // define BitArrayIterator per compiler
	template<class T>
	class BitArrayIterator {
	public:
		BitArrayIterator(std::shared_ptr<BitArray<T>> bs) : bitarray(bs),\
			current_long((unsigned long long*) bs->data()), last_bit(0), current_value(-1) { ++(*this); };
		virtual ~BitArrayIterator() {};

		size_t& operator++() {
			auto length = bitarray->size();
			auto end = (unsigned long long*) bitarray->data() + (ROUND_DOWN(bitarray->size(), 64) >> 6);
			for (current_value++; current_long < end ; current_long++) {
				if (*current_long && last_bit < 64) {
					unsigned long long tmp = *current_long >> last_bit;
					if (__builtin_popcountll(tmp)) {
						last_bit += __builtin_ctzll(tmp);
						current_value = last_bit++ + ((current_long - start) << 6);
						return current_value;
					}
				}
				else {
					last_bit = 0;
				}
			}
			for (; current_value < length; ++current_value) {
				if ((*bitarray)[current_value]) {
					return current_value;
				}
			}
			current_value = length;
			return current_value;
		};

		const size_t &operator*() const {
			return current_value;
		}

	private:
		int last_bit;
		unsigned long long* current_long;
		const unsigned long long* const start = current_long;
		std::shared_ptr<BitArray<T>> bitarray;
		size_t current_value;
	};

#elif defined(_MSC_VER)
	template<class T>
	class BitArrayIterator {
	public:
		BitArrayIterator(std::shared_ptr<BitArray<T>> bs) : bitarray(bs),\
			current_int((unsigned int*) bs->data()), last_bit(0), current_value(-1), start(current_int) { ++(*this); };
		virtual ~BitArrayIterator() {};

		size_t& operator++() {
			auto length = bitarray->size();
			auto end = (unsigned int*) bitarray->data() + (ROUND_DOWN(bitarray->size(), 32) >> 5);
			for (current_value++; current_int < end; current_int++) {
				if (*current_int && last_bit < 32) {
					unsigned int tmp = *current_int >> last_bit;
					if (__popcnt(tmp)) {
						unsigned long ret;
						_BitScanForward(&ret, tmp);
						last_bit += ret;
						current_value = last_bit++ + ((current_int - start) << 5);
						return current_value;
					}
				}
				else {
					last_bit = 0;
				}
			}
			for (; current_value < length; ++current_value) {
				if ((*bitarray)[current_value]) {
					return current_value;
				}
			}
			current_value = length;
			return current_value;
		};

		const size_t &operator*() const {
			return current_value;
		}

	private:
		unsigned char last_bit;
		unsigned int* current_int;
		const unsigned int* const start;
		std::shared_ptr<BitArray<T>> bitarray;
		size_t current_value;
	};
#else // other compilers : not optimized at all
	template<class T>
	class BitArrayIterator {
	public:
		BitArrayIterator(std::shared_ptr<BitArray<T>> bs) : bitarray(bs), current_value(-1) { ++(*this); };
		virtual ~BitArrayIterator() {};

		size_t& operator++() {
			auto length = bitarray->size();
			for (current_value++; current_value < length; ++current_value) {
				if ((*bitarray)[current_value]) {
					return current_value;
				}
			}
			current_value = length;
			return current_value;
		};

		const size_t &operator*() const {
			return current_value;
		}

	private:
		const unsigned long long* const start = current_long;
		std::shared_ptr<BitArray<T>> bitarray;
		size_t current_value;
	};
#endif
}

#endif // BITARRAY_H_INCLUDED
