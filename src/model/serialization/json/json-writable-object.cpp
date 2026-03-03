#include "serialization/json/json-writable-object.hpp"

#include "serialization/json/json-writable-list.hpp"


namespace tf {

    JsonWritableObject::JsonWritableObject(nlohmann::json& json)
        :   json(json)
    { }

    void JsonWritableObject::write(std::string_view name, std::int32_t value) {
        json[name] = value;
    }

    void JsonWritableObject::write(std::string_view name, std::int64_t value) {
        json[name] = value;
    }

    void JsonWritableObject::write(std::string_view name, float value) {
        json[name] = value;
    }

    void JsonWritableObject::write(std::string_view name, double value) {
        json[name] = value;
    }

    void JsonWritableObject::write(std::string_view name, bool value) {
        json[name] = value;
    }

    void JsonWritableObject::write(std::string_view name, std::string_view value) {
        json[name] = value;
    }

    void JsonWritableObject::write_to_object(
        std::string_view name, 
        std::function<void(WritableObject& object)> write_callback
    ) {
		nlohmann::json& inserted_json_object = (json[name] = nlohmann::json::object());
		
		JsonWritableObject writable_object{ inserted_json_object };
		write_callback(writable_object);
	}

	void JsonWritableObject::write_to_list(
        std::string_view name, 
        std::function<void(WritableList& list)> write_callback
    ) {
		nlohmann::json& inserted_json_list = (json[name] = nlohmann::json::array());

		JsonWritableList writable_object{ inserted_json_list };
		write_callback(writable_object);
	}

}
