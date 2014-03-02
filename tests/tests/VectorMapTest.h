#ifndef VECTORMAPTEST_H_INCLUDED
#define VECTORMAPTEST_H_INCLUDED

#include "VectorMap.hpp"
#include <stdexcept>
#include <random>
#include <chrono>
#include <memory>


#include "gtest/gtest.h"

    class VectorMapTest: public ::testing::Test {
	public:
		VectorMapTest() : wp16(16), seed((unsigned long int) std::chrono::system_clock::now().time_since_epoch().count()), random(seed) {
			this->original = 1.0;
		}
    protected:
        virtual void SetUp() {};

        int FillUntilResize(Sigma::VectorMap<unsigned int, double>* wp, int n = 16) {
            int index = -1;
            auto start = wp->getVector()->data();
            witness.clear();
            stored.clear();
            while (wp->getVector()->data() == start) {
                auto x = next();
                (*wp)[++index] = x;
                stored.push_back(wp->at(index));
                witness.push_back(x);
            }
            return index;
        }

        void FillRandom(std::vector<double>& v, const size_t size) {
            for (size_t i = 0; i < size; i++) {
                v.push_back(next()/10000);
            }
        }

        double next() {
            return ((double) (random() - (random.max()>> 1)));
        }

        Sigma::VectorMap<unsigned int, double> wp256;     // default capacity is 256 elements
        double original;
        Sigma::VectorMap<unsigned int, double> wp16; // initial capacity of 16 elements
        std::vector<double> witness;
        std::vector<std::weak_ptr<double>> stored;
        unsigned long int seed;
        std::default_random_engine random;
    };

using Sigma::VectorMap;

namespace Sigma {
    TEST_F(VectorMapTest, VectorMapBasic) {
        EXPECT_THROW(wp256.at(0).expired(), std::out_of_range) << "Element with key 0 should not exist";
        ASSERT_NO_THROW(wp256[0] = original) << "Failed to create element";
        ASSERT_FALSE(wp256.at(0).expired()) << "Failed to fetch element created";
        EXPECT_EQ((size_t) wp256.at(0).lock().get() & (size_t)0xF, 0) << "Vector is not 16-byte aligned";
        EXPECT_EQ(wp256.at(0), original) << "Element retrieved is different";
        EXPECT_NO_THROW(wp256.clear(0)) << "Could not remove element";
        EXPECT_THROW(wp256.at(0).expired(), std::out_of_range) << "Fail to remove element";
    }

    TEST_F(VectorMapTest, VectorMapFill) {
        auto index = FillUntilResize(&wp256);
        EXPECT_EQ(index, 256) << "Resizing occurred before insertion of the 256th element";
        index = FillUntilResize(&wp16);
        EXPECT_EQ(index, 16) << "Resizing occurred before insertion of the 16th element";
        for (unsigned int i = 0; i <= index ; i++) {
            ASSERT_FALSE(wp16.at(i).expired()) << "Element " << i << " has expired";
            ASSERT_TRUE(stored[i].expired() || stored[i].lock().get() == wp16.at(i).lock().get()) << "Stored weak_ptr " << i << " has not expired";
            EXPECT_EQ(wp16.at(i), witness[i]) << "Element " << i << " is not equal after resize";
        }
    }

    TEST_F(VectorMapTest, VectorMapOperations) {
        auto index = FillUntilResize(&wp256);
        index++;
        ASSERT_NO_THROW(wp256[index] = original) << "Failed to create element";
        ASSERT_FALSE(wp256.at(index).expired()) << "Failed to fetch element created";
        EXPECT_EQ(wp256.at(index), original) << "Element retrieved is different";
        auto ptr = wp256.at(index);
        EXPECT_FALSE(ptr.expired()) << "stored weak_ptr has yet expired, should be still valid";
        EXPECT_NO_THROW(wp256.clear(index)) << "Could not remove element";
        EXPECT_THROW(wp256.at(index).expired(), std::out_of_range) << "Failed to remove element";
        EXPECT_TRUE(ptr.expired()) << "stored weak_ptr has not expired";
    }

    TEST_F(VectorMapTest, VectorMapCopyElement) {
        auto index = FillUntilResize(&wp256, 32);
        index++;
        wp256[index] = wp256.at(12);
        ASSERT_EQ(wp256.at(index), wp256.at(12)) << "Failed to copy element";
    }

    TEST_F(VectorMapTest, VectorMapMoveElement) {
        auto index = FillUntilResize(&wp256, 32);
        index++;
        wp256[index] = 1.0;
        const double x = wp256.at(index);
        ASSERT_EQ(x, 1.0) << "Failed to move element";
    }
}
#endif // VECTORMAPTEST_H_INCLUDED
