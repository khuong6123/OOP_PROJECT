#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include "../LIB/common.h"
#include "../DTO/Enums.h"
#include "../LIB/DateUtils.h"
#include <string>
#include <chrono>

class InputValidator {
public:
    // Task validation
    static bool ValidateTaskTitle(const std::string& title);
    static bool ValidateTaskDescription(const std::string& description);
    static bool ValidateDueDate(const std::chrono::system_clock::time_point& dueDate);
    static bool ValidatePriority(const std::string& priority);
    static bool ValidateStatus(const std::string& status);
    
    // Category validation
    static bool ValidateCategoryName(const std::string& name);
    static bool ValidateCategoryDescription(const std::string& description);
    static bool ValidateColor(const std::string& color);
    
    // Recurrence validation
    static bool ValidateRecurrenceType(const std::string& recurrenceType);
    static bool ValidateRecurrenceInterval(int interval);
    static bool ValidateDaysOfWeek(const std::vector<Enums::DayOfWeek>& days);
    
    // General validation
    static bool ValidateId(int id);
    static bool ValidateDateRange(const std::chrono::system_clock::time_point& start, 
                                 const std::chrono::system_clock::time_point& end);
    static bool ValidateTimeString(const std::string& timeStr);
    
    // Sanitization methods
    static std::string SanitizeString(const std::string& input);
    static std::string SanitizeColor(const std::string& color);
    
private:
    static bool IsValidHexColor(const std::string& color);
    static bool IsValidDateString(const std::string& dateStr);
};

#endif // INPUT_VALIDATOR_H