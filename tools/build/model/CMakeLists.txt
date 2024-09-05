cmake_minimum_required(VERSION 3.21)


set(TF_MODEL_SOURCES 
	${TF_DIR_SRC}/model/workspace.hpp
	${TF_DIR_SRC}/model/workspace.cpp
)

add_library(taskforce.model ${TF_MODEL_SOURCES})

target_include_directories(taskforce.model
	PUBLIC ${TF_DIR_SRC}/model)

target_link_libraries(taskforce.model
	PUBLIC json)

source_group(TREE ${TF_DIR_SRC}/model PREFIX src FILES ${TF_MODEL_SOURCES})