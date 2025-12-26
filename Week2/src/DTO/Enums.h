#ifndef ENUMS_H
#define ENUMS_H

#include <string>

namespace Enums {
    enum class Priority {
        LOW = 0,
        MEDIUM = 1,
        HIGH = 2,
        URGENT = 3
    };

    enum class TaskStatus {
        PENDING = 0,
        IN_PROGRESS = 1,
        COMPLETED = 2,
        CANCELLED = 3
    };

    enum class RecurrenceType {
        NONE = 0,
        DAILY = 1,
        WEEKLY = 2,
        MONTHLY = 3,
        YEARLY = 4
    };

    enum class DayOfWeek {
        SUNDAY = 0,
        MONDAY = 1,
        TUESDAY = 2,
        WEDNESDAY = 3,
        THURSDAY = 4,
        FRIDAY = 5,
        SATURDAY = 6
    };

    // Utility functions
    std::string PriorityToString(Priority priority);
    Priority StringToPriority(const std::string& str);
    
    std::string TaskStatusToString(TaskStatus status);
    TaskStatus StringToTaskStatus(const std::string& str);
    
    std::string RecurrenceTypeToString(RecurrenceType type);
    RecurrenceType StringToRecurrenceType(const std::string& str);
    
    std::string DayOfWeekToString(DayOfWeek day);
    DayOfWeek StringToDayOfWeek(const std::string& str);
}

#endif // ENUMS_H