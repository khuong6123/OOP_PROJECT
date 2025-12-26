#include "Task.h"
#include "../LIB/DateUtils.h"
#include <algorithm>
#include <stdexcept>

Task::Task()
    : id_(0)
    , title_("")
    , description_("")
    , dueDate_(DateUtils::Now())
    , createdAt_(DateUtils::Now())
    , updatedAt_(createdAt_)
    , completedAt_(std::chrono::system_clock::time_point::min())
    , priority_(Enums::Priority::MEDIUM)
    , status_(Enums::TaskStatus::PENDING)
    , category_(nullptr)
    , recurrencePattern_(nullptr) {
}

Task::Task(const std::string& title, const std::string& description,
           const std::chrono::system_clock::time_point& dueDate,
           Enums::Priority priority, CategoryPtr category)
    : id_(0)
    , title_(title)
    , description_(description)
    , dueDate_(dueDate)
    , createdAt_(DateUtils::Now())
    , updatedAt_(createdAt_)
    , completedAt_(std::chrono::system_clock::time_point::min())
    , priority_(priority)
    , status_(Enums::TaskStatus::PENDING)
    , category_(category)
    , recurrencePattern_(nullptr) {
}

// Getters
int Task::GetId() const {
    return id_;
}

const std::string& Task::GetTitle() const {
    return title_;
}

const std::string& Task::GetDescription() const {
    return description_;
}

const std::chrono::system_clock::time_point& Task::GetDueDate() const {
    return dueDate_;
}

const std::chrono::system_clock::time_point& Task::GetCreatedAt() const {
    return createdAt_;
}

const std::chrono::system_clock::time_point& Task::GetUpdatedAt() const {
    return updatedAt_;
}

const std::chrono::system_clock::time_point& Task::GetCompletedAt() const {
    return completedAt_;
}

Enums::Priority Task::GetPriority() const {
    return priority_;
}

Enums::TaskStatus Task::GetStatus() const {
    return status_;
}

CategoryPtr Task::GetCategory() const {
    return category_;
}

RecurrencePatternPtr Task::GetRecurrencePattern() const {
    return recurrencePattern_;
}

const std::vector<std::string>& Task::GetTags() const {
    return tags_;
}

// Setters
void Task::SetId(int id) {
    if (id < 0) {
        throw std::invalid_argument("Task ID cannot be negative");
    }
    id_ = id;
}

void Task::SetTitle(const std::string& title) {
    if (title.empty()) {
        throw std::invalid_argument("Task title cannot be empty");
    }
    title_ = title;
}

void Task::SetDescription(const std::string& description) {
    description_ = description;
}

void Task::SetDueDate(const std::chrono::system_clock::time_point& dueDate) {
    if (dueDate < createdAt_) {
        throw std::invalid_argument("Due date cannot be before creation date");
    }
    dueDate_ = dueDate;
}

void Task::SetPriority(Enums::Priority priority) {
    priority_ = priority;
}

void Task::SetStatus(Enums::TaskStatus status) {
    if (status == Enums::TaskStatus::COMPLETED && status_ != Enums::TaskStatus::COMPLETED) {
        completedAt_ = DateUtils::Now();
    }
    status_ = status;
}

void Task::SetCategory(CategoryPtr category) {
    category_ = category;
}

void Task::SetRecurrencePattern(RecurrencePatternPtr pattern) {
    recurrencePattern_ = pattern;
}

void Task::SetTags(const std::vector<std::string>& tags) {
    tags_ = tags;
}

void Task::SetUpdatedAt(const std::chrono::system_clock::time_point& time) {
    updatedAt_ = time;
}

// Utility methods
void Task::UpdateTimestamp() {
    updatedAt_ = DateUtils::Now();
}

bool Task::IsRecurring() const {
    return recurrencePattern_ && recurrencePattern_->IsRecurring();
}

bool Task::HasCategory() const {
    return category_ != nullptr;
}

void Task::AddTag(const std::string& tag) {
    if (tag.empty()) {
        return;
    }
    
    // Check if tag already exists
    auto it = std::find(tags_.begin(), tags_.end(), tag);
    if (it == tags_.end()) {
        tags_.push_back(tag);
    }
}

void Task::RemoveTag(const std::string& tag) {
    auto it = std::find(tags_.begin(), tags_.end(), tag);
    if (it != tags_.end()) {
        tags_.erase(it);
    }
}