#pragma once

#include <string>
#include <vector>
#include <optional>

#include <json.hpp>

#include "core/expect.hpp"
#include "serialization/serialization-exception.hpp"
#include "serialization/readable-object.hpp"

namespace tf {
    
    class JsonReadableObject : public ReadableObject {
	public:

		JsonReadableObject(const nlohmann::json& json) 
			:	json(json)
		{ }

		template<typename TParsedType>
		std::optional<TParsedType> read(std::string_view name) const {
			
			auto it = json.find(name);
			if( it == json.end() ) return {};

			const nlohmann::json& json_value = *it;

			try {
				return json_value.template get<TParsedType>();
			}
			catch( const nlohmann::json::type_error& e ) {
				throw SerializationException(e.what());
			}
		}

		std::optional<int> read_int(std::string_view name) const override {
			return read<int>(name);
		};

		std::optional<std::string> read_string(std::string_view name) const override {
			return read<std::string>(name);
		};

		bool read_object(std::string_view name, std::function<void(const ReadableObject&)> read_callback) const override {

			auto it = json.find(name);
			if( it == json.end() ) return false;

			const nlohmann::json& json_object = *it;
			JsonReadableObject object{ json_object };

			read_callback(object);

			return true;
		};

		std::optional<std::vector<int>> read_int_list(std::string_view name) const override {

			auto it = json.find(name);
			if( it == json.end() ) return {};

			const nlohmann::json& list = *it;
			if( !list.is_array() ) {
				throw new SerializationException(
					"Property '" + std::string(name) + "' is not a list, but a " + list.type_name());
			}

			std::vector<int> values;
			for( const nlohmann::json& value : list ) {

				if( !value.is_number_integer() ) {
					throw new SerializationException(
						"Property '" + std::string(name) + "' is not an integer, but a " + value.type_name());
				}

				values.push_back(value.template get<int>());
			}

			return values;
		};

		std::optional<std::vector<std::string>> read_string_list(std::string_view name) const override {
			auto it = json.find(name);
			if( it == json.end() ) return {};

			const nlohmann::json& list = *it;
			if( !list.is_array() ) {
				throw new SerializationException(
					"Property '" + std::string(name) + "' is not a list, but a " + list.type_name());
			}

			std::vector<std::string> values;
			for( const nlohmann::json& value : list ) {

				if( !value.is_string() ) {
					throw new SerializationException(
						"Property '" + std::string(name) + "' is not a string, but a " + value.type_name());
				}
				
				values.push_back(value.template get<std::string>());
			}

			return values;
		};

		bool read_object_list(std::string_view name, std::function<void(const ReadableObject&)> read_callback) const override {
			auto it = json.find(name);
			if( it == json.end() ) return false;

			const nlohmann::json& list = *it;
			if( !list.is_array() ) {
				throw new SerializationException(
					"Property '" + std::string(name) + "' is not a list, but a " + list.type_name());
			}

			for( const nlohmann::json& value : list ) {

				if( !value.is_object() ) {
					throw new SerializationException(
						"Property '" + std::string(name) + "' is not an object, but a " + value.type_name());
				}
				
				JsonReadableObject value_object{ value };

				read_callback(value_object);
			}

			return true;
		};

	private:

		const nlohmann::json& json;

	};

}