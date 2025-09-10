#include "core/string-util.hpp"

#include <algorithm>

namespace tf::string_util {

	std::string remove_whitespace(std::string_view input) {
		
        if( input.size() == 0 ) return std::string{};

        std::string cleaned_input{ input };

        std::string::iterator new_end = std::remove_if(
            cleaned_input.begin(),
            cleaned_input.end(),
            [](int i) -> int { return std::isspace(i); });

        cleaned_input.erase(new_end, cleaned_input.end());

        return cleaned_input;
	}

}