#ifndef _DATAMANAGERFACTORY_H_
#define _DATAMANAGERFACTORY_H_

#include "../DAL/ITaskRepository.h"
#include "../DAL/ICategoryRepository.h"
#include "../LIB/common.h"
#include <string>
#include <memory>

enum class DataFormat {
    JSON,
    CSV
};

class DataManagerFactory {
public:
    // Tạo repository cho Task
    static Common::Ref<ITaskRepository> CreateTaskRepository(DataFormat format, 
                                                            const std::string& dataFolder = "data/");
    
    // Tạo repository cho Category
    static Common::Ref<ICategoryRepository> CreateCategoryRepository(DataFormat format, 
                                                                    const std::string& dataFolder = "data/");
    
    // Tạo repository mặc định cho Task (JSON)
    static Common::Ref<ITaskRepository> CreateDefaultTaskRepository();
    
    // Tạo repository mặc định cho Category (JSON)
    static Common::Ref<ICategoryRepository> CreateDefaultCategoryRepository();
    
    // Chuyển đổi chuỗi thành DataFormat
    static DataFormat FormatFromString(const std::string& formatStr);
};

#endif // _DATAMANAGERFACTORY_H_`