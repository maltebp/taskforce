#include <gtest/gtest.h>

TEST(MyTestSuite, MyTestName) {

	EXPECT_EQ(10, 10);
	EXPECT_EQ(15, 10);
	EXPECT_EQ(15, 10);
}

TEST(MyTestSuite, Test2) {

	EXPECT_EQ(10, 10);
}