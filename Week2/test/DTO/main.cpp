#include <gtest/gtest.h>
#include "../../src/LIB/StringUtils.h"
#include "../../src/LIB/DateUtils.h"
#include "../../src/LIB/InputValidator.h"
#include "../../src/DTO/Task.h"
#include "../../src/DTO/Category.h"
#include "../../src/DTO/RecurrencePattern.h"
#include "../../src/DTO/Enums.h"
#include "../../src/LIB/IdGenerator.h"
#include "../../src/LIB/Logger.h"
#include "../../src/DTO/ProductivityReport.h"
#include "../../src/LIB/common.h"
#include "../../src/LIB/Constants.h"  
// Test fixture for common setup
class TaskManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize logger if needed
        Logger::GetInstance().SetMinLevel(LogLevel::DEBUG);
    }
    void TearDown() override {
        // Cleanup if needed
    }
};

// Test StringUtils
TEST_F(TaskManagerTest, StringUtils_Trim) {
    EXPECT_EQ(StringUtils::Trim("  hello  "), "hello");
    EXPECT_EQ(StringUtils::Trim("\t\nworld\r "), "world");
    EXPECT_EQ(StringUtils::Trim(""), "");
    EXPECT_EQ(StringUtils::Trim("   "), "");
}

TEST_F(TaskManagerTest, StringUtils_ToLower) {
    EXPECT_EQ(StringUtils::ToLower("HELLO"), "hello");
    EXPECT_EQ(StringUtils::ToLower("World123!"), "world123!");
}

TEST_F(TaskManagerTest, StringUtils_Contains) {
    EXPECT_TRUE(StringUtils::Contains("hello world", "world"));
    EXPECT_FALSE(StringUtils::Contains("hello", "world"));
}

TEST_F(TaskManagerTest, StringUtils_Split) {
    auto parts = StringUtils::Split("a,b,c", ',');
    ASSERT_EQ(parts.size(), 3u);
    EXPECT_EQ(parts[0], "a");
    EXPECT_EQ(parts[1], "b");
    EXPECT_EQ(parts[2], "c");
}

// Test DateUtils
TEST_F(TaskManagerTest, DateUtils_AddDays) {
    auto now = DateUtils::Now();
    auto future = DateUtils::AddDays(now, 5);
    EXPECT_EQ(DateUtils::DaysBetween(now, future), 5);
}

TEST_F(TaskManagerTest, DateUtils_IsSameDay) {
    auto tp1 = DateUtils::StringToTimePoint("2025-12-26 10:00:00");
    auto tp2 = DateUtils::StringToTimePoint("2025-12-26 20:00:00");
    EXPECT_TRUE(DateUtils::IsSameDay(tp1, tp2));

    auto tp3 = DateUtils::StringToTimePoint("2025-12-27 10:00:00");
    EXPECT_FALSE(DateUtils::IsSameDay(tp1, tp3));
}

// Test InputValidator
TEST_F(TaskManagerTest, InputValidator_TaskTitle) {
    EXPECT_TRUE(InputValidator::ValidateTaskTitle("Valid Title"));
    EXPECT_FALSE(InputValidator::ValidateTaskTitle(""));
    EXPECT_FALSE(InputValidator::ValidateTaskTitle(std::string(Constants::MAX_TASK_TITLE_LENGTH + 1, 'a')));  // Exceeds max length
}

TEST_F(TaskManagerTest, InputValidator_Color) {
    EXPECT_TRUE(InputValidator::ValidateColor("#FF0000"));
    EXPECT_TRUE(InputValidator::ValidateColor(" #ff0000 "));
    EXPECT_FALSE(InputValidator::ValidateColor("invalid"));
    EXPECT_FALSE(InputValidator::ValidateColor(""));
}

// Test Enums
TEST_F(TaskManagerTest, Enums_PriorityConversions) {
    EXPECT_EQ(Enums::PriorityToString(Enums::Priority::HIGH), "HIGH");
    EXPECT_EQ(Enums::StringToPriority("urgent"), Enums::Priority::URGENT);
    
    EXPECT_THROW(Enums::StringToPriority("invalid"), std::invalid_argument);
}

TEST_F(TaskManagerTest, Enums_TaskStatusConversions) {
    EXPECT_EQ(Enums::TaskStatusToString(Enums::TaskStatus::COMPLETED), "COMPLETED");
    EXPECT_EQ(Enums::StringToTaskStatus("in_progress"), Enums::TaskStatus::IN_PROGRESS);
    
    EXPECT_THROW(Enums::StringToTaskStatus("invalid"), std::invalid_argument);
}

TEST_F(TaskManagerTest, Enums_RecurrenceTypeConversions) {
    EXPECT_EQ(Enums::RecurrenceTypeToString(Enums::RecurrenceType::WEEKLY), "WEEKLY");
    EXPECT_EQ(Enums::StringToRecurrenceType("monthly"), Enums::RecurrenceType::MONTHLY);
    
    EXPECT_THROW(Enums::StringToRecurrenceType("invalid"), std::invalid_argument);
}

TEST_F(TaskManagerTest, Enums_DayOfWeekConversions) {
    EXPECT_EQ(Enums::DayOfWeekToString(Enums::DayOfWeek::FRIDAY), "FRIDAY");
    EXPECT_EQ(Enums::StringToDayOfWeek("saturday"), Enums::DayOfWeek::SATURDAY);
    
    EXPECT_THROW(Enums::StringToDayOfWeek("invalid"), std::invalid_argument);
}

// Test Category
TEST_F(TaskManagerTest, Category_ConstructionAndGetters) {
    Category cat("Work", "Work tasks", "#0000FF");
    EXPECT_EQ(cat.GetName(), "Work");
    EXPECT_EQ(cat.GetDescription(), "Work tasks");
    EXPECT_EQ(cat.GetColor(), "#0000FF");
}

TEST_F(TaskManagerTest, Category_Setters) {
    Category cat;
    cat.SetName("Personal");
    EXPECT_EQ(cat.GetName(), "Personal");

    cat.SetColor("#FF0000");
    EXPECT_EQ(cat.GetColor(), "#FF0000");

    EXPECT_THROW(cat.SetName(""), std::invalid_argument);
    EXPECT_THROW(cat.SetColor(""), std::invalid_argument);
    EXPECT_THROW(cat.SetId(-1), std::invalid_argument);
}

// Test RecurrencePattern
TEST_F(TaskManagerTest, RecurrencePattern_Construction) {
    std::vector<Enums::DayOfWeek> days = {Enums::DayOfWeek::MONDAY, Enums::DayOfWeek::FRIDAY};
    RecurrencePattern rec(Enums::RecurrenceType::WEEKLY, 1, days);
    EXPECT_EQ(rec.GetType(), Enums::RecurrenceType::WEEKLY);
    EXPECT_EQ(rec.GetInterval(), 1);
    EXPECT_EQ(rec.GetDaysOfWeek(), days);
    EXPECT_TRUE(rec.IsRecurring());
}

TEST_F(TaskManagerTest, RecurrencePattern_InvalidConstruction) {
    EXPECT_THROW(RecurrencePattern(Enums::RecurrenceType::DAILY, 0), std::invalid_argument);
    EXPECT_THROW(RecurrencePattern(Enums::RecurrenceType::WEEKLY, 1, {}), std::invalid_argument);
}

TEST_F(TaskManagerTest, RecurrencePattern_Setters) {
    RecurrencePattern rec;
    rec.SetType(Enums::RecurrenceType::MONTHLY);
    EXPECT_EQ(rec.GetType(), Enums::RecurrenceType::MONTHLY);

    rec.SetInterval(2);
    EXPECT_EQ(rec.GetInterval(), 2);

    EXPECT_THROW(rec.SetInterval(0), std::invalid_argument);
    EXPECT_THROW(rec.SetOccurrenceCount(-1), std::invalid_argument);
}

// Test Task
TEST_F(TaskManagerTest, Task_ConstructionAndGetters) {
    auto dueDate = DateUtils::AddDays(DateUtils::Now(), 1);
    CategoryPtr cat = std::make_shared<Category>("TestCat");
    Task task("Test Task", "Description", dueDate, Enums::Priority::HIGH, cat);

    EXPECT_EQ(task.GetTitle(), "Test Task");
    EXPECT_EQ(task.GetDescription(), "Description");
    EXPECT_EQ(task.GetPriority(), Enums::Priority::HIGH);
    EXPECT_EQ(task.GetStatus(), Enums::TaskStatus::PENDING);
    EXPECT_TRUE(task.HasCategory());
    EXPECT_EQ(task.GetCategory()->GetName(), "TestCat");
}

TEST_F(TaskManagerTest, Task_Setters) {
    Task task;
    task.SetTitle("New Title");
    EXPECT_EQ(task.GetTitle(), "New Title");

    auto newDue = DateUtils::AddDays(DateUtils::Now(), 2);
    task.SetDueDate(newDue);
    EXPECT_EQ(task.GetDueDate(), newDue);

    task.SetStatus(Enums::TaskStatus::COMPLETED);
    EXPECT_EQ(task.GetStatus(), Enums::TaskStatus::COMPLETED);
    EXPECT_NE(task.GetCompletedAt(), std::chrono::system_clock::time_point::min());
}

TEST_F(TaskManagerTest, Task_Tags) {
    Task task;
    task.AddTag("tag1");
    task.AddTag("tag2");
    task.AddTag("tag1");  // Duplicate, should not add
    auto tags = task.GetTags();
    ASSERT_EQ(tags.size(), 2u);
    EXPECT_EQ(tags[0], "tag1");
    EXPECT_EQ(tags[1], "tag2");

    task.RemoveTag("tag1");
    tags = task.GetTags();
    ASSERT_EQ(tags.size(), 1u);
    EXPECT_EQ(tags[0], "tag2");
}

TEST_F(TaskManagerTest, Task_Recurrence) {
    Task task;
    RecurrencePatternPtr rec = std::make_shared<RecurrencePattern>(Enums::RecurrenceType::DAILY, 1);
    task.SetRecurrencePattern(rec);
    EXPECT_TRUE(task.IsRecurring());
}

// Test ProductivityReport
TEST_F(TaskManagerTest, ProductivityReport_Construction) {
    auto start = DateUtils::StringToTimePoint("2025-12-01 00:00:00");
    auto end = DateUtils::StringToTimePoint("2025-12-31 23:59:59");
    ProductivityReport report(start, end);

    EXPECT_EQ(report.GetStartDate(), start);
    EXPECT_EQ(report.GetEndDate(), end);

    EXPECT_THROW(ProductivityReport(end, start), std::invalid_argument);
}

TEST_F(TaskManagerTest, ProductivityReport_EmptyReports) {
    auto start = DateUtils::Now();
    auto end = DateUtils::AddDays(start, 7);
    ProductivityReport report(start, end);

    std::string summary = report.GenerateSummaryReport();

    // Summary checks
    EXPECT_TRUE(summary.find("OVERALL STATISTICS") != std::string::npos);
    EXPECT_TRUE(summary.find("Total Tasks: 0") != std::string::npos);
    EXPECT_TRUE(summary.find("Completed: 0 (0.0%)") != std::string::npos);
    EXPECT_TRUE(summary.find("Pending: 0") != std::string::npos);
    EXPECT_TRUE(summary.find("Overdue: 0") != std::string::npos);

    std::string detailed = report.GenerateDetailedReport();

    // Detailed report checks
    EXPECT_TRUE(detailed.find("OVERALL STATISTICS") != std::string::npos);
    EXPECT_TRUE(detailed.find("PERFORMANCE ANALYSIS") != std::string::npos);

    // Ensure no performance evaluation is shown when no tasks
    EXPECT_TRUE(detailed.find("Efficiency Score") == std::string::npos);
    EXPECT_TRUE(detailed.find("Performance:") == std::string::npos);
}

// Test IdGenerator
TEST_F(TaskManagerTest, IdGenerator_UniqueIds) {
    int id1 = IdGenerator::GetInstance().GenerateTaskId();
    int id2 = IdGenerator::GetInstance().GenerateTaskId();
    EXPECT_EQ(id2, id1 + 1);

    int cat1 = IdGenerator::GetInstance().GenerateCategoryId();
    int cat2 = IdGenerator::GetInstance().GenerateCategoryId();
    EXPECT_EQ(cat2, cat1 + 1);
}

// Main for running tests
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}