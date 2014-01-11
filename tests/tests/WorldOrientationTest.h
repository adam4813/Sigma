#ifndef WORLDORIENTATIONTEST_H_INCLUDED
#define WORLDORIENTATIONTEST_H_INCLUDED

#include "components/WorldOrientation.h"
#include <stdexcept>
#include <random>

#include "gtest/gtest.h"

    class WorldOrientationTest: public ::testing::Test {
    protected:
        virtual void SetUp() {};

        int Fill(Sigma::WorldOrientation* wp, int n = 16) {
            std::minstd_rand0 random;
            int index = -1;
            witness.empty();
            stored.empty();
            while (index < n) {
                auto v = Sigma::orientation_type(static_cast<float> (random()), static_cast<float> (random()), static_cast<float> (random()));
                (*wp)[++index] = v;
                stored.push_back(wp->at(index));
                witness.push_back(v);
            }
            return index;
        }


        Sigma::WorldOrientation wp0;
        Sigma::orientation_type original = Sigma::orientation_type(1.0, 2.0, 3.0);
        Sigma::WorldOrientation wp1;
        std::vector<Sigma::orientation_type> witness;
        std::vector<std::weak_ptr<Sigma::orientation_type>> stored;
    };

using Sigma::WorldOrientation;

namespace Sigma {


    TEST_F(WorldOrientationTest, WorldOrientationBasic) {
        EXPECT_THROW(wp0.at(0).expired(), std::out_of_range) << "Entity 0 should not exist";
        ASSERT_NO_THROW(wp0[0] = original) << "Failed to create element";
        ASSERT_FALSE(wp0.at(0).expired()) << "Failed to fetch element created";
        EXPECT_EQ(original, wp0.at(0)) << "Orientation retrieved is different";
        EXPECT_NO_THROW(wp0.RemoveEntityOrientation(0)) << "Could not remove element";
        EXPECT_THROW(wp0.at(0).expired(), std::out_of_range) << "Fail to remove element";
    }

    TEST_F(WorldOrientationTest, WorldOrientationFill) {
        auto index = Fill(&wp1);

        for (unsigned int i = 0; i <= index ; i++) {
            ASSERT_FALSE(wp1.at(i).expired()) << "Element " << i << " has expired";
            ASSERT_TRUE(stored[i].expired() || stored[i].lock().get() == wp1.at(i).lock().get()) << "Stored weak_ptr " << i << " has not expired";
            EXPECT_EQ(witness[i], wp1.at(i)) << "Element " << i << " is not equal after resize";
        }
    }

    TEST_F(WorldOrientationTest, WorldOrientationOperations) {
        auto index = Fill(&wp0);
        index++;
        ASSERT_NO_THROW(wp0[index] = original) << "Failed to create element";
        ASSERT_FALSE(wp0.at(index).expired()) << "Failed to fetch element created";
        EXPECT_EQ(original, wp0.at(index)) << "Orientation retrieved is different";
        auto ptr = wp0.at(index);
        EXPECT_FALSE(ptr.expired()) << "stored weak_ptr has yet expired, should be still valid";
        EXPECT_NO_THROW(wp0.RemoveEntityOrientation(index)) << "Could not remove element";
        EXPECT_THROW(wp0.at(index).expired(), std::out_of_range) << "Failed to remove element";
        EXPECT_TRUE(ptr.expired()) << "stored weak_ptr has not expired";
    }

    TEST_F(WorldOrientationTest, WorldOrientationCopyElement) {
        auto index = Fill(&wp0, 32);
        index++;
        wp0[index] = wp0.at(12);
        ASSERT_EQ((orientation_type) wp0.at(index), (orientation_type) wp0.at(12)) << "Failed to copy element";
    }

    TEST_F(WorldOrientationTest, WorldOrientationMoveElement) {
        auto index = Fill(&wp0, 32);
        index++;
        wp0[index] = Sigma::orientation_type(1.0, 2.0, 3.0);
        ASSERT_EQ(Sigma::orientation_type(1.0, 2.0, 3.0), (orientation_type) wp0.at(index)) << "Failed to move element";
    }
}

#endif // WORLDPOSITIONTEST_H_INCLUDED
