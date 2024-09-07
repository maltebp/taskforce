cmake_minimum_required(VERSION 3.21)

set(TF_TESTS_SOURCES 
	${TF_DIR_SRC}/tests/test_test.cpp
)

add_library(taskforce.tests ${TF_TESTS_SOURCES})

set_target_properties(taskforce.tests PROPERTIES
	CXX_STANDARD ${TF_CXX_STANDARD}
)

target_include_directories(taskforce.tests
	PRIVATE ${TF_DIR_ROOT}/external/json/include)

target_link_libraries(taskforce.tests
	PRIVATE taskforce.model
	PRIVATE gtest_main)

source_group(TREE ${TF_DIR_SRC}/tests PREFIX src FILES ${TF_TESTS_SOURCES})