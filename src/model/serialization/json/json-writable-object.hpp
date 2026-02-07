#pragma once

#include <json.hpp>

#include "serialization/writable-object.hpp"


namespace tf {

    class JsonWritableObject : public WritableObject {
    public:

        JsonWritableObject(nlohmann::json& json);

        void write(std::string_view name, int value) override;

        void write(std::string_view name, std::string_view value) override;

    protected:

        void write_to_object(
            std::string_view name, 
            std::function<void(WritableObject& object)> write_callback
        ) override;

        void write_to_list(
            std::string_view name, 
            std::function<void(WritableList& list)> write_callback
        ) override;

    private:

        nlohmann::json& json;

    };

    
}

