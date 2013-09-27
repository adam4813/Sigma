#pragma once

#include "Property.h"
#include <string>

namespace {
	// Basic move and copy tests with POD int
	TEST(PropertyTest, PropertyName) {
		const std::string name = "PropertyTestName";
		int testINT = 10;
		Property p(name, testINT);
		EXPECT_EQ(name, p.GetName());
	}
	TEST(PropertyTest, PropertyMovePOD) {
		const std::string name = "PropertyTestName";
		int testINT = 10;
		Property p(name, testINT);
		Property moved_P = std::move(p);
		EXPECT_EQ(10, moved_P.Get<int>());
	}
	TEST(PropertyTest, PropertyCopyPOD) {
		const std::string name = "PropertyTestName";
		int testINT = 10;
		Property p(name, testINT);
		Property copied_P(p);
		EXPECT_EQ(10, copied_P.Get<int>());
	}

	// Testing move and copy with complex type
	TEST(PropertyTest, PropertyMoveComplex) {
		const std::string name = "PropertyTestName";
		std::vector<int> testVec;
		testVec.push_back(10);
		Property p(name, testVec);
		Property moved_P = std::move(p);
		EXPECT_EQ(10, moved_P.Get<std::vector<int>>()[0]);
	}
	TEST(PropertyTest, PropertyCopyComplex) {
		const std::string name = "PropertyTestName";
		std::vector<int> testVec;
		testVec.push_back(10);
		Property p(name, testVec);
		Property copied_P(p);
		EXPECT_EQ(10, copied_P.Get<std::vector<int>>()[0]);
	}

	// Advanced move and copy tests using pointers
	TEST(PropertyTest, PropertyMovePTR) {
		const std::string name = "PropertyTestName";
		int* testINT = new int();
		*testINT = 10;
		Property p(name, testINT);
		Property moved_P = std::move(p);
		EXPECT_EQ(10, *moved_P.Get<int*>());
		delete testINT;
	}
	TEST(PropertyTest, PropertyCopyPTR) {
		const std::string name = "PropertyTestName";
		int* testINT = new int();
		*testINT = 10;
		Property p(name, testINT);
		Property copied_P(p);
		EXPECT_EQ(10, *copied_P.Get<int*>());
		delete testINT;
	}

	// Advanced move and copy tests using pointers
	// The pointer is deleted early and as such the value held is now invalid
	TEST(PropertyTest, PropertyMovePTREarlyDelete) {
		const std::string name = "PropertyTestName";
		int* testINT = new int();
		*testINT = 10;
		Property p(name, testINT);
		Property moved_P = std::move(p);
		delete testINT;
		EXPECT_NE(10,*moved_P.Get<int*>());
	}
	TEST(PropertyTest, PropertyCopyPTREarlyDelete) {
		const std::string name = "PropertyTestName";
		int* testINT = new int();
		*testINT = 10;
		Property p(name, testINT);
		Property copied_P(p);
		delete testINT;
		EXPECT_NE(10, *copied_P.Get<int*>());
	}

	// Testing get without a different type than the set
	TEST(PropertyTest, PropertyGetPODDifferentType) {
		const std::string name = "PropertyTestName";

		// First we make sure storing and retrieving the complex type works.
		std::vector<int> testVec;
		testVec.push_back(10);
		Property p(name, testVec);
		EXPECT_NO_THROW(p.Get<std::vector<int>>());

		// Next we will try to retrieve the complex type when we only stored a POD.
		int testINT = 10;
		Property p2(name, testINT);
		EXPECT_ANY_THROW(p2.Get<std::vector<int>>());
	}
	TEST(PropertyTest, PropertyGetPTRDifferentType) {
		const std::string name = "PropertyTestName";

		// First we make sure storing and retrieving the complex type works.
		std::vector<int>* testVec = new std::vector<int>();
		testVec->push_back(10);
		Property p(name, testVec);
		EXPECT_NO_THROW(p.Get<std::vector<int>*>());
		delete testVec;

		// Next we will try to retrieve the complex type when we only stored a POD.
		int* testINT = new int();
		*testINT = 10;
		Property p2(name, testINT);
		EXPECT_ANY_THROW(p2.Get<std::vector<int>*>()->push_back(10));
		delete testINT;
	}
}  // namespace