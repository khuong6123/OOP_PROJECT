#ifndef _ICATEGORYREPOSITORY_H_
#define _ICATEGORYREPOSITORY_H_

#include "../DTO/Category.h"
#include <vector>

class ICategoryRepository {
public:
    virtual ~ICategoryRepository() = default;
    virtual bool SaveCategories(const std::vector<CategoryPtr>& categories) = 0;
    virtual std::vector<CategoryPtr> LoadCategories() = 0;
};

#endif // _ICATEGORYREPOSITORY_H_