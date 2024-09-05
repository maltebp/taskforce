cmake_minimum_required(VERSION 3.21)

set(TF_APP_SOURCES 
	${TF_DIR_SRC}/app/main.cpp
)

add_executable(taskforce.app ${TF_APP_SOURCES})

target_link_libraries(taskforce.app 
	PRIVATE taskforce.model
	PRIVATE imgui
	PUBLIC directx)

source_group(TREE ${TF_DIR_SRC}/app PREFIX src FILES ${TF_APP_SOURCES})