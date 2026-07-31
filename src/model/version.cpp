#include "version.hpp"

#include "core/result.hpp"
#include <regex>
#include <string>


const std::regex number_regex{"^\\d$"};
const tf::Version tf::Version::FIRST = tf::Version(1, 0, 0);

[[nodiscard]] 
tf::Result<tf::Version> tf::Version::parse_from_string(
    std::string_view string_to_parse
) noexcept {

    uint32_t major = 0;
    
    size_t first_dot_index = string_to_parse.find('.');
    if (first_dot_index == std::string::npos) {
        // Parse whole string as major

        
        

    }

    

}