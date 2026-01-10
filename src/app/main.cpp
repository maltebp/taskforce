#include <chrono>
#include <iostream>

#include <json.hpp>

#include "model/core/expect.hpp"

#include "model/workspace.hpp"
#include "model/time-entry.hpp"
#include "model/serialization/json-serialization.hpp"

#include "app/views/main-window.hpp"

using namespace std::chrono_literals;


struct MyStructB {
    int some_number;
};

struct MyStructA {
    int some_number;
    std::string some_string;
    std::vector<int> some_numbers;
    MyStructB other_struct;
};

template<>
struct tf::ObjectSerializer<MyStructB> {

    void write(tf::WritableObject& writable_object, const MyStructB& value) {
        writable_object.write("some_number", value.some_number);
    }

    MyStructB read(const tf::ReadableObject& readable_object) {
        MyStructB a;
        a.some_number = readable_object.read<int>("some_number");
        return a;
    }

};

template<>
struct tf::ObjectSerializer<MyStructA> {

    void write(tf::WritableObject& writable_object, const MyStructA& value) {
        writable_object.write("some_number", value.some_number);
        writable_object.write("some_string", value.some_string);
        writable_object.write("some_numbers", value.some_numbers.begin(), value.some_numbers.end());
        writable_object.write("other_struct", value.other_struct);
    }

    MyStructA read(const tf::ReadableObject& readable_object) {
        MyStructA a;
        a.some_number = readable_object.read<int>("some_number");
        a.some_string = readable_object.read<std::string>("some_string");
        a.some_numbers = readable_object.read_list<int>("some_numbers");
        a.other_struct = readable_object.read<MyStructB>("other_struct");
        return a;
    }

};

void test_json_serialization() {

    MyStructA a;
    a.some_number = 42;
    a.some_string = "Hello, world!";
    a.some_numbers = { 1, 2, 3, 4 };
    a.other_struct.some_number = 1337;

    std::ostringstream ostream;

    tf::JsonSerializer serializer{};
    serializer.write_value(ostream, a);

    std::string output= ostream.str();

    std::cout << "JSON 1: " << output << std::endl;

    std::istringstream istream{ output }; 

    tf::Result<MyStructA> a2 = serializer.read_value<MyStructA>(istream);

    tf::expect(a2.is_ok());

    tf::expect(a2.get_ok().some_number == 42);
    tf::expect(a2.get_ok().some_string == "Hello, world!");
    tf::expect(a2.get_ok().some_numbers[0] == 1);
    tf::expect(a2.get_ok().some_numbers[1] == 2);
    tf::expect(a2.get_ok().some_numbers[2] == 3);
    tf::expect(a2.get_ok().some_numbers[3] == 4);
    tf::expect(a2.get_ok().other_struct.some_number == 1337);

    serializer.write_value(std::cout, a2.get_ok());
}

namespace tf {

    namespace {
        std::shared_ptr<Workspace> g_workspace;
        std::shared_ptr<MainWindow> g_main_window;
    }

    std::shared_ptr<Workspace> create_test_workspace() {
        std::shared_ptr<Workspace> workspace = std::make_shared<Workspace>();

        workspace->name = "IO Workspace";
        workspace->time_entries.push_back(std::make_shared<TimeEntry>(
            "Meeting discussing new feature",
            std::chrono::year_month_day(2024y, std::chrono::month{5}, 17d),
            8h,
            9h + 20min
        ));

        workspace->time_entries.push_back(std::make_shared<TimeEntry>(
            "Planning new feature",
            std::chrono::year_month_day(2024y, std::chrono::month{5}, 17d),
            9h + 30min,
            10h + 10min
        ));

        workspace->time_entries.push_back(std::make_shared<TimeEntry>(
            "Monthly planning",
            std::chrono::year_month_day(2024y, std::chrono::month{5}, 18d),
            10h + 15min,
            12h + 0min
        ));

        workspace->time_entries.push_back(std::make_shared<TimeEntry>(
            "Fixing bug reported by Bob",
            std::chrono::year_month_day(2024y, std::chrono::month{5}, 18d),
            13h + 30min,
            16h + 10min
        ));

        workspace->time_entries.push_back(std::make_shared<TimeEntry>(
            "Starting up new feature",
            std::chrono::year_month_day(2024y, std::chrono::month{5}, 18d),
            16h + 10min,
            20h + 5min
        ));

        return workspace;
    }

    void initialize() {
        g_workspace = create_test_workspace();
        g_main_window = std::make_shared<MainWindow>(g_workspace);
    }

    void app_loop() {
        g_main_window->draw();
    }

    void app_shutdown() {
        
    }

}

int main() {

    tf::s_expect_report_function = tf::report_expect_to_dialog;

    test_json_serialization();

    //tf::initialize();
    //return tp::run_imgui_app(tf::app_loop, tf::app_shutdown);
}