#include <chrono>
#include <iostream>
#include <filesystem>
#include <fstream>

#include <json.hpp>

#include "model/core/expect.hpp"
#include "app/imgui-app.hpp"

#include "model/workspace.hpp"
#include "model/time-entry.hpp"
#include "model/serialization/json/json-serializer.hpp"

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
        a.some_numbers = readable_object.read<std::vector<int>>("some_numbers");
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
    tf::Result<void> write_result = serializer.write(ostream, a);
    if (write_result.is_err()) {
        std::cerr << "Failed to write object" << std::endl;
        return;
    }

    std::string output= ostream.str();

    std::cout << "JSON 1: " << output << std::endl;

    std::istringstream istream{ output }; 

    tf::Result<MyStructA> a2 = serializer.read<MyStructA>(istream);

    tf::expect(a2.is_ok());

    tf::expect(a2.get_ok().some_number == 42);
    tf::expect(a2.get_ok().some_string == "Hello, world!");
    tf::expect(a2.get_ok().some_numbers[0] == 1);
    tf::expect(a2.get_ok().some_numbers[1] == 2);
    tf::expect(a2.get_ok().some_numbers[2] == 3);
    tf::expect(a2.get_ok().some_numbers[3] == 4);
    tf::expect(a2.get_ok().other_struct.some_number == 1337);

    tf::Result<void> write_result_2 = serializer.write(std::cout, a2.get_ok());
       if( write_result_2.is_err() ) {
        std::cerr << "Failed to write object" << std::endl;
        return;
    }
}

namespace tf {

    namespace {
        std::shared_ptr<Workspace> g_workspace;
        std::shared_ptr<MainWindow> g_main_window;
    }

    static std::shared_ptr<Workspace> create_test_workspace() {
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

    

    static std::shared_ptr<tf::Workspace> open_workspace(const std::filesystem::path& path) {
        
        if (!std::filesystem::is_regular_file(path)) {
            return nullptr;
        }

        std::ifstream file{path};
        if (!file.good()) {
            // TODO: Report proper error
            return nullptr;
        }

        JsonSerializer serializer{};
        Result<Workspace> read_workspace_result = serializer.read<tf::Workspace>(file);
        if( read_workspace_result.is_err() ) {
            // TODO: Report proper error
            return nullptr;
        }

        return std::make_shared<Workspace>(Workspace{std::move(read_workspace_result.get_ok())});
    }

    static std::shared_ptr<Workspace> get_initial_workspace() {
        std::shared_ptr<Workspace> workspace = open_workspace("c:\\Users\\malte\\Desktop\\test-workspace.json");
        if( workspace != nullptr ) {
            return workspace;
        }

        return create_test_workspace();;
    }

    void initialize() {
        g_workspace = get_initial_workspace();
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

    tf::initialize();
    return tf::run_imgui_app(tf::app_loop, tf::app_shutdown);
}