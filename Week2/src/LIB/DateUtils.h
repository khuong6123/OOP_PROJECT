#ifndef DATE_UTILS_H
#define DATE_UTILS_H

#include <chrono>
#include <string>

class DateUtils {
public:
    static std::chrono::system_clock::time_point Now();
    static std::string TimePointToString(const std::chrono::system_clock::time_point& tp);
    static std::chrono::system_clock::time_point StringToTimePoint(const std::string& str);
    static bool IsWeekend(const std::chrono::system_clock::time_point& tp);
    static bool IsSameDay(const std::chrono::system_clock::time_point& lhs, 
                         const std::chrono::system_clock::time_point& rhs);
    static std::chrono::system_clock::time_point AddDays(
        const std::chrono::system_clock::time_point& tp, int days);
    static int DaysBetween(const std::chrono::system_clock::time_point& from, 
                          const std::chrono::system_clock::time_point& to);
};

#endif // DATE_UTILS_H