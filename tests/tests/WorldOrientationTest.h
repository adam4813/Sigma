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
                wp->OrientationWrite(++index) = v;
                stored.push_back((*wp)(index));
                witness.push_back(v);
            }
            return index;
        }


        Sigma::WorldOrientation wp0;
        Sigma::orientation_type original = Sigma::orientation_type(1.0, 2.0, 3.0);
        Sigma::WorldOrientation wp1;
        std::vector<Sigma::orientation_type> witness;
        std::vector<std::weak_ptr<const Sigma::orientation_type>> stored;
    };

using Sigma::WorldOrientation;

namespace Sigma {


    TEST_F(WorldOrientationTest, WorldOrientationBasic) {
        EXPECT_THROW(wp0(0).expired(), std::out_of_range) << "Entity 0 should not exist";
        ASSERT_NO_THROW(wp0.OrientationWrite(0) = original) << "Failed to create element";
        ASSERT_FALSE(wp0(0).expired()) << "Failed to fetch element created";
        EXPECT_EQ(*wp0(0).lock(), original) << "Orientation retrieved is different";
        EXPECT_NO_THROW(wp0.RemoveEntityOrientation(0)) << "Could not remove element";
        EXPECT_THROW(wp0(0).expired(), std::out_of_range) << "Fail to remove element";
    }

    TEST_F(WorldOrientationTest, WorldOrientationFill) {
        auto index = Fill(&wp1);

        for (unsigned int i = 0; i <= index ; i++) {
            ASSERT_FALSE(wp1(i).expired()) << "Element " << i << " has expired";
            ASSERT_TRUE(stored[i].expired() || stored[i].lock().get() == wp1(i).lock().get()) << "Stored weak_ptr " << i << " has not expired";
            EXPECT_EQ(*wp1(i).lock(), witness[i]) << "Element " << i << " is not equal after resize";
        }
    }

    TEST_F(WorldOrientationTest, WorldOrientationOperations) {
        auto index = Fill(&wp0);
        index++;
        ASSERT_NO_THROW(wp0.OrientationWrite(index) = original) << "Failed to create element";
        ASSERT_FALSE(wp0(index).expired()) << "Failed to fetch element created";
        EXPECT_EQ(*wp0(index).lock(), original) << "Orientation retrieved is different";
        auto ptr = wp0(index);
        EXPECT_FALSE(ptr.expired()) << "stored weak_ptr has yet expired, should be still valid";
        EXPECT_NO_THROW(wp0.RemoveEntityOrientation(index)) << "Could not remove element";
        EXPECT_THROW(wp0(index).expired(), std::out_of_range) << "Failed to remove element";
        EXPECT_TRUE(ptr.expired()) << "stored weak_ptr has not expired";
    }

    TEST_F(WorldOrientationTest, WorldOrientationCopyElement) {
        auto index = Fill(&wp0, 32);
        index++;
        wp0.OrientationWrite(index) = *wp0(12).lock();
        ASSERT_EQ(*wp0(index).lock(), *wp0(12).lock()) << "Failed to copy element";
    }

    TEST_F(WorldOrientationTest, WorldOrientationMoveElement) {
        auto index = Fill(&wp0, 32);
        index++;
        wp0.OrientationWrite(index) = Sigma::orientation_type(1.0, 2.0, 3.0);
        ASSERT_EQ(*wp0(index).lock(), Sigma::orientation_type(1.0, 2.0, 3.0)) << "Failed to move element";
    }
}

#endif // WORLDPOSITIONTEST_H_INCLUDED
