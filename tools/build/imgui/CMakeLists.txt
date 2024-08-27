cmake_minimum_required(VERSION 3.21)

set(TP_IMGUI_SOURCES	
	${TP_DIR_ROOT}/external/imgui/src/imgui_demo.cpp
	${TP_DIR_ROOT}/external/imgui/src/imgui_draw.cpp
	${TP_DIR_ROOT}/external/imgui/src/imgui_internal.h
	${TP_DIR_ROOT}/external/imgui/src/imgui_tables.cpp
	${TP_DIR_ROOT}/external/imgui/src/imgui_widgets.cpp
	${TP_DIR_ROOT}/external/imgui/src/imgui.cpp
	${TP_DIR_ROOT}/external/imgui/src/imgui_impl_win32.cpp
	${TP_DIR_ROOT}/external/imgui/src/imgui_impl_dx11.cpp
)

add_library(imgui STATIC ${TP_IMGUI_SOURCES})

target_include_directories(imgui
	PUBLIC ${TP_DIR_ROOT}/external/imgui/include
	PRIVATE ${TP_DIR_ROOT}/external/imgui/src
)