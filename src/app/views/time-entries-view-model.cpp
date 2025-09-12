#include "time-entries-view-model.hpp"

#include <chrono>

#include "app/views/time-entry-input.hpp"
#include "model/workspace.hpp"

namespace tf {

    TimeEntriesViewModel::TimeEntriesViewModel(std::shared_ptr<Workspace> workspace)
        : m_workspace(workspace)
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

}
