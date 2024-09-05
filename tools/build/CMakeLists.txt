cmake_minimum_required(VERSION 3.21)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(
	taskforce
	LANGUAGES    C CXX
	VERSION      1.0.0
	DESCRIPTION  ""
)

get_filename_component(TF_DIR_ROOT ${CMAKE_CURRENT_LIST_DIR}/../.. ABSOLUTE)
get_filename_component(TF_DIR_SRC ${TF_DIR_ROOT}/src ABSOLUTE)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/imgui)

set(TF_SOURCES 
	${TF_DIR_SRC}/main.cpp
)

add_executable(taskforce ${TF_SOURCES})

target_include_directories(taskforce 
	PRIVATE ${TF_DIR_ROOT}/external/imgui/include
	PRIVATE ${TF_DIR_ROOT}/external/json/include
	PRIVATE "C:/Program Files\ (x86)/Windows\ Kits/10/Include/10.0.26100.0/shared")

if (MSVC)
    target_compile_options(taskforce
		PRIVATE /W4
	)
endif()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT taskforce)

add_library(directx STATIC IMPORTED)
set_target_properties(directx PROPERTIES IMPORTED_LOCATION "C:/Program Files\ (x86)/Windows\ Kits/10/Lib/10.0.26100.0/um/x64/d3d11.lib " )

target_link_libraries(taskforce 
	PRIVATE imgui
	PRIVATE directx)