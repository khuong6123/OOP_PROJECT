#ifndef CONSTANT_H
#define CONSTANT_H

#include <string>

namespace Constants {
    constexpr int MAX_TASK_TITLE_LENGTH = 100;
    constexpr int MAX_CATEGORY_NAME_LENGTH = 50;
    constexpr int MAX_DESCRIPTION_LENGTH = 500;
    
    const std::string DATA_FOLDER = "data/";
    const std::string TASKS_FILE = "tasks.json";
    const std::string CATEGORIES_FILE = "categories.json";
    
    constexpr int HOURS_IN_DAY = 24;
    constexpr int MINUTES_IN_HOUR = 60;
    constexpr int SECONDS_IN_MINUTE = 60;
}

#endif // CONSTANT_H