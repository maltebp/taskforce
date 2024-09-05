cmake_minimum_required(VERSION 3.21)

add_library(json INTERFACE)

target_include_directories(json
	INTERFACE ${TF_DIR_ROOT}/external/json/include
)