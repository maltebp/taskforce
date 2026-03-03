#include "serialization/json/json-writable-list.hpp"

#include "serialization/json/json-writable-object.hpp"

namespace tf {

    JsonWritableList::JsonWritableList(nlohmann::json& json)
        : json(json)
    { }

    void JsonWritableList::write(std::int32_t value) {
        json.push_back(value);
    }

    void JsonWritableList::write(std::int64_t value) {
        json.push_back(value);
    }

    void JsonWritableList::write(float value) {
        json.push_back(value);
    }

    void JsonWritableList::write(double value) {
        json.push_back(value);
    }

    void JsonWritableList::write(bool value) {
        json.push_back(value);
    }

    void JsonWritableList::write(std::string_view value) {
        json.push_back(value);
    }

    void JsonWritableList::write_to_object(
        std::function<void(WritableObject& object)> write_callback
    ) {
        nlohmann::json json_object = nlohmann::json::object();
        JsonWritableObject sub_object{ json_object };
        write_callback(sub_object);
        json.push_back(std::move(json_object));
    }

    void JsonWritableList::write_to_list(
        std::function<void(WritableList& list)> write_callback
    ) {
        nlohmann::json json_list = nlohmann::json::array();
        JsonWritableList sub_list{ json_list };
        write_callback(sub_list);
        json.push_back(std::move(json_list));
    }

}