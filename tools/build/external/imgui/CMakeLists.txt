cmake_minimum_required(VERSION 3.21)

set(TF_IMGUI_SOURCES	
	${TF_DIR_ROOT}/external/imgui/src/imgui_demo.cpp
	${TF_DIR_ROOT}/external/imgui/src/imgui_draw.cpp
	${TF_DIR_ROOT}/external/imgui/src/imgui_internal.h
	${TF_DIR_ROOT}/external/imgui/src/imgui_tables.cpp
	${TF_DIR_ROOT}/external/imgui/src/imgui_widgets.cpp
	${TF_DIR_ROOT}/external/imgui/src/imgui.cpp
	${TF_DIR_ROOT}/external/imgui/src/imgui_stdlib.cpp
	${TF_DIR_ROOT}/external/imgui/src/imgui_impl_win32.cpp
	${TF_DIR_ROOT}/external/imgui/src/imgui_impl_dx11.cpp
)

add_library(imgui STATIC ${TF_IMGUI_SOURCES})

set_target_properties(imgui PROPERTIES
	CXX_STANDARD ${TF_CXX_STANDARD}
)

target_include_directories(imgui
	PUBLIC ${TF_DIR_ROOT}/external/imgui/include
	PRIVATE ${TF_DIR_ROOT}/external/imgui/src
)

set_target_properties(imgui PROPERTIES FOLDER "external")