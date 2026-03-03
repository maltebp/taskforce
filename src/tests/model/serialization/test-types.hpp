#pragma once

#include <string>
#include <vector>

#include "serialization/object-serializer.hpp"

struct Primitives {
    bool operator ==(const Primitives& other) const {
        return 
            some_int32 == other.some_int32 && 
            some_int64 == other.some_int64 && 
            some_float == other.some_float && 
            some_double == other.some_double && 
            some_string == other.some_string;
    }

    std::int32_t some_int32;
    std::int64_t some_int64;
    float some_float;
    double some_double;
    std::string some_string;
};

template<>
struct tf::ObjectSerializer<Primitives> {

    void write(tf::WritableObject& writable_object, const Primitives& value) {
        writable_object.write("some_int32", value.some_int32);
        writable_object.write("some_int64", value.some_int64);
        writable_object.write("some_float", value.some_float);
        writable_object.write("some_double", value.some_double);
        writable_object.write("some_string", value.some_string);
    }

    Primitives read(const tf::ReadableObject& readable_object) {
        return Primitives{
            readable_object.read<std::int32_t>("some_int32"),
            readable_object.read<std::int64_t>("some_int64"),
            readable_object.read<float>("some_float"),
            readable_object.read<double>("some_double"),
            readable_object.read<std::string>("some_string")
        };
    }

};


struct ListsOfPrimitives {

    bool operator ==(const ListsOfPrimitives& other) const {
        return 
            some_int32s == other.some_int32s && 
            some_int64s == other.some_int64s && 
            some_floats == other.some_floats && 
            some_doubles == other.some_doubles && 
            some_strings == other.some_strings;
    }

    std::vector<std::int32_t> some_int32s;
    std::vector<std::int64_t> some_int64s;
    std::vector<float> some_floats;
    std::vector<double> some_doubles;
    std::vector<std::string> some_strings;
};

template<>
struct tf::ObjectSerializer<ListsOfPrimitives> {

    void write(tf::WritableObject& writable_object, const ListsOfPrimitives& value) {
        writable_object.write("some_int32s", value.some_int32s.begin(), value.some_int32s.end());
        writable_object.write("some_int64s", value.some_int64s.begin(), value.some_int64s.end());
        writable_object.write("some_floats", value.some_floats.begin(), value.some_floats.end());
        writable_object.write("some_doubles", value.some_doubles.begin(), value.some_doubles.end());
        writable_object.write("some_strings", value.some_strings.begin(), value.some_strings.end());
    }

    ListsOfPrimitives read(const tf::ReadableObject& readable_object) {
        return ListsOfPrimitives{
            readable_object.read<std::vector<std::int32_t>>("some_int32s"),
            readable_object.read<std::vector<std::int64_t>>("some_int64s"),
            readable_object.read<std::vector<float>>("some_floats"),
            readable_object.read<std::vector<double>>("some_doubles"),
            readable_object.read<std::vector<std::string>>("some_strings")
        };
    }

};


struct NestingTypes {

    bool operator==(const NestingTypes& other) const {
        return
            nested_primitives == other.nested_primitives &&
            nested_list_of_primitives == other.nested_list_of_primitives;
    }

    Primitives nested_primitives;
    std::vector<Primitives> nested_list_of_primitives;
};

template<>
struct tf::ObjectSerializer<NestingTypes> {

    void write(tf::WritableObject& writable_object, const NestingTypes& value) {
        writable_object.write("nested_primitives", value.nested_primitives);
        writable_object.write("nested_list_of_primitives", value.nested_list_of_primitives.begin(), value.nested_list_of_primitives.end());
    }

    NestingTypes read(const tf::ReadableObject& readable_object) {
        return NestingTypes{
            readable_object.read<Primitives>("nested_primitives"),
            readable_object.read<std::vector<Primitives>>("nested_list_of_primitives")  
        };
    }

};

struct AllOptionalValues {

    bool operator==(const AllOptionalValues& other) const {
        return
            some_int32 == other.some_int32 &&
            some_int64 == other.some_int64 &&
            some_float == other.some_float &&
            some_double == other.some_double &&
            some_string == other.some_string &&
            nested_primitives == other.nested_primitives &&
            nested_list_of_primitives == other.nested_list_of_primitives;
    }

    std::int32_t some_int32;
    std::int64_t some_int64;
    float some_float;
    double some_double;
    std::string some_string;
    Primitives nested_primitives;
    std::vector<Primitives> nested_list_of_primitives;
};

template<>
struct tf::ObjectSerializer<AllOptionalValues> {

    void write(tf::WritableObject& writable_object, const AllOptionalValues& value) {
        writable_object.write("some_int32", value.some_int32);
        writable_object.write("some_int64", value.some_int64);
        writable_object.write("some_float", value.some_float);
        writable_object.write("some_double", value.some_double);
        writable_object.write("some_string", value.some_string);
        writable_object.write("nested_primitives", value.nested_primitives),
        writable_object.write("nested_list_of_primitives", value.nested_list_of_primitives.begin(), value.nested_list_of_primitives.end());
    }

    AllOptionalValues read(const tf::ReadableObject& readable_object) {
        return AllOptionalValues{
            readable_object.read_optional<std::int32_t>("some_int32").value_or(-1),
            readable_object.read_optional<std::int64_t>("some_int64").value_or(-1),
            readable_object.read_optional<float>("some_float").value_or(-1),
            readable_object.read_optional<double>("some_double").value_or(-1),
            readable_object.read_optional<std::string>("some_string").value_or("empty"),
            readable_object.read_optional<Primitives>("nested_primitives").value_or(Primitives{-1, -1, -1.0f, -1.0, "empty"}),
            readable_object.read_optional<std::vector<Primitives>>("nested_list_of_primitives").value_or(std::vector{Primitives{-1, -1, -1.0f, -1.0, "empty"}})
        };
    }

};

template<typename TDistinguisher>
struct ConstructorTracker {

    ConstructorTracker() {
        num_default_called++;
    }
    
    ConstructorTracker(const ConstructorTracker<TDistinguisher>& other)
        : some_int(other.some_int)
    { 
        num_copies_called++;
    }
    
    ConstructorTracker(ConstructorTracker<TDistinguisher>&& other) 
        : some_int(other.some_int)
    { 
        other.some_int = -1;
        num_moves_called++;
    }

    ConstructorTracker<TDistinguisher>& operator=(const ConstructorTracker<TDistinguisher>& other)
    {
        some_int = other.some_int;
        return *this;
    }

    static void reset() {
        num_default_called = 0;
        num_copies_called = 0;
        num_moves_called = 0;    
    }

    static inline int num_default_called = 0;
    static inline int num_copies_called = 0;
    static inline int num_moves_called = 0;

    int some_int = 0; // Just so there is something to serialize

};

template<typename TDistinguisher>
struct tf::ObjectSerializer<ConstructorTracker<TDistinguisher>> {

    void write(tf::WritableObject& writable_object, const ConstructorTracker<TDistinguisher>& value) {
        writable_object.write("some_int", value.some_int);
    }

    ConstructorTracker<TDistinguisher> read(const tf::ReadableObject& readable_object) {
        ConstructorTracker<TDistinguisher> deserialized;
        deserialized.some_int = readable_object.read<int>("some_int");
        return deserialized;
    }

};