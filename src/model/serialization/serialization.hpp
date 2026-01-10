#pragma once

#include <optional>
#include <variant>
#include <ostream>
#include <string_view>
#include <vector>
#include <functional>

#include "core/expect.hpp"
#include "core/result.hpp"

namespace tf {

	class SerializationException : public std::runtime_error {
	public:

		SerializationException(std::string info)
			:	std::runtime_error(info)
		{ 
			// TODO: Figure out how to rethrow (not sure how the scope of the nested exception works)
			// We make want to add more specific exceptions in the future
		}

	};

	class WritableObject;
	class WritableList;
	class ReadableObject;

	// TODO: Comment here
	template<typename T>
	struct ObjectSerializer {

		// TODO: False type here

		void write(WritableObject& object, const T& value);

		// TODO: Figure out how to do proper move here
		T read(const ReadableObject& object);

	};

	class WritableObject {
	public:
		// TODO: Implement rest of write

		virtual void write(std::string_view name, int value) = 0;

		virtual void write(std::string_view name, std::string_view value) = 0;

		void write(std::string_view name, const std::string& value) { write(name, std::string_view{ value }); }

		// TODO: Use concept here (+figure out if we can have static test tests whether parameters are)
		template<typename TIterator>
		void write(std::string_view name, TIterator begin, TIterator end);

		template<typename T>
		void write(std::string_view name, const T& value) {
			write_to_object(
				name,
				 [&](WritableObject& sub_object) {
					// Why the callback: the serialization implementation must be able to construct
					// a derived version of WritableObject, which is then the object written to. A
					// function that creates such a derived WritableObject would have to return it 
					// by reference/pointer, but in order to ensure it stays alive for the writing
					// scope, it would have heap allocate (or do other, more complicated tricks).
					// It is my assumption that this lambda here will not cause a heap allocation.

					ObjectSerializer<T> serializer;
					serializer.write(sub_object, value);
				}
			);
		}

	protected:

		virtual void write_to_object(std::string_view name, std::function<void(WritableObject&)> write_callback ) = 0;

		virtual void write_to_list(std::string_view name, std::function<void(WritableList&)> write_callback ) = 0;

	};

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
			
			std::optional<T> value{};
			bool object_found = read_object(
				name,
				[&](const ReadableObject& object_to_read) {
					ObjectSerializer<T> object_serializer{};
					value = object_serializer.read(object_to_read);
				}
			);

			return object_found ? value : std::nullopt;
		}

		template<typename T>
		T read(std::string_view name) const {
			std::optional<T> value = read_optional<T>(name);
			
			if( !value.has_value() ) throw SerializationException("Property " + std::string(name) + " does not exist");
			
			return value.value();
		}

		template<typename T>
		std::optional<std::vector<T>> read_optional_list(std::string_view name) const  {
			
			ObjectSerializer<T> serializer{};
			std::vector<T> values;

			bool property_existed = read_object_list(
				name,
				[&](const ReadableObject& readable_object) {
					values.push_back(serializer.read(readable_object)); // TODO: Move instead
				}
			);

			if( !property_existed ) return std::nullopt;

			return values;
		}

		template<typename T>
		std::vector<T> read_list(std::string_view name) const {
			// TODO: It is an annoying pattern that this is not just read() as well - figure out if I can fix this

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

		template<typename T>
		Result<T> read_value(std::istream& stream) {	
			try {
				const ReadableObject& root_object = read(stream);
				ObjectSerializer<T> object_serializer{};
				T value = object_serializer.read(root_object);
				reset();
				return Result<T>::ok(std::move(value));
			}
			catch( SerializationException e ) {
				return Error{e.what()};
			}
		}

	protected:

		virtual void write(std::ostream& stream, std::function<void(WritableObject& root_object)> write_callback) = 0;

		/**
		 * @return 
		 * Reference to the last object read from this method. I.e. if this method is called again 
		 * the reference of the old call now points to the object read from the new call. In other
		 * words, this value should not be stored.
		 */
		[[nodiscard]] virtual const ReadableObject& read(std::istream& stream) = 0;

		virtual void reset() = 0;

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