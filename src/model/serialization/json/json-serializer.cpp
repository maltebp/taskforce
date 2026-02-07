#include "serialization/json/json-serializer.hpp"

#include <json.hpp>

#include "serialization/json/json-writable-object.hpp"
#include "serialization/json/json-readable-object.hpp"

namespace tf {

    class JsonSerializer::Impl {
    public:

        void write(std::ostream& stream, std::function<void(WritableObject& root_object)> write_callback) {
			
			nlohmann::json root_json_object = nlohmann::json::object();
			JsonWritableObject root_object{ root_json_object };
			write_callback(root_object);
			stream << root_json_object;

		}	

		[[nodiscard]] const ReadableObject& read(std::istream& stream) {
			current_read_root_json_object.emplace();
			stream >> current_read_root_json_object.value();
			
			current_read_root_object.emplace(current_read_root_json_object.value());
			return current_read_root_object.value();
		};

		void reset() {
			current_read_root_json_object.reset();
			current_read_root_object.reset();
		}

		// These are held as optional, because (from what I could gather) the nlohmann::json will
		// allocate json objects when deserializing and hold on to them for its life time. As the
		// user of the serializer may hold on to object for a prolonged period of time, we want to
		// be able clean it without destroying it. Could also be achieved with pointers, but
		// std::optional avoids heap allocations.
		std::optional<nlohmann::json> current_read_root_json_object;
		std::optional<JsonReadableObject> current_read_root_object;

	};

    JsonSerializer::JsonSerializer() 
        :   impl(std::make_unique<JsonSerializer::Impl>())
    { }

    JsonSerializer::~JsonSerializer() = default;

    void JsonSerializer::write_internal(std::ostream& stream, std::function<void(WritableObject& root_object)> write_callback) {
        impl->write(stream, write_callback);
    }	

    [[nodiscard]] const ReadableObject& JsonSerializer::read_internal(std::istream& stream) {
        return impl->read(stream);
    };

    void JsonSerializer::reset()  {
        impl->reset();
    }

}

