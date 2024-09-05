cmake_minimum_required(VERSION 3.21)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(
	taskforce
	LANGUAGES    C CXX
	VERSION      1.0.0
	DESCRIPTION  ""
)

if (MSVC)
    add_compile_options(/W4)
endif()

get_filename_component(TF_DIR_ROOT ${CMAKE_CURRENT_LIST_DIR}/../.. ABSOLUTE)
get_filename_component(TF_DIR_SRC ${TF_DIR_ROOT}/src ABSOLUTE)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/imgui)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/external/directx)
add_subdirectory(${TF_DIR_ROOT}/external/googletest ${TF_DIR_ROOT}/build/googletest)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/tests)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/app)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT taskforce.app)