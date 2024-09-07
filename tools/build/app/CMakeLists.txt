cmake_minimum_required(VERSION 3.21)

set(TF_APP_SOURCES 
	${TF_DIR_SRC}/app/main.cpp

	${TF_DIR_SRC}/app/imgui-app.hpp
	${TF_DIR_SRC}/app/imgui-app.cpp

	${TF_DIR_SRC}/app/views/time-entries-view.hpp
	${TF_DIR_SRC}/app/views/time-entries-view.cpp

	${TF_DIR_SRC}/app/views/main-window.hpp
	${TF_DIR_SRC}/app/views/main-window.cpp
)

add_executable(taskforce.app ${TF_APP_SOURCES})

set_target_properties(taskforce.app PROPERTIES
	CXX_STANDARD ${TF_CXX_STANDARD}
)

target_link_libraries(taskforce.app 
	PRIVATE taskforce.model
	PRIVATE imgui
	PUBLIC directx)

target_include_directories(taskforce.app
	PRIVATE ${TF_DIR_SRC})

source_group(TREE ${TF_DIR_SRC}/app PREFIX src FILES ${TF_APP_SOURCES})