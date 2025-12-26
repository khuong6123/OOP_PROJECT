#include "Enums.h"
#include <algorithm>
#include <stdexcept>

namespace Enums {
    std::string PriorityToString(Priority priority) {
        switch (priority) {
            case Priority::LOW:    return "LOW";
            case Priority::MEDIUM: return "MEDIUM";
            case Priority::HIGH:   return "HIGH";
            case Priority::URGENT: return "URGENT";
            default:               return "UNKNOWN";
        }
    }

    Priority StringToPriority(const std::string& str) {
        std::string upper = str;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        
        if (upper == "LOW") return Priority::LOW;
        if (upper == "MEDIUM") return Priority::MEDIUM;
        if (upper == "HIGH") return Priority::HIGH;
        if (upper == "URGENT") return Priority::URGENT;
        
        throw std::invalid_argument("Invalid priority string: " + str);
    }

    std::string TaskStatusToString(TaskStatus status) {
        switch (status) {
            case TaskStatus::PENDING:      return "PENDING";
            case TaskStatus::IN_PROGRESS:  return "IN_PROGRESS";
            case TaskStatus::COMPLETED:    return "COMPLETED";
            case TaskStatus::CANCELLED:    return "CANCELLED";
            default:                       return "UNKNOWN";
        }
    }

    TaskStatus StringToTaskStatus(const std::string& str) {
        std::string upper = str;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        
        if (upper == "PENDING") return TaskStatus::PENDING;
        if (upper == "IN_PROGRESS" || upper == "IN PROGRESS") return TaskStatus::IN_PROGRESS;
        if (upper == "COMPLETED") return TaskStatus::COMPLETED;
        if (upper == "CANCELLED") return TaskStatus::CANCELLED;
        
        throw std::invalid_argument("Invalid task status string: " + str);
    }

    std::string RecurrenceTypeToString(RecurrenceType type) {
        switch (type) {
            case RecurrenceType::NONE:    return "NONE";
            case RecurrenceType::DAILY:   return "DAILY";
            case RecurrenceType::WEEKLY:  return "WEEKLY";
            case RecurrenceType::MONTHLY: return "MONTHLY";
            case RecurrenceType::YEARLY:  return "YEARLY";
            default:                      return "UNKNOWN";
        }
    }

    RecurrenceType StringToRecurrenceType(const std::string& str) {
        std::string upper = str;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        
        if (upper == "NONE") return RecurrenceType::NONE;
        if (upper == "DAILY") return RecurrenceType::DAILY;
        if (upper == "WEEKLY") return RecurrenceType::WEEKLY;
        if (upper == "MONTHLY") return RecurrenceType::MONTHLY;
        if (upper == "YEARLY") return RecurrenceType::YEARLY;
        
        throw std::invalid_argument("Invalid recurrence type string: " + str);
    }

    std::string DayOfWeekToString(DayOfWeek day) {
        switch (day) {
            case DayOfWeek::SUNDAY:    return "SUNDAY";
            case DayOfWeek::MONDAY:    return "MONDAY";
            case DayOfWeek::TUESDAY:   return "TUESDAY";
            case DayOfWeek::WEDNESDAY: return "WEDNESDAY";
            case DayOfWeek::THURSDAY:  return "THURSDAY";
            case DayOfWeek::FRIDAY:    return "FRIDAY";
            case DayOfWeek::SATURDAY:  return "SATURDAY";
            default:                   return "UNKNOWN";
        }
    }

    DayOfWeek StringToDayOfWeek(const std::string& str) {
        std::string upper = str;
        std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
        
        if (upper == "SUNDAY") return DayOfWeek::SUNDAY;
        if (upper == "MONDAY") return DayOfWeek::MONDAY;
        if (upper == "TUESDAY") return DayOfWeek::TUESDAY;
        if (upper == "WEDNESDAY") return DayOfWeek::WEDNESDAY;
        if (upper == "THURSDAY") return DayOfWeek::THURSDAY;
        if (upper == "FRIDAY") return DayOfWeek::FRIDAY;
        if (upper == "SATURDAY") return DayOfWeek::SATURDAY;
        
        throw std::invalid_argument("Invalid day of week string: " + str);
    }
}