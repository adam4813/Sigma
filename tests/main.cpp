#include "gtest/gtest.h"
#include "tests/EntityManagerTest.h"
#include "tests/WorldOrientationTest.h"
#include "tests/BitArrayTest.h"
#include "tests/WorldPositionTest.h"
#include "tests/PropertyTest.h"

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	int ret = RUN_ALL_TESTS();
	system("Pause");
	return ret;
}
