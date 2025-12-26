#ifndef _JSONDATAMANAGER_H_
#define _JSONDATAMANAGER_H_

#include "../DAL/ITaskRepository.h"
#include "../DAL/ICategoryRepository.h"
#include "../LIB/Constants.h"
#include "../DTO/Task.h"
#include "../DTO/Category.h"
#include <filesystem>
#include <fstream>

class JSONDataManager : public ITaskRepository, public ICategoryRepository {
public:
    JSONDataManager(const std::string& dataFolder = Constants::DATA_FOLDER);
    
    // ITaskRepository
    bool SaveTasks(const std::vector<TaskPtr>& tasks) override;
    std::vector<TaskPtr> LoadTasks() override;
    
    // ICategoryRepository
    bool SaveCategories(const std::vector<CategoryPtr>& categories) override;
    std::vector<CategoryPtr> LoadCategories() override;

private:
    std::string dataFolder_;
    std::string tasksFile_;
    std::string categoriesFile_;
    
    // JSON serialization/deserialization
    std::string SerializeTask(const TaskPtr& task) const;
    std::string SerializeCategory(const CategoryPtr& category) const;
    std::string SerializeRecurrencePattern(const RecurrencePatternPtr& pattern) const;
    
    TaskPtr DeserializeTask(const std::string& jsonStr) const;
    CategoryPtr DeserializeCategory(const std::string& jsonStr) const;
    RecurrencePatternPtr DeserializeRecurrencePattern(const std::string& jsonStr) const;
    
    // File operations
    bool EnsureDataFolderExists() const;
    std::string ReadFile(const std::string& filename) const;
    bool WriteFile(const std::string& filename, const std::string& content) const;
    
    // JSON parsing utilities
    std::string ExtractStringValue(const std::string& json, const std::string& key) const;
    int ExtractIntValue(const std::string& json, const std::string& key) const;
    bool ExtractBoolValue(const std::string& json, const std::string& key) const;
    std::vector<std::string> ExtractStringArray(const std::string& json, const std::string& key) const;
    std::vector<Enums::DayOfWeek> ExtractDayOfWeekArray(const std::string& json, const std::string& key) const;

    // Helper methods for JSON string escaping
    std::string EscapeJsonString(const std::string& str) const;
    std::string UnescapeJsonString(const std::string& str) const;
    // Helper to extract recurrence pattern JSON
    std::string ExtractRecurrencePatternJson(const std::string& json) const;
};

#endif // _JSONDATAMANAGER_H_