#ifndef TASK_H
#define TASK_H

#include "Category.h"
#include "RecurrencePattern.h"
#include "Enums.h"
#include "../LIB/common.h"
#include <string>
#include <chrono>
#include <vector>

class Task;
using TaskPtr = Common::Ref<Task>;

class Task {
public:
    Task();
    Task(const std::string& title, const std::string& description,
         const std::chrono::system_clock::time_point& dueDate,
         Enums::Priority priority = Enums::Priority::MEDIUM,
         CategoryPtr category = nullptr);

    // Getters
    int GetId() const;
    const std::string& GetTitle() const;
    const std::string& GetDescription() const;
    const std::chrono::system_clock::time_point& GetDueDate() const;
    const std::chrono::system_clock::time_point& GetCreatedAt() const;
    const std::chrono::system_clock::time_point& GetUpdatedAt() const;
    const std::chrono::system_clock::time_point& GetCompletedAt() const;
    Enums::Priority GetPriority() const;
    Enums::TaskStatus GetStatus() const;
    CategoryPtr GetCategory() const;
    RecurrencePatternPtr GetRecurrencePattern() const;
    const std::vector<std::string>& GetTags() const;

    // Setters
    void SetId(int id);
    void SetTitle(const std::string& title);
    void SetDescription(const std::string& description);
    void SetDueDate(const std::chrono::system_clock::time_point& dueDate);
    void SetPriority(Enums::Priority priority);
    void SetStatus(Enums::TaskStatus status);
    void SetCategory(CategoryPtr category);
    void SetRecurrencePattern(RecurrencePatternPtr pattern);
    void SetTags(const std::vector<std::string>& tags);
    void SetUpdatedAt(const std::chrono::system_clock::time_point& time);

    // Utility methods
    void UpdateTimestamp();
    bool IsRecurring() const;
    bool HasCategory() const;
    void AddTag(const std::string& tag);
    void RemoveTag(const std::string& tag);

private:
    int id_;
    std::string title_;
    std::string description_;
    std::chrono::system_clock::time_point dueDate_;
    std::chrono::system_clock::time_point createdAt_;
    std::chrono::system_clock::time_point updatedAt_;
    std::chrono::system_clock::time_point completedAt_;
    Enums::Priority priority_;
    Enums::TaskStatus status_;
    CategoryPtr category_;
    RecurrencePatternPtr recurrencePattern_;
    std::vector<std::string> tags_;
};

#endif // TASK_H