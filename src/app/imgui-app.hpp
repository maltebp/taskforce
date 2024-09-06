#pragma once

#include <functional>

namespace tp {
	
	int run_imgui_app(std::function<void()> main_loop_callback, std::function<void()> shutdown_callback);

}