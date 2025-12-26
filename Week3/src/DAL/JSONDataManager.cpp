#include "JSONDataManager.h"
#include "../LIB/Logger.h"
#include "../LIB/DateUtils.h"
#include "../LIB/StringUtils.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>
// Add mutex for thread safety (not declared in header, but needed)
#include <mutex>
static std::mutex mutex_;

namespace fs = std::filesystem;

JSONDataManager::JSONDataManager(const std::string& dataFolder)
    : dataFolder_(dataFolder)
    , tasksFile_(dataFolder + Constants::TASKS_FILE)
    , categoriesFile_(dataFolder + Constants::CATEGORIES_FILE) {
    
    LOG_INFO("JSONDataManager initialized with data folder: " + dataFolder);
    
    // Ensure data folder exists
    if (!EnsureDataFolderExists()) {
        LOG_ERROR("Failed to create data folder: " + dataFolder);
        throw std::runtime_error("Failed to create data folder");
    }
}

// ITaskRepository implementation
bool JSONDataManager::SaveTasks(const std::vector<TaskPtr>& tasks) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    try {
        std::ostringstream json;
        json << "[\n";
        
        for (size_t i = 0; i < tasks.size(); ++i) {
            json << SerializeTask(tasks[i]);
            if (i < tasks.size() - 1) {
                json << ",\n";
            }
        }
        
        json << "\n]";
        
        if (!WriteFile(tasksFile_, json.str())) {
            LOG_ERROR("Failed to write tasks to file: " + tasksFile_);
            return false;
        }
        
        LOG_INFO("Saved " + std::to_string(tasks.size()) + " tasks to " + tasksFile_);
        return true;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error saving tasks: " + std::string(e.what()));
        return false;
    }
}

std::vector<TaskPtr> JSONDataManager::LoadTasks() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<TaskPtr> tasks;
    
    try {
        std::string content = ReadFile(tasksFile_);
        if (content.empty()) {
            LOG_INFO("No tasks file found or empty file: " + tasksFile_);
            return tasks;
        }
        
        // Simple JSON parsing - in a real application, use a proper JSON library
        // This implementation assumes each task is separated by "},{"
        std::string cleaned = StringUtils::Trim(content);
        
        if (cleaned.front() == '[' && cleaned.back() == ']') {
            cleaned = cleaned.substr(1, cleaned.length() - 2);
        }
        
        // Split into individual task JSON objects
        std::vector<std::string> taskJsons;
        std::string current;
        int braceCount = 0;
        
        for (char c : cleaned) {
            current += c;
            
            if (c == '{') braceCount++;
            else if (c == '}') {
                braceCount--;
                if (braceCount == 0) {
                    taskJsons.push_back(StringUtils::Trim(current));
                    current.clear();
                }
            } else if (c == ',' && braceCount == 0 && !current.empty()) {
                // Skip commas between objects
                current.clear();
            }
        }
        
        // Deserialize each task
        for (const auto& jsonStr : taskJsons) {
            try {
                TaskPtr task = DeserializeTask(jsonStr);
                if (task) {
                    tasks.push_back(task);
                }
            } catch (const std::exception& e) {
                LOG_WARNING("Failed to deserialize task: " + std::string(e.what()));
            }
        }
        
        LOG_INFO("Loaded " + std::to_string(tasks.size()) + " tasks from " + tasksFile_);
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error loading tasks: " + std::string(e.what()));
    }
    
    return tasks;
}

// ICategoryRepository implementation
bool JSONDataManager::SaveCategories(const std::vector<CategoryPtr>& categories) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    try {
        std::ostringstream json;
        json << "[\n";
        
        for (size_t i = 0; i < categories.size(); ++i) {
            json << SerializeCategory(categories[i]);
            if (i < categories.size() - 1) {
                json << ",\n";
            }
        }
        
        json << "\n]";
        
        if (!WriteFile(categoriesFile_, json.str())) {
            LOG_ERROR("Failed to write categories to file: " + categoriesFile_);
            return false;
        }
        
        LOG_INFO("Saved " + std::to_string(categories.size()) + " categories to " + categoriesFile_);
        return true;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error saving categories: " + std::string(e.what()));
        return false;
    }
}

std::vector<CategoryPtr> JSONDataManager::LoadCategories() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::vector<CategoryPtr> categories;
    
    try {
        std::string content = ReadFile(categoriesFile_);
        if (content.empty()) {
            LOG_INFO("No categories file found or empty file: " + categoriesFile_);
            return categories;
        }
        
        std::string cleaned = StringUtils::Trim(content);
        
        if (cleaned.front() == '[' && cleaned.back() == ']') {
            cleaned = cleaned.substr(1, cleaned.length() - 2);
        }
        
        // Split into individual category JSON objects
        std::vector<std::string> categoryJsons;
        std::string current;
        int braceCount = 0;
        
        for (char c : cleaned) {
            current += c;
            
            if (c == '{') braceCount++;
            else if (c == '}') {
                braceCount--;
                if (braceCount == 0) {
                    categoryJsons.push_back(StringUtils::Trim(current));
                    current.clear();
                }
            } else if (c == ',' && braceCount == 0 && !current.empty()) {
                current.clear();
            }
        }
        
        // Deserialize each category
        for (const auto& jsonStr : categoryJsons) {
            try {
                CategoryPtr category = DeserializeCategory(jsonStr);
                if (category) {
                    categories.push_back(category);
                }
            } catch (const std::exception& e) {
                LOG_WARNING("Failed to deserialize category: " + std::string(e.what()));
            }
        }
        
        LOG_INFO("Loaded " + std::to_string(categories.size()) + " categories from " + categoriesFile_);
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error loading categories: " + std::string(e.what()));
    }
    
    return categories;
}

// JSON serialization/deserialization
std::string JSONDataManager::SerializeTask(const TaskPtr& task) const {
    std::ostringstream json;
    
    json << "  {\n";
    json << "    \"id\": " << task->GetId() << ",\n";
    json << "    \"title\": \"" << EscapeJsonString(task->GetTitle()) << "\",\n";
    json << "    \"description\": \"" << EscapeJsonString(task->GetDescription()) << "\",\n";
    json << "    \"dueDate\": \"" << DateUtils::TimePointToString(task->GetDueDate()) << "\",\n";
    json << "    \"createdAt\": \"" << DateUtils::TimePointToString(task->GetCreatedAt()) << "\",\n";
    json << "    \"updatedAt\": \"" << DateUtils::TimePointToString(task->GetUpdatedAt()) << "\",\n";
    json << "    \"completedAt\": \"" << DateUtils::TimePointToString(task->GetCompletedAt()) << "\",\n";
    json << "    \"priority\": \"" << Enums::PriorityToString(task->GetPriority()) << "\",\n";
    json << "    \"status\": \"" << Enums::TaskStatusToString(task->GetStatus()) << "\",\n";
    
    if (task->GetCategory()) {
        json << "    \"categoryId\": " << task->GetCategory()->GetId() << ",\n";
    } else {
        json << "    \"categoryId\": null,\n";
    }
    
    if (task->GetRecurrencePattern()) {
        json << "    \"recurrence\": " << SerializeRecurrencePattern(task->GetRecurrencePattern()) << ",\n";
    } else {
        json << "    \"recurrence\": null,\n";
    }
    
    // Serialize tags
    json << "    \"tags\": [";
    const auto& tags = task->GetTags();
    for (size_t i = 0; i < tags.size(); ++i) {
        json << "\"" << EscapeJsonString(tags[i]) << "\"";
        if (i < tags.size() - 1) {
            json << ", ";
        }
    }
    json << "]\n";
    json << "  }";
    
    return json.str();
}

std::string JSONDataManager::SerializeCategory(const CategoryPtr& category) const {
    std::ostringstream json;
    
    json << "  {\n";
    json << "    \"id\": " << category->GetId() << ",\n";
    json << "    \"name\": \"" << EscapeJsonString(category->GetName()) << "\",\n";
    json << "    \"description\": \"" << EscapeJsonString(category->GetDescription()) << "\",\n";
    json << "    \"color\": \"" << EscapeJsonString(category->GetColor()) << "\",\n";
    json << "    \"createdAt\": \"" << DateUtils::TimePointToString(category->GetCreatedAt()) << "\",\n";
    json << "    \"updatedAt\": \"" << DateUtils::TimePointToString(category->GetUpdatedAt()) << "\"\n";
    json << "  }";
    
    return json.str();
}

std::string JSONDataManager::SerializeRecurrencePattern(const RecurrencePatternPtr& pattern) const {
    std::ostringstream json;
    
    json << "{\n";
    json << "      \"type\": \"" << Enums::RecurrenceTypeToString(pattern->GetType()) << "\",\n";
    json << "      \"interval\": " << pattern->GetInterval() << ",\n";
    
    // Serialize days of week
    json << "      \"daysOfWeek\": [";
    const auto& days = pattern->GetDaysOfWeek();
    for (size_t i = 0; i < days.size(); ++i) {
        json << "\"" << Enums::DayOfWeekToString(days[i]) << "\"";
        if (i < days.size() - 1) {
            json << ", ";
        }
    }
    json << "],\n";
    
    json << "      \"occurrenceCount\": " << pattern->GetOccurrenceCount() << ",\n";
    json << "      \"endDate\": \"" << DateUtils::TimePointToString(pattern->GetEndDate()) << "\"\n";
    json << "    }";
    
    return json.str();
}

TaskPtr JSONDataManager::DeserializeTask(const std::string& jsonStr) const {
    try {
        TaskPtr task = std::make_shared<Task>();
        
        task->SetId(ExtractIntValue(jsonStr, "id"));
        task->SetTitle(ExtractStringValue(jsonStr, "title"));
        task->SetDescription(ExtractStringValue(jsonStr, "description"));
        
        std::string dueDateStr = ExtractStringValue(jsonStr, "dueDate");
        task->SetDueDate(DateUtils::StringToTimePoint(dueDateStr));
        
        // Set timestamps
        std::string createdAtStr = ExtractStringValue(jsonStr, "createdAt");
        std::string updatedAtStr = ExtractStringValue(jsonStr, "updatedAt");
        std::string completedAtStr = ExtractStringValue(jsonStr, "completedAt");
        
        // Note: We don't set these through setters as they're internal timestamps
        // In a real implementation, these would be set through private methods
        
        // Set priority and status
        std::string priorityStr = ExtractStringValue(jsonStr, "priority");
        task->SetPriority(Enums::StringToPriority(priorityStr));
        
        std::string statusStr = ExtractStringValue(jsonStr, "status");
        task->SetStatus(Enums::StringToTaskStatus(statusStr));
        
        // Set category ID (category will be linked later)
        std::string categoryIdStr = ExtractStringValue(jsonStr, "categoryId");
        if (categoryIdStr != "null") {
            // Category will be linked by the service layer
        }
        
        // Set recurrence pattern
        std::string recurrenceStr = ExtractRecurrencePatternJson(jsonStr);
        if (recurrenceStr != "null") {
            RecurrencePatternPtr pattern = DeserializeRecurrencePattern(recurrenceStr);
            task->SetRecurrencePattern(pattern);
        }
        
        // Set tags
        std::vector<std::string> tags = ExtractStringArray(jsonStr, "tags");
        task->SetTags(tags);
        
        return task;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to deserialize task from JSON: " + std::string(e.what()));
        return nullptr;
    }
}

CategoryPtr JSONDataManager::DeserializeCategory(const std::string& jsonStr) const {
    try {
        CategoryPtr category = std::make_shared<Category>();
        
        category->SetId(ExtractIntValue(jsonStr, "id"));
        category->SetName(ExtractStringValue(jsonStr, "name"));
        category->SetDescription(ExtractStringValue(jsonStr, "description"));
        category->SetColor(ExtractStringValue(jsonStr, "color"));
        
        // Note: Timestamps are read-only in Category
        // In a real implementation, these would be set through private methods
        
        return category;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to deserialize category from JSON: " + std::string(e.what()));
        return nullptr;
    }
}

RecurrencePatternPtr JSONDataManager::DeserializeRecurrencePattern(const std::string& jsonStr) const {
    try {
        std::string typeStr = ExtractStringValue(jsonStr, "type");
        Enums::RecurrenceType type = Enums::StringToRecurrenceType(typeStr);
        
        int interval = ExtractIntValue(jsonStr, "interval");
        
        std::vector<Enums::DayOfWeek> daysOfWeek = ExtractDayOfWeekArray(jsonStr, "daysOfWeek");
        
        RecurrencePatternPtr pattern = std::make_shared<RecurrencePattern>(type, interval, daysOfWeek);
        
        pattern->SetOccurrenceCount(ExtractIntValue(jsonStr, "occurrenceCount"));
        
        std::string endDateStr = ExtractStringValue(jsonStr, "endDate");
        if (!endDateStr.empty() && endDateStr != "null") {
            pattern->SetEndDate(DateUtils::StringToTimePoint(endDateStr));
        }
        
        return pattern;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to deserialize recurrence pattern from JSON: " + std::string(e.what()));
        return nullptr;
    }
}

// File operations
bool JSONDataManager::EnsureDataFolderExists() const {
    try {
        if (!fs::exists(dataFolder_)) {
            return fs::create_directories(dataFolder_);
        }
        return true;
    } catch (const fs::filesystem_error& e) {
        LOG_ERROR("Filesystem error creating data folder: " + std::string(e.what()));
        return false;
    }
}

std::string JSONDataManager::ReadFile(const std::string& filename) const {
    try {
        if (!fs::exists(filename)) {
            return "";
        }
        
        std::ifstream file(filename);
        if (!file.is_open()) {
            LOG_ERROR("Failed to open file for reading: " + filename);
            return "";
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error reading file: " + std::string(e.what()));
        return "";
    }
}

bool JSONDataManager::WriteFile(const std::string& filename, const std::string& content) const {
    try {
        // Create backup of existing file
        if (fs::exists(filename)) {
            std::string backupFile = filename + ".bak";
            fs::copy_file(filename, backupFile, fs::copy_options::overwrite_existing);
        }
        
        // Write to temporary file first
        std::string tempFile = filename + ".tmp";
        {
            std::ofstream file(tempFile);
            if (!file.is_open()) {
                LOG_ERROR("Failed to open file for writing: " + tempFile);
                return false;
            }
            file << content;
        }
        
        // Replace original file with temp file
        fs::rename(tempFile, filename);
        
        return true;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error writing file: " + std::string(e.what()));
        return false;
    }
}

// JSON parsing utilities
std::string JSONDataManager::ExtractStringValue(const std::string& json, const std::string& key) const {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        return "";
    }
    
    pos += searchKey.length();
    
    // Skip whitespace
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }
    
    if (pos >= json.length()) {
        return "";
    }
    
    if (json[pos] == '\"') {
        // String value
        pos++;
        size_t endPos = json.find('\"', pos);
        if (endPos == std::string::npos) {
            return "";
        }
        
        std::string value = json.substr(pos, endPos - pos);
        return UnescapeJsonString(value);
        
    } else if (json.substr(pos, 4) == "null") {
        // Null value
        return "null";
    }
    
    return "";
}

int JSONDataManager::ExtractIntValue(const std::string& json, const std::string& key) const {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        return 0;
    }
    
    pos += searchKey.length();
    
    // Skip whitespace
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }
    
    if (pos >= json.length()) {
        return 0;
    }
    
    // Find end of number
    size_t endPos = pos;
    while (endPos < json.length() && 
           (std::isdigit(json[endPos]) || json[endPos] == '-')) {
        endPos++;
    }
    
    std::string numStr = json.substr(pos, endPos - pos);
    return std::stoi(numStr);
}

bool JSONDataManager::ExtractBoolValue(const std::string& json, const std::string& key) const {
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        return false;
    }
    
    pos += searchKey.length();
    
    // Skip whitespace
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }
    
    if (pos >= json.length()) {
        return false;
    }
    
    if (json.substr(pos, 4) == "true") {
        return true;
    }
    
    return false;
}

std::vector<std::string> JSONDataManager::ExtractStringArray(const std::string& json, const std::string& key) const {
    std::vector<std::string> result;
    
    std::string searchKey = "\"" + key + "\":";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        return result;
    }
    
    pos += searchKey.length();
    
    // Skip whitespace
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }
    
    if (pos >= json.length() || json[pos] != '[') {
        return result;
    }
    
    pos++;
    
    while (pos < json.length() && json[pos] != ']') {
        // Skip whitespace
        while (pos < json.length() && std::isspace(json[pos])) {
            pos++;
        }
        
        if (json[pos] == '\"') {
            pos++;
            size_t endPos = json.find('\"', pos);
            if (endPos == std::string::npos) {
                break;
            }
            
            std::string value = json.substr(pos, endPos - pos);
            result.push_back(UnescapeJsonString(value));
            pos = endPos + 1;
        }
        
        // Skip comma
        while (pos < json.length() && json[pos] != ',' && json[pos] != ']') {
            pos++;
        }
        
        if (json[pos] == ',') {
            pos++;
        }
    }
    
    return result;
}

std::vector<Enums::DayOfWeek> JSONDataManager::ExtractDayOfWeekArray(const std::string& json, const std::string& key) const {
    std::vector<Enums::DayOfWeek> result;
    
    std::vector<std::string> dayStrings = ExtractStringArray(json, key);
    
    for (const auto& dayStr : dayStrings) {
        try {
            Enums::DayOfWeek day = Enums::StringToDayOfWeek(dayStr);
            result.push_back(day);
        } catch (const std::exception& e) {
            LOG_WARNING("Invalid day of week string: " + dayStr);
        }
    }
    
    return result;
}

// Helper methods for JSON string escaping
std::string JSONDataManager::EscapeJsonString(const std::string& str) const {
    std::string result;
    result.reserve(str.length());
    
    for (char c : str) {
        switch (c) {
            case '\"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\b': result += "\\b"; break;
            case '\f': result += "\\f"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default:
                if (static_cast<unsigned char>(c) < 0x20) {
                    // Control character
                    char buffer[7];
                    snprintf(buffer, sizeof(buffer), "\\u%04x", c);
                    result += buffer;
                } else {
                    result += c;
                }
        }
    }
    
    return result;
}

std::string JSONDataManager::UnescapeJsonString(const std::string& str) const {
    std::string result;
    result.reserve(str.length());
    
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            switch (str[i + 1]) {
                case '\"': result += '\"'; i++; break;
                case '\\': result += '\\'; i++; break;
                case '/': result += '/'; i++; break;
                case 'b': result += '\b'; i++; break;
                case 'f': result += '\f'; i++; break;
                case 'n': result += '\n'; i++; break;
                case 'r': result += '\r'; i++; break;
                case 't': result += '\t'; i++; break;
                case 'u':
                    // Unicode escape sequence
                    if (i + 5 < str.length()) {
                        // In a real implementation, parse the Unicode escape
                        result += '?'; // Placeholder
                        i += 5;
                    } else {
                        result += str[i];
                    }
                    break;
                default:
                    result += str[i];
            }
        } else {
            result += str[i];
        }
    }
    
    return result;
}

// Helper to extract recurrence pattern JSON
std::string JSONDataManager::ExtractRecurrencePatternJson(const std::string& json) const {
    std::string searchKey = "\"recurrence\":";
    size_t pos = json.find(searchKey);
    
    if (pos == std::string::npos) {
        return "null";
    }
    
    pos += searchKey.length();
    
    // Skip whitespace
    while (pos < json.length() && std::isspace(json[pos])) {
        pos++;
    }
    
    if (pos >= json.length()) {
        return "null";
    }
    
    if (json.substr(pos, 4) == "null") {
        return "null";
    }
    
    if (json[pos] != '{') {
        return "null";
    }
    
    // Find matching brace
    int braceCount = 0;
    size_t startPos = pos;
    
    while (pos < json.length()) {
        if (json[pos] == '{') {
            braceCount++;
        } else if (json[pos] == '}') {
            braceCount--;
            if (braceCount == 0) {
                return json.substr(startPos, pos - startPos + 1);
            }
        }
        pos++;
    }
    
    return "null";
}

