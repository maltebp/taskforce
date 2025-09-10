#include <gtest/gtest.h>

#include "core/string-util.hpp"

TEST(remove_whitespace, whitespace_in_middle) {
	std::string cleaned_string = tf::string_util::remove_whitespace("hello world");
	ASSERT_EQ("helloworld", cleaned_string);
}

TEST(string_util, whitespace_at_end_and_middle) {
	std::string cleaned_string = tf::string_util::remove_whitespace("hello world ");
	ASSERT_EQ("helloworld", cleaned_string);
}

TEST(string_util, whitespace_at_beginning) {
	std::string cleaned_string = tf::string_util::remove_whitespace(" hello world");
	ASSERT_EQ("helloworld", cleaned_string);
}

TEST(string_util, empty_string) {
	std::string cleaned_string = tf::string_util::remove_whitespace("");
	ASSERT_EQ("", cleaned_string);
}

TEST(string_util, only_multiple_whitespace) {
	std::string cleaned_string = tf::string_util::remove_whitespace("  ");
	ASSERT_EQ("", cleaned_string);
}

TEST(string_util, various_spaces) {

	std::string cleaned_string = tf::string_util::remove_whitespace("a \nb\tc\n\rd");
	ASSERT_EQ("abcd", cleaned_string);
}