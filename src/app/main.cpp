#include <imgui.h>

#include "imgui-app.hpp"

// Tab bar: https://github.com/ocornut/imgui/issues/261

//bool BeginTabBar(const char* str_id, ImGuiTabBarFlags flags = 0);        // create and append into a TabBar
//void EndTabBar();
//bool BeginTabItem(const char* label, bool* p_open = NULL, ImGuiTabItemFlags flags = 0);// create a Tab. Returns true if the Tab is selected.
//void EndTabItem();                                                       // only call EndTabItem() if BeginTabItem() returns true!
//void SetTabItemClosed(const char* tab_or_docked_window_label)

namespace tp {

    namespace {
        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    }

    void app_loop() {

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::Begin("Another Window", &show_another_window, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)

        if (ImGui::BeginTabBar("MainTabBar")) {

            if (ImGui::BeginTabItem("Avocado"))
            {
                ImGui::Text("This is the Avocado tab!\nblah blah blah blah blah");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Broccoli"))
            {
                ImGui::Text("This is the Broccoli tab!\nblah blah blah blah blah");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Cucumber"))
            {
                ImGui::Text("Hello from another window!");
                ImGui::Columns(2);
                ImGui::Text("Column 1");
                ImGui::NextColumn();
                ImGui::Text("Column 2");
                ImGui::NextColumn();

                ImGui::Separator();
                ImGui::Text("Column 2");
                ImGui::NextColumn();
                ImGui::Text("Column 2");
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }


        ImGui::End();
        ImGui::PopStyleVar(1);
    }

    void app_shutdown() {

    }

}

int main() {
    return tp::run_imgui_app(tp::app_loop, tp::app_shutdown);
}