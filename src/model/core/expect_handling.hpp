#pragma once

#include <functional>
#include <source_location>
#include <string_view>

namespace tf {

	struct ExpectInfo {
		std::string_view message;
		std::source_location& source_location;
	};

	namespace internal {
		void report_expect(const ExpectInfo&);
	}

	/**
	  * @brief The function which will be called to report an error. If a debugger is attached it
	  * will break after this is called, and exit with an error after that. By default it will use
	  * \ref report_expect_to_console()
	  */
	extern std::function<void(const ExpectInfo&)> s_expect_report_function;

	void report_expect_to_cerr(const ExpectInfo&);

	/**
	 * @brief Reports the failed expect to a cross-platform dialog.
	*/
	void report_expect_to_dialog(const ExpectInfo&);
}

