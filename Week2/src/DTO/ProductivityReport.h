#ifndef PRODUCTIVITY_REPORT_H
#define PRODUCTIVITY_REPORT_H

#include "ProductivityStats.h"
#include "../LIB/common.h"
#include <chrono>
#include <map>

class ProductivityReport {
public:
    ProductivityReport(const std::chrono::system_clock::time_point& startDate,
                      const std::chrono::system_clock::time_point& endDate);
    
    // Getters
    const ProductivityStats& GetOverallStats() const;
    const std::map<int, ProductivityStats>& GetCategoryStats() const; // key: category ID
    const std::chrono::system_clock::time_point& GetStartDate() const;
    const std::chrono::system_clock::time_point& GetEndDate() const;
    
    // Report generation
    std::string GenerateSummaryReport() const;
    std::string GenerateDetailedReport() const;

private:
    std::chrono::system_clock::time_point startDate_;
    std::chrono::system_clock::time_point endDate_;
    ProductivityStats overallStats_;
    std::map<int, ProductivityStats> categoryStats_;
};

using ProductivityReportPtr = Common::Ref<ProductivityReport>;

#endif // PRODUCTIVITY_REPORT_H