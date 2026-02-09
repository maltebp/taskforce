#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <functional>
#include <optional>

#include "serialization/serialization-exception.hpp"
#include "serialization/object-serializer.hpp"

namespace tf {

    class ReadableObject {
		
		// Purpose of this is to allow a partial specialization of container types, so that we can
		// use the same public read<T>/read_optional<T> functions for such container types as well
		template<typename T>
		struct ReadProxy {
			static std::optional<T> read_optional(
				const ReadableObject& readable_object, std::string_view name);
		};
	
	public:

		template<typename T>
		std::optional<T> read_optional(std::string_view name) const {			
			return ReadProxy<T>::read_optional(*this, name);
		}

		template<typename T>
		T read(std::string_view name) const {
			std::optional<T> value = read_optional<T>(name);
			
			if( !value.has_value() ) {
				throw SerializationException(
					"Property " + std::string(name) + " does not exist"
				);
			}
			
			return value.value();
		}

	protected:

		virtual std::optional<int> read_int(std::string_view name) const = 0;

		virtual std::optional<std::string> read_string(std::string_view name) const = 0;

		virtual bool read_object(std::string_view name, std::function<void(const ReadableObject&)> read_callback) const = 0;

		virtual std::optional<std::vector<int>> read_int_list(std::string_view name) const = 0;

		virtual std::optional<std::vector<std::string>> read_string_list(std::string_view name) const = 0;

		virtual bool read_object_list(std::string_view name, std::function<void(const ReadableObject&)> read_callback) const = 0;

	};

	template<typename T>
	std::optional<T> ReadableObject::ReadProxy<T>::read_optional(
		const ReadableObject& readable_object, std::string_view name
	) { 
		std::optional<T> value{};
		bool object_found = readable_object.read_object(
			name,
			[&](const ReadableObject& object_to_read) {
				ObjectSerializer<T> object_serializer{};
				value = std::move(object_serializer.read(object_to_read));
			}
		);

		return object_found ? value : std::nullopt;
	}

	template<typename T>
	struct ReadableObject::ReadProxy<std::vector<T>> {
	
		static std::optional<std::vector<T>> read_optional(
			const ReadableObject& readable_object, std::string_view name
		) { 
			ObjectSerializer<T> serializer{};
			std::vector<T> values;

			bool property_existed = readable_object.read_object_list(
				name,
				[&](const ReadableObject& readable_object) {
					values.push_back(std::move(serializer.read(readable_object)));
				}
			);

			if( !property_existed ) return std::nullopt;

			return values;
		}
	};

	// Type specializations

	template<>
	inline std::optional<int> ReadableObject::ReadProxy<int>::read_optional(
		const ReadableObject& readable_object, std::string_view name
	) { 
		return readable_object.read_int(name);
	}
	
	template<>
	inline std::optional<std::string> ReadableObject::ReadProxy<std::string>::read_optional(
		const ReadableObject& readable_object, std::string_view name
	) { 
		return readable_object.read_string(name);
	}

	template<>
	inline std::optional<std::vector<int>> ReadableObject::ReadProxy<std::vector<int>>::read_optional(
		const ReadableObject& readable_object, std::string_view name
	) { 
		return readable_object.read_int_list(name);
	}

	template<>
	inline std::optional<std::vector<std::string>> 
	ReadableObject::ReadProxy<std::vector<std::string>>::read_optional(
		const ReadableObject& readable_object, std::string_view name
	) { 
		return readable_object.read_string_list(name);
	}
}