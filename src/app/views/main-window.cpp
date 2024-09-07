#include "main-window.hpp"

#include <imgui.h>

#include "model/workspace.hpp"

#include "app/views/time-entries-view.hpp"

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


        ImGui::End();
        ImGui::PopStyleVar(1);
	}

}

