#pragma once

#include <string>
#include <vector>
#include <functional>

#include "writable-object.fwd.hpp"
#include "object-serializer.dec.hpp"

namespace tf {

    class WritableList{
	public:

		virtual void write(int value) = 0;

		virtual void write(std::string_view value) = 0;

		void write(const std::string& value) {
			write(static_cast<std::string_view>(value));
		}

		template<typename TIterator>
		void write(TIterator begin, TIterator end) {
			write_to_list(
				[&](WritableList& list) {
					for( ; begin != end; begin++ ) {
						list.write(*begin);
					}
				}
			);
		}

		template<typename T>
		void write(const std::vector<T>& value) {
			write(value.begin(), value.end());
		}

		template<typename T>
		void write(const T& value) {
			write_to_object(
				[&value](WritableObject& object) {
					ObjectSerializer<T> serializer;
					serializer.write(object, value);
				}
            );
		}

	protected:

		virtual void write_to_object(std::function<void(WritableObject& object)> write_callback) = 0;

		virtual void write_to_list(std::function<void(WritableList& object)> write_callback) = 0;

	};

}