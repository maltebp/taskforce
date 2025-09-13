#include "assert.hpp"

#include <format>
#include <iostream>
#include <sstream>

#ifdef _WIN32
#include <windows.h>
#define TF_DEBUG_BREAK() DebugBreak()
#else
#define TF_DEBUG_BREAK()
#endif

namespace tf {

    std::function<void(const AssertInfo&)> s_assert_report_function = report_assert_to_cerr;

    void internal::report_assert(const AssertInfo& assert_info) {
        
        if( s_assert_report_function != nullptr ) {
            s_assert_report_function(assert_info);
        }

        TF_DEBUG_BREAK();

        std::exit(1);
    }

    void report_assert_to_cerr(const AssertInfo& assert_info) {

        std::cerr
            << std::vformat(
                std::string("Assert occured at {} in {}:{},{}: "),
                std::make_format_args(
                    assert_info.source_location.function_name(),
                    assert_info.source_location.file_name(),
                    assert_info.source_location.line(),
                    assert_info.source_location.column()
                )
            );

        if( assert_info.message.size() > 0 ) {
            std::cerr << assert_info.message;
        }

        std::cerr << std::endl;
    }

    void report_assert_to_dialog(const AssertInfo& assert_info) {

        std::ostringstream message_stream{};

        message_stream
            << std::vformat(
                std::string("Assert occured at\n{}\nin\n{}:{},{}"),
                std::make_format_args(
                    assert_info.source_location.function_name(),
                    assert_info.source_location.file_name(),
                    assert_info.source_location.line(),
                    assert_info.source_location.column()
                )
            );

        if( assert_info.message.size() > 0 ) {
            message_stream << ":\n\n" << assert_info.message;
        }

        std::string message = message_stream.str();

        HWND owner_window = NULL;
        LPCSTR caption = "Unexpected implementation error";
        LPCSTR text = message.c_str();

        MessageBox(
            owner_window,
            text,
            caption,
            MB_OK | MB_ICONERROR);
    }

}

