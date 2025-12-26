# BÁO CÁO TIẾN ĐỘ – TUẦN 2  
**Môn học**: OOP
**Dự án**: Task Scheduler Application

**MSSV**: 24120388
**Tên**: Lê Đại Nghĩa  
**Vai trò**: Developer

**MSSV**: 24120401  
**Tên**: Trần Hùng Nhân  
**Vai trò**: Supporter

**Tuần:**: 2  

---

## MỤC TIÊU TUẦN 2
- Hoàn thiện DTO Layer  
- Chuẩn hóa Enums và các đối tượng dữ liệu  
- Viết unit test cho LIB và DTO  
- Đảm bảo code chạy ổn định, đúng kiến trúc  

---

## KIẾN TRÚC DỰ ÁN
Dự án được tổ chức theo kiến trúc Layered Architecture:

```

src/
├── LIB/
├── DTO/
tests/
├── LIB/
├── DTO/

```

- LIB: Chứa các thư viện dùng chung (Utils, Validator, Logger, Constants, …)  
- DTO: Định nghĩa các đối tượng truyền dữ liệu (Data Transfer Objects)  
- tests: Unit test sử dụng GoogleTest  

---

## CÁC CHỨC NĂNG ĐÃ HOÀN THÀNH

### 1. DTO Layer

#### Enums
- Các enum được xây dựng:
  - Priority  
  - TaskStatus  
  - RecurrenceType  
  - DayOfWeek  
- Hỗ trợ chuyển đổi hai chiều giữa Enum và String  
- Xử lý không phân biệt chữ hoa – chữ thường  
- Ném ngoại lệ `std::invalid_argument` khi dữ liệu không hợp lệ  

File liên quan:
- `DTO/Enums.h`  
- `DTO/Enums.cpp`  

---

#### Category
- Các thuộc tính chính:
  - id  
  - name  
  - description  
  - color  
  - createdAt  
  - updatedAt  
- Có kiểm tra dữ liệu đầu vào (validation)  
- Hỗ trợ cập nhật thời gian chỉnh sửa  

File liên quan:
- `DTO/Category.h`  
- `DTO/Category.cpp`  

---

#### Task
- Quản lý các thông tin:
  - Tiêu đề, mô tả  
  - Hạn hoàn thành (due date)  
  - Mức độ ưu tiên (priority)  
  - Trạng thái công việc (status)  
- Hỗ trợ gán Category và RecurrencePattern  
- Tự động cập nhật `completedAt` khi task được đánh dấu hoàn thành  
- Hỗ trợ thêm và xóa tag, không cho phép trùng lặp  

File liên quan:
- `DTO/Task.h`  
- `DTO/Task.cpp`  

---

#### RecurrencePattern
- Hỗ trợ các kiểu lặp:
  - NONE  
  - DAILY  
  - WEEKLY  
  - MONTHLY  
  - YEARLY  
- Kiểm tra hợp lệ:
  - interval phải lớn hơn 0  
  - Kiểu WEEKLY bắt buộc phải có ít nhất một ngày trong tuần  

File liên quan:
- `DTO/RecurrencePattern.h`  
- `DTO/RecurrencePattern.cpp`  

---

#### Productivity
- `ProductivityStats`: cấu trúc dữ liệu lưu trữ các thống kê  
- `ProductivityReport`: sinh báo cáo tổng hợp và báo cáo chi tiết theo danh mục  

File liên quan:
- `DTO/ProductivityStats.h`  
- `DTO/ProductivityReport.h`  
- `DTO/ProductivityReport.cpp`  

---

## LIB LAYER
Các module trong LIB Layer đã được sử dụng và kiểm thử:
- StringUtils  
- DateUtils  
- InputValidator  
- IdGenerator  
- Logger  
- Constants  

---

## UNIT TEST
- Unit test được xây dựng bằng GoogleTest  
- Bao phủ đầy đủ cho các module trong LIB và DTO  
- Kiểm tra các trường hợp:
  - Dữ liệu hợp lệ  
  - Dữ liệu không hợp lệ  
  - Các trường hợp biên (edge case)  
- Toàn bộ unit test đều chạy thành công  

Thư mục:
```

tests/unit/

```

---

## CHECKLIST TUẦN 2
- Có code chạy được  
- Đúng Layer: LIB và DTO  
- Có unit test  
- Code rõ ràng, đúng quy ước đặt tên  
- Push GitHub có lịch sử commit  

---

## ĐÁNH GIÁ TỔNG KẾT
- Kiến trúc dự án rõ ràng, phân tách trách nhiệm tốt  
- DTO không chứa logic nghiệp vụ  
- Unit test đầy đủ, thuận lợi cho bảo trì và mở rộng  
- Dự án sẵn sàng phát triển các layer tiếp theo  

---

## KẾ HOẠCH TUẦN 3
### Tuần 3 (02/12 - 08/12): Triển khai DAL Layer
- Triển khai Data Access Layer (DAL): DataManager, JSONDataManager, CSVDataManager
- Cài đặt DataManagerFactory
- Viết unit tests cho DAL layer
