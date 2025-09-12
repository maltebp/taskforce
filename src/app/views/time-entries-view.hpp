#pragma once

#include <memory>
#include <vector>
#include <string>

#include "app/views/time-entries-view-model.hpp"

namespace tf {

    class Workspace;

    class TimeEntriesView {
    public:

        TimeEntriesView(std::shared_ptr<Workspace> workspace);

        void draw();

    private:

        TimeEntriesViewModel m_view_model;

    };

}
