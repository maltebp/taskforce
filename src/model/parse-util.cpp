#include "parse-util.hpp"


namespace tf {

    std::optional<std::chrono::minutes> parse_util::parse_hours_minutes(const std::string& text_to_parse) {
        if( text_to_parse.size() == 0 ) return std::chrono::minutes(0);

        std::string cleaned_input = text_to_parse;
        std::string::iterator new_end = std::remove_if(cleaned_input.begin(), cleaned_input.end(), [](int i) -> int { return std::isspace(i); });
        cleaned_input.erase(new_end, cleaned_input.end());

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