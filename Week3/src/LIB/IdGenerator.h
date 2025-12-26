#ifndef ID_GENERATOR_H
#define ID_GENERATOR_H

#include <atomic>

class IdGenerator {
public:
    static IdGenerator& GetInstance();
    int GenerateTaskId();
    int GenerateCategoryId();
private:
    IdGenerator() = default;
    std::atomic<int> nextTaskId_{1};
    std::atomic<int> nextCategoryId_{1};
};

#endif // ID_GENERATOR_H