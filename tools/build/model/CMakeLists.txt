cmake_minimum_required(VERSION 3.21)


set(TF_MODEL_SOURCES 
	${TF_DIR_SRC}/model/workspace.hpp
	${TF_DIR_SRC}/model/workspace.cpp

	${TF_DIR_SRC}/model/time-entry.hpp
	${TF_DIR_SRC}/model/time-entry.cpp
)

add_library(taskforce.model ${TF_MODEL_SOURCES})

set_target_properties(taskforce.model PROPERTIES
	CXX_STANDARD ${TF_CXX_STANDARD}
)

target_include_directories(taskforce.model
	PUBLIC ${TF_DIR_SRC}/model)

target_link_libraries(taskforce.model
	PUBLIC json)

source_group(TREE ${TF_DIR_SRC}/model PREFIX src FILES ${TF_MODEL_SOURCES})