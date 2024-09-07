#pragma once

#include <memory>

namespace tf {

    class Workspace;

    class TimeEntriesView {
    public:

        TimeEntriesView(std::shared_ptr<Workspace> workspace);

        void draw();

    private:

        const std::shared_ptr<Workspace> m_workspace;

    };

}
