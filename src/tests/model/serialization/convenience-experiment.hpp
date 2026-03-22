// An experiment to create a simplifying API layer on top of the seriazliation
// frammework. Primarily, I combine the two serialization and deserialization
// points into one, such that you don't have to keep them in sync. 
//
// The code has never actually been run.
// 
// I thought I had come up with an ingenious working solution, but then I 
// realized that something critical was missing: the solution that relies on
// strings for member names, have no way to convert a string to an actual member
// access syntax <A>("someMember") cannot become A.someMember in the code.
// 
// Potential solution: if I allow for the use of macros just for the definition
// API then I could achieve what I want. The macro parameter someMember could be
// turned to A.someMember and "someMember".

// #pragma once

// #include <string>
// #include <optional>

// #include "core/false_type.hpp"
// #include "serialization/object-serializer.hpp"
// #include "serialization/readable-object.hpp"


// namespace tf {

//     struct MemberBase {
//         std::string name;
//     };

//     template<typename T>
//     struct Member : MemberBase {

//         T read(const ReadableObject& readable_object) const {
//             return readable_object.read<T>(name);
//         }

//         void write(const WritableObject& writable_object, T value) const {
//             return writable_object.write<T>(name, value);
//         }
        
//     };

//     template<typename T>
//     struct OptionalMember : MemberBase {
//         T default_value;

//         T read(const ReadableObject& readable_object) const {
//             return readable_object.read_optional<T>(name).value_or(default_value);
//         }

//         void write(const WritableObject& writable_object, T value) const {
//             return writable_object.write<T>(name, value);
//         }
        
//     };

//     template<typename T, typename ... TMembers>
//     class ObjectMetaData;

//     template<typename T>
//     class MetaData {
//     public:
        
//         virtual T read(const ReadableObject& readable_object);
//         virtual void write(T v);

//         template<typename ... TMembers>
//         static ObjectMetaData<T, TMembers...> define(TMembers ... members);
//     };

//     template<typename T, typename ... TMembers>
//     class ObjectMetaData : public MetaData<T> {
//     public:

//         ObjectMetaData(TMembers ... members) : members(members...) { }

//         T read(const ReadableObject& readable_object) override {
//             return T { std::get<TMembers>(members).read(readable_object)... };
//         }

//         void write(WritableObject& writable_object, T value) {
//             (std::get<TMembers>(members).write(writable_object), ... );
//         }

//         std::tuple<TMembers...> members;
//     };

//     template<typename T>
//     template<typename ... TMembers>
//     ObjectMetaData<T, TMembers...> MetaData<T>::define(TMembers ... members) {
//         return ObjectMetaData<T, TMembers...>{members...};
//     }

//     template<typename T>
//     class ObjectSerializerX {
//         static_assert(tf::FalseType<T>::value, "Not implemented for this type");
//     };

//     class A {
//     public:
//         std::int32_t some_int32;
//         std::string some_string;
//     };

//     template<>
//     class ObjectSerializerX<A> {

//         static inline MetaData<A> data = MetaData<A>::define(
//             Member<decltype(A::some_int32)>{"some_int32"},
//             Member<decltype(A::some_string)>{"some_string"}
//         );
//     };

//     // name, type, default value (makes it optional)

//     // internal usage: ObjectSerializer<T>::data::read()

// }