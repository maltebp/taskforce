#pragma once

#include <string_view>
#include <optional>
#include <chrono>

namespace tf::parse_util {

	std::optional<std::chrono::minutes> parse_hours_minutes(std::string_view text_to_parse);

	std::optional<std::chrono::year_month_day> parse_year_month_day(std::string_view text_to_parse);

}