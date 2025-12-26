#include "DataManagerFactory.h"
#include "../DAL/JSONDataManager.h"
#include "../DAL/CSVDataManager.h"
#include <algorithm>
#include <stdexcept>

Common::Ref<ITaskRepository> DataManagerFactory::CreateTaskRepository(DataFormat format, 
                                                                      const std::string& dataFolder) {
    switch (format) {
        case DataFormat::JSON:
            return std::make_shared<JSONDataManager>(dataFolder);
        case DataFormat::CSV:
            return std::make_shared<CSVDataManager>(dataFolder);
        default:
            throw std::invalid_argument("Unsupported data format");
    }
}

Common::Ref<ICategoryRepository> DataManagerFactory::CreateCategoryRepository(DataFormat format, 
                                                                              const std::string& dataFolder) {
    switch (format) {
        case DataFormat::JSON:
            return std::make_shared<JSONDataManager>(dataFolder);
        case DataFormat::CSV:
            return std::make_shared<CSVDataManager>(dataFolder);
        default:
            throw std::invalid_argument("Unsupported data format");
    }
}

Common::Ref<ITaskRepository> DataManagerFactory::CreateDefaultTaskRepository() {
    return CreateTaskRepository(DataFormat::JSON);
}

Common::Ref<ICategoryRepository> DataManagerFactory::CreateDefaultCategoryRepository() {
    return CreateCategoryRepository(DataFormat::JSON);
}

DataFormat DataManagerFactory::FormatFromString(const std::string& formatStr) {
    std::string upper = formatStr;
    std::transform(upper.begin(), upper.end(), upper.begin(), ::toupper);
    
    if (upper == "JSON") return DataFormat::JSON;
    if (upper == "CSV") return DataFormat::CSV;
    
    throw std::invalid_argument("Unknown data format: " + formatStr);
}