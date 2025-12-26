#include "InputValidator.h"
#include "../LIB/Constants.h"
#include "../LIB/StringUtils.h"
#include <regex>
#include <chrono>

using namespace std::chrono;

bool InputValidator::ValidateTaskTitle(const std::string& title) {
    if (title.empty()) {
        return false;
    }
    
    std::string trimmed = StringUtils::Trim(title);
    if (trimmed.empty()) {
        return false;
    }
    
    if (trimmed.length() > Constants::MAX_TASK_TITLE_LENGTH) {
        return false;
    }
    
    return true;
}

bool InputValidator::ValidateTaskDescription(const std::string& description) {
    return description.length() <= Constants::MAX_DESCRIPTION_LENGTH;
}

bool InputValidator::ValidateDueDate(const system_clock::time_point& dueDate) {
    auto now = system_clock::now();
    return dueDate >= now;
}

bool InputValidator::ValidatePriority(const std::string& priority) {
    std::string upper = StringUtils::ToUpper(priority);
    return upper == "LOW" || upper == "MEDIUM" || 
           upper == "HIGH" || upper == "URGENT";
}

bool InputValidator::ValidateStatus(const std::string& status) {
    std::string upper = StringUtils::ToUpper(status);
    return upper == "PENDING" || upper == "IN_PROGRESS" || 
           upper == "COMPLETED" || upper == "CANCELLED";
}

bool InputValidator::ValidateCategoryName(const std::string& name) {
    if (name.empty()) {
        return false;
    }
    
    std::string trimmed = StringUtils::Trim(name);
    if (trimmed.empty()) {
        return false;
    }
    
    if (trimmed.length() > Constants::MAX_CATEGORY_NAME_LENGTH) {
        return false;
    }
    
    return true;
}

bool InputValidator::ValidateCategoryDescription(const std::string& description) {
    return description.length() <= Constants::MAX_DESCRIPTION_LENGTH;
}

bool InputValidator::ValidateColor(const std::string& color) {
    if (color.empty()) {
        return false;
    }
    
    std::string sanitized = SanitizeColor(color);
    return IsValidHexColor(sanitized);
}

bool InputValidator::ValidateRecurrenceType(const std::string& recurrenceType) {
    std::string upper = StringUtils::ToUpper(recurrenceType);
    return upper == "NONE" || upper == "DAILY" || upper == "WEEKLY" || 
           upper == "MONTHLY" || upper == "YEARLY";
}

bool InputValidator::ValidateRecurrenceInterval(int interval) {
    return interval > 0 && interval <= 365; // Reasonable upper limit
}

bool InputValidator::ValidateDaysOfWeek(const std::vector<Enums::DayOfWeek>& days) {
    if (days.empty()) {
        return false;
    }
    
    // Check for duplicates
    for (size_t i = 0; i < days.size(); ++i) {
        for (size_t j = i + 1; j < days.size(); ++j) {
            if (days[i] == days[j]) {
                return false;
            }
        }
    }
    
    return true;
}

bool InputValidator::ValidateId(int id) {
    return id > 0;
}

bool InputValidator::ValidateDateRange(const system_clock::time_point& start, 
                                     const system_clock::time_point& end) {
    return end > start;
}

bool InputValidator::ValidateTimeString(const std::string& timeStr) {
    static const std::regex timeRegex(R"(^([01]?[0-9]|2[0-3]):[0-5][0-9](:[0-5][0-9])?$)");
    return std::regex_match(timeStr, timeRegex);
}

std::string InputValidator::SanitizeString(const std::string& input) {
    // Remove control characters
    std::string result;
    result.reserve(input.length());
    
    for (char c : input) {
        if (c >= 32 && c != 127) { // Keep printable characters
            result += c;
        }
    }
    
    return StringUtils::Trim(result);
}

std::string InputValidator::SanitizeColor(const std::string& color) {
    std::string result = StringUtils::Trim(color);
    
    // Convert to uppercase
    result = StringUtils::ToUpper(result);
    
    // Remove # if present
    if (!result.empty() && result[0] == '#') {
        result = result.substr(1);
    }
    
    // Add # back
    if (!result.empty()) {
        result = "#" + result;
    }
    
    return result;
}

bool InputValidator::IsValidHexColor(const std::string& color) {
    if (color.empty() || color[0] != '#') {
        return false;
    }
    
    std::string hex = color.substr(1);
    
    // Check length (3, 6, or 8 characters)
    if (hex.length() != 3 && hex.length() != 6 && hex.length() != 8) {
        return false;
    }
    
    // Check if all characters are valid hex digits
    static const std::regex hexRegex(R"(^[0-9A-Fa-f]+$)");
    return std::regex_match(hex, hexRegex);
}

bool InputValidator::IsValidDateString(const std::string& dateStr) {
    // YYYY-MM-DD or YYYY-MM-DD HH:MM:SS format
    static const std::regex dateRegex1(R"(^\d{4}-\d{2}-\d{2}$)");
    static const std::regex dateRegex2(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$)");
    
    return std::regex_match(dateStr, dateRegex1) || 
           std::regex_match(dateStr, dateRegex2);
}