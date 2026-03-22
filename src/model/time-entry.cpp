#include "time-entry.hpp"

#include "parse-util.hpp"
#include "serialization/object-serializer.hpp"
#include "serialization/serialization-exception.hpp"

// Serialization
// -------------------------------------------------------------------------

void tf::ObjectSerializer<tf::TimeEntry>::write(
    WritableObject& writable_object, 
    const TimeEntry& object_to_write
) {
    writable_object.write("description", object_to_write.description);
    writable_object.write("date", std::format("{:%y-%m-%d}", object_to_write.date));
    writable_object.write("startTime", std::format("{:%H:%M}", object_to_write.start_time));
    writable_object.write("endTime", std::format("{:%H:%M}", object_to_write.end_time));
}

tf::TimeEntry tf::ObjectSerializer<tf::TimeEntry>::read(const ReadableObject& readable_object) {
        
    std::string description = readable_object.read<std::string>("description");			

    std::string date_string = readable_object.read<std::string>("date");
    std::optional<std::chrono::year_month_day> date = tf::parse_util::parse_year_month_day(date_string);
    if (!date.has_value()) throw tf::SerializationException("Invalid date string '" + date_string + "'" );

    std::string start_time_string = readable_object.read<std::string>("startTime");
    std::optional<std::chrono::minutes> start_time = tf::parse_util::parse_hours_minutes(start_time_string);
    if (!start_time.has_value()) throw tf::SerializationException("Invalid time string '" + start_time_string + "'" );

    std::string end_time_string = readable_object.read<std::string>("endTime");
    std::optional<std::chrono::minutes> end_time = tf::parse_util::parse_hours_minutes(end_time_string);
    if (!end_time.has_value()) throw tf::SerializationException("Invalid time string '" + end_time_string + "'" );
    
    return TimeEntry{description, date.value(), start_time.value(), end_time.value() };
}