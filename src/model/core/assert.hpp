#pragma once

#include <functional>
#include <format>
#include <source_location>
#include <string_view>

#include "core/assert_handling.hpp"

// TODO: Rename this to expect

namespace tf {

    namespace internal {

        /**
         * @brief This should not be constructed manually but only used implicitly within \ref assert()
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
    [[noreturn]] static void expect(bool assertedValue, std::source_location source_location = {})
    {
        if( assertedValue ) return;

        AssertInfo assert_info{ "", source_location };
        tf::internal::report_assert(assert_info);
    }

    template<typename ... TArgs>
    [[noreturn]] static void expect(
        bool assertedValue,
        internal::FormatWithLocation formatWithLocation,
        TArgs... format_args)
    {
        if( assertedValue ) return;

        std::ostringstream formatted_message_stream{};
        formatted_message_stream << std::vformat(formatWithLocation.message, std::make_format_args(format_args...));
        std::string formatted_message = formatted_message_stream.str();

        AssertInfo assert_info{ formatted_message, formatWithLocation.source_location };
        tf::internal::report_assert(assert_info);
    }

}