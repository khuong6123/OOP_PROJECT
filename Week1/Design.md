P# CHECKLIST TUẦN 1 – PHÂN TÍCH & THIẾT KẾ HỆ THỐNG  
Đề tài: Task Scheduler Application  
Thời gian: Tuần 1 (18/11 – 24/11)

---

## 1. Phân tích yêu cầu hệ thống

Hệ thống Task Scheduler Application được xây dựng nhằm hỗ trợ người dùng:

- Quản lý công việc (Task)
- Phân loại công việc theo danh mục (Category)
- Thiết lập công việc định kỳ (Recurrence)
- Theo dõi tiến độ, trạng thái và năng suất làm việc
- Nhắc nhở trước hạn công việc
- Lưu trữ dữ liệu bền vững (JSON / CSV)

Yêu cầu phi chức năng:
- Ngôn ngữ: C++23
- Kiến trúc: Layered Architecture
- Mã nguồn rõ ràng, dễ mở rộng, dễ bảo trì
- Sử dụng smart pointer, logging, validation

---

## 2. Liệt kê các chức năng chính

### Quản lý công việc (Task)
- Thêm, sửa, xóa công việc
- Gán độ ưu tiên, trạng thái, hạn chót
- Gán category và tags
- Đánh dấu hoàn thành / đang làm

### Công việc định kỳ
- Daily / Weekly / Monthly / Yearly
- Cấu hình interval và ngày trong tuần
- Tự động sinh task tiếp theo

### Quản lý danh mục (Category)
- Thêm, sửa, xóa category
- Gán màu sắc và mô tả
- Tìm kiếm category theo tên

### Nhắc nhở (Reminder)
- Thiết lập nhắc trước hạn (offset)
- Callback khi đến thời điểm nhắc
- Tự động dọn nhắc không cần thiết

### Thống kê & báo cáo
- Tổng số task, task hoàn thành, task quá hạn
- Tỷ lệ hoàn thành
- Thống kê theo priority, status, category
- Báo cáo năng suất theo thời gian

### Lưu trữ dữ liệu
- Hỗ trợ JSON và CSV
- Factory lựa chọn DataManager
- Load và Save dữ liệu

---

## 3. Thiết kế Layered Architecture

Hệ thống được thiết kế theo Layered Architecture gồm:

```

UI (ConsoleUI, CalendarRenderer)
↓
Use Case (AddTask, Report, Recurrence, MarkCompleted)
↓
Business Logic (TaskService, CategoryService, StatisticsManager, ReminderManager)
↓
Data Access (ITaskRepository, JSONDataManager, CSVDataManager)
↓
LIB & DTO (Utils, Constants, Task, Category, Enums)

```

Ưu điểm:
- Tách biệt rõ ràng trách nhiệm
- Dễ kiểm thử từng layer
- Dễ mở rộng UI hoặc thay đổi cơ chế lưu trữ

---

## 4. Chốt DTO & Data Model

### DTO chính
- Task
- Category
- RecurrencePattern
- ProductivityStats
- ProductivityReport

### Enum
- Priority
- TaskStatus
- RecurrenceType
- DayOfWeek

Đặc điểm thiết kế:
- DTO không chứa logic nghiệp vụ phức tạp
- Sử dụng shared_ptr để chia sẻ dữ liệu giữa các layer
- Thời gian xử lý bằng std::chrono

---

## 5. Mô tả các Use Case chính

### AddTaskUseCase
- Nhận dữ liệu từ UI
- Kiểm tra hợp lệ input
- Gọi TaskService để thêm task

### MarkTaskCompletedUseCase
- Đánh dấu task hoàn thành
- Cập nhật thời gian hoàn thành
- Phục vụ thống kê năng suất

### ProcessRecurringTasksUseCase
- Duyệt các task định kỳ
- Sinh task mới dựa trên RecurrencePattern

### GenerateProductivityReportUseCase
- Thu thập dữ liệu từ TaskService
- Gọi StatisticsManager
- Sinh báo cáo tổng hợp

---

## 6. Lập kế hoạch chi tiết các tuần

| Tuần | Nội dung |
|------|----------|
| Tuần 1 | Phân tích yêu cầu và thiết kế |
| Tuần 2 | Triển khai LIB và DTO |
| Tuần 3 | Triển khai DAL |
| Tuần 4 | Triển khai Business Logic |
| Tuần 5 | UI và Use Case |
| Tuần 6 | Kiểm thử, tối ưu và báo cáo |

Kế hoạch phù hợp với tiến độ thực tế của nhóm.

---

## 7. Tạo repository & cấu trúc project

Cấu trúc thư mục:

```

LIB/
DTO/
DAL/
BUS/
USECASE/
UI/
data/
tests/

```

Công cụ quản lý:
- Git và GitHub
- Branch và Pull Request
- Commit theo từng chức năng

---

## Kết luận tuần 1

- Phân tích yêu cầu hoàn chỉnh  
- Liệt kê đầy đủ chức năng  
- Thiết kế kiến trúc rõ ràng  
- Chốt DTO và Data Model  
- Mô tả các Use Case chính  
- Lập kế hoạch 6 tuần  
- Tạo repository và cấu trúc project  

