#include <gtest/gtest.h>
#include "../../src/DAL/CSVDataManager.h"
#include "../../src/DAL/JSONDataManager.h"
#include "../../src/DAL/DataManagerFactory.h"
#include "../../src/DAL/ITaskRepository.h"
#include "../../src/DAL/ICategoryRepository.h"
#include "../../src/DTO/Task.h"
#include "../../src/DTO/Category.h"
#include "../../src/DTO/RecurrencePattern.h"
#include "../../src/LIB/DateUtils.h"
#include "../../src/LIB/StringUtils.h"
#include "../../src/LIB/Constants.h"
#include "../../src/DTO/Enums.h"
#include "../../src/LIB/common.h"
#include <filesystem>
#include <fstream>
#include <chrono>
#include <thread>  // For unique folder names

namespace fs = std::filesystem;

// Test fixture for data managers
class DataManagerTest : public ::testing::Test {
protected:
    std::string testFolder_;

    void SetUp() override {
        // Create unique test folder using timestamp
        auto now = std::chrono::system_clock::now();
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
        std::ostringstream oss;
        oss << "test_data_" << std::chrono::system_clock::to_time_t(now) << "_" << ms.count() << "/";
        testFolder_ = oss.str();

        fs::create_directories(testFolder_);
    }

    void TearDown() override {
        // Clean up test folder
        fs::remove_all(testFolder_);
    }

    // Helper to create a sample task
    TaskPtr CreateSampleTask(int id) {
        auto dueDate = DateUtils::AddDays(DateUtils::Now(), 1);
        auto cat = std::make_shared<Category>("TestCat", "Description", "#FF0000");
        cat->SetId(1);

        auto task = std::make_shared<Task>("Test Task", "Description", dueDate, Enums::Priority::HIGH, cat);
        task->SetId(id);
        task->AddTag("tag1");
        task->AddTag("tag2");

        auto rec = std::make_shared<RecurrencePattern>(Enums::RecurrenceType::DAILY, 1);
        task->SetRecurrencePattern(rec);

        return task;
    }

    // Helper to create a sample category
    CategoryPtr CreateSampleCategory(int id) {
        auto cat = std::make_shared<Category>("TestCat", "Description", "#FF0000");
        cat->SetId(id);
        return cat;
    }
};

// Test DataManagerFactory
TEST_F(DataManagerTest, DataManagerFactory_CreateRepositories) {
    auto taskRepoJson = DataManagerFactory::CreateTaskRepository(DataFormat::JSON, testFolder_);
    EXPECT_NE(taskRepoJson, nullptr);

    auto catRepoCsv = DataManagerFactory::CreateCategoryRepository(DataFormat::CSV, testFolder_);
    EXPECT_NE(catRepoCsv, nullptr);

    auto defaultTaskRepo = DataManagerFactory::CreateDefaultTaskRepository();
    EXPECT_NE(defaultTaskRepo, nullptr);

    EXPECT_THROW(DataManagerFactory::CreateTaskRepository(static_cast<DataFormat>(99), testFolder_), std::invalid_argument);
}

TEST_F(DataManagerTest, DataManagerFactory_FormatFromString) {
    EXPECT_EQ(DataManagerFactory::FormatFromString("json"), DataFormat::JSON);
    EXPECT_EQ(DataManagerFactory::FormatFromString("CSV"), DataFormat::CSV);
    EXPECT_THROW(DataManagerFactory::FormatFromString("invalid"), std::invalid_argument);
}

// Test JSONDataManager
TEST_F(DataManagerTest, JSONDataManager_SaveAndLoadTasks) {
    JSONDataManager manager(testFolder_);

    // Create sample tasks
    std::vector<TaskPtr> tasks;
    tasks.push_back(CreateSampleTask(1));
    tasks.push_back(CreateSampleTask(2));

    // Save
    EXPECT_TRUE(manager.SaveTasks(tasks));

    // Load
    auto loadedTasks = manager.LoadTasks();
    ASSERT_EQ(loadedTasks.size(), 2u);

    // Verify first task
    EXPECT_EQ(loadedTasks[0]->GetId(), 1);
    EXPECT_EQ(loadedTasks[0]->GetTitle(), "Test Task");
    EXPECT_EQ(loadedTasks[0]->GetPriority(), Enums::Priority::HIGH);
    EXPECT_TRUE(loadedTasks[0]->IsRecurring());
    EXPECT_EQ(loadedTasks[0]->GetTags().size(), 2u);
    EXPECT_EQ(loadedTasks[0]->GetCategory()->GetName(), "TestCat");
}

TEST_F(DataManagerTest, JSONDataManager_SaveAndLoadCategories) {
    JSONDataManager manager(testFolder_);

    // Create sample categories
    std::vector<CategoryPtr> categories;
    categories.push_back(CreateSampleCategory(1));
    categories.push_back(CreateSampleCategory(2));

    // Save
    EXPECT_TRUE(manager.SaveCategories(categories));

    // Load
    auto loadedCategories = manager.LoadCategories();
    ASSERT_EQ(loadedCategories.size(), 2u);

    // Verify first category
    EXPECT_EQ(loadedCategories[0]->GetId(), 1);
    EXPECT_EQ(loadedCategories[0]->GetName(), "TestCat");
    EXPECT_EQ(loadedCategories[0]->GetColor(), "#FF0000");
}

TEST_F(DataManagerTest, JSONDataManager_InvalidFolder) {
    EXPECT_THROW(JSONDataManager("/invalid/path/"), std::runtime_error);
}

// Test CSVDataManager
TEST_F(DataManagerTest, CSVDataManager_SaveAndLoadTasks) {
    CSVDataManager manager(testFolder_);

    // Create sample tasks
    std::vector<TaskPtr> tasks;
    tasks.push_back(CreateSampleTask(1));
    tasks.push_back(CreateSampleTask(2));

    // Save
    EXPECT_TRUE(manager.SaveTasks(tasks));

    // Load
    auto loadedTasks = manager.LoadTasks();
    ASSERT_EQ(loadedTasks.size(), 2u);

    // Verify first task (Note: CSV might not support all fields like recurrence or tags perfectly, depending on implementation)
    EXPECT_EQ(loadedTasks[0]->GetId(), 1);
    EXPECT_EQ(loadedTasks[0]->GetTitle(), "Test Task");
    EXPECT_EQ(loadedTasks[0]->GetPriority(), Enums::Priority::HIGH);
    // Add more checks based on what CSV serializes
}

TEST_F(DataManagerTest, CSVDataManager_SaveAndLoadCategories) {
    CSVDataManager manager(testFolder_);

    // Create sample categories
    std::vector<CategoryPtr> categories;
    categories.push_back(CreateSampleCategory(1));
    categories.push_back(CreateSampleCategory(2));

    // Save
    EXPECT_TRUE(manager.SaveCategories(categories));

    // Load
    auto loadedCategories = manager.LoadCategories();
    ASSERT_EQ(loadedCategories.size(), 2u);

    // Verify first category
    EXPECT_EQ(loadedCategories[0]->GetId(), 1);
    EXPECT_EQ(loadedCategories[0]->GetName(), "TestCat");
    EXPECT_EQ(loadedCategories[0]->GetColor(), "#FF0000");
}

TEST_F(DataManagerTest, CSVDataManager_InvalidFolder) {
    EXPECT_THROW(CSVDataManager("/invalid/path/"), std::runtime_error);
}

// Main for running tests
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}