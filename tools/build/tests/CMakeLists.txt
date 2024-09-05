cmake_minimum_required(VERSION 3.21)


set(TF_TESTS_SOURCES 
	${TF_DIR_SRC}/tests/test_test.cpp
)

add_executable(taskforce.tests ${TF_TESTS_SOURCES})

target_include_directories(taskforce.tests
	PRIVATE ${TF_DIR_ROOT}/external/json/include)

target_link_libraries(taskforce.tests
	PRIVATE gtest_main)