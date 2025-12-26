#ifndef PRODUCTIVITY_STATS_H
#define PRODUCTIVITY_STATS_H

#include "Enums.h"
#include <map>

struct ProductivityStats {
    int totalTasks = 0;
    int completedTasks = 0;
    int overdueTasks = 0;
    int pendingTasks = 0;
    double completionRate = 0.0;
    double averageCompletionTimeHours = 0.0;
    std::map<Enums::Priority, int> tasksByPriority;
    std::map<Enums::TaskStatus, int> tasksByStatus;
};

#endif // PRODUCTIVITY_STATS_H