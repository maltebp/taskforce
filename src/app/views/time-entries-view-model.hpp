#pragma once

#include <memory>
#include <vector>

#include "app/views/time-entry-input.hpp"

namespace tf{

    class Workspace;
    class TimeEntryInput;

    class TimeEntriesViewModel {
    public:

        TimeEntriesViewModel(std::shared_ptr<Workspace> workspace);

        void add_new_entry_today();

        const std::shared_ptr<Workspace> m_workspace;
        std::vector<std::unique_ptr<TimeEntryInput>> m_time_entries_inputs;
    };

}