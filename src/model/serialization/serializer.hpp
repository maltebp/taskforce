#pragma once

#include <iostream>

#include "core/result.hpp"

#include "serialization/serialization-exception.hpp"
#include "serialization/serializer.dec.hpp"
#include "serialization/readable-object.hpp"

namespace tf {

    template<typename T>
    [[nodiscard]] Result<void> Serializer::write(std::ostream& stream, const T& value) {
        try {
            write_internal(
                stream, 
                [&value](WritableObject& root_object) {
                    ObjectSerializer<T> object_serializer{};
                    object_serializer.write(root_object, value);
                }
            );

            return Result<void>::ok();
            
        } catch( SerializationException e ) {
            return Result<void>::err(Error{e.what()});
        }
    }

    template<typename T>
    Result<T> Serializer::read(std::istream& stream) {	
        try {
            const ReadableObject& root_object = read_internal(stream);
            ObjectSerializer<T> object_serializer{};
            T value = object_serializer.read(root_object);

            // Deallocate any data the derived may have allocated
            reset();

            return Result<T>::ok(std::move(value));
        }
        catch( SerializationException e ) {
            return Error{e.what()};
        }
    }

}