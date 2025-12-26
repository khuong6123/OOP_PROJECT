#include "StringUtils.h"
#include <algorithm>
#include <cctype>
#include <sstream>

std::string StringUtils::Trim(const std::string& str) {
    auto start = str.begin();
    auto end = str.end();
    
    // Trim leading spaces
    while (start != end && std::isspace(static_cast<unsigned char>(*start))) {
        ++start;
    }
    
    // Trim trailing spaces
    while (start != end && std::isspace(static_cast<unsigned char>(*(end - 1)))) {
        --end;
    }
    
    return std::string(start, end);
}

std::string StringUtils::ToLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

std::string StringUtils::ToUpper(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

bool StringUtils::Contains(const std::string& str, const std::string& substring) {
    return str.find(substring) != std::string::npos;
}

bool StringUtils::StartsWith(const std::string& str, const std::string& prefix) {
    if (prefix.length() > str.length()) {
        return false;
    }
    return std::equal(prefix.begin(), prefix.end(), str.begin());
}

bool StringUtils::EndsWith(const std::string& str, const std::string& suffix) {
    if (suffix.length() > str.length()) {
        return false;
    }
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

std::vector<std::string> StringUtils::Split(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::istringstream stream(str);
    std::string token;
    
    while (std::getline(stream, token, delimiter)) {
        result.push_back(token);
    }
    
    return result;
}

std::string StringUtils::Join(const std::vector<std::string>& strings, const std::string& delimiter) {
    std::ostringstream result;
    
    for (size_t i = 0; i < strings.size(); ++i) {
        if (i > 0) {
            result << delimiter;
        }
        result << strings[i];
    }
    
    return result.str();
}

bool StringUtils::IsEmpty(const std::string& str) {
    return str.empty() || Trim(str).empty();
}

std::string StringUtils::Replace(const std::string& str, const std::string& from, const std::string& to) {
    std::string result = str;
    size_t start_pos = 0;
    
    while ((start_pos = result.find(from, start_pos)) != std::string::npos) {
        result.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    
    return result;
}