#include <gtest/gtest.h>

#include "parse-util.hpp"

TEST(parse_hours_minutes, only_minutes) {

	std::optional<std::chrono::minutes> result = tf::parse_util::parse_hours_minutes("00:34");

	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(result.value().count(), 34);
}

TEST(parse_hours_minutes, only_hours) {

	std::optional<std::chrono::minutes> result = tf::parse_util::parse_hours_minutes("04:00");

	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(result.value().count(), 240);
}

TEST(parse_hours_minutes, hours_and_minutes) {

	std::optional<std::chrono::minutes> result = tf::parse_util::parse_hours_minutes("04:34");

	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(result.value().count(), 274);
}

TEST(parse_hours_minutes, empty_input) {

	std::optional<std::chrono::minutes> result = tf::parse_util::parse_hours_minutes("");

	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(result.value().count(), 0);
}

TEST(parse_hours_minutes, invalid_minutes) {

	std::optional<std::chrono::minutes> result = tf::parse_util::parse_hours_minutes("00:61");

	ASSERT_FALSE(result.has_value());
}

TEST(parse_hours_minutes, invalid_hours) {

	std::optional<std::chrono::minutes> result = tf::parse_util::parse_hours_minutes("25:00");

	ASSERT_FALSE(result.has_value());
}

TEST(parse_hours_minutes, invalid_characters) {

	std::optional<std::chrono::minutes> result = tf::parse_util::parse_hours_minutes("a!$");
	ASSERT_FALSE(result.has_value());
}

TEST(parse_year_month_day, valid_date) {
	std::optional<std::chrono::year_month_day> result = tf::parse_util::parse_year_month_day("25-09-12");
	ASSERT_TRUE(result.has_value());
	ASSERT_EQ(result.value(), std::chrono::year_month_day(std::chrono::September / 12 / 2025));
}

TEST(parse_year_month_day, invalid_day) {
	std::optional<std::chrono::year_month_day> result = tf::parse_util::parse_year_month_day("25-09-32");
	ASSERT_FALSE(result.has_value());
}

TEST(parse_year_month_day, invalid_month) {
	std::optional<std::chrono::year_month_day> result = tf::parse_util::parse_year_month_day("25-13-12");
	ASSERT_FALSE(result.has_value());
}

TEST(parse_year_month_day, invalid_characters) {
	std::optional<std::chrono::year_month_day> result = tf::parse_util::parse_year_month_day("a!$");
	ASSERT_FALSE(result.has_value());
}

TEST(parse_year_month_day, empty) {
	std::optional<std::chrono::year_month_day> result = tf::parse_util::parse_year_month_day("");
	ASSERT_FALSE(result.has_value());
}

TEST(parse_year_month_day, leap_year) {
	std::optional<std::chrono::year_month_day> result1 = tf::parse_util::parse_year_month_day("25-02-29");
	ASSERT_FALSE(result1.has_value());

	std::optional<std::chrono::year_month_day> result2 = tf::parse_util::parse_year_month_day("28-02-29");
	ASSERT_TRUE(result2.has_value());
	ASSERT_EQ(result2.value(), std::chrono::year(2028)/2/29);
}

TEST(parse_year_month_day, invalid_month_day_combination) {
	std::optional<std::chrono::year_month_day> result = tf::parse_util::parse_year_month_day("25-04-31");
	ASSERT_FALSE(result.has_value());
}