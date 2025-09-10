#pragma once

#include <functional>
#include <source_location>
#include <string_view>

namespace tf {

	struct AssertInfo {
		std::string_view message;
		std::source_location& source_location;
	};

	namespace internal {
		void report_assert(const AssertInfo&);
	}

	/**
	  * @brief The function which will be called to report an error. If a debugger is attached it
	  * will break after this is called, and exit with an error after that. By default it will use
	  * \ref report_assert_to_console()
	  */
	extern std::function<void(const AssertInfo&)> s_assert_report_function;

	void report_assert_to_cerr(const AssertInfo&);

	/**
	 * @brief Reports the assert to a cross-platform dialog.
	*/
	void report_assert_to_dialog(const AssertInfo&);
}

