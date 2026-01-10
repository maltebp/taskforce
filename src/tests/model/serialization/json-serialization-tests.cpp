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
    T deserialized_object = serializer.read_value<T>(istream);
    
	ASSERT_EQ(deserialized_object, object_to_serialize);
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
    
    ASSERT_THROW(
        serializer.read_value<NestingTypes>(istream), 
        tf::SerializationException);
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

    AllOptionalValues deserialized = serializer.read_value<AllOptionalValues>(istream);

    ASSERT_EQ(-1, deserialized.some_int);
    ASSERT_EQ("empty", deserialized.some_string);

    Primitives expected_primitives{ -1, "empty" };
    ASSERT_EQ(expected_primitives, deserialized.nested_primitives);

    std::vector<Primitives> expected_nested_primitives_list{ Primitives{ -1 , "empty" }};
    ASSERT_EQ(expected_nested_primitives_list, deserialized.nested_list_of_primitives);
}