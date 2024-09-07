#include "time-entries-view.hpp"

#include <imgui.h>

#include "workspace.hpp"


namespace tf {

	TimeEntriesView::TimeEntriesView(std::shared_ptr<Workspace> workspace) 
		:	m_workspace(workspace)
	{ }

    //const int COLUMNS_COUNT = 3;
    //if (ImGui::BeginTable("table_custom_headers", COLUMNS_COUNT, ImGuiTableFlags_Borders | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable))
    //{
    //    ImGui::TableSetupColumn("Apricot");
    //    ImGui::TableSetupColumn("Banana");
    //    ImGui::TableSetupColumn("Cherry");

    //    // Dummy entire-column selection storage
    //    // FIXME: It would be nice to actually demonstrate full-featured selection using those checkbox.
    //    static bool column_selected[3] = {};

    //    // Instead of calling TableHeadersRow() we'll submit custom headers ourselves
    //    ImGui::TableNextRow(ImGuiTableRowFlags_Headers);
    //    for (int column = 0; column < COLUMNS_COUNT; column++)
    //    {
    //        ImGui::TableSetColumnIndex(column);
    //        const char* column_name = ImGui::TableGetColumnName(column); // Retrieve name passed to TableSetupColumn()
    //        ImGui::PushID(column);
    //        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    //        ImGui::Checkbox("##checkall", &column_selected[column]);
    //        ImGui::PopStyleVar();
    //        ImGui::SameLine(0.0f, ImGui::GetStyle().ItemInnerSpacing.x);
    //        ImGui::TableHeader(column_name);
    //        ImGui::PopID();
    //    }

    //    for (int row = 0; row < 5; row++)
    //    {
    //        ImGui::TableNextRow();
    //        for (int column = 0; column < 3; column++)
    //        {
    //            char buf[32];
    //            sprintf(buf, "Cell %d,%d", column, row);
    //            ImGui::TableSetColumnIndex(column);
    //            ImGui::Selectable(buf, column_selected[column]);
    //        }
    //    }
    //    ImGui::EndTable();
    //}
    //ImGui::TreePop();

    // For advanced: 6764

	void TimeEntriesView::draw() {

        static ImGuiTableFlags flags = ImGuiTableFlags_ScrollY | ImGuiTableFlags_RowBg | ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | ImGuiTableFlags_Hideable;

        // When using ScrollX or ScrollY we need to specify a size for our table container!
        // Otherwise by default the table will fit all available space, like a BeginChild() call.
        if (ImGui::BeginTable("TimeEntriesTable", 4, flags))
        {
            ImGui::TableSetupScrollFreeze(0, 1); // Make top row always visible
            ImGui::TableSetupColumn("Description", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("Start time", ImGuiTableColumnFlags_None);
            ImGui::TableSetupColumn("End time", ImGuiTableColumnFlags_None);
            ImGui::TableHeadersRow();

            // Demonstrate using clipper for large vertical lists
            ImGuiListClipper clipper;
            clipper.Begin(m_workspace->time_entries.size());
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    ImGui::TableNextRow();

                    TimeEntry& entry = m_workspace->time_entries[row];

                    ImGui::TableSetColumnIndex(0);
                    char buffer[100];
                    int i = 0;
                    for (;  i < entry.description.size() && i < 99; i++) {
                        buffer[i] = entry.description[i];
                    }
                    buffer[i] = '\0';
                    
                    ImGui::InputText((std::string("c") + std::to_string(row)).c_str(), buffer, 100);
                    entry.description = buffer;


                    ImGui::TableSetColumnIndex(1);
                    ImGui::Text("%d-%d-%d", entry.date.year(), entry.date.month(), entry.date.day());

                    ImGui::TableSetColumnIndex(2);
                    ImGui::Text(
                        "%02d:%02d", 
                        std::chrono::duration_cast<std::chrono::hours>(entry.start_time).count(),
                        std::chrono::duration_cast<std::chrono::minutes>(entry.start_time).count() % 60
                    );

                    ImGui::TableSetColumnIndex(3);

                    ImGui::Text(
                        "%02d:%02d",
                        std::chrono::duration_cast<std::chrono::hours>(entry.end_time).count(),
                        std::chrono::duration_cast<std::chrono::minutes>(entry.end_time).count() % 60
                    );;

                    for (int column = 0; column < 4; column++)
                    {
                        
                    }
                }
            }
            ImGui::EndTable();
        }
	}

}
