#pragma once

#include <json.hpp>

#include "serialization/serialization.hpp"

namespace tf {

	class JsonWritableObject : public WritableObject {
	public:

		JsonWritableObject(nlohmann::json& json)
			: json(json)
		{ }

		void write(std::string_view name, int value) override {
			json[name] = value;
		}

		void write(std::string_view name, std::string_view value) override {
			json[name] = value;
		}

	protected:
	
		void write_to_object(
			std::string_view name, 
			std::function<void(WritableObject& object)> write_callback
		) override;

		void write_to_list(std::string_view name, std::function<void(WritableList& list)> write_callback) override;

	private:

		nlohmann::json& json;

	};

	class JsonWritableList : public WritableList {
	public:

		
		JsonWritableList(nlohmann::json& json)
			: json(json)
		{ }

		void write(int value) override {
			json.push_back(value);
		}

		void write(std::string_view value) override {
			json.push_back(value);
		}

	protected:

		virtual void write_to_object(std::function<void(WritableObject& object)> write_callback) override {
			nlohmann::json json_object = nlohmann::json::object();
			JsonWritableObject sub_object{ json_object };
			write_callback(sub_object);
			json.push_back(std::move(json_object));
		}

		virtual void write_to_list(std::function<void(WritableList& list)> write_callback) override {
			nlohmann::json json_list = nlohmann::json::array();
			JsonWritableList sub_list{ json_list };
			write_callback(sub_list);
			json.push_back(std::move(json_list));
		}

	private:

		nlohmann::json& json;

	};

	void JsonWritableObject::write_to_object(std::string_view name, std::function<void(WritableObject& object)> write_callback) {
		nlohmann::json& inserted_json_object = (json[name] = nlohmann::json::object());
		
		JsonWritableObject writable_object{ inserted_json_object };
		write_callback(writable_object);
	}

	void JsonWritableObject::write_to_list(std::string_view name, std::function<void(WritableList& list)> write_callback) {
		nlohmann::json& inserted_json_list = (json[name] = nlohmann::json::array());

		JsonWritableList writable_object{ inserted_json_list };
		write_callback(writable_object);
	}

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
			tf::expect(list.is_array()); // TODO: change to exception

			std::vector<int> values;
			for( const nlohmann::json& value : list ) {

				tf::expect(value.is_number_integer()); // TODO: Change to exception
				values.push_back(value.template get<int>());
			}

			return values;
		};

		std::optional<std::vector<std::string>> read_string_list(std::string_view name) const override {
			auto it = json.find(name);
			if( it == json.end() ) return {};

			const nlohmann::json& list = *it;
			tf::expect(list.is_array()); // TODO: change to exception

			std::vector<std::string> values;
			for( const nlohmann::json& value : list ) {

				tf::expect(value.is_string()); // TODO: Change to exception
				values.push_back(value.template get<std::string>());
			}

			return values;
		};

		bool read_object_list(std::string_view name, std::function<void(const ReadableObject&)> read_callback) const override {
			auto it = json.find(name);
			if( it == json.end() ) return false;

			const nlohmann::json& list = *it;
			tf::expect(list.is_array()); // TODO: change to exception

			for( const nlohmann::json& value : list ) {

				tf::expect(value.is_object()); // TODO: Change to exception
				JsonReadableObject value_object{ value };

				read_callback(value_object);
			}

			return true;
		};

	private:

		const nlohmann::json& json;

	};


	class JsonSerializer : public Serializer {

		void write(std::ostream& stream, std::function<void(WritableObject& root_object)> write_callback) override {

			
			nlohmann::json root_json_object = nlohmann::json::object();
			JsonWritableObject root_object{ root_json_object };
			write_callback(root_object);
			stream << root_json_object;

		}

		[[nodiscard]] virtual const ReadableObject& read(std::istream& stream) override {
			current_read_root_json_object.emplace();
			stream >> current_read_root_json_object.value();
			
			current_read_root_object.emplace(current_read_root_json_object.value());
			return current_read_root_object.value();
		};

		void reset() override {
			current_read_root_json_object.reset();
			current_read_root_object.reset();
		}

	private:

		// These are held as optional, because (from what I could gather) the nlohmann::json will
		// allocate json objects when deserializing and hold on to them for its life time. As the
		// user of the serializer may hold on to object for a prolonged period of time, we want to
		// be able clean it without destroying it. Could also be achieved with pointers, but
		// std::optional avoids heap allocations.
		std::optional<nlohmann::json> current_read_root_json_object;
		std::optional<JsonReadableObject> current_read_root_object;

	};

}