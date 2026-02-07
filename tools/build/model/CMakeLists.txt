cmake_minimum_required(VERSION 3.21)


set(TF_MODEL_SOURCES 

	${TF_DIR_SRC}/model/core/expect.hpp
	${TF_DIR_SRC}/model/core/expect_handling.hpp
	${TF_DIR_SRC}/model/core/expect_handling.cpp

	${TF_DIR_SRC}/model/core/string-util.hpp
	${TF_DIR_SRC}/model/core/string-util.cpp

	${TF_DIR_SRC}/model/core/result.fwd.hpp
	${TF_DIR_SRC}/model/core/result.hpp

	${TF_DIR_SRC}/model/workspace.hpp
	${TF_DIR_SRC}/model/workspace.cpp

	${TF_DIR_SRC}/model/time-entry.hpp
	${TF_DIR_SRC}/model/time-entry.cpp
	
	${TF_DIR_SRC}/model/parse-util.hpp
	${TF_DIR_SRC}/model/parse-util.cpp

	${TF_DIR_SRC}/model/serialization/writable-object.hpp
	${TF_DIR_SRC}/model/serialization/writable-object.fwd.hpp
	${TF_DIR_SRC}/model/serialization/writable-list.hpp
	${TF_DIR_SRC}/model/serialization/serializer.hpp
	${TF_DIR_SRC}/model/serialization/serializer.fwd.hpp
	${TF_DIR_SRC}/model/serialization/serializer.dec.hpp
	${TF_DIR_SRC}/model/serialization/serialization-exception.hpp
	${TF_DIR_SRC}/model/serialization/serializable-type.hpp
	${TF_DIR_SRC}/model/serialization/readable-object.hpp
	${TF_DIR_SRC}/model/serialization/readable-object.fwd.hpp
	${TF_DIR_SRC}/model/serialization/object-serializer.hpp
	${TF_DIR_SRC}/model/serialization/object-serializer.dec.hpp

	${TF_DIR_SRC}/model/serialization/json/json-readable-object.hpp
	${TF_DIR_SRC}/model/serialization/json/json-serializer.cpp
	${TF_DIR_SRC}/model/serialization/json/json-serializer.fwd.hpp
	${TF_DIR_SRC}/model/serialization/json/json-serializer.hpp
	${TF_DIR_SRC}/model/serialization/json/json-writable-list.cpp
	${TF_DIR_SRC}/model/serialization/json/json-writable-list.hpp
	${TF_DIR_SRC}/model/serialization/json/json-writable-object.cpp
	${TF_DIR_SRC}/model/serialization/json/json-writable-object.hpp
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