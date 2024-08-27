cmake_minimum_required(VERSION 3.21)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(
	task-planner
	LANGUAGES    C CXX
	VERSION      1.0.0
	DESCRIPTION  ""
)

get_filename_component(TP_DIR_ROOT ${CMAKE_CURRENT_LIST_DIR}/../.. ABSOLUTE)
get_filename_component(TP_DIR_SRC ${TP_DIR_ROOT}/src ABSOLUTE)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/imgui)

set(TP_SOURCES 
	${TP_DIR_SRC}/main.cpp
)

add_executable(task-planner ${TP_SOURCES})

target_include_directories(task-planner 
	PRIVATE ${TP_DIR_ROOT}/external/imgui
	PRIVATE "C:/Program Files\ (x86)/Windows\ Kits/10/Include/10.0.26100.0/shared")

if (MSVC)
    target_compile_options(task-planner
		PRIVATE /W4
	)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT task-planner)

add_library(directx STATIC IMPORTED)
set_target_properties(directx PROPERTIES IMPORTED_LOCATION "C:/Program Files\ (x86)/Windows\ Kits/10/Lib/10.0.26100.0/um/x64/d3d11.lib " )

target_link_libraries(task-planner 
	PRIVATE imgui
	PRIVATE directx)