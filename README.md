# 🛰️ BKIT Communication Protocol
Giao thức truyền thông đa tầng (Multi-layer), hiệu suất cao dành cho hệ thống nhúng, hỗ trợ tuần tự hóa dữ liệu bằng Google Protocol Buffers và cơ chế tự phục hồi phần cứng.

---

## 📖 Tổng quan
**BKIT Protocol** là một giải pháp truyền thông tin cậy được thiết kế để tối ưu hóa việc trao đổi dữ liệu giữa các node vi điều khiển. Giao thức giải quyết triệt để các vấn đề về nghẽn cổ chai UART, mất đồng bộ gói tin và sự phụ thuộc vào phần cứng bằng cách tách biệt logic xử lý thành 3 lớp riêng biệt.

---

## 📐 Kiến trúc phân lớp (Layered Architecture)

### Layer 1: Hardware Driver (`hw_driver.c`)
Tầng vật lý chịu trách nhiệm tương tác trực tiếp với ngoại vi và quản lý tài nguyên DMA:
* **Cấu hình "Một dòng lệnh" (One-line Config)**: Cho phép chuyển đổi linh hoạt toàn bộ phương thức giao tiếp giữa **UART DMA Circular** và **I2C Master/Slave** chỉ bằng cách thay đổi một Macro duy nhất trong file cấu hình.
* **DMA Ring Buffer**: Sử dụng cơ chế bộ đệm vòng kết hợp truy xuất trực tiếp counter của DMA (`__HAL_DMA_GET_COUNTER`) để nhận dữ liệu liên tục ở tốc độ cao, đảm bảo không mất gói tin mà không làm tiêu tốn tài nguyên CPU.
* **Cơ chế tự phục hồi (Hardware Watchdog)**: Tích hợp xử lý lỗi trong `ErrorCallback` để tự động phát hiện và xóa các cờ lỗi phần cứng như Overrun (ORE), Noise (NE), hay Frame Error (FE), đảm bảo tái kích hoạt DMA ngay lập tức để duy trì kết nối.

### Layer 2: Protocol Layer (`bkit_protocol.c`)
Tầng xử lý logic gói tin, tuần tự hóa và đảm bảo tính toàn vẹn dữ liệu:
* **Tuần tự hóa với Nanopb**: Tích hợp **Google Protocol Buffers (Nanopb)** để mã hóa dữ liệu nhị phân. Kỹ thuật này giúp tối ưu hóa dung lượng Payload và đảm bảo tính tương thích dữ liệu cao hơn so với truyền cấu trúc (struct) thô.
* **Cấu trúc gói tin (Framing)**: Định dạng gói tin chặt chẽ: `START(1B) - LENGTH(2B) - CRC_CHECK(2B) - FLAGS(1B) - PAYLOAD(NB)`.
* **Kiểm soát lỗi (Integrity)**: Sử dụng giải thuật **CRC16-CCITT (0x1021)** để kiểm tra toàn vẹn dữ liệu cho mọi gói tin truyền nhận, loại bỏ hoàn toàn các dữ liệu sai lệch do nhiễu đường truyền.

### Layer 3: Application Layer (`app_main.c`)
Tầng cung cấp giao diện lập trình (API) cấp cao cho người dùng:
* **API đơn giản**: Cung cấp các hàm `bkit_send_message()` và `bkit_receive_message()` để làm việc trực tiếp với các đối tượng dữ liệu định nghĩa sẵn (`SensorData`) thay vì mảng byte thô.
* **Xử lý không chặn (Non-blocking)**: Thiết kế tối ưu để tích hợp vào các vòng lặp Super Loop hoặc bộ lập lịch (Scheduler) mà không gây treo hệ thống (CPU stalling).

---

## 🛠 Tech Stack
* **Ngôn ngữ**: C (Embedded Standard).
* **Thư viện**: **Nanopb** (Protocol Buffers for Embedded).
* **Phần cứng**: Dòng vi điều khiển STM32F4 (Hỗ trợ UART DMA và I2C).
* **Công cụ**: STM32CubeIDE, Git.

---

## 📂 Cấu trúc mã nguồn
* `hw_driver.c / .h`: Quản lý tầng vật lý, cấu hình DMA, UART, I2C và các hàm xử lý lỗi phần cứng.
* `bkit_protocol.c / .h`: Hiện thực logic đóng gói gói tin, tính toán CRC16 và mã hóa/giải mã Protobuf.
* `app_main.c / .h`: Cung cấp API tầng ứng dụng và quản lý logic luồng dữ liệu người dùng.
* `sensor.pb.c / .h`: Các file mã nguồn được sinh ra tự động từ định nghĩa `.proto`.

---
*Dự án này là một sản phẩm nghiên cứu kỹ thuật nhúng chuyên sâu, tập trung vào khả năng làm chủ thiết bị ngoại vi và tối ưu hóa hiệu suất giao tiếp giữa các hệ thống vi điều khiển.*
