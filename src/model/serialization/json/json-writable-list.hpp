#pragma once

#include <json.hpp>

#include "serialization/writable-list.hpp"

namespace tf {

    class JsonWritableList : public WritableList {
	public:

		
		JsonWritableList(nlohmann::json& json);

		void write(int value) override;

		void write(std::string_view value) override;

	protected:

		virtual void write_to_object(std::function<void(WritableObject& object)> write_callback) override;

		virtual void write_to_list(std::function<void(WritableList& list)> write_callback) override;

	private:

		nlohmann::json& json;

	};

}