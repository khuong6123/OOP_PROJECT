#include "Category.h"
#include "../LIB/DateUtils.h"
#include <stdexcept>

Category::Category() 
    : id_(0)
    , name_("")
    , description_("")
    , color_("#000000")
    , createdAt_(DateUtils::Now())
    , updatedAt_(createdAt_) {
}

Category::Category(const std::string& name, const std::string& description, 
                   const std::string& color)
    : id_(0)
    , name_(name)
    , description_(description)
    , color_(color)
    , createdAt_(DateUtils::Now())
    , updatedAt_(createdAt_) {
}

// Getters
int Category::GetId() const {
    return id_;
}

const std::string& Category::GetName() const {
    return name_;
}

const std::string& Category::GetDescription() const {
    return description_;
}

const std::string& Category::GetColor() const {
    return color_;
}

const std::chrono::system_clock::time_point& Category::GetCreatedAt() const {
    return createdAt_;
}

const std::chrono::system_clock::time_point& Category::GetUpdatedAt() const {
    return updatedAt_;
}

// Setters
void Category::SetId(int id) {
    if (id < 0) {
        throw std::invalid_argument("Category ID cannot be negative");
    }
    id_ = id;
}

void Category::SetName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Category name cannot be empty");
    }
    name_ = name;
}

void Category::SetDescription(const std::string& description) {
    description_ = description;
}

void Category::SetColor(const std::string& color) {
    if (color.empty()) {
        throw std::invalid_argument("Category color cannot be empty");
    }
    color_ = color;
}

void Category::SetUpdatedAt(const std::chrono::system_clock::time_point& time) {
    updatedAt_ = time;
}

// Utility methods
void Category::UpdateTimestamp() {
    updatedAt_ = DateUtils::Now();
}