#include "DateUtils.h"
#include <iomanip>
#include <sstream>
#include <chrono>
#include <ctime>

using namespace std::chrono;

system_clock::time_point DateUtils::Now() {
    return system_clock::now();
}

std::string DateUtils::TimePointToString(const system_clock::time_point& tp) {
    auto time = system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&time);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

system_clock::time_point DateUtils::StringToTimePoint(const std::string& str) {
    std::tm tm = {};
    std::istringstream iss(str);
    iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    if (iss.fail()) {
        throw std::runtime_error("Failed to parse date string: " + str);
    }
    
    auto time = std::mktime(&tm);
    return system_clock::from_time_t(time);
}

bool DateUtils::IsWeekend(const system_clock::time_point& tp) {
    auto time = system_clock::to_time_t(tp);
    std::tm tm = *std::localtime(&time);
    return tm.tm_wday == 0 || tm.tm_wday == 6; // Sunday = 0, Saturday = 6
}

bool DateUtils::IsSameDay(const system_clock::time_point& lhs, 
                         const system_clock::time_point& rhs) {
    auto lhs_time = system_clock::to_time_t(lhs);
    auto rhs_time = system_clock::to_time_t(rhs);
    
    std::tm lhs_tm = *std::localtime(&lhs_time);
    std::tm rhs_tm = *std::localtime(&rhs_time);
    
    return lhs_tm.tm_year == rhs_tm.tm_year &&
           lhs_tm.tm_mon == rhs_tm.tm_mon &&
           lhs_tm.tm_mday == rhs_tm.tm_mday;
}

system_clock::time_point DateUtils::AddDays(const system_clock::time_point& tp, int days) {
    return tp + hours(24 * days);
}

int DateUtils::DaysBetween(const system_clock::time_point& from, 
                          const system_clock::time_point& to) {
    auto duration = to - from;
    return static_cast<int>(duration_cast<hours>(duration).count() / 24);
}