#include "parse-util.hpp"

#include "core/string-util.hpp"


namespace tf::parse_util {

    std::optional<std::chrono::minutes> parse_hours_minutes(std::string_view text_to_parse) {
        
        if( text_to_parse.size() == 0 ) return std::nullopt;

        std::string cleaned_input = tf::string_util::remove_whitespace(text_to_parse);
        
        std::size_t colon_index = cleaned_input.find(':');
        std::string hours_string = cleaned_input.substr(0, colon_index);

        try {

            int hours = hours_string.size() == 0 ? 0 : std::stoi(hours_string);
            if (hours < 0 || hours > 24) return {};

            if (hours_string.size() == cleaned_input.size()) return std::chrono::minutes(hours * 60);

            std::string minutes_string = cleaned_input.substr(colon_index + 1);

            int minutes = minutes_string.size() == 0 ? 0 : std::stoi(minutes_string);
            if (minutes < 0 || minutes > 60) return {};

            return std::chrono::minutes(hours * 60 + minutes);
        }
        catch (std::invalid_argument e) {
            return {};
        }        
	}

}