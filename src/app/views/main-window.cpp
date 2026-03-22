#include "main-window.hpp"

#include <fstream>
#include <imgui.h>

#include "model/workspace.hpp"

#include "app/views/time-entries-view.hpp"
#include "serialization/json/json-serializer.hpp"

// Tab bar: https://github.com/ocornut/imgui/issues/261
//bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);        // create and append into a TabBar
//void EndTabBar();
//bool BeginTabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0);// create a Tab. Returns true if the Tab is selected.
//void EndTabItem();                                                       // only call EndTabItem() if BeginTabItem() returns true!
//void SetTabItemClosed(const char* tab_or_docked_window_label)

namespace tf {

	MainWindow::MainWindow(std::shared_ptr<Workspace> workspace)
		:	m_workspace(workspace),
			m_time_entries_view(std::make_shared<TimeEntriesView>(workspace))
	{ }

	void MainWindow::draw() {

        ImGui::ShowDemoWindow();

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGuiViewport* viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        bool show_window = true;
        ImGui::Begin("Another Window", &show_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

        if( ImGui::BeginTabBar("MainTabBar") ) {

            if (ImGui::BeginTabItem("Time entries")) { 
                m_time_entries_view->draw();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        if( ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_S, ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_RouteOverFocused) ) {
            std::ofstream file{"c:\\Users\\malte\\Desktop\\test-workspace.json"};
            if( file.good() ) {
                JsonSerializer serializer;
                Result<void> write_result = serializer.write(file, *m_workspace);    
                if( write_result.is_err() ) {
                    // TODO: Better error reporting
                    std::cerr << "Failed to save: " << write_result.get_err().info << std::endl;    
                }
            }
            else {
                // TODO: Better error reporting
                std::cerr << "Failed to save!" << std::endl;
            }
            
        }

        ImGui::End();
        ImGui::PopStyleVar(1);
	}

}

