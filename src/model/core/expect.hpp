#pragma once

#include <functional>
#include <format>
#include <source_location>
#include <string_view>

#include "core/expect_handling.hpp"

namespace tf {

    namespace internal {

        /**
         * @brief This should not be constructed manually but only used implicitly within \ref tf::expect()
        */
        struct FormatWithLocation {
            const char* message;
            std::source_location source_location;

            FormatWithLocation(
                const char* message,
                const std::source_location& source_location = std::source_location::current()
            ) 
                :   message(message), 
                    source_location(source_location) { }
        };
    }

    template<typename ... TArgs>
    static void expect(bool expectedValue, std::source_location source_location = {})
    {
        if( expectedValue ) return;

        ExpectInfo expect_info{ "", source_location };
        tf::internal::report_expect(expect_info);
    }

    template<typename ... TArgs>
    [[noreturn]] static void expect(
        bool expected,
        internal::FormatWithLocation formatWithLocation,
        TArgs... format_args)
    {
        if( expected ) return;

        std::ostringstream formatted_message_stream{};
        formatted_message_stream << std::vformat(formatWithLocation.message, std::make_format_args(format_args...));
        std::string formatted_message = formatted_message_stream.str();

        ExpectInfo expect_info{ formatted_message, formatWithLocation.source_location };
        tf::internal::report_expect(expect_info);
    }

}