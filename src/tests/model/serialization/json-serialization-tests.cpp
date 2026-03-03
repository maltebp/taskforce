#include <gtest/gtest.h>


#include "serialization/json/json-serializer.hpp"
#include "model/serialization/test-types.hpp"


template<typename T>
void write_then_read_and_assert(const T& object_to_serialize) {
    tf::JsonSerializer serializer;
    
    std::ostringstream ostream;
    tf::Result<void> write_result = serializer.write(ostream, object_to_serialize);
    ASSERT_TRUE(write_result.is_ok());

    std::string output= ostream.str();
    std::istringstream istream{ output }; 

    tf::Result<T> read_result = serializer.read<T>(istream);

    ASSERT_TRUE(read_result.is_ok());
    T& read_object = read_result.get_ok();
	ASSERT_EQ(read_object, object_to_serialize);
}

TEST(json_serialization, primitive_values) {

    Primitives object_to_serialize{
        42,
        42'000'000'000'000,
        1.234f,
        2.345678f, // This could probably have a better value
        "Hello, world!"
    };
    write_then_read_and_assert(object_to_serialize);
}

TEST(json_serialization, list_values) {

    ListsOfPrimitives object_to_serialize{
        { 42, 1337, 7 }, 
        { 42'000'000'000, 1234, -42 }, 
        { 1.23f, 0.01f, 1000.1f }, 
        { 0.00001, 10.123, 12345.6789 }, 
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
        Primitives{ 42, 42'000'000'000'000, 1.234f, 2.345678, "Hello, world!"},
        {
            Primitives{ 42, 42'000'000'000'000, 1.234f, 2.345678, "First element"},
            Primitives{ 42, 42'000'000'000'000, 1.234f, 2.345678, "Second element"},
            Primitives{ 42, 42'000'000'000'000, 1.234f, 2.345678, "Third element"},
        }
    };
    write_then_read_and_assert(object_to_serialize);
}

TEST(json_serialization, error_on_wrong_object_deserialized) {
    Primitives object_to_serialize{
        42, 
        42'000'000'000'000,
        1.234f,
        2.345678f,
        "Hello, world!"
    };

    tf::JsonSerializer serializer;

    std::ostringstream ostream;
    tf::Result<void> write_result = serializer.write(ostream, object_to_serialize);
    ASSERT_TRUE(write_result.is_ok());

    std::string output= ostream.str();
    std::istringstream istream{ output };

    tf::Result<NestingTypes> read_result = serializer.read<NestingTypes>(istream);
    ASSERT_TRUE(read_result.is_err());
}

TEST(json_serialization, optional_values) {
    
    AllOptionalValues object_with_values{
        42, 
        42'000'000'000'000,
        1.234f,
        2.345678f,
        "Hello, world!",
        Primitives{1337, 123'000'000'000, 10.1234f, 2.345678, "Nested string"},
        { 
            Primitives{ 42, 42'000'000'000'000, 1.234f, 2.345678, "First element"},
            Primitives{ 42, 42'000'000'000'000, 1.234f, 2.345678, "Second element"},
        }
    };
    
    write_then_read_and_assert(object_with_values);

    tf::JsonSerializer serializer;
    
    std::string empty_json = "{ }";
    std::istringstream istream{ empty_json };

    tf::Result<AllOptionalValues> read_result = serializer.read<AllOptionalValues>(istream);
    
    AllOptionalValues& deserialized = read_result.get_ok();

    ASSERT_EQ(-1, deserialized.some_int32);
    ASSERT_EQ(-1, deserialized.some_int64);
    ASSERT_EQ(-1.0f, deserialized.some_float);
    ASSERT_EQ(-1.0, deserialized.some_double);
    ASSERT_EQ("empty", deserialized.some_string);

    Primitives expected_primitives{ -1, -1, -1.0f, -1.0, "empty" };
    ASSERT_EQ(expected_primitives, deserialized.nested_primitives);

    std::vector<Primitives> expected_nested_primitives_list{ 
        Primitives{ -1, -1, -1.0f, -1.0, "empty" }
    };
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
    tf::Result<void> write_result = serializer.write(ostream, object_to_serialize);
    ASSERT_TRUE(write_result.is_ok());

    ASSERT_EQ(1, ConstructorTracker<Distinguisher>::num_default_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_copies_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_moves_called);

    std::istringstream istream{ ostream.str() }; 

    tf::Result<ConstructorTracker<Distinguisher>> read_result = serializer.read<ConstructorTracker<Distinguisher>>(istream);
    [[maybe_unused]] ConstructorTracker<Distinguisher>& deserialized = read_result.get_ok();


    ASSERT_EQ(2, ConstructorTracker<Distinguisher>::num_default_called);
    ASSERT_EQ(0, ConstructorTracker<Distinguisher>::num_copies_called);
    ASSERT_EQ(1, ConstructorTracker<Distinguisher>::num_moves_called);
    
	ASSERT_EQ(object_to_serialize.some_int, deserialized.some_int);
 
}