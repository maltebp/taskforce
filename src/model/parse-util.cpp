#include "parse-util.hpp"

#include "core/string-util.hpp"


namespace tf::parse_util {

    std::optional<std::chrono::minutes> parse_hours_minutes(std::string_view text_to_parse) {
        
        if( text_to_parse.size() == 0 ) return std::chrono::minutes(0);

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

    std::optional<std::chrono::year_month_day> parse_year_month_day(std::string_view text_to_parse) {
        
        if( text_to_parse.size() == 0 ) return {};

        std::string cleaned_input = tf::string_util::remove_whitespace(text_to_parse);

        std::size_t first_dash_index = cleaned_input.find('-');
        if( first_dash_index == std::string::npos ) return {};

        std::size_t second_dash_index = cleaned_input.find('-', first_dash_index + 1);
        if( second_dash_index == std::string::npos ) return {};

        std::string year_string = cleaned_input.substr(0, first_dash_index);
        if( year_string.size() == 0 ) return {};

        std::string month_string = cleaned_input.substr(first_dash_index + 1, second_dash_index);
        if( month_string.size() == 0 ) return {};

        std::string day_string = cleaned_input.substr(second_dash_index + 1);
        if( day_string.size() == 0 ) return {};

        try {

            int year = std::stoi(year_string);
            if( year < 0 ) return {};
            if( year < 100 ) {
                year += 2000;
            }

            int month = std::stoi(month_string);
            if( month < 1 || month > 12 ) return {};

            int day = std::stoi(day_string);
            if( month < 1 || month > 31 ) return {};

            std::chrono::year_month_day result{ std::chrono::year(year) / month / day };

            if( !result.ok() ) return {};

            return result;
        }
        catch( std::invalid_argument e ) {
            return {};
        }
    }

}