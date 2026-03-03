#pragma once

#include <string>
#include <vector>
#include <optional>

#include <json.hpp>

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

		template<typename TElementType>
		std::optional<std::vector<TElementType>> read_list(std::string_view name) const {

			auto it = json.find(name);
			if( it == json.end() ) return {};

			const nlohmann::json& json_object = *it;
			if( !json_object.is_array() ) {
				throw new SerializationException(
					"Property '" + std::string(name) + "' is not a list, but a " + json_object.type_name());
			}

			std::vector<TElementType> values;
			try {

				// Ther emust be a more efficient way to parse a list
				for( const nlohmann::json& value : json_object ) {
					values.push_back(value.template get<TElementType>());
				}
			
			} catch( const nlohmann::json::type_error& e ) {
				throw SerializationException(e.what());
			}

			return values;
		}

		std::optional<std::int32_t> read_int32(std::string_view name) const override {
			return read<int32_t>(name);
		}

		std::optional<std::vector<std::int32_t>> read_int32_list(std::string_view name) const override {
			return read_list<std::int32_t>(name);
		};

		std::optional<std::int64_t> read_int64(std::string_view name) const override {
			return read<int64_t>(name);
		};

		std::optional<std::vector<std::int64_t>> read_int64_list(std::string_view name) const override {
			return read_list<std::int64_t>(name);
		};

		std::optional<float> read_float32(std::string_view name) const override {
			return read<float>(name);
		};

		std::optional<std::vector<float>> read_float32_list(std::string_view name) const override {
			return read_list<float>(name);
		};

		std::optional<double> read_float64(std::string_view name) const override {
			return read<double>(name);
		};

		std::optional<std::vector<double>> read_float64_list(std::string_view name) const override {
			return read_list<double>(name);
		};

		std::optional<bool> read_bool(std::string_view name) const override {
			return read<bool>(name);
		};

		std::optional<std::vector<bool>> read_bool_list(std::string_view name) const override {
			return read_list<bool>(name);
		};

		std::optional<std::string> read_string(std::string_view name) const override {
			return read<std::string>(name);
		};

		std::optional<std::vector<std::string>> read_string_list(std::string_view name) const override {
			return read_list<std::string>(name);
		};

		bool read_object(std::string_view name, std::function<void(const ReadableObject&)> read_callback) const override {

			auto it = json.find(name);
			if( it == json.end() ) return false;

			const nlohmann::json& json_object = *it;
			JsonReadableObject object{ json_object };

			read_callback(object);

			return true;
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