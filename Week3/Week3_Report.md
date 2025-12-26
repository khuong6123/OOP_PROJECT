# BÁO CÁO TIẾN ĐỘ – TUẦN 3
**Môn học**: OOP
**Dự án**: Task Scheduler Application

**MSSV**: 24120388
**Tên**: Lê Đại Nghĩa  
**Vai trò**: Developer

**MSSV**: 24120401  
**Tên**: Trần Hùng Nhân  
**Vai trò**: Supporter

**Tuần**: 3  

---

## 1. MỤC TIÊU TUẦN 3

Trong tuần 3, mục tiêu chính của dự án là xây dựng **Data Access Layer (DAL)** nhằm tách biệt hoàn toàn việc lưu trữ dữ liệu ra khỏi logic nghiệp vụ, đảm bảo tính mở rộng, linh hoạt và dễ bảo trì cho hệ thống.

Các mục tiêu cụ thể:
- Thiết kế interface cho tầng truy cập dữ liệu
- Cài đặt lưu trữ dữ liệu bằng JSON và CSV
- Áp dụng Factory Pattern để tạo DataManager
- Viết unit test cho toàn bộ DAL

---

## 2. KIẾN TRÚC DATA ACCESS LAYER

DAL được thiết kế theo hướng **interface-based**, cho phép thay đổi cơ chế lưu trữ mà không ảnh hưởng đến các layer phía trên.

Cấu trúc thư mục:
```

DAL/
├── ITaskRepository.h
├── ICategoryRepository.h
├── JSONDataManager.h
├── JSONDataManager.cpp
├── CSVDataManager.h
├── CSVDataManager.cpp
├── DataManagerFactory.h
├── DataManagerFactory.cpp

```

---

## 3. CÁC THÀNH PHẦN ĐÃ TRIỂN KHAI

### 3.1 DataManager Interface

#### ITaskRepository
- Định nghĩa các phương thức:
  - Lưu danh sách Task
  - Tải danh sách Task
- Không chứa logic cài đặt
- Đóng vai trò là hợp đồng cho các DataManager cụ thể

#### ICategoryRepository
- Định nghĩa các phương thức:
  - Lưu danh sách Category
  - Tải danh sách Category
- Đảm bảo tính nhất quán khi thay đổi backend lưu trữ

---

### 3.2 JSONDataManager

JSONDataManager chịu trách nhiệm lưu trữ và truy xuất dữ liệu dưới dạng JSON.

Các đặc điểm chính:
- Serialize và deserialize Task, Category, RecurrencePattern
- Lưu dữ liệu ra file JSON
- Đọc dữ liệu từ file JSON
- Xử lý escape và unescape chuỗi JSON
- Ghi file an toàn (ghi file tạm, sau đó replace)
- Tạo file backup trước khi ghi
- Sử dụng mutex để đảm bảo thread safety

Định dạng dữ liệu:
- Task được lưu dưới dạng JSON object
- Danh sách Task và Category được lưu trong JSON array

---

### 3.3 CSVDataManager

CSVDataManager cung cấp cơ chế lưu trữ thay thế bằng định dạng CSV.

Các đặc điểm chính:
- Ghi dữ liệu theo định dạng CSV chuẩn
- Có header cho mỗi file CSV
- Escape và parse các trường có dấu phẩy hoặc dấu ngoặc kép
- Hỗ trợ lưu Task có RecurrencePattern và Tags
- Dễ dàng mở file bằng các công cụ bảng tính

---

### 3.4 DataManagerFactory (Factory Pattern)

DataManagerFactory được sử dụng để:
- Tạo DataManager phù hợp (JSON hoặc CSV)
- Ẩn chi tiết cài đặt khỏi các layer phía trên
- Dễ dàng mở rộng thêm các kiểu lưu trữ mới trong tương lai

Factory Pattern giúp hệ thống tuân thủ:
- Open/Closed Principle
- Dependency Inversion Principle

---

## 4. UNIT TEST CHO DAL

- Viết unit test cho JSONDataManager và CSVDataManager
- Kiểm thử các chức năng:
  - Lưu dữ liệu
  - Đọc dữ liệu
  - Xử lý file trống
  - Dữ liệu không hợp lệ
- Đảm bảo dữ liệu sau khi load tương đương dữ liệu ban đầu
- Sử dụng GoogleTest
- Tất cả test đều PASS

---

## 5. CHECKLIST NỘP TUẦN 3

- Có DataManager interface  
- Có JSONDataManager và CSVDataManager  
- Có áp dụng Factory Pattern  
- Có unit test cho DAL  

Tất cả các yêu cầu trong checklist Tuần 3 đều đã được hoàn thành đầy đủ.

---

## 6. ĐÁNH GIÁ TỔNG KẾT

- DAL được thiết kế rõ ràng, tuân thủ kiến trúc nhiều tầng
- Dễ dàng thay đổi hoặc mở rộng cơ chế lưu trữ
- Code có tính tái sử dụng cao
- Đảm bảo an toàn dữ liệu và thread safety
- Phù hợp cho các hệ thống có yêu cầu mở rộng lâu dài

---

## 7. KẾ HOẠCH TUẦN 4

### Tuần 4 (09/12 - 15/12): Triển khai BUS Layer
- Triển khai Business Logic Layer (BUS): TaskManager, CategoryManager, ReminderManager, StatisticsManager, InputValidator
- Cài đặt các thuật toán quản lý task, reminder, thống kê
- Viết unit tests cho BUS layer