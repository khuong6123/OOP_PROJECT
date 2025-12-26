#include "CSVDataManager.h"
#include "../LIB/Logger.h"
#include "../LIB/DateUtils.h"
#include "../LIB/StringUtils.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace fs = std::filesystem;

CSVDataManager::CSVDataManager(const std::string& dataFolder)
    : dataFolder_(dataFolder)
    , tasksFile_(dataFolder + Constants::TASKS_FILE)
    , categoriesFile_(dataFolder + Constants::CATEGORIES_FILE) {
    
    LOG_INFO("CSVDataManager initialized with data folder: " + dataFolder);
    
    // Ensure data folder exists
    if (!EnsureDataFolderExists()) {
        LOG_ERROR("Failed to create data folder: " + dataFolder);
        throw std::runtime_error("Failed to create data folder");
    }
    
    // Convert .json extensions to .csv
    size_t jsonPos = tasksFile_.find(".json");
    if (jsonPos != std::string::npos) {
        tasksFile_.replace(jsonPos, 5, ".csv");
    }
    
    jsonPos = categoriesFile_.find(".json");
    if (jsonPos != std::string::npos) {
        categoriesFile_.replace(jsonPos, 5, ".csv");
    }
}

// ITaskRepository implementation
bool CSVDataManager::SaveTasks(const std::vector<TaskPtr>& tasks) {
    try {
        // Create backup of existing file
        if (fs::exists(tasksFile_)) {
            std::string backupFile = tasksFile_ + ".bak";
            fs::copy_file(tasksFile_, backupFile, fs::copy_options::overwrite_existing);
        }
        
        // Write to temporary file first
        std::string tempFile = tasksFile_ + ".tmp";
        std::ofstream file(tempFile);
        if (!file.is_open()) {
            LOG_ERROR("Failed to open file for writing: " + tempFile);
            return false;
        }
        
        // Write CSV header
        file << "id,title,description,dueDate,createdAt,updatedAt,completedAt,priority,status,categoryId,recurrenceType,recurrenceInterval,daysOfWeek,occurrenceCount,endDate,tags\n";
        
        // Write each task
        for (const auto& task : tasks) {
            file << SerializeTask(task) << "\n";
        }
        
        file.close();
        
        // Replace original file with temp file
        fs::rename(tempFile, tasksFile_);
        
        LOG_INFO("Saved " + std::to_string(tasks.size()) + " tasks to " + tasksFile_);
        return true;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error saving tasks: " + std::string(e.what()));
        return false;
    }
}

std::vector<TaskPtr> CSVDataManager::LoadTasks() {
    std::vector<TaskPtr> tasks;
    
    try {
        if (!fs::exists(tasksFile_)) {
            LOG_INFO("No tasks file found: " + tasksFile_);
            return tasks;
        }
        
        std::ifstream file(tasksFile_);
        if (!file.is_open()) {
            LOG_ERROR("Failed to open file for reading: " + tasksFile_);
            return tasks;
        }
        
        std::string line;
        bool isFirstLine = true;
        
        while (std::getline(file, line)) {
            if (isFirstLine) {
                // Skip header
                isFirstLine = false;
                continue;
            }
            
            if (line.empty()) {
                continue;
            }
            
            try {
                TaskPtr task = DeserializeTask(line);
                if (task) {
                    tasks.push_back(task);
                }
            } catch (const std::exception& e) {
                LOG_WARNING("Failed to deserialize task from CSV: " + std::string(e.what()));
            }
        }
        
        LOG_INFO("Loaded " + std::to_string(tasks.size()) + " tasks from " + tasksFile_);
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error loading tasks: " + std::string(e.what()));
    }
    
    return tasks;
}

// ICategoryRepository implementation
bool CSVDataManager::SaveCategories(const std::vector<CategoryPtr>& categories) {
    try {
        // Create backup of existing file
        if (fs::exists(categoriesFile_)) {
            std::string backupFile = categoriesFile_ + ".bak";
            fs::copy_file(categoriesFile_, backupFile, fs::copy_options::overwrite_existing);
        }
        
        // Write to temporary file first
        std::string tempFile = categoriesFile_ + ".tmp";
        std::ofstream file(tempFile);
        if (!file.is_open()) {
            LOG_ERROR("Failed to open file for writing: " + tempFile);
            return false;
        }
        
        // Write CSV header
        file << "id,name,description,color,createdAt,updatedAt\n";
        
        // Write each category
        for (const auto& category : categories) {
            file << SerializeCategory(category) << "\n";
        }
        
        file.close();
        
        // Replace original file with temp file
        fs::rename(tempFile, categoriesFile_);
        
        LOG_INFO("Saved " + std::to_string(categories.size()) + " categories to " + categoriesFile_);
        return true;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error saving categories: " + std::string(e.what()));
        return false;
    }
}

std::vector<CategoryPtr> CSVDataManager::LoadCategories() {
    std::vector<CategoryPtr> categories;
    
    try {
        if (!fs::exists(categoriesFile_)) {
            LOG_INFO("No categories file found: " + categoriesFile_);
            return categories;
        }
        
        std::ifstream file(categoriesFile_);
        if (!file.is_open()) {
            LOG_ERROR("Failed to open file for reading: " + categoriesFile_);
            return categories;
        }
        
        std::string line;
        bool isFirstLine = true;
        
        while (std::getline(file, line)) {
            if (isFirstLine) {
                // Skip header
                isFirstLine = false;
                continue;
            }
            
            if (line.empty()) {
                continue;
            }
            
            try {
                CategoryPtr category = DeserializeCategory(line);
                if (category) {
                    categories.push_back(category);
                }
            } catch (const std::exception& e) {
                LOG_WARNING("Failed to deserialize category from CSV: " + std::string(e.what()));
            }
        }
        
        LOG_INFO("Loaded " + std::to_string(categories.size()) + " categories from " + categoriesFile_);
        
    } catch (const std::exception& e) {
        LOG_ERROR("Error loading categories: " + std::string(e.what()));
    }
    
    return categories;
}

// CSV serialization/deserialization
std::string CSVDataManager::SerializeTask(const TaskPtr& task) const {
    std::ostringstream csv;
    
    // Basic fields
    csv << task->GetId() << ",";
    csv << EscapeCSVField(task->GetTitle()) << ",";
    csv << EscapeCSVField(task->GetDescription()) << ",";
    csv << EscapeCSVField(DateUtils::TimePointToString(task->GetDueDate())) << ",";
    csv << EscapeCSVField(DateUtils::TimePointToString(task->GetCreatedAt())) << ",";
    csv << EscapeCSVField(DateUtils::TimePointToString(task->GetUpdatedAt())) << ",";
    csv << EscapeCSVField(DateUtils::TimePointToString(task->GetCompletedAt())) << ",";
    csv << EscapeCSVField(Enums::PriorityToString(task->GetPriority())) << ",";
    csv << EscapeCSVField(Enums::TaskStatusToString(task->GetStatus())) << ",";
    
    // Category
    if (task->GetCategory()) {
        csv << task->GetCategory()->GetId() << ",";
    } else {
        csv << "0,";
    }
    
    // Recurrence pattern
    if (task->GetRecurrencePattern()) {
        const auto& pattern = task->GetRecurrencePattern();
        csv << EscapeCSVField(Enums::RecurrenceTypeToString(pattern->GetType())) << ",";
        csv << pattern->GetInterval() << ",";
        
        // Days of week
        std::ostringstream daysStream;
        const auto& days = pattern->GetDaysOfWeek();
        for (size_t i = 0; i < days.size(); ++i) {
            daysStream << Enums::DayOfWeekToString(days[i]);
            if (i < days.size() - 1) {
                daysStream << ";";
            }
        }
        csv << EscapeCSVField(daysStream.str()) << ",";
        csv << pattern->GetOccurrenceCount() << ",";
        csv << EscapeCSVField(DateUtils::TimePointToString(pattern->GetEndDate())) << ",";
    } else {
        csv << "NONE,0,,0,,"; // Empty fields for non-recurring tasks
    }
    
    // Tags
    std::ostringstream tagsStream;
    const auto& tags = task->GetTags();
    for (size_t i = 0; i < tags.size(); ++i) {
        tagsStream << tags[i];
        if (i < tags.size() - 1) {
            tagsStream << ";";
        }
    }
    csv << EscapeCSVField(tagsStream.str());
    
    return csv.str();
}

std::string CSVDataManager::SerializeCategory(const CategoryPtr& category) const {
    std::ostringstream csv;
    
    csv << category->GetId() << ",";
    csv << EscapeCSVField(category->GetName()) << ",";
    csv << EscapeCSVField(category->GetDescription()) << ",";
    csv << EscapeCSVField(category->GetColor()) << ",";
    csv << EscapeCSVField(DateUtils::TimePointToString(category->GetCreatedAt())) << ",";
    csv << EscapeCSVField(DateUtils::TimePointToString(category->GetUpdatedAt()));
    
    return csv.str();
}

TaskPtr CSVDataManager::DeserializeTask(const std::string& csvLine) const {
    try {
        std::vector<std::string> fields = ParseCSVLine(csvLine);
        
        if (fields.size() < 16) {
            LOG_WARNING("Invalid CSV line for task: insufficient fields");
            return nullptr;
        }
        
        TaskPtr task = std::make_shared<Task>();
        
        // Basic fields
        task->SetId(std::stoi(fields[0]));
        task->SetTitle(fields[1]);
        task->SetDescription(fields[2]);
        
        if (!fields[3].empty()) {
            task->SetDueDate(DateUtils::StringToTimePoint(fields[3]));
        }
        
        // Note: Timestamps are read-only
        // Priority and status
        if (!fields[7].empty()) {
            task->SetPriority(Enums::StringToPriority(fields[7]));
        }
        
        if (!fields[8].empty()) {
            task->SetStatus(Enums::StringToTaskStatus(fields[8]));
        }
        
        // Category ID (will be linked later)
        // Fields[9] contains category ID
        
        // Recurrence pattern
        if (!fields[10].empty() && fields[10] != "NONE") {
            try {
                Enums::RecurrenceType type = Enums::StringToRecurrenceType(fields[10]);
                int interval = std::stoi(fields[11]);
                
                std::vector<Enums::DayOfWeek> daysOfWeek;
                if (!fields[12].empty()) {
                    std::vector<std::string> dayStrings = StringUtils::Split(fields[12], ';');
                    for (const auto& dayStr : dayStrings) {
                        if (!dayStr.empty()) {
                            daysOfWeek.push_back(Enums::StringToDayOfWeek(dayStr));
                        }
                    }
                }
                
                RecurrencePatternPtr pattern = std::make_shared<RecurrencePattern>(type, interval, daysOfWeek);
                
                if (!fields[13].empty()) {
                    pattern->SetOccurrenceCount(std::stoi(fields[13]));
                }
                
                if (!fields[14].empty()) {
                    pattern->SetEndDate(DateUtils::StringToTimePoint(fields[14]));
                }
                
                task->SetRecurrencePattern(pattern);
            } catch (const std::exception& e) {
                LOG_WARNING("Failed to parse recurrence pattern: " + std::string(e.what()));
            }
        }
        
        // Tags
        if (!fields[15].empty()) {
            std::vector<std::string> tags = StringUtils::Split(fields[15], ';');
            task->SetTags(tags);
        }
        
        return task;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to deserialize task from CSV: " + std::string(e.what()));
        return nullptr;
    }
}

CategoryPtr CSVDataManager::DeserializeCategory(const std::string& csvLine) const {
    try {
        std::vector<std::string> fields = ParseCSVLine(csvLine);
        
        if (fields.size() < 6) {
            LOG_WARNING("Invalid CSV line for category: insufficient fields");
            return nullptr;
        }
        
        CategoryPtr category = std::make_shared<Category>();
        
        category->SetId(std::stoi(fields[0]));
        category->SetName(fields[1]);
        category->SetDescription(fields[2]);
        category->SetColor(fields[3]);
        
        // Note: Timestamps are read-only
        
        return category;
        
    } catch (const std::exception& e) {
        LOG_ERROR("Failed to deserialize category from CSV: " + std::string(e.what()));
        return nullptr;
    }
}

// CSV parsing utilities
std::vector<std::string> CSVDataManager::ParseCSVLine(const std::string& line) const {
    std::vector<std::string> fields;
    std::string currentField;
    bool inQuotes = false;
    
    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];
        
        if (c == '"') {
            if (inQuotes && i + 1 < line.length() && line[i + 1] == '"') {
                // Escaped quote
                currentField += '"';
                i++;
            } else {
                // Start or end of quoted field
                inQuotes = !inQuotes;
            }
        } else if (c == ',' && !inQuotes) {
            // End of field
            fields.push_back(currentField);
            currentField.clear();
        } else {
            currentField += c;
        }
    }
    
    // Add last field
    fields.push_back(currentField);
    
    return fields;
}

std::string CSVDataManager::EscapeCSVField(const std::string& field) const {
    // Check if field needs quoting
    bool needsQuotes = false;
    
    if (field.empty()) {
        return "\"\"";
    }
    
    for (char c : field) {
        if (c == '"' || c == ',' || c == '\n' || c == '\r') {
            needsQuotes = true;
            break;
        }
    }
    
    if (!needsQuotes) {
        return field;
    }
    
    // Escape quotes and wrap in quotes
    std::string result = "\"";
    
    for (char c : field) {
        if (c == '"') {
            result += "\"\"";
        } else {
            result += c;
        }
    }
    
    result += "\"";
    return result;
}

// File operations
bool CSVDataManager::EnsureDataFolderExists() const {
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