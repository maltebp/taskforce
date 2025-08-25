#include "assert.hpp"

#include <format>
#include <iostream>

namespace tf {

    static void report_to_cerr(const AssertInfo&);

    std::function<void(const AssertInfo&)> s_assert_report_function = report_to_cerr;
    
    static void report_to_cerr(const AssertInfo& assert_info) {

        std::cerr
            << std::vformat(
                std::string("Assert occured in {} at {}:{},{}: "),
                std::make_format_args(
                    assert_info.source_location.file_name(),
                    assert_info.source_location.function_name(),
                    assert_info.source_location.line(),
                    assert_info.source_location.column()
                )
            )
            << assert_info.message 
            << std::endl;
    }

}

