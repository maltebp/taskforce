#pragma once

#include <string>
#include <optional>
#include <chrono>

namespace tf::parse_util {

	std::optional<std::chrono::minutes> parse_hours_minutes(const std::string& text_to_parse);

}