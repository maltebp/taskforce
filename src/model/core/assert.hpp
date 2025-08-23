#pragma once

#include <cstdlib>
#include <format>
#include <iostream>
#include <sstream>
#include <source_location>
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#define TF_DEBUG_BREAK() DebugBreak()
#else
#define TF_DEBUG_BREAK()
#endif

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
    [[noreturn]] static void assert(
        bool assertedValue,
        internal::FormatWithLocation formatWithLocation,
        TArgs... format_args)
    {
        if( assertedValue ) return;

        std::ostringstream message;
        message << std::vformat(
            std::string("Assert occured in {} at {}:{},{}: ") + formatWithLocation.message,
            std::make_format_args(
                formatWithLocation.source_location.file_name(),
                formatWithLocation.source_location.function_name(),
                formatWithLocation.source_location.line(),
                formatWithLocation.source_location.column(),
                format_args...)
        );

        std::cerr << message.str() << std::endl;

        TF_DEBUG_BREAK();

        std::exit(-1);
    }

}