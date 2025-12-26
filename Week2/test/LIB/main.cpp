#include <gtest/gtest.h>
#include <chrono>
#include <vector>
#include "../../src/LIB/StringUtils.h"  // Assuming relative path based on provided documents
#include "../../src/LIB/InputValidator.h"  // Assuming relative path
#include "../../src/LIB/DateUtils.h"  // Assuming relative path
#include "../../src/DTO/Enums.h"  // Assuming Enums.h is available for DayOfWeek
#include "../../src/LIB/Constants.h"  // Assuming Constants.h is available
// Test fixture for shared setup if needed
class InputValidatorTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}

};

// Tests for StringUtils (used by InputValidator)
TEST(StringUtilsTest, Trim) {
    EXPECT_EQ(StringUtils::Trim("  hello world  "), "hello world");
    EXPECT_EQ(StringUtils::Trim("\t\nhello\r"), "hello");
    EXPECT_EQ(StringUtils::Trim(""), "");
    EXPECT_EQ(StringUtils::Trim("   "), "");
}

TEST(StringUtilsTest, ToLower) {
    EXPECT_EQ(StringUtils::ToLower("Hello World"), "hello world");
    EXPECT_EQ(StringUtils::ToLower("123 ABC"), "123 abc");
    EXPECT_EQ(StringUtils::ToLower(""), "");
}

TEST(StringUtilsTest, ToUpper) {
    EXPECT_EQ(StringUtils::ToUpper("hello world"), "HELLO WORLD");
    EXPECT_EQ(StringUtils::ToUpper("123 abc"), "123 ABC");
    EXPECT_EQ(StringUtils::ToUpper(""), "");
}

TEST(StringUtilsTest, Contains) {
    EXPECT_TRUE(StringUtils::Contains("hello world", "world"));
    EXPECT_FALSE(StringUtils::Contains("hello", "world"));
    EXPECT_TRUE(StringUtils::Contains("", ""));
}

TEST(StringUtilsTest, StartsWith) {
    EXPECT_TRUE(StringUtils::StartsWith("hello world", "hello"));
    EXPECT_FALSE(StringUtils::StartsWith("hello", "world"));
    EXPECT_TRUE(StringUtils::StartsWith("hello", ""));
}

TEST(StringUtilsTest, EndsWith) {
    EXPECT_TRUE(StringUtils::EndsWith("hello world", "world"));
    EXPECT_FALSE(StringUtils::EndsWith("world", "hello"));
    EXPECT_TRUE(StringUtils::EndsWith("hello", ""));
}

TEST(StringUtilsTest, Split) {
    std::vector<std::string> expected = {"hello", "world", "test"};
    EXPECT_EQ(StringUtils::Split("hello,world,test", ','), expected);
    EXPECT_EQ(StringUtils::Split("hello", ','), std::vector<std::string>{"hello"});
    EXPECT_EQ(StringUtils::Split("", ','), std::vector<std::string>{});
}

TEST(StringUtilsTest, Join) {
    EXPECT_EQ(StringUtils::Join({"hello", "world"}, ", "), "hello, world");
    EXPECT_EQ(StringUtils::Join({"single"}, ", "), "single");
    EXPECT_EQ(StringUtils::Join({}, ", "), "");
}

TEST(StringUtilsTest, IsEmpty) {
    EXPECT_TRUE(StringUtils::IsEmpty(""));
    EXPECT_TRUE(StringUtils::IsEmpty("   "));
    EXPECT_FALSE(StringUtils::IsEmpty("hello"));
}

TEST(StringUtilsTest, Replace) {
    EXPECT_EQ(StringUtils::Replace("hello world", "world", "universe"), "hello universe");
    EXPECT_EQ(StringUtils::Replace("aaa", "a", "b"), "bbb");
    EXPECT_EQ(StringUtils::Replace("hello", "notfound", "replace"), "hello");
}

// Tests for InputValidator
TEST_F(InputValidatorTest, ValidateTaskTitle) {
    EXPECT_TRUE(InputValidator::ValidateTaskTitle("Valid Title"));
    EXPECT_FALSE(InputValidator::ValidateTaskTitle(""));
    EXPECT_FALSE(InputValidator::ValidateTaskTitle("   "));
    std::string longTitle(101, 'a');  // Longer than MAX_TASK_TITLE_LENGTH (100)
    EXPECT_FALSE(InputValidator::ValidateTaskTitle(longTitle));
}

TEST_F(InputValidatorTest, ValidateTaskDescription) {
    EXPECT_TRUE(InputValidator::ValidateTaskDescription("Short desc"));
    std::string longDesc(501, 'a');  // Longer than MAX_DESCRIPTION_LENGTH (500)
    EXPECT_FALSE(InputValidator::ValidateTaskDescription(longDesc));
    EXPECT_TRUE(InputValidator::ValidateTaskDescription(""));  // Empty is allowed
}

TEST_F(InputValidatorTest, ValidateDueDate) {
    auto future = std::chrono::system_clock::now() + std::chrono::hours(1);
    EXPECT_TRUE(InputValidator::ValidateDueDate(future));
    auto past = std::chrono::system_clock::now() - std::chrono::hours(1);
    EXPECT_FALSE(InputValidator::ValidateDueDate(past));
}

TEST_F(InputValidatorTest, ValidatePriority) {
    EXPECT_TRUE(InputValidator::ValidatePriority("low"));
    EXPECT_TRUE(InputValidator::ValidatePriority("MEDIUM"));
    EXPECT_TRUE(InputValidator::ValidatePriority("HiGh"));
    EXPECT_TRUE(InputValidator::ValidatePriority("URGENT"));
    EXPECT_FALSE(InputValidator::ValidatePriority("invalid"));
    EXPECT_FALSE(InputValidator::ValidatePriority(""));
}

TEST_F(InputValidatorTest, ValidateStatus) {
    EXPECT_TRUE(InputValidator::ValidateStatus("pending"));
    EXPECT_TRUE(InputValidator::ValidateStatus("IN_PROGRESS"));
    EXPECT_TRUE(InputValidator::ValidateStatus("Completed"));
    EXPECT_TRUE(InputValidator::ValidateStatus("CANCELLED"));
    EXPECT_FALSE(InputValidator::ValidateStatus("invalid"));
    EXPECT_FALSE(InputValidator::ValidateStatus(""));
}

TEST_F(InputValidatorTest, ValidateCategoryName) {
    EXPECT_TRUE(InputValidator::ValidateCategoryName("Work"));
    EXPECT_FALSE(InputValidator::ValidateCategoryName(""));
    EXPECT_FALSE(InputValidator::ValidateCategoryName("   "));
    std::string longName(51, 'a');  // Longer than MAX_CATEGORY_NAME_LENGTH (50)
    EXPECT_FALSE(InputValidator::ValidateCategoryName(longName));
}

TEST_F(InputValidatorTest, ValidateColor) {
    EXPECT_TRUE(InputValidator::ValidateColor("#FFFFFF"));
    EXPECT_TRUE(InputValidator::ValidateColor(" #abc "));
    EXPECT_TRUE(InputValidator::ValidateColor("#12345678"));  // With alpha
    EXPECT_FALSE(InputValidator::ValidateColor("invalid"));
    EXPECT_FALSE(InputValidator::ValidateColor(""));
    EXPECT_FALSE(InputValidator::ValidateColor("#GGGGG"));  // Invalid hex
}

TEST_F(InputValidatorTest, ValidateRecurrenceType) {
    EXPECT_TRUE(InputValidator::ValidateRecurrenceType("daily"));
    EXPECT_TRUE(InputValidator::ValidateRecurrenceType("WEEKLY"));
    EXPECT_TRUE(InputValidator::ValidateRecurrenceType("Monthly"));
    EXPECT_TRUE(InputValidator::ValidateRecurrenceType("YEARLY"));
    EXPECT_TRUE(InputValidator::ValidateRecurrenceType("none"));
    EXPECT_FALSE(InputValidator::ValidateRecurrenceType("invalid"));
}

TEST_F(InputValidatorTest, ValidateRecurrenceInterval) {
    EXPECT_TRUE(InputValidator::ValidateRecurrenceInterval(1));
    EXPECT_TRUE(InputValidator::ValidateRecurrenceInterval(365));
    EXPECT_FALSE(InputValidator::ValidateRecurrenceInterval(0));
    EXPECT_FALSE(InputValidator::ValidateRecurrenceInterval(-1));
    EXPECT_FALSE(InputValidator::ValidateRecurrenceInterval(366));  // Exceeds limit
}

TEST_F(InputValidatorTest, ValidateDaysOfWeek) {
    EXPECT_TRUE(InputValidator::ValidateDaysOfWeek({Enums::DayOfWeek::MONDAY, Enums::DayOfWeek::WEDNESDAY}));
    EXPECT_FALSE(InputValidator::ValidateDaysOfWeek({}));  // Empty
    EXPECT_FALSE(InputValidator::ValidateDaysOfWeek({Enums::DayOfWeek::MONDAY, Enums::DayOfWeek::MONDAY}));  // Duplicates
}

TEST_F(InputValidatorTest, ValidateId) {
    EXPECT_TRUE(InputValidator::ValidateId(1));
    EXPECT_FALSE(InputValidator::ValidateId(0));
    EXPECT_FALSE(InputValidator::ValidateId(-1));
}

TEST_F(InputValidatorTest, ValidateDateRange) {
    auto start = std::chrono::system_clock::now();
    auto end = start + std::chrono::hours(1);
    EXPECT_TRUE(InputValidator::ValidateDateRange(start, end));
    EXPECT_FALSE(InputValidator::ValidateDateRange(end, start));
    EXPECT_FALSE(InputValidator::ValidateDateRange(start, start));  // end should be > start
}

TEST_F(InputValidatorTest, ValidateTimeString) {
    EXPECT_TRUE(InputValidator::ValidateTimeString("12:30"));
    EXPECT_TRUE(InputValidator::ValidateTimeString("23:59:59"));
    EXPECT_FALSE(InputValidator::ValidateTimeString("24:00"));
    EXPECT_FALSE(InputValidator::ValidateTimeString("12:60"));
    EXPECT_FALSE(InputValidator::ValidateTimeString("invalid"));
}

TEST_F(InputValidatorTest, SanitizeString) {
    EXPECT_EQ(InputValidator::SanitizeString(" hello \t world "), "hello  world");  // Trim not applied here, but controls removed
    EXPECT_EQ(InputValidator::SanitizeString(std::string(1, 31)), "");  // Control char removed
}

TEST_F(InputValidatorTest, SanitizeColor) {
    EXPECT_EQ(InputValidator::SanitizeColor(" #abc "), "#ABC");
    EXPECT_EQ(InputValidator::SanitizeColor("FFFFFF"), "#FFFFFF");
    EXPECT_EQ(InputValidator::SanitizeColor(""), "");
}

// Tests for DateUtils
TEST(DateUtilsTest, Now) {
    auto now = DateUtils::Now();
    // Can't assert exact value, but check it's recent
    auto diff = std::chrono::system_clock::now() - now;
    EXPECT_LT(std::chrono::duration_cast<std::chrono::seconds>(diff).count(), 1);
}

TEST(DateUtilsTest, TimePointToStringAndBack) {
    auto tp = std::chrono::system_clock::now();
    std::string str = DateUtils::TimePointToString(tp);
    auto parsed = DateUtils::StringToTimePoint(str);
    // Check if same second (ignoring ms)
    EXPECT_TRUE(DateUtils::IsSameDay(tp, parsed));
}

TEST(DateUtilsTest, IsWeekend) {
    // This depends on current date, so test with known dates
    auto sunday = DateUtils::StringToTimePoint("2023-10-01 12:00:00");  // 2023-10-01 was Sunday
    EXPECT_TRUE(DateUtils::IsWeekend(sunday));
    auto monday = DateUtils::StringToTimePoint("2023-10-02 12:00:00");
    EXPECT_FALSE(DateUtils::IsWeekend(monday));
}

TEST(DateUtilsTest, IsSameDay) {
    auto tp1 = DateUtils::StringToTimePoint("2023-10-01 00:00:00");
    auto tp2 = DateUtils::StringToTimePoint("2023-10-01 23:59:59");
    EXPECT_TRUE(DateUtils::IsSameDay(tp1, tp2));
    auto tp3 = DateUtils::StringToTimePoint("2023-10-02 00:00:00");
    EXPECT_FALSE(DateUtils::IsSameDay(tp1, tp3));
}

TEST(DateUtilsTest, AddDays) {
    auto tp = DateUtils::StringToTimePoint("2023-10-01 12:00:00");
    auto added = DateUtils::AddDays(tp, 1);
    std::string expected = "2023-10-02 12:00:00";
    EXPECT_EQ(DateUtils::TimePointToString(added).substr(0, 19), expected);  // Compare without ms if any
}

TEST(DateUtilsTest, DaysBetween) {
    auto from = DateUtils::StringToTimePoint("2023-10-01 12:00:00");
    auto to = DateUtils::StringToTimePoint("2023-10-03 12:00:00");
    EXPECT_EQ(DateUtils::DaysBetween(from, to), 2);
    EXPECT_EQ(DateUtils::DaysBetween(to, from), -2);  // Negative if from > to
}

// --------------------------------------------------
// Entry point
// --------------------------------------------------
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}