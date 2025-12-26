#pragma once
#include "../DAL/ITaskRepository.h"
#include "../DAL/ICategoryRepository.h"
#include "../LIB/Constants.h"
#include "../DTO/Task.h"
#include "../DTO/Category.h"
#include <filesystem>

class CSVDataManager : public ITaskRepository, public ICategoryRepository {
public:
    CSVDataManager(const std::string& dataFolder = Constants::DATA_FOLDER);
    
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
    
    // CSV serialization/deserialization
    std::string SerializeTask(const TaskPtr& task) const;
    std::string SerializeCategory(const CategoryPtr& category) const;
    
    TaskPtr DeserializeTask(const std::string& csvLine) const;
    CategoryPtr DeserializeCategory(const std::string& csvLine) const;
    
    // CSV parsing utilities
    std::vector<std::string> ParseCSVLine(const std::string& line) const;
    std::string EscapeCSVField(const std::string& field) const;
    
    // File operations
    bool EnsureDataFolderExists() const;
};