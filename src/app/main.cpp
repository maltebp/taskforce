#include <chrono>

#include "model/workspace.hpp"
#include "model/time-entry.hpp"

#include "app/imgui-app.hpp"
#include "app/views/main-window.hpp"

using namespace std::chrono_literals;

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
    tf::initialize();
    return tp::run_imgui_app(tf::app_loop, tf::app_shutdown);
}