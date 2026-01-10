#include "expect_handling.hpp"

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

    std::function<void(const ExpectInfo&)> s_expect_report_function = report_expect_to_cerr;

    void internal::report_expect(const ExpectInfo& expect_info) {
        
        if( s_expect_report_function != nullptr ) {
            s_expect_report_function(expect_info);
        }

        TF_DEBUG_BREAK();

        std::exit(1);
    }

    void report_expect_to_cerr(const ExpectInfo& expect_info) {

        std::cerr
            << std::vformat(
                std::string("Failed 'expect' statement at {} in {}:{},{}: "),
                std::make_format_args(
                    expect_info.source_location.function_name(),
                    expect_info.source_location.file_name(),
                    expect_info.source_location.line(),
                    expect_info.source_location.column()
                )
            );

        if( expect_info.message.size() > 0 ) {
            std::cerr << expect_info.message;
        }

        std::cerr << std::endl;
    }

    void report_expect_to_dialog(const ExpectInfo& expect_info) {

        std::ostringstream message_stream{};

        message_stream
            << std::vformat(
                std::string("Invalid value found in function \n'{}'\nin file\n'{}' (line {}, column {})"),
                std::make_format_args(
                    expect_info.source_location.function_name(),
                    expect_info.source_location.file_name(),
                    expect_info.source_location.line(),
                    expect_info.source_location.column()
                )
            );

        if( expect_info.message.size() > 0 ) {
            message_stream << ":\n\n" << expect_info.message;
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

