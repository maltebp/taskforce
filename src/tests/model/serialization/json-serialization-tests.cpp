// struct MyStructB {
// 	int some_number;
// };
// struct MyStructA {
// 	int some_number;
// 	std::string some_string;
// 	std::vector<int> some_numbers;
// 	MyStructB other_struct;
// };

// template<>
// struct tf::ObjectSerializer<MyStructB> {

// 	void write(tf::WritableObject& writable_object, const MyStructB& value) {
// 		writable_object.write("some_number", value.some_number);

// 	}

// };

// template<>
// struct tf::ObjectSerializer<MyStructA> {

// 	void write(tf::WritableObject& writable_object, const MyStructA& value) {
// 		writable_object.write("some_number", value.some_number);
// 		writable_object.write("some_string", value.some_string);
// 		writable_object.write("some_numbers", value.some_numbers.begin(), value.some_numbers.end());
// 		writable_object.write("other_struct", value.other_struct);
// 	}

// };