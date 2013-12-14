#ifndef BITARRAYTEST_H_INCLUDED
#define BITARRAYTEST_H_INCLUDED

#include <stdexcept>
#include "components/BitArray.h"

namespace Sigma {
    TEST(BitArrayTest, BitArrayBasic) {
        auto bit_array_ptr = BitArray::Create((size_t) 550); //550 bits
        auto bit_array = *bit_array_ptr;
        ASSERT_FALSE(bit_array[200]) << "Default value is not false";
        bit_array[200] = true;
        ASSERT_TRUE(bit_array[200]) << "Failed to write in array";
        bit_array[200] = false;
        ASSERT_FALSE(bit_array[200]) << "Failed to rewrite in array";
    }

    TEST(BitArrayTest, BitArrayExtend) {
        auto bit_array_ptr = BitArray::Create((size_t) 550); //550 bits
        auto bit_array = *bit_array_ptr;
        EXPECT_FALSE(bit_array[600]) << "Default value is not false";
        EXPECT_NO_THROW(bit_array[600] = true) << "Failed to extend array";
        EXPECT_TRUE(bit_array[600]) << "Failed to write in extended array";
    }

    TEST(BitArrayTest, BitArrayDefaultValue) {
        auto bit_array_ptr = BitArray::Create((size_t) 550, true); //550 bits
        auto bit_array = *bit_array_ptr;
        ASSERT_TRUE(bit_array[203]) << "Default value is not true";
        bit_array[203] = false;
        ASSERT_FALSE(bit_array[203]) << "Failed to write in array";
    }

    TEST(BitArrayTest, BitArrayIterator) {
        auto bit_array = BitArray::Create((size_t) 550); //550 bits
        auto it = bit_array->iterator_word();
        ASSERT_LT(0, it++) << "Iterator found non-zero value in initialized array";
        (*bit_array)[151] = true;
        ASSERT_TRUE((*bit_array)[151]) << "Failed to write in array at pos 151";
        it = bit_array->iterator_word();
        ASSERT_EQ(ROUND_DOWN(151, 16), it++) << "Iterator don't stop on non-zero value";
        (*bit_array)[350] = true;
        ASSERT_TRUE((*bit_array)[350]) << "Failed to write in array at pos 350";
        it = bit_array->iterator_word();
        ASSERT_EQ(ROUND_DOWN(151, 16), it++) << "Iterator don't stop on 1st non-zero value";
        EXPECT_EQ(ROUND_DOWN(350, 16), it++) << "Iterator don't stop on second non-zero value";
    }

    TEST(BitArrayTest, BitArrayIteratorLastElement) {
        auto bit_array = BitArray::Create((size_t) 570); //570 bits
        (*bit_array)[569] = true;
        ASSERT_TRUE((*bit_array)[569]) << "Failed to write in array at pos 569";
        auto it = bit_array->iterator_word();
        ASSERT_LE(570, it++) << "Iterator gives the last elements who do not fill a full 16-element chunk";
    }

    TEST(BitArrayTest, BitArrayAND) {
        auto bit_arrayA = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayB = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayC = BitArray::Create((size_t) 100); //100 bits
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
        auto bit_arrayA = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayB = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayC = BitArray::Create((size_t) 100); //100 bits
        EXPECT_ANY_THROW(auto fail = *bit_arrayA | *bit_arrayC) << "Arrays of different size cannot be ANDed";
        (*bit_arrayA)[100] = true;
        (*bit_arrayB)[300] = true;
        auto result = *bit_arrayA | *bit_arrayB;
        ASSERT_TRUE(result[100]) << "true | false == false !";
        ASSERT_TRUE(result[300]) << "false | true == true !";
    }

    TEST(BitArrayTest, BitArrayXOR) {
        auto bit_arrayA = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayB = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayC = BitArray::Create((size_t) 100); //100 bits
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
        auto bit_arrayA = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayB = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayC = BitArray::Create((size_t) 100); //100 bits
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
        auto bit_arrayA = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayB = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayC = BitArray::Create((size_t) 100); //100 bits
        EXPECT_ANY_THROW(*bit_arrayA |= *bit_arrayC) << "Arrays of different size cannot be ANDed";
        (*bit_arrayA)[100] = true;
        (*bit_arrayB)[300] = true;
        *bit_arrayA |= *bit_arrayB;
        ASSERT_TRUE((*bit_arrayA)[100]) << "true | false == false !";
        ASSERT_TRUE((*bit_arrayA)[300]) << "false | true == true !";
    }

    TEST(BitArrayTest, BitArrayXORcompound) {
        auto bit_arrayA = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayB = BitArray::Create((size_t) 569); //569 bits
        auto bit_arrayC = BitArray::Create((size_t) 100); //100 bits
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
        auto bit_arrayA = BitArray::Create((size_t) 569); //569 bits
        (*bit_arrayA)[151] = true;
        *bit_arrayA = ~*bit_arrayA;
        ASSERT_FALSE((*bit_arrayA)[151]) << "~true == true !";
        ASSERT_TRUE((*bit_arrayA)[100]) << "~false == false !";
    }

    TEST(BitArrayTest, BitArrayCount) {
        auto bit_array = BitArray::Create((size_t) 573); //573 bits
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
}

#endif // BITARRAYTEST_H_INCLUDED
