# BÁO CÁO ĐỒ ÁN: HỆ THỐNG QUẢN LÝ NHÀ THUỐC

## Chương 1: Mở đầu

**1. Lý do chọn đề tài**
Quản lý dược phẩm và vật tư y tế tại các nhà thuốc luôn đòi hỏi sự chính xác và kịp thời. Việc quản lý thủ công có nguy cơ mất kiểm soát về hàng tồn kho cũng như khó khăn trong việc theo dõi hạn sử dụng của thuốc. Đề tài "Hệ thống quản lý nhà thuốc" (C++ Pharmacy Manager - CPM) được lựa chọn với mong muốn áp dụng các kiến thức lập trình hướng đối tượng (OOP) và cơ sở dữ liệu để tạo ra một phần mềm hỗ trợ tự động hóa các quy trình quản lý, giảm thiểu sai sót.

**2. Mục tiêu**
Xây dựng một hệ thống phần mềm quản lý kho thuốc, vật tư y tế bằng ngôn ngữ C++ (chuẩn C++17 trở lên). Các mục tiêu cụ thể gồm: quản lý sản phẩm, theo dõi và cảnh báo hạn sử dụng, và thực hiện quá trình giao dịch bán lẻ cơ bản.

**3. Phạm vi ứng dụng**
Chương trình phục vụ chủ yếu cho các nhà thuốc, quầy thuốc quy mô vừa và nhỏ. Hệ thống chạy trên môi trường Console với giao diện được thiết kế màu sắc trực quan, dữ liệu lưu trữ vật lý thông qua SQLite.

---

## Chương 2: Phân tích yêu cầu

**1. Các chức năng chính**
- **Chức năng CRUD cơ bản**: 
  - Thêm mới sản phẩm (Thuốc hoặc Vật tư y tế).
  - Cập nhật thông tin và cập nhật số lượng nhập kho.
  - Xóa sản phẩm khỏi hệ thống (yêu cầu xác nhận tránh xóa nhầm).
  - Tìm kiếm sản phẩm theo tên hoặc Mã ID.
- **Nghiệp vụ đặc thù (Bán hàng)**: Thực hiện bán hàng nhanh thông qua việc nhập ID, số lượng báo bán. Hệ thống tự động kiểm tra lượng tồn kho, thực hiện trừ kho và in "hóa đơn" trực tiếp trên màn hình Console.
- **Báo cáo và Thống kê**: 
  - Hiển thị danh sách sản phẩm tổng quát dưới dạng bảng biểu dễ nhìn trực quan.
  - Tính năng cảnh báo hạn sử dụng: Lọc tự động các danh sách thuốc sắp hết hạn trong vòng 30 ngày tính từ ngày hiện tại của hệ thống.

**2. Yêu cầu phi chức năng**
- Giao diện thân thiện và dễ sử dụng bằng bảng màu đặc tả thông báo từ hệ thống (Console UI kết hợp mã màu ANSI).
- Tính bền vững của dữ liệu nhờ sự tích hợp cùng hệ quản trị cơ sở dữ liệu SQLite, giúp dữ liệu không bị mất khi thoát chương trình.
- Mã nguồn cần phải triển khai sạch sẽ, áp dụng chuẩn OOP để dễ bảo trì sau này.

---

## Chương 3: Thiết kế hệ thống

**1. Cấu trúc và phân chia Layer (Lớp)**
Hệ thống được thiết kế chia thành các tầng để tách biệt được UI và logic xử lý:
- **Lớp Database (DatabaseHandler)**: Chuyên trách mở/đóng kết nối SQLite và thực thi các câu lệnh SQL.
- **Lớp Middleware/Controller (PharmacyManager)**: Xử lý các nghiệp vụ (CRUD, trừ kho, logic kiểm tra ngày giờ) như một cầu nối giữa cơ sở dữ liệu và giao diện màn hình.
- **Lớp View/UI (Console UI)**: Giao diện dòng lệnh giao tiếp với người dùng và in kết quả bằng ANSI escape codes.

**2. Thiết kế Cơ sở dữ liệu**
Để phù hợp với quy mô đồ án nhỏ vừa, cơ sở dữ liệu được tổ chức thành 1 bảng duy nhất (Single-Table Strategy) có tên là `Products` kết hợp sử dụng cột nhận diện phân loại:
- `id` (TEXT, Khóa Chính): Mã nhận diện sản phẩm (Ví dụ: T001, V001).
- `name` (TEXT): Tên sản phẩm.
- `price` (REAL): Đơn giá bán.
- `quantity` (INTEGER): Số lượng đang tồn kho.
- `expiry_date` (TEXT): Định dạng YYYY-MM-DD để dễ dàng tiến hành các câu truy vấn so sánh thời gian.
- `type` (INTEGER): Phân biệt thuộc tính (1 - Thuốc; 2 - Vật tư).
- `ingredient` (TEXT), `dosage` (TEXT): Hoạt chất và liều dùng (Dành riêng cho Loại Thuốc).
- `unit` (TEXT): Đơn vị như Cái, Bộ (Dành riêng cho Loại Vật tư).

**3. Thiết kế Hướng đối tượng (Dựa trên 4 tính chất cốt lõi)**
Hệ thống tập trung cốt lõi vào 4 tính chất cơ bản nhất của OOP để thiết kế kiến trúc phần mềm (Sơ đồ lớp):
- **Tính đóng gói (Encapsulation)**: Mọi dữ liệu như ID, Tên, Giá, Số lượng đều để ở thuộc tính truy cập bằng từ khoá `protected` hoặc `private`. Các thuộc tính này không bị sửa đổi tùy tiện mà phải bị kiểm soát điều tiết qua các hàm `getter/setter`.
- **Tính trừu tượng (Abstraction)**: Định nghĩa lớp trừu tượng base tên là `Product`. Lớp này không được khởi tạo trực tiếp và bao gồm các thông tin chung của sản phẩm cùng hàm thuần ảo (pure virtual method) `virtual void display() = 0;`.
- **Tính kế thừa (Inheritance)**: Định nghĩa cụ thể ra 2 lớp dẫn xuất là `Medicine` (Thuốc) và `MedicalSupply` (Vật tư). `Medicine` kế thừa toàn bộ thuộc tính chung và mang các thuộc tính riêng (Thành phần, Liều lượng), cấu trúc tương tự dành cho `MedicalSupply` với thuộc tính đơn vị.
- **Tính đa hình (Polymorphism)**: Khi ứng dụng tải dữ liệu lớn với nhiều loại hàng lên Controller lên bộ nhớ dưới dạng mảng con trỏ của siêu lớp `Product*`, việc gọi lệnh `product->display()` sẽ dựa vào con trỏ ở thời điểm Runtime để tìm và xuất đúng nội dung bảng ứng với từng lớp `Medicine` hay `MedicalSupply` phù hợp (Hàm ghi đè `override` phương thức display).

---

## Chương 4: Cài đặt và Luồng xử lý chính

Một số logic lập trình quan trọng khi thực hiện:
- **Luồng quản lý Ngày/Tháng (Date-time cho Hạn sử dụng)**: Vì thời gian ngày theo chuỗi "YYYY-MM-DD" có tính chất alphabet-sortable nên hỗ trợ rất tốt cho SQLite khi so sánh. Tại trong ứng dụng sử dụng thêm library `<chrono>` hoặc `<ctime>` để trích xuất ngày thực tế, cộng thêm 30 ngày làm mốc, sau đó đưa chuỗi so sánh trực tiếp với chuỗi lấy từ SQLite để xuất danh sách cận limit.
- **Luồng Xử lý Dữ liệu trên SQLite qua C++**: Khi Data lấy từ SQLite thường phải dùng con trỏ `char*`. Những thành phần như giá hay số lượng có luồng converter đặc thù về kiểu nguyên thủy `int`/`double` cho Object. Các bước ID auto-generate hoặc check duplication cần được xác minh cẩn thận thông qua DatabaseHandler trước khi `INSERT`.
- **Luồng Bán Hàng Trừ Kho**: Từ giao diện màn hình chọn sản phẩm, ứng dụng lập tức check DB số lượng (quantity). Nếu `kho < yêu cầu`, lập tức `cout` cảnh báo bằng màu chữ Đỏ (macro `ANSI \033[31m`). Nếu đạt chuẩn, hàm thực hiện lệnh SQL `UPDATE` lại quantity và cout hệ thống hoá đơn bằng màu chữ Vàng/Xanh lá.

---

## Chương 5: Tổng kết

**1. Đánh giá kết quả**
Đồ án đã đáp ứng hoàn chỉnh hầu hết các yêu cầu đặt ra từ ban đầu với việc kiểm soát dữ liệu hiệu quả và vận dụng được đầy đủ bản chất cốt lõi của OOP. Chức năng chạy mượt, Console UI tổ chức khoa học, dễ tiếp cận chức năng. 

**2. Ưu điểm**
- Có khả năng cấu trúc tốt, phân biệt module thành layer rõ ràng. 
- Tính năng xử lý hạn sử dụng thiết yếu và hữu ích. 
- Xử lý UX qua màu sắc thông minh cho Console đơn giản. SQLite đi kèm là điểm sáng về dữ liệu bền.

**3. Hạn chế**
- Do ứng dụng kiến trúc 1 Bảng (Single-Table) trong Database do quy mô nên các cột không sử dụng (đối với Vật tư y tế có null thành phần Thuốc) bị lãng phí cục bộ, khó mở rộng hơn về sau.
- Chưa có giao diện người dùng bằng cửa sổ (GUI). 

**4. Hướng phát triển**
- Áp dụng các thư viện GUI tiên tiến với phiên bản 2.0 (ví dụ: ImGui cho cấu trúc thời gian thực hoặc Qt cho Cross-Platform).
- Chuẩn hóa CSDL (Database Normalization) với các bảng riêng biệt kết nối Primary/Foreign Key.
- Khai thác mở rộng các lớp mới như Quản lý người dùng/Khách hàng, Theo dõi History giao dịch nhập kho, và phân quyền quản trị cao cấp.
