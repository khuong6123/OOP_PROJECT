#include "RecurrencePattern.h"
#include "../LIB/DateUtils.h"
#include <stdexcept>

RecurrencePattern::RecurrencePattern()
    : type_(Enums::RecurrenceType::NONE)
    , interval_(1)
    , occurrenceCount_(0)
    , endDate_(std::chrono::system_clock::time_point::max()) {
}

RecurrencePattern::RecurrencePattern(Enums::RecurrenceType type, int interval, 
                                   const std::vector<Enums::DayOfWeek>& daysOfWeek)
    : type_(type)
    , interval_(interval)
    , daysOfWeek_(daysOfWeek)
    , occurrenceCount_(0)
    , endDate_(std::chrono::system_clock::time_point::max()) {
    
    if (interval <= 0) {
        throw std::invalid_argument("Recurrence interval must be positive");
    }
    
    if (type == Enums::RecurrenceType::WEEKLY && daysOfWeek_.empty()) {
        throw std::invalid_argument("Weekly recurrence requires at least one day of week");
    }
}

// Getters
Enums::RecurrenceType RecurrencePattern::GetType() const {
    return type_;
}

int RecurrencePattern::GetInterval() const {
    return interval_;
}

const std::vector<Enums::DayOfWeek>& RecurrencePattern::GetDaysOfWeek() const {
    return daysOfWeek_;
}

int RecurrencePattern::GetOccurrenceCount() const {
    return occurrenceCount_;
}

const std::chrono::system_clock::time_point& RecurrencePattern::GetEndDate() const {
    return endDate_;
}

// Setters
void RecurrencePattern::SetType(Enums::RecurrenceType type) {
    type_ = type;
}

void RecurrencePattern::SetInterval(int interval) {
    if (interval <= 0) {
        throw std::invalid_argument("Recurrence interval must be positive");
    }
    interval_ = interval;
}

void RecurrencePattern::SetDaysOfWeek(const std::vector<Enums::DayOfWeek>& daysOfWeek) {
    if (type_ == Enums::RecurrenceType::WEEKLY && daysOfWeek.empty()) {
        throw std::invalid_argument("Weekly recurrence requires at least one day of week");
    }
    daysOfWeek_ = daysOfWeek;
}

void RecurrencePattern::SetOccurrenceCount(int count) {
    if (count < 0) {
        throw std::invalid_argument("Occurrence count cannot be negative");
    }
    occurrenceCount_ = count;
}

void RecurrencePattern::SetEndDate(const std::chrono::system_clock::time_point& endDate) {
    endDate_ = endDate;
}

// Utility methods
bool RecurrencePattern::IsRecurring() const {
    return type_ != Enums::RecurrenceType::NONE;
}
