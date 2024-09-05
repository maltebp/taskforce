cmake_minimum_required(VERSION 3.21)

set(TF_APP_SOURCES 
	${TF_DIR_SRC}/app/main.cpp
)

add_executable(taskforce.app ${TF_APP_SOURCES})

target_include_directories(taskforce.app
	PRIVATE ${TF_DIR_ROOT}/external/json/include
	PRIVATE "C:/Program Files\ (x86)/Windows\ Kits/10/Include/10.0.26100.0/shared")

add_library(directx STATIC IMPORTED)
set_target_properties(directx PROPERTIES IMPORTED_LOCATION "C:/Program Files\ (x86)/Windows\ Kits/10/Lib/10.0.26100.0/um/x64/d3d11.lib " )

target_link_libraries(taskforce.app 
	PRIVATE imgui
	PRIVATE directx)