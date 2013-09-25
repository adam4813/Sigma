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
	TEST(PropertyPODTest, PropertyMove) {
		const std::string name = "PropertyTestName";
		int testINT = 10;
		Property p(name, testINT);
		Property moved_P = std::move(p);
		EXPECT_EQ(10, moved_P.Get<int>());
	}
	TEST(PropertyPODTest, PropertyCopy) {
		const std::string name = "PropertyTestName";
		int testINT = 10;
		Property p(name, testINT);
		Property copied_P(p);
		EXPECT_EQ(10, copied_P.Get<int>());
	}

	// Advanced move and copy tests using pointers
	TEST(PropertyPTRTest, PropertyMove) {
		const std::string name = "PropertyTestName";
		int* testINT = new int();
		*testINT = 10;
		Property p(name, testINT);
		Property moved_P = std::move(p);
		EXPECT_EQ(10, *moved_P.Get<int*>());
		delete testINT;
	}
	TEST(PropertyPTRTest, PropertyCopy) {
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
	TEST(PropertyPTREarlyDeleteTest, PropertyMove) {
		const std::string name = "PropertyTestName";
		int* testINT = new int();
		*testINT = 10;
		Property p(name, testINT);
		Property moved_P = std::move(p);
		delete testINT;
		EXPECT_NE(10, *moved_P.Get<int*>());
	}
	TEST(PropertyPTREarlyDeleteTest, PropertyCopy) {
		const std::string name = "PropertyTestName";
		int* testINT = new int();
		*testINT = 10;
		Property p(name, testINT);
		Property copied_P(p);
		delete testINT;
		EXPECT_NE(10, *copied_P.Get<int*>());
	}
}  // namespace