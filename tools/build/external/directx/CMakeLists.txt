cmake_minimum_required(VERSION 3.21)

add_library(directx STATIC IMPORTED GLOBAL)

set_target_properties(directx PROPERTIES 
	IMPORTED_LOCATION "C:/Program Files\ (x86)/Windows\ Kits/10/Lib/10.0.26100.0/um/x64/d3d11.lib " 
)

target_include_directories(directx
	INTERFACE "C:/Program Files\ (x86)/Windows\ Kits/10/Include/10.0.26100.0/shared"
)