#include "time-entries-view.hpp"

#include <format>
#include <algorithm>

#include <imgui.h>
#include <imgui_stdlib.h>

#include "workspace.hpp"


namespace tf {

    // TODO: Unit test this
    std::optional<std::chrono::minutes> parse_time(const std::string& minutes_string_input)
    {
        if( minutes_string_input.size() == 0 ) return std::chrono::minutes(0);

        std::string cleaned_input = minutes_string_input;
        std::string::iterator new_end = std::remove_if(cleaned_input.begin(), cleaned_input.end(), [](int i) -> int { return std::isspace(i); });
        cleaned_input.erase(new_end, cleaned_input.end());

        std::size_t colon_index = cleaned_input.find(':');
        std::string hours_string = cleaned_input.substr(0, colon_index);

        try {

            int hours = hours_string.size() == 0 ? 0 : std::stoi(hours_string);
            if (hours < 0 || hours > 24) return {};

            if (hours_string.size() == cleaned_input.size()) return std::chrono::minutes(hours * 60);
                
            std::string minutes_string = cleaned_input.substr(colon_index + 1);

            int minutes = minutes_string.size() == 0 ? 0 : std::stoi(minutes_string);
            if (minutes < 0 || minutes > 60) return {};

            return std::chrono::minutes(hours * 60 + minutes);
        }
        catch (std::invalid_argument e) {
            return {};
        }        
    }

    TimeEntryInput::TimeEntryInput(std::shared_ptr<TimeEntry> time_entry)
        :   m_time_entry(time_entry),
            m_description(m_time_entry->description),
            m_date(std::format("{:%y-%m-%d}", m_time_entry->date)),
            m_start_time(std::format("{:%H:%M}", m_time_entry->start_time)),
            m_end_time(std::format("{:%H:%M}", m_time_entry->end_time))
    { }

    bool TimeEntryInput::update_description() {
        m_time_entry->description = m_description;
        return true;
    }

    bool TimeEntryInput::update_date() {
        return true;
    }

    bool TimeEntryInput::update_start_time() {
        std::optional<std::chrono::minutes> time = parse_time(m_start_time);
        if (time.has_value()) {
            m_time_entry->start_time = time.value();
            return true;
        }
        return false;
    }

    bool TimeEntryInput::update_end_time() {
        std::optional<std::chrono::minutes> time = parse_time(m_end_time);
        if (time.has_value()) {
            m_time_entry->end_time = time.value();
            return true;
        }
        return false;
    }


    TimeEntriesViewModel::TimeEntriesViewModel(std::shared_ptr<Workspace> workspace)
        :   m_workspace(workspace)
    { 
        m_time_entries_inputs.reserve(m_workspace->time_entries.size());

        for( std::shared_ptr<TimeEntry> time_entry : m_workspace->time_entries ) {
            m_time_entries_inputs.push_back(std::make_unique<TimeEntryInput>(time_entry));
        }
    }

	TimeEntriesView::TimeEntriesView(std::shared_ptr<Workspace> workspace) 
		:	m_view_model(workspace)
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
            clipper.Begin(static_cast<int>(m_view_model.m_time_entries_inputs.size()));
            while (clipper.Step())
            {
                for (int row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    ImGui::PushID(row);

                    auto add_editable_cell = [](const std::string& label, std::string& text_to_edit, bool is_valid_value) {
                        ImGui::PushStyleColor(
                            ImGuiCol_FrameBg, 
                            is_valid_value ? IM_COL32(0, 0, 0, 0) : IM_COL32(255, 0, 0, 255)
                        );
                        ImGui::PushItemWidth(-FLT_MIN);
                        ImGui::InputText(label.c_str(), &text_to_edit);
                        ImGui::PopItemWidth();
                        ImGui::PopStyleColor();
                    };

                    ImGui::TableNextRow();

                    TimeEntryInput& entry_input = *m_view_model.m_time_entries_inputs[row];

                    bool is_valid_value;

                    is_valid_value = entry_input.update_description();
                    ImGui::TableSetColumnIndex(0);
                    add_editable_cell("description", entry_input.m_description, is_valid_value);
                    entry_input.update_description();

                    is_valid_value = entry_input.update_date();
                    ImGui::TableSetColumnIndex(1);
                    add_editable_cell("date", entry_input.m_date, is_valid_value);
                    entry_input.update_date();

                    is_valid_value = entry_input.update_start_time();
                    ImGui::TableSetColumnIndex(2);
                    add_editable_cell("start_time", entry_input.m_start_time, is_valid_value);
                    entry_input.update_start_time();

                    is_valid_value = entry_input.update_end_time();
                    ImGui::TableSetColumnIndex(3);
                    add_editable_cell("end_time", entry_input.m_end_time, is_valid_value);
                    entry_input.update_end_time();

                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }
	}

}
