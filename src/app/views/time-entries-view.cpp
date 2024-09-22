#include "time-entries-view.hpp"

#include <format>
#include <algorithm>

#include <imgui.h>
#include <imgui_stdlib.h>

#include "parse-util.hpp"
#include "workspace.hpp"


namespace tf {

    TimeEntryInput::TimeEntryInput(std::shared_ptr<TimeEntry> time_entry)
        :   m_time_entry(time_entry),
            m_description(m_time_entry->description),
            m_date(std::format("{:%y-%m-%d}", m_time_entry->date)),
            m_start_time(std::format("{:%H:%M}", m_time_entry->start_time)),
            m_end_time(std::format("{:%H:%M}", m_time_entry->end_time))
    { }

    bool TimeEntryInput::description_is_valid() const {
        return true;
    }

    bool TimeEntryInput::update_description() {
        if( description_is_valid() ) {
            m_time_entry->description = m_description;
            return true;
        }
        return false;
    }

    bool TimeEntryInput::date_is_valid() const{
        return true; 
    }

    bool TimeEntryInput::update_date() {
        return true;
    }

    bool TimeEntryInput::start_time_is_valid() const {
        std::optional<std::chrono::minutes> time = parse_util::parse_hours_minutes(m_start_time);
        return time.has_value();
    }

    bool TimeEntryInput::update_start_time() {
        std::optional<std::chrono::minutes> time = parse_util::parse_hours_minutes(m_start_time);
        if (time.has_value()) {
            m_time_entry->start_time = time.value();
            return true;
        }
        return false;
    }

    bool TimeEntryInput::end_time_is_valid() const {
        std::optional<std::chrono::minutes> time = parse_util::parse_hours_minutes(m_end_time);
        return time.has_value();
    }

    bool TimeEntryInput::update_end_time() {
        std::optional<std::chrono::minutes> time = parse_util::parse_hours_minutes(m_end_time);
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

    void TimeEntriesViewModel::add_new_entry_today() {
        std::shared_ptr<TimeEntry> new_entry = std::make_shared<TimeEntry>();
        new_entry->date = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
        m_workspace->time_entries.push_back(new_entry);
        m_time_entries_inputs.push_back(std::make_unique<TimeEntryInput>(new_entry));
    }

	TimeEntriesView::TimeEntriesView(std::shared_ptr<Workspace> workspace) 
		:	m_view_model(workspace)
	{ }

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

            bool created_new = false;
            if( ImGui::Shortcut(ImGuiMod_Ctrl | ImGuiKey_Enter, ImGuiInputFlags_RouteGlobal | ImGuiInputFlags_RouteOverFocused) ) {
                m_view_model.add_new_entry_today();
                created_new = true;
            }

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
                        bool enter_was_pressed = ImGui::InputText(label.c_str(), &text_to_edit, ImGuiInputTextFlags_EnterReturnsTrue);
                        ImGui::PopItemWidth();
                        ImGui::PopStyleColor();

                        return enter_was_pressed;
                    };

                    ImGui::TableNextRow();

                    TimeEntryInput& entry_input = *m_view_model.m_time_entries_inputs[row];

                    if( created_new && row == clipper.DisplayEnd - 1 ) {
                        ImGui::SetKeyboardFocusHere();
                    }

                    { // Description
                        ImGui::TableSetColumnIndex(0);
                        bool enter_was_pressed = add_editable_cell("description", entry_input.m_description, entry_input.description_is_valid());
                        if( enter_was_pressed ) {
                            entry_input.update_description();
                            ImGui::SetKeyboardFocusHere();
                        }
                    }

                    { // Date
                        ImGui::TableSetColumnIndex(1);
                        bool enter_was_pressed = add_editable_cell("date", entry_input.m_date, entry_input.date_is_valid());
                        if( enter_was_pressed ) {
                            ImGui::SetKeyboardFocusHere();
                            entry_input.update_date();
                        }
                    }

                    { // Start time
                        ImGui::TableSetColumnIndex(2);
                        bool enter_was_pressed = add_editable_cell("start_time", entry_input.m_start_time, entry_input.start_time_is_valid());
                        if( enter_was_pressed ) {
                            entry_input.update_start_time();
                            ImGui::SetKeyboardFocusHere();
                        }
                    }

                    { // End time
                        ImGui::TableSetColumnIndex(3);
                        bool enter_was_pressed = add_editable_cell("end_time", entry_input.m_end_time, entry_input.end_time_is_valid());
                        if( enter_was_pressed ) {
                            entry_input.update_end_time();
                            ImGui::SetKeyboardFocusHere();
                        }
                    }

                    ImGui::PopID();
                }
            }
            ImGui::EndTable();
        }
	}

}
