#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <string>
#include <vector>

class StringUtils {
public:
    static std::string Trim(const std::string& str);
    static std::string ToLower(const std::string& str);
    static std::string ToUpper(const std::string& str);
    static bool Contains(const std::string& str, const std::string& substring);
    static bool StartsWith(const std::string& str, const std::string& prefix);
    static bool EndsWith(const std::string& str, const std::string& suffix);
    static std::vector<std::string> Split(const std::string& str, char delimiter);
    static std::string Join(const std::vector<std::string>& strings, const std::string& delimiter);
    static bool IsEmpty(const std::string& str);
    static std::string Replace(const std::string& str, const std::string& from, const std::string& to);
};

#endif // STRING_UTILS_H