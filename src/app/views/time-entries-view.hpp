#pragma once

#include <memory>
#include <vector>
#include <string>

namespace tf {

    class Workspace;
    class TimeEntry;

    class TimeEntryInput {
    public:

        TimeEntryInput(std::shared_ptr<TimeEntry> time_entry);

        // Parses the input string, updates the m_time_entry if it valid, returns whether it was updated

        bool description_is_valid() const;

        bool update_description();

        bool date_is_valid() const;

        bool update_date();

        bool start_time_is_valid() const;

        bool update_start_time();

        bool end_time_is_valid() const;

        bool update_end_time();

        const std::shared_ptr<TimeEntry> m_time_entry;
        std::string m_description;
        std::string m_date;
        std::string m_start_time;
        std::string m_end_time;
    };

    class TimeEntriesViewModel {
    public:
           
        TimeEntriesViewModel(std::shared_ptr<Workspace> workspace);

        void add_new_entry_today();

        const std::shared_ptr<Workspace> m_workspace;
        std::vector<std::unique_ptr<TimeEntryInput>> m_time_entries_inputs;
    };

    class TimeEntriesView {
    public:

        TimeEntriesView(std::shared_ptr<Workspace> workspace);

        void draw();

    private:

        TimeEntriesViewModel m_view_model;

    };

}
