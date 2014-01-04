#ifndef BITARRAYTEST_H_INCLUDED
#define BITARRAYTEST_H_INCLUDED

#include <stdexcept>
#include "BitArray.hpp"

namespace Sigma {
	TEST(BitArrayTest, BitArrayBasic) {
		auto bit_array_ptr = BitArray<unsigned short>::Create((size_t) 550); //550 bits
		auto bit_array = *bit_array_ptr;
		ASSERT_FALSE(bit_array[200]) << "Default value is not false";
		bit_array[200] = true;
		ASSERT_TRUE(bit_array[200]) << "Failed to write in array";
		bit_array[200] = false;
		ASSERT_FALSE(bit_array[200]) << "Failed to rewrite in array";
	}

	TEST(BitArrayTest, BitArrayExtend) {
		auto bit_array_ptr = BitArray<unsigned short>::Create((size_t) 550); //550 bits
		auto bit_array = *bit_array_ptr;
		EXPECT_FALSE(bit_array[600]) << "Default value is not false";
		EXPECT_NO_THROW(bit_array[600] = true) << "Failed to extend array";
		EXPECT_TRUE(bit_array[600]) << "Failed to write in extended array";
	}

	TEST(BitArrayTest, BitArrayDefaultValue) {
		auto bit_array_ptr = BitArray<unsigned short>::Create((size_t) 550, true); //550 bits
		auto bit_array = *bit_array_ptr;
		ASSERT_TRUE(bit_array[203]) << "Default value is not true";
		bit_array[203] = false;
		ASSERT_FALSE(bit_array[203]) << "Failed to write in array";
	}

	TEST(BitArrayTest, BitArrayAND) {
		auto bit_arrayA = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayB = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayC = BitArray<unsigned short>::Create((size_t) 100); //100 bits
		EXPECT_ANY_THROW(auto fail = *bit_arrayA & *bit_arrayC) << "Arrays of different size cannot be ANDed";
		(*bit_arrayA)[151] = true;
		(*bit_arrayB)[151] = true;
		(*bit_arrayA)[100] = true;
		(*bit_arrayB)[300] = true;
		auto result = *bit_arrayA & *bit_arrayB;
		ASSERT_TRUE(result[151]) << "true & true == false !";
		ASSERT_FALSE(result[1]) << "false & false == true !";
		ASSERT_FALSE(result[100]) << "true & false == true !";
		ASSERT_FALSE(result[300]) << "false & true == true !";
	}

	TEST(BitArrayTest, BitArrayOR) {
		auto bit_arrayA = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayB = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayC = BitArray<unsigned short>::Create((size_t) 100); //100 bits
		EXPECT_ANY_THROW(auto fail = *bit_arrayA | *bit_arrayC) << "Arrays of different size cannot be ANDed";
		(*bit_arrayA)[100] = true;
		(*bit_arrayB)[300] = true;
		auto result = *bit_arrayA | *bit_arrayB;
		ASSERT_TRUE(result[100]) << "true | false == false !";
		ASSERT_TRUE(result[300]) << "false | true == true !";
	}

	TEST(BitArrayTest, BitArrayXOR) {
		auto bit_arrayA = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayB = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayC = BitArray<unsigned short>::Create((size_t) 100); //100 bits
		EXPECT_ANY_THROW(auto fail = *bit_arrayA ^ *bit_arrayC) << "Arrays of different size cannot be ANDed";
		(*bit_arrayA)[151] = true;
		(*bit_arrayB)[151] = true;
		(*bit_arrayA)[100] = true;
		(*bit_arrayB)[300] = true;
		auto result = *bit_arrayA ^ *bit_arrayB;
		ASSERT_FALSE(result[151]) << "true ^ true == true !";
		ASSERT_FALSE(result[1]) << "false ^ false == true !";
		ASSERT_TRUE(result[100]) << "true ^ false == false !";
		ASSERT_TRUE(result[300]) << "false ^ true == false !";
	}

	TEST(BitArrayTest, BitArrayANDcompound) {
		auto bit_arrayA = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayB = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayC = BitArray<unsigned short>::Create((size_t) 100); //100 bits
		EXPECT_ANY_THROW(*bit_arrayA &= *bit_arrayC) << "Arrays of different size cannot be ANDed";
		(*bit_arrayA)[151] = true;
		(*bit_arrayB)[151] = true;
		(*bit_arrayA)[100] = true;
		(*bit_arrayB)[300] = true;
		*bit_arrayA &= *bit_arrayB;
		ASSERT_TRUE((*bit_arrayA)[151]) << "true & true == false !";
		ASSERT_FALSE((*bit_arrayA)[1]) << "false & false == true !";
		ASSERT_FALSE((*bit_arrayA)[100]) << "true & false == true !";
		ASSERT_FALSE((*bit_arrayA)[300]) << "false & true == true !";
	}

	TEST(BitArrayTest, BitArrayORcompound) {
		auto bit_arrayA = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayB = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayC = BitArray<unsigned short>::Create((size_t) 100); //100 bits
		EXPECT_ANY_THROW(*bit_arrayA |= *bit_arrayC) << "Arrays of different size cannot be ANDed";
		(*bit_arrayA)[100] = true;
		(*bit_arrayB)[300] = true;
		*bit_arrayA |= *bit_arrayB;
		ASSERT_TRUE((*bit_arrayA)[100]) << "true | false == false !";
		ASSERT_TRUE((*bit_arrayA)[300]) << "false | true == true !";
	}

	TEST(BitArrayTest, BitArrayXORcompound) {
		auto bit_arrayA = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayB = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		auto bit_arrayC = BitArray<unsigned short>::Create((size_t) 100); //100 bits
		EXPECT_ANY_THROW(*bit_arrayA ^= *bit_arrayC) << "Arrays of different size cannot be ANDed";
		(*bit_arrayA)[151] = true;
		(*bit_arrayB)[151] = true;
		(*bit_arrayA)[100] = true;
		(*bit_arrayB)[300] = true;
		*bit_arrayA ^= *bit_arrayB;
		ASSERT_FALSE((*bit_arrayA)[151]) << "true ^ true == true !";
		ASSERT_FALSE((*bit_arrayA)[1]) << "false ^ false == true !";
		ASSERT_TRUE((*bit_arrayA)[100]) << "true ^ false == false !";
		ASSERT_TRUE((*bit_arrayA)[300]) << "false ^ true == false !";
	}

	TEST(BitArrayTest, BitArrayNOT) {
		auto bit_arrayA = BitArray<unsigned short>::Create((size_t) 569); //569 bits
		(*bit_arrayA)[151] = true;
		*bit_arrayA = ~*bit_arrayA;
		ASSERT_FALSE((*bit_arrayA)[151]) << "~true == true !";
		ASSERT_TRUE((*bit_arrayA)[100]) << "~false == false !";
	}

	TEST(BitArrayTest, BitArrayCount) {
		auto bit_array = BitArray<unsigned short>::Create((size_t) 573); //573 bits
		auto count = bit_array->count();
		ASSERT_EQ(0, count) << "Empty array count() should return 0";
		(*bit_array)[0] = true;
		count = bit_array->count();
		ASSERT_EQ(1, count) << "count() should return 1";
		(*bit_array)[1] = true;
		count = bit_array->count();
		ASSERT_EQ(2, count) << "count() should return 2";
		(*bit_array)[15] = true;
		count = bit_array->count();
		ASSERT_EQ(3, count) << "count() should return 3";
		(*bit_array)[16] = true;
		count = bit_array->count();
		ASSERT_EQ(4, count) << "count() should return 4";
		(*bit_array)[100] = true;
		count = bit_array->count();
		ASSERT_EQ(5, count) << "count() should return 5";
		(*bit_array)[300] = true;
		count = bit_array->count();
		ASSERT_EQ(6, count) << "count() should return 6";
		(*bit_array)[569] = true;
		count = bit_array->count();
		ASSERT_EQ(7, count) << "count() should return 7";
		(*bit_array)[570] = true;
		count = bit_array->count();
		ASSERT_EQ(8, count) << "count() should return 8";
		(*bit_array)[571] = true;
		count = bit_array->count();
		ASSERT_EQ(9, count) << "count() should return 9";
		(*bit_array)[572] = true;
		count = bit_array->count();
		ASSERT_EQ(10, count) << "count() should return 10";
	}

	TEST(BitArrayTest, BitArrayIterator) {
		auto bit_array = BitArray<unsigned int>::Create((size_t) 576); //576 bits
		auto it = bit_array->iterator();
		ASSERT_LE(576, ++*it) << "it++ should return size when BitArray is empty";
		(*bit_array)[0] = true;
		it = bit_array->iterator();
		ASSERT_EQ(0, **it) << "it++ should return 0";
		(*bit_array)[1] = true;
		ASSERT_EQ(1, ++*it) << "it++ should return 1";
		(*bit_array)[15] = true;
		ASSERT_EQ(15, ++*it) << "it++ should return 15";
		(*bit_array)[63] = true;
		ASSERT_EQ(63, ++*it) << "it++ should return 63";
		(*bit_array)[64] = true;
		ASSERT_EQ(64, ++*it) << "it++ should return 64";
		(*bit_array)[65] = true;
		ASSERT_EQ(65, ++*it) << "it++ should return 65";
		(*bit_array)[300] = true;
		ASSERT_EQ(300, ++*it) << "it++ should return 300";
		(*bit_array)[574] = true;
		ASSERT_EQ(574, ++*it) << "it++ should return 574";
		(*bit_array)[575] = true;
		ASSERT_EQ(575, ++*it) << "it++ should return 575";
		ASSERT_LE(576, ++*it) << "it++ should return size";

		bit_array = BitArray<unsigned int>::Create((size_t) 576); //576 bits
		(*bit_array)[1] = true;
		it = bit_array->iterator();
		ASSERT_EQ(1, **it) << "it++ should return 1";
	}
}

#endif // BITARRAYTEST_H_INCLUDED
