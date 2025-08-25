#pragma once

#include <functional>
#include <format>
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

    struct AssertInfo {
        std::string_view message;
        std::source_location& source_location;
    };

    /**
     * @brief The function which will be called to report an error. If a debugger is attached it 
     * will break after this is called, and exit with an error after that. By default it will 
     * just print the message to 
    */
    extern std::function<void(const AssertInfo&)> s_assert_report_function;

    template<typename ... TArgs>
    [[noreturn]] static void assert(
        bool assertedValue,
        internal::FormatWithLocation formatWithLocation,
        TArgs... format_args)
    {
        if( assertedValue ) return;

        std::ostringstream formattedMessage;
        formattedMessage << std::vformat(formatWithLocation.message, std::make_format_args(format_args...));

        if( s_assert_report_function != nullptr ) {
            AssertInfo assert_info{ formattedMessage.str(), formatWithLocation.source_location };
            s_assert_report_function(assert_info);
        }

        TF_DEBUG_BREAK();

        std::exit(-1);
    }

}