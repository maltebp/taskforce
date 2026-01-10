#include <gtest/gtest.h>

#include "serialization/json-serialization.hpp"
#include "serialization/serialization.hpp"

#include "model/serialization/test-types.hpp"


template<typename T>
void write_then_read_and_assert(const T& object_to_serialize) {
    tf::JsonSerializer serializer;
    
    std::ostringstream ostream;
    serializer.write_value(ostream, object_to_serialize);

    std::string output= ostream.str();
    std::istringstream istream{ output }; 

    tf::Result<T> read_result = serializer.read_value<T>(istream);
    
    ASSERT_TRUE(read_result.is_ok()) << read_result.get_err().info;
	ASSERT_EQ(read_result.get_ok(), object_to_serialize);
}

TEST(json_serialization, primitive_values) {

    Primitives object_to_serialize{
        42, 
        "Hello, world!"
    };
    write_then_read_and_assert(object_to_serialize);
}

TEST(json_serialization, list_values) {

    ListsOfPrimitives object_to_serialize{
        { 42, 1337, 7 }, 
        { "Hello", ", " "world!" }
    };
    write_then_read_and_assert(object_to_serialize);
}

TEST(json_serialization, empty_lists) {

    ListsOfPrimitives object_to_serialize{
        { }, 
        { }
    };
    write_then_read_and_assert(object_to_serialize);
}

TEST(json_serialization, nested_primitives) {

    NestingTypes object_to_serialize{
        Primitives{ 42, "Hello, world!"},
        {
            Primitives{ 1, "First element"},
            Primitives{ 2, "Second element"},
            Primitives{ 3, "Third element"},
        }
    };
    write_then_read_and_assert(object_to_serialize);
}

TEST(json_serialization, error_on_wrong_object_deserialized) {
    Primitives object_to_serialize{
        42, 
        "Hello, world!"
    };

    tf::JsonSerializer serializer;

    std::ostringstream ostream;
    serializer.write_value(ostream, object_to_serialize);

    std::string output= ostream.str();
    std::istringstream istream{ output };

    tf::Result<NestingTypes> read_result = serializer.read_value<NestingTypes>(istream);
    ASSERT_TRUE(read_result.is_err());
}

TEST(json_serialization, optional_values) {
    
    AllOptionalValues object_with_values{
        42,
        "Hello, world!",
        Primitives{1337, "Nested string"},
        { 
            Primitives{1, "First primitive"},
            Primitives{2, "Second primitive"} 
        }
    };
    
    write_then_read_and_assert(object_with_values);

    tf::JsonSerializer serializer;
    
    std::string empty_json = "{ }";
    std::istringstream istream{ empty_json };

    tf::Result<AllOptionalValues> read_result = serializer.read_value<AllOptionalValues>(istream);
    ASSERT_TRUE(read_result.is_ok()) << read_result.get_err().info;
    
    AllOptionalValues& deserialized = read_result.get_ok();

    ASSERT_EQ(-1, deserialized.some_int);
    ASSERT_EQ("empty", deserialized.some_string);

    Primitives expected_primitives{ -1, "empty" };
    ASSERT_EQ(expected_primitives, deserialized.nested_primitives);

    std::vector<Primitives> expected_nested_primitives_list{ Primitives{ -1 , "empty" }};
    ASSERT_EQ(expected_nested_primitives_list, deserialized.nested_list_of_primitives);
}

TEST(json_serialization, construction_call_check) {
    struct Distinguisher{};

    static_assert(std::is_move_constructible<ConstructorTracker<Distinguisher>>());

    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_default_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_copies_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_moves_called);

    ConstructorTracker<Distinguisher> object_to_serialize;
    object_to_serialize.some_int = 0;

    ASSERT_EQ(1, ConstructorTracker<Distinguisher>::num_default_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_copies_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_moves_called);

    tf::JsonSerializer serializer;
    
    std::ostringstream ostream;
    serializer.write_value(ostream, object_to_serialize);

    ASSERT_EQ(1, ConstructorTracker<Distinguisher>::num_default_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_copies_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_moves_called);

    std::istringstream istream{ ostream.str() }; 

    tf::Result<ConstructorTracker<Distinguisher>> read_result = serializer.read_value<ConstructorTracker<Distinguisher>>(istream);
    //ASSERT_TRUE(read_result.is_ok()) << read_result.get_err().info;

    ASSERT_EQ(2, ConstructorTracker<Distinguisher>::num_default_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_copies_called);
    ASSERT_EQ(1, ConstructorTracker<Distinguisher>::num_moves_called);
    
    ConstructorTracker<Distinguisher>& deserialized = read_result.get_ok();
    
	ASSERT_EQ(object_to_serialize.some_int, deserialized.some_int);
 
}