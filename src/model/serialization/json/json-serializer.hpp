#pragma once

#include <memory>

#include <json.hpp>

#include "serialization/json/json-serializer.fwd.hpp"
#include "serialization/serializer.hpp" // IWYU pragma: export

namespace tf {

    class JsonSerializer : public Serializer {
	public:

		JsonSerializer();

		~JsonSerializer();

	protected:

		void write_internal(
			std::ostream& stream,
			std::function<void(WritableObject& root_object)> write_callback
		) override;

		[[nodiscard]] const ReadableObject& read_internal(std::istream& stream) override;

		void reset() override;

	private:

		class Impl;

		std::unique_ptr<Impl> impl;

	};

}