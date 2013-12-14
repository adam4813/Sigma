#ifndef WORLDPOSITIONTEST_H_INCLUDED
#define WORLDPOSITIONTEST_H_INCLUDED

#include "components/WorldPosition.h"
#include "components/BitArray.h"
#include <stdexcept>
#include <random>
#include <chrono>
#include <forward_list>
#include <memory>
#include <bitset>


#include "gtest/gtest.h"

    class WorldPositionTest: public ::testing::Test {
    protected:
        virtual void SetUp() {};

        int FillUntilResize(Sigma::WorldPosition* wp, int n = 16) {
            int index = -1;
            auto start = wp->All_x()->data();
            witness.clear();
            stored_x.clear();
            stored_y.clear();
            stored_z.clear();
            while (wp->All_x()->data() == start) {
                auto x = next();
                auto y = next();
                auto z = next();
                wp->PositionWrite_x(++index) = x;
                wp->PositionWrite_y(index) = y;
                wp->PositionWrite_z(index) = z;
                stored_x.push_back(wp->x(index));
                stored_y.push_back(wp->y(index));
                stored_z.push_back(wp->z(index));
                witness.push_back(Sigma::position_type(x, y, z));
            }
            return index;
        }

        void FillRandom(std::vector<Sigma::coordinate_type>& v, const size_t size) {
            for (size_t i = 0; i < size; i++) {
                v.push_back(next()/10000);
            }
        }

        Sigma::coordinate_type next() {
            return ((Sigma::coordinate_type) (random() - (random.max()>> 1)));
        }

        void PopulateBitmap(const size_t vec_size, const size_t bitarray_size) {
            for (auto i = 0; i < vec_size; i++) {
                bitmaps.push_back(Sigma::BitArray::Create(bitarray_size));
            }
        }


        Sigma::WorldPosition wp256;     // default capacity is 256 elements
        Sigma::WorldPosition wp4096{10000};     // initial capacity of 4096 elements
        Sigma::WorldPosition wp512{10000};     // initial capacity of 512 elements
        Sigma::position_type original = Sigma::position_type(1.0, 2.0, 3.0);
        Sigma::WorldPosition wp16{16}; // initial capacity of 16 elements
        std::vector<Sigma::position_type> witness;
        std::vector<std::weak_ptr<const Sigma::coordinate_type>> stored_x;
        std::vector<std::weak_ptr<const Sigma::coordinate_type>> stored_y;
        std::vector<std::weak_ptr<const Sigma::coordinate_type>> stored_z;
        unsigned long int seed = (unsigned long int) std::chrono::system_clock::now().time_since_epoch().count();
        std::default_random_engine random{seed};
        std::vector<Sigma::coordinate_type> translation_x;
        std::vector<Sigma::coordinate_type> translation_y;
        std::vector<Sigma::coordinate_type> translation_z;
        std::vector<std::shared_ptr<Sigma::BitArray>> bitmaps;
    };

using Sigma::WorldPosition;

namespace Sigma {
    TEST_F(WorldPositionTest, WorldPositionBasicX) {
        EXPECT_THROW(wp256.x(0).expired(), std::out_of_range) << "Entity 0 should not exist";
        ASSERT_NO_THROW(wp256.PositionWrite_x(0) = original.x) << "Failed to create element";
        ASSERT_FALSE(wp256.x(0).expired()) << "Failed to fetch element created";
        EXPECT_EQ((size_t) wp256.x(0).lock().get() & (size_t)0xF, 0) << "Vector is not 16-byte aligned";
        EXPECT_EQ(wp256.x(0), original.x) << "Position retrieved is different";
        wp256.PositionWrite_y(0) = original.y;
        wp256.PositionWrite_z(0) = original.z;
        EXPECT_NO_THROW(wp256.RemoveEntityPosition(0)) << "Could not remove element";
        EXPECT_THROW(wp256.x(0).expired(), std::out_of_range) << "Fail to remove element";
    }

    TEST_F(WorldPositionTest, WorldPositionFill) {
        auto index = FillUntilResize(&wp256);
        EXPECT_EQ(index, 256) << "Resizing occurred before insertion of the 256th element";
        index = FillUntilResize(&wp16);
        EXPECT_EQ(index, 16) << "Resizing occurred before insertion of the 16th element";
        for (unsigned int i = 0; i <= index ; i++) {
            ASSERT_FALSE(wp16.x(i).expired()) << "Element x " << i << " has expired";
            ASSERT_FALSE(wp16.y(i).expired()) << "Element y " << i << " has expired";
            ASSERT_FALSE(wp16.z(i).expired()) << "Element z " << i << " has expired";
            ASSERT_TRUE(stored_x[i].expired() || stored_x[i].lock().get() == wp16.x(i).lock().get()) << "Stored weak_ptr x " << i << " has not expired";
            ASSERT_TRUE(stored_y[i].expired() || stored_y[i].lock().get() == wp16.y(i).lock().get()) << "Stored weak_ptr y " << i << " has not expired";
            ASSERT_TRUE(stored_z[i].expired() || stored_z[i].lock().get() == wp16.z(i).lock().get()) << "Stored weak_ptr z " << i << " has not expired";
            EXPECT_EQ(wp16.x(i), witness[i].x) << "Element x " << i << " is not equal after resize";
            EXPECT_EQ(wp16.y(i), witness[i].y) << "Element y " << i << " is not equal after resize";
            EXPECT_EQ(wp16.z(i), witness[i].z) << "Element z " << i << " is not equal after resize";
        }
    }

    TEST_F(WorldPositionTest, WorldPositionOperations) {
        auto index = FillUntilResize(&wp256);
        index++;
        ASSERT_NO_THROW(wp256.PositionWrite_x(index) = original.x) << "Failed to create x element";
        ASSERT_NO_THROW(wp256.PositionWrite_y(index) = original.y) << "Failed to create y element";
        ASSERT_NO_THROW(wp256.PositionWrite_z(index) = original.z) << "Failed to create z element";
        ASSERT_FALSE(wp256.x(index).expired()) << "Failed to fetch x element created";
        ASSERT_FALSE(wp256.y(index).expired()) << "Failed to fetch y element created";
        ASSERT_FALSE(wp256.z(index).expired()) << "Failed to fetch z element created";
        EXPECT_EQ(wp256.x(index), original.x) << "x Position retrieved is different";
        EXPECT_EQ(wp256.y(index), original.y) << "y Position retrieved is different";
        EXPECT_EQ(wp256.z(index), original.z) << "z Position retrieved is different";
        auto ptr_x = wp256.x(index);
        auto ptr_y = wp256.y(index);
        auto ptr_z = wp256.z(index);
        EXPECT_FALSE(ptr_x.expired()) << "stored x weak_ptr has yet expired, should be still valid";
        EXPECT_FALSE(ptr_y.expired()) << "stored y weak_ptr has yet expired, should be still valid";
        EXPECT_FALSE(ptr_z.expired()) << "stored z weak_ptr has yet expired, should be still valid";
        EXPECT_NO_THROW(wp256.RemoveEntityPosition(index)) << "Could not remove element";
        EXPECT_THROW(wp256.x(index).expired(), std::out_of_range) << "Failed to remove x element";
        EXPECT_THROW(wp256.y(index).expired(), std::out_of_range) << "Failed to remove y element";
        EXPECT_THROW(wp256.z(index).expired(), std::out_of_range) << "Failed to remove z element";
        EXPECT_TRUE(ptr_x.expired()) << "stored x weak_ptr has not expired";
        EXPECT_TRUE(ptr_y.expired()) << "stored y weak_ptr has not expired";
        EXPECT_TRUE(ptr_z.expired()) << "stored z weak_ptr has not expired";
    }

    TEST_F(WorldPositionTest, WorldPositionCopyElement) {
        auto index = FillUntilResize(&wp256, 32);
        index++;
        wp256.PositionWrite_x(index) = wp256.x(12);
        ASSERT_EQ(wp256.x(index), wp256.x(12)) << "Failed to copy element";
    }

    TEST_F(WorldPositionTest, WorldPositionMoveElement) {
        auto index = FillUntilResize(&wp256, 32);
        index++;
        wp256.PositionWrite_x(index) = 1.0;
        const coordinate_type x = wp256.x(index);
        ASSERT_EQ(x, 1.0) << "Failed to move element";
    }

    TEST_F(WorldPositionTest, WorldPositionRelative) {
        FillUntilResize(&wp4096);
        FillRandom(translation_x, 5000);
        FillRandom(translation_y, 5000);
        FillRandom(translation_z, 5000);
        std::unique_ptr<position_array> w;
        auto bm = BitArray::Create((size_t) 5000);
        using namespace std::chrono;
        steady_clock::time_point t1 = steady_clock::now();
        for (int i =0; i < 5000; i++) {
            bm = wp4096.InViewPositions(translation_x[i], translation_y[i], translation_z[i], FAR_DISTANCE_MAX);
            w = wp4096.RelativeTo(translation_x[i], translation_y[i], translation_z[i]);
        }
        steady_clock::time_point t2 = steady_clock::now();
        duration<double> time_span = duration_cast<duration<double>>(t2 - t1);

        for(int i = 0; i < w->length; i++) {
                if ((*bm)[i]) {
                    EXPECT_EQ((w->x.get())[i],(float)((*wp4096.All_x())[i] - translation_x[4999])) << "Translation error";
                    EXPECT_GT(FAR_DISTANCE_MAX, fabs((w->x.get())[i])) << "View distance error at " << i;
                }
                else {
                    EXPECT_TRUE(FAR_DISTANCE_MAX < fabs((*wp4096.All_x())[i] - translation_x[4999])\
                              || FAR_DISTANCE_MAX < fabs((*wp4096.All_y())[i] - translation_y[4999])\
                              || FAR_DISTANCE_MAX < fabs((*wp4096.All_z())[i] - translation_z[4999])) << "View distance error at " << i;
                }
        }
        std::cout << w->length * 5000 / time_span.count() << " entities/s." << std::endl;
    }
}
#endif // WORLDPOSITIONTEST_H_INCLUDED
