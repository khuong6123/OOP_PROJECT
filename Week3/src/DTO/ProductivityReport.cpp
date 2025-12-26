#include "ProductivityReport.h"
#include "../LIB/DateUtils.h"
#include <sstream>
#include <iomanip>
#include <algorithm>

ProductivityReport::ProductivityReport(const std::chrono::system_clock::time_point& startDate,
                                     const std::chrono::system_clock::time_point& endDate)
    : startDate_(startDate)
    , endDate_(endDate) {
    
    if (endDate <= startDate) {
        throw std::invalid_argument("End date must be after start date");
    }
}

// Getters
const ProductivityStats& ProductivityReport::GetOverallStats() const {
    return overallStats_;
}

const std::map<int, ProductivityStats>& ProductivityReport::GetCategoryStats() const {
    return categoryStats_;
}

const std::chrono::system_clock::time_point& ProductivityReport::GetStartDate() const {
    return startDate_;
}

const std::chrono::system_clock::time_point& ProductivityReport::GetEndDate() const {
    return endDate_;
}

// Report generation
std::string ProductivityReport::GenerateSummaryReport() const {
    std::ostringstream oss;
    
    // Report header
    oss << "PRODUCTIVITY REPORT\n";
    oss << "===================\n";
    oss << "Period: " << DateUtils::TimePointToString(startDate_) 
        << " to " << DateUtils::TimePointToString(endDate_) << "\n";
    oss << "\n";
    
    // Overall statistics
    oss << "OVERALL STATISTICS\n";
    oss << "------------------\n";
    oss << "Total Tasks: " << overallStats_.totalTasks << "\n";
    oss << "Completed: " << overallStats_.completedTasks 
        << " (" << std::fixed << std::setprecision(1) 
        << overallStats_.completionRate * 100 << "%)\n";
    oss << "Pending: " << overallStats_.pendingTasks << "\n";
    oss << "Overdue: " << overallStats_.overdueTasks << "\n";
    oss << "Avg Completion Time: " << std::fixed << std::setprecision(1) 
        << overallStats_.averageCompletionTimeHours << " hours\n";
    oss << "\n";
    
    // Tasks by priority
    if (!overallStats_.tasksByPriority.empty()) {
        oss << "TASKS BY PRIORITY\n";
        oss << "-----------------\n";
        for (const auto& [priority, count] : overallStats_.tasksByPriority) {
            oss << Enums::PriorityToString(priority) << ": " << count << "\n";
        }
        oss << "\n";
    }
    
    // Category statistics summary
    if (!categoryStats_.empty()) {
        oss << "CATEGORY SUMMARY\n";
        oss << "----------------\n";
        for (const auto& [categoryId, stats] : categoryStats_) {
            oss << "Category ID: " << categoryId << "\n";
            oss << "  Tasks: " << stats.totalTasks 
                << " (Completed: " << stats.completedTasks 
                << ", Rate: " << std::fixed << std::setprecision(1) 
                << stats.completionRate * 100 << "%)\n";
        }
    }
    
    return oss.str();
}

std::string ProductivityReport::GenerateDetailedReport() const {
    std::ostringstream oss;
    
    // Include summary
    oss << GenerateSummaryReport();
    oss << "\n";
    
    // Detailed category breakdown
    if (!categoryStats_.empty()) {
        oss << "DETAILED CATEGORY ANALYSIS\n";
        oss << "-------------------------\n";
        
        for (const auto& [categoryId, stats] : categoryStats_) {
            oss << "\nCATEGORY ID: " << categoryId << "\n";
            oss << std::string(15 + std::to_string(categoryId).length(), '-') << "\n";
            
            oss << "Total Tasks: " << stats.totalTasks << "\n";
            oss << "Completed: " << stats.completedTasks << "\n";
            oss << "Pending: " << stats.pendingTasks << "\n";
            oss << "Overdue: " << stats.overdueTasks << "\n";
            oss << "Completion Rate: " << std::fixed << std::setprecision(1) 
                << stats.completionRate * 100 << "%\n";
            oss << "Average Completion Time: " << std::fixed << std::setprecision(1) 
                << stats.averageCompletionTimeHours << " hours\n";
            
            // Tasks by priority for this category
            if (!stats.tasksByPriority.empty()) {
                oss << "Priority Breakdown:\n";
                for (const auto& [priority, count] : stats.tasksByPriority) {
                    oss << "  " << Enums::PriorityToString(priority) 
                        << ": " << count << "\n";
                }
            }
            
            // Tasks by status for this category
            if (!stats.tasksByStatus.empty()) {
                oss << "Status Breakdown:\n";
                for (const auto& [status, count] : stats.tasksByStatus) {
                    oss << "  " << Enums::TaskStatusToString(status) 
                        << ": " << count << "\n";
                }
            }
        }
    }
    
    // Performance analysis
    oss << "\nPERFORMANCE ANALYSIS\n";
    oss << "--------------------\n";
    
    if (overallStats_.totalTasks > 0) {
        double efficiencyScore = overallStats_.completionRate * 100;
        if (overallStats_.averageCompletionTimeHours > 0) {
            efficiencyScore /= overallStats_.averageCompletionTimeHours;
        }
        
        oss << "Efficiency Score: " << std::fixed << std::setprecision(2) 
            << efficiencyScore << "\n";
        
        if (overallStats_.completionRate >= 0.8) {
            oss << "Performance: EXCELLENT\n";
        } else if (overallStats_.completionRate >= 0.6) {
            oss << "Performance: GOOD\n";
        } else if (overallStats_.completionRate >= 0.4) {
            oss << "Performance: FAIR\n";
        } else {
            oss << "Performance: NEEDS IMPROVEMENT\n";
        }
        
        if (overallStats_.overdueTasks > 0) {
            double overdueRate = static_cast<double>(overallStats_.overdueTasks) / 
                               overallStats_.totalTasks;
            oss << "Overdue Rate: " << std::fixed << std::setprecision(1) 
                << overdueRate * 100 << "%\n";
            
            if (overdueRate > 0.2) {
                oss << "Recommendation: Focus on meeting deadlines\n";
            }
        }
    }

    
    return oss.str();
}