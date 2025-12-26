#ifndef CATEGORY_H
#define CATEGORY_H

#include "../LIB/common.h"
#include <string>
#include <chrono>

class Category {
public:
    Category();
    Category(const std::string& name, const std::string& description = "", 
             const std::string& color = "#000000");

    // Getters
    int GetId() const;
    const std::string& GetName() const;
    const std::string& GetDescription() const;
    const std::string& GetColor() const;
    const std::chrono::system_clock::time_point& GetCreatedAt() const;
    const std::chrono::system_clock::time_point& GetUpdatedAt() const;

    // Setters
    void SetId(int id);
    void SetName(const std::string& name);
    void SetDescription(const std::string& description);
    void SetColor(const std::string& color);
    void SetUpdatedAt(const std::chrono::system_clock::time_point& time);

    // Utility methods
    void UpdateTimestamp();

private:
    int id_;
    std::string name_;
    std::string description_;
    std::string color_; // Hex color code
    std::chrono::system_clock::time_point createdAt_;
    std::chrono::system_clock::time_point updatedAt_;
};

using CategoryPtr = Common::Ref<Category>;

#endif // CATEGORY_H