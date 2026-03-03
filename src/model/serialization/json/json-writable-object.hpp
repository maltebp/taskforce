#pragma once

#include <json.hpp>

#include "serialization/writable-object.hpp"


namespace tf {

    class JsonWritableObject : public WritableObject {
    public:

        JsonWritableObject(nlohmann::json& json);

        void write(std::string_view name, std::int32_t value) override;

		void write(std::string_view name, std::int64_t value) override;

		void write(std::string_view name, float value) override;

		void write(std::string_view name, double value) override;

        void write(std::string_view name, bool value) override;

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

