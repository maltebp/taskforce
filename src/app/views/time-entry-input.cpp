#include "time-entry-input.hpp"

#include <optional>
#include <format>

#include "model/time-entry.hpp"
#include "model/parse-util.hpp"


namespace tf {

    TimeEntryInput::TimeEntryInput(std::shared_ptr<TimeEntry> time_entry)
        : m_time_entry(time_entry),
        m_description(m_time_entry->description),
        m_date(std::format("{:%y-%m-%d}", m_time_entry->date)),
        m_start_time(std::format("{:%H:%M}", m_time_entry->start_time)),
        m_end_time(std::format("{:%H:%M}", m_time_entry->end_time))
    { }

    bool TimeEntryInput::description_is_valid() const {
        // Description is always valid for now (will it ever not be?)
        return true;
    }

    bool TimeEntryInput::update_description() {
        if( description_is_valid() ) {
            m_time_entry->description = m_description;
            return true;
        }
        return false;
    }

    bool TimeEntryInput::date_is_valid() const {
        return parse_util::parse_year_month_day(m_date).has_value();
    }

    bool TimeEntryInput::update_date() {
        std::optional<std::chrono::year_month_day> date = parse_util::parse_year_month_day(m_date);

        if( !date.has_value() ) return false;

        m_time_entry->date = date.value();
        return true;
    }

    bool TimeEntryInput::start_time_is_valid() const {
        std::optional<std::chrono::minutes> time = parse_util::parse_hours_minutes(m_start_time);
        return time.has_value() && time < m_time_entry->end_time;
    }

    bool TimeEntryInput::update_start_time() {
        std::optional<std::chrono::minutes> time = parse_util::parse_hours_minutes(m_start_time);
        if( time.has_value() ) {
            m_time_entry->start_time = time.value();
            return true;
        }
        return false;
    }

    bool TimeEntryInput::end_time_is_valid() const {
        std::optional<std::chrono::minutes> time = parse_util::parse_hours_minutes(m_end_time);
        return time.has_value() && time > m_time_entry->start_time;
    }

    bool TimeEntryInput::update_end_time() {
        std::optional<std::chrono::minutes> time = parse_util::parse_hours_minutes(m_end_time);
        if( time.has_value() ) {
            m_time_entry->end_time = time.value();
            return true;
        }
        return false;
    }

}