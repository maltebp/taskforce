#include "workspace.hpp"

#include "serialization/object-serializer.hpp"

// Serialization
// -------------------------------------------------------------------------

void tf::ObjectSerializer<tf::Workspace>::write(
    WritableObject& writable_object, 
    const Workspace& object_to_write
) {
    writable_object.write("name", object_to_write.name);

    // TODO: We need an easier way to write a list of smart pointers as their objects
    std::vector<tf::TimeEntry> time_entries{};
    time_entries.reserve(object_to_write.time_entries.size());
    for( const std::shared_ptr<tf::TimeEntry>& time_entry_ptr : object_to_write.time_entries ) {
        time_entries.push_back(*time_entry_ptr);
    }
    writable_object.write("timeEntries", time_entries);
}

tf::Workspace tf::ObjectSerializer<tf::Workspace>::read(const ReadableObject& readable_object) {

    tf::Workspace workspace {
        readable_object.read<std::string>("name")
    };

    std::vector<tf::TimeEntry> time_entries = readable_object.read<std::vector<tf::TimeEntry>>("timeEntries");
    workspace.time_entries.reserve(time_entries.size());
    for( const tf::TimeEntry& time_entry : time_entries) {
        workspace.time_entries.push_back(std::make_shared<tf::TimeEntry>(time_entry));
    }

    return workspace;
}