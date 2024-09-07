#pragma once

#include <memory>

namespace tf {

	class Workspace;
	class TimeEntriesView;

	class MainWindow {
	public:

		MainWindow(std::shared_ptr<Workspace> workspace);

		void draw();

	private:

		const std::shared_ptr<Workspace> m_workspace;
		const std::shared_ptr<TimeEntriesView> m_time_entries_view;

	};

}