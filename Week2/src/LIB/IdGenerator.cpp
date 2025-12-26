#include "IdGenerator.h"

IdGenerator& IdGenerator::GetInstance() {
    static IdGenerator instance;
    return instance;
}

int IdGenerator::GenerateTaskId() {
    return nextTaskId_++;
}

int IdGenerator::GenerateCategoryId() {
    return nextCategoryId_++;
}