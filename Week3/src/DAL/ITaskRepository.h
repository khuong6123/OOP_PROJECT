#ifndef _ITASKREPOSITORY_H_
#define _ITASKREPOSITORY_H_

#include "../DTO/Task.h"
#include <vector>

class ITaskRepository {
public:
    virtual ~ITaskRepository() = default;
    virtual bool SaveTasks(const std::vector<TaskPtr>& tasks) = 0;
    virtual std::vector<TaskPtr> LoadTasks() = 0;
};

#endif // _ITASKREPOSITORY_H_