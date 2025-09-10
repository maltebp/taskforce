#include <gtest/gtest.h>

#include "parse-util.hpp"

TEST(SomeTestSuite, MyTest) {

	std::optional<std::chrono::minutes> result = tf::parse_util::parse_hours_minutes("00:34");

	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(result.value().count(), 34);
}