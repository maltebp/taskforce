#pragma once


#include <ostream>
#include <string_view>
#include <iterator>
#include <vector>
#include <functional>

#include "core/expect.hpp"

namespace tf {

	class WritableObject;
	class WritableList;
	class ReadableObject;

	template<typename T>
	struct ObjectSerializer {

		// TODO: False type here

		void write(WritableObject& object, const T& value);

		// TODO: Figure out how to do proper move here
		T read(const ReadableObject& object);

	};

	class WritableObject {
	public:
		// TODO: Implement rest of writes

		virtual void write(std::string_view name, int value) = 0;

		virtual void write(std::string_view name, std::string_view value) = 0;

		void write(std::string_view name, const std::string& value) { write(name, std::string_view{ value }); }

		template<typename TIterator>
		void write(std::string_view name, TIterator begin, TIterator end);

		template<typename T>
		void write(std::string_view name, const T& value) {
			write_to_object(
				name, 
				[&value](WritableObject& object) { 
					ObjectSerializer<T> serializer;
					serializer.write(object, value);
				}
			);
		}

	protected:

		virtual void write_to_object(std::string_view name, std::function<void(WritableObject& object)> write_callback) = 0;

		virtual void write_to_list(std::string_view name, std::function<void(WritableList& object)> write_callback) = 0;

	};

	class WritableList{
	public:

		virtual void write(int value) = 0;

		virtual void write(std::string_view value) = 0;

		template<typename TIterator>
		void write(TIterator begin, TIterator end) {
			write_to_list(
				[&begin, &end](WritableList& list) {
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
				[&value](WritableList& list) {
					ObjectSerializer<T> serializer;
					serializer.write(list, value);
				});
		}

	protected:

		virtual void write_to_object(std::function<void(WritableObject& object)> write_callback) = 0;

		virtual void write_to_list(std::function<void(WritableList& object)> write_callback) = 0;

	};

	class ReadableObject {
	public:

		template<typename T>
		std::optional<T> read_optional(std::string_view name) const {
			
			std::optional<T> value;
			bool object_found = read_object(
				name,
				[&](const ReadableObject& object_to_read) {
					ObjectSerializer<T> object_serializer{};
					value = object_serializer.read(object_to_read);
				}
			);

			return value;
		}

		template<typename T>
		T read(std::string_view name) const {
			std::optional<T> value = read_optional<T>(name);
			tf::expect(value.has_value()); // TODO: Change to exception
			return value.value();
		}

		template<typename T>
		std::optional<std::vector<T>> read_optional_list(std::string_view name) const  {
			
			ObjectSerializer<T> serializer{};
			std::vector<T> values;

			bool property_existed = read_object_list(
				name,
				[&](ReadableObject& readable_object) {
					values.push_back(serializer.read()); // TODO: Move instead
				}
			);

			return property_existed ? values : std::nullopt;
		}

		template<typename T>
		std::vector<T> read_list(std::string_view name) const {
			std::optional<std::vector<T>> values = read_optional_list<T>(name);
			tf::expect(values.has_value()); // TODO: Change to exception
			return values.value();
		}

	protected:

		virtual std::optional<int> read_int(std::string_view name) const = 0;

		virtual std::optional<std::string> read_string(std::string_view name) const = 0;

		virtual bool read_object(std::string_view name, std::function<void(const ReadableObject&)> read_callback) const = 0;

		virtual std::optional<std::vector<int>> read_int_list(std::string_view name) const = 0;

		virtual std::optional<std::vector<std::string>> read_string_list(std::string_view name) const = 0;

		virtual bool read_object_list(std::string_view name, std::function<void(const ReadableObject&)> read_callback) const = 0;

	};

	template<>
	std::optional<int> ReadableObject::read_optional<int>(std::string_view name) const { return read_int(name); }

	template<>
	std::optional<std::string> ReadableObject::read_optional<std::string>(std::string_view name) const { return read_string(name); }

	template<>
	std::optional<std::vector<int>> ReadableObject::read_optional_list<int>(std::string_view name) const { return read_int_list(name); }

	template<>
	std::optional<std::vector<std::string>> ReadableObject::read_optional_list<std::string>(std::string_view name) const { return read_string_list(name); }



	class Serializer {
	public:

		virtual void write(std::ostream& stream, std::function<void(WritableObject& root_object)> write_callback) = 0;

		template<typename T>
		void write_value(std::ostream& stream, const T& value) {
			write(
				stream, 
				[&value](WritableObject& root_object) {
					ObjectSerializer<T> object_serializer{};
					object_serializer.write(root_object, value);
				}
			);
		}

		// TODO: Return proper error type here
		virtual void read(std::istream& stream, std::function<void(const ReadableObject& root_object)> read_callback) = 0;

		// TODO: Return proper error type here
		template<typename T>
		T read_value(std::istream& stream) {
			T value;
			
			read(
				stream,
				[&](const ReadableObject& root_object) {
					ObjectSerializer<T> object_serializer{};
					value = object_serializer.read(root_object);
				}
			);

			return value;
		}

	};


	template<typename TIterator>
	void WritableObject::write(std::string_view name, TIterator begin, TIterator end) {
		write_to_list(
			name,
			[&begin, &end](WritableList& list) {
				for( ; begin != end; begin++ ) {
					list.write(*begin);
				}
			}
		);
	}
	
}