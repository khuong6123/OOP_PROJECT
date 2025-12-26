#ifndef RECURRENCE_PATTERN_H
#define RECURRENCE_PATTERN_H

#include "Enums.h"
#include "../LIB/common.h"
#include <chrono>
#include <vector>
#include <memory>

class RecurrencePattern {
public:
    RecurrencePattern();
    RecurrencePattern(Enums::RecurrenceType type, int interval, 
                     const std::vector<Enums::DayOfWeek>& daysOfWeek = {});

    // Getters
    Enums::RecurrenceType GetType() const;
    int GetInterval() const;
    const std::vector<Enums::DayOfWeek>& GetDaysOfWeek() const;
    int GetOccurrenceCount() const;
    const std::chrono::system_clock::time_point& GetEndDate() const;

    // Setters
    void SetType(Enums::RecurrenceType type);
    void SetInterval(int interval);
    void SetDaysOfWeek(const std::vector<Enums::DayOfWeek>& daysOfWeek);
    void SetOccurrenceCount(int count);
    void SetEndDate(const std::chrono::system_clock::time_point& endDate);

    // Utility methods
    bool IsRecurring() const;

private:
    Enums::RecurrenceType type_;
    int interval_; // e.g., every 2 days, every 3 weeks
    std::vector<Enums::DayOfWeek> daysOfWeek_; // For weekly recurrence
    int occurrenceCount_; // 0 means infinite
    std::chrono::system_clock::time_point endDate_;
};

using RecurrencePatternPtr = Common::Ref<RecurrencePattern>;

#endif // RECURRENCE_PATTERN_H