///*
// * bkit_protocol.c
// *
// *  Created on: Jan 25, 2026
// *      Author: DUONG DEP TRAI
// */
//
//
//#include "bkit_protocol.h"
//#include "hw_driver.h"
//#include "pb_encode.h"
//#include "pb_decode.h"
//#include <string.h>
//#include "lcd.h"
//
////HAM TINH TOAN CRC 16
//uint16_t calculate_crc16(const uint8_t *data, uint16_t len) {
//    uint16_t crc = 0xFFFF; // Giá trị khởi tạo
//    for (uint16_t i = 0; i < len; i++) {
//        crc ^= (uint16_t)data[i] << 8;
//        for (uint8_t j = 0; j < 8; j++) {
//            if (crc & 0x8000) crc = (crc << 1) ^ 0x1021; // Polynomial 0x1021
//            else crc <<= 1;
//        }
//    }
//    return crc;
//}
//
//
///* CAU TRUC GOI TIN:
// * START(1) - LENGTH(2) - CRC_CHECK(2) - FLAGS(1) - PAYLOAD(LENGTH)
//*/
//
//// HAM GUI
//bool bkit_protocol_send(const SensorData *data) {
//    uint8_t pb_buffer[64];
//    pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));
//
//    // TUAN TU HOA
//    if (!pb_encode(&stream, SensorData_fields, data)) return false;
//    uint16_t payload_len = (uint16_t)stream.bytes_written;
//
//    // TAO HEADER
//    bkit_header_t header;
//    header.start_of_packet = BKIT_START_BYTE;
//    header.packet_length = payload_len; // LITTLE ENDIAN
//    header.crc16 = calculate_crc16(pb_buffer, payload_len);
//    header.flags = 0x00;
//
//    // 3. GUI HEADER XUONG LAYER 1
//    uint8_t *header_ptr = (uint8_t *)&header;
//    for (int i = 0; i < sizeof(bkit_header_t); i++) {
//        hw_send_byte(header_ptr[i]);
//    }
//
//    // 4. GUI PAYLOAD
//    for (int i = 0; i < payload_len; i++) {
//        hw_send_byte(pb_buffer[i]);
//    }
//
//    return true;
//}
//
//// HAM NHAN
//bool bkit_protocol_receive(SensorData *data) {
//    uint8_t rx_byte;
//
//    // DO TIM START BYTE
//    if (!hw_receive_byte(&rx_byte) || rx_byte != BKIT_START_BYTE) return false;
//
//    // DOC PHAN CON LAI CUA HEADER
//    bkit_header_t header;
//    header.start_of_packet = BKIT_START_BYTE;
//    uint8_t *header_ptr = (uint8_t *)&header + 1; // Bắt đầu từ byte sau Start
//    for (int i = 0; i < 5; i++) {
//        if (!hw_receive_byte(header_ptr++)) return false;
//    }
//
//    // DOC PAYLOAD DUA TREN DO DAI LENGTH
//    uint8_t payload[64];
//    for (int i = 0; i < header.packet_length; i++) {
//        if (!hw_receive_byte(&payload[i])) return false;
//    }
//
//    // CRC CHECK
//    if (calculate_crc16(payload, header.packet_length) != header.crc16) {
//        // SAI CRC: BO QUA GOI TIN, XU LY LOI (NANG CAP SAU)
//    	lcd_show_string(10, 100, "CRC False", RED, WHITE, 24, 0);
//        return false;
//    }
//
//    // DECODE
//    pb_istream_t stream = pb_istream_from_buffer(payload, header.packet_length);
//    if (!pb_decode(&stream, SensorData_fields, data)) return false;
//
//    return true;
//}


/*
 * bkit_protocol.c
 *
 * FIX: Thêm cơ chế chờ (Timeout) để đảm bảo không bị mất gói tin
 * khi CPU chạy nhanh hơn tốc độ truyền UART/I2C.
 */

#include "bkit_protocol.h"
#include "hw_driver.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include <string.h>
#include "lcd.h"

// --- HÀM PHỤ TRỢ: NHẬN CÓ CHỜ ĐỢI (BLOCKING WITH TIMEOUT) ---
// Hàm này giúp CPU kiên nhẫn chờ dữ liệu về đủ
static bool hw_receive_byte_wait(uint8_t *data, uint32_t timeout_ms) {
    uint32_t start_tick = HAL_GetTick();
    while (HAL_GetTick() - start_tick < timeout_ms) {
        if (hw_receive_byte(data)) {
            return true; // Đã nhận được
        }
    }
    return false; // Hết giờ mà chưa thấy đâu
}

// HAM TINH TOAN CRC 16
uint16_t calculate_crc16(const uint8_t *data, uint16_t len) {
    uint16_t crc = 0xFFFF;
    for (uint16_t i = 0; i < len; i++) {
        crc ^= (uint16_t)data[i] << 8;
        for (uint8_t j = 0; j < 8; j++) {
            if (crc & 0x8000) crc = (crc << 1) ^ 0x1021;
            else crc <<= 1;
        }
    }
    return crc;
}

/* CAU TRUC GOI TIN:
 * START(1) - LENGTH(2) - CRC_CHECK(2) - FLAGS(1) - PAYLOAD(LENGTH)
*/

// HAM GUI
bool bkit_protocol_send(const SensorData *data) {
    uint8_t pb_buffer[64];
    pb_ostream_t stream = pb_ostream_from_buffer(pb_buffer, sizeof(pb_buffer));

    if (!pb_encode(&stream, SensorData_fields, data)) return false;
    uint16_t payload_len = (uint16_t)stream.bytes_written;

    bkit_header_t header;
    header.start_of_packet = BKIT_START_BYTE;
    header.packet_length = payload_len;
    header.crc16 = calculate_crc16(pb_buffer, payload_len);
    header.flags = 0x00;

    uint8_t *header_ptr = (uint8_t *)&header;
    for (int i = 0; i < sizeof(bkit_header_t); i++) {
        hw_send_byte(header_ptr[i]);
    }

    for (int i = 0; i < payload_len; i++) {
        hw_send_byte(pb_buffer[i]);
    }

    return true;
}

// --- HAM NHAN (ĐÃ SỬA LOGIC) ---
bool bkit_protocol_receive(SensorData *data) {
    uint8_t rx_byte;

    // 1. DO TIM START BYTE (Không cần chờ, cứ quét liên tục)
    if (!hw_receive_byte(&rx_byte) || rx_byte != BKIT_START_BYTE) return false;


    // [FIX]: MỘT KHI ĐÃ THẤY START, PHẢI QUYẾT TÂM CHỜ

    // 2. DOC HEADER (Dùng hàm có Timeout 10ms)
    bkit_header_t header;
    header.start_of_packet = BKIT_START_BYTE;
    uint8_t *header_ptr = (uint8_t *)&header + 1; // Bắt đầu từ byte sau Start

    for (int i = 0; i < 5; i++) {
        // Chờ tối đa 10ms cho mỗi byte header về
        if (!hw_receive_byte_wait(header_ptr++, 10)) return false;
    }

    // Kiểm tra độ dài gói tin hợp lý (tránh treo nếu Length quá lớn)
    if (header.packet_length > 64) return false;

    // 3. DOC PAYLOAD (Cũng phải chờ)
    uint8_t payload[64];
    for (int i = 0; i < header.packet_length; i++) {
        // Chờ tối đa 10ms cho mỗi byte payload
        if (!hw_receive_byte_wait(&payload[i], 10)) return false;
    }

    // 4. CRC CHECK
    if (calculate_crc16(payload, header.packet_length) != header.crc16) {
        lcd_show_string(10, 100, "CRC False  ", RED, WHITE, 24, 0);
        return false;
    } else {
        // Xóa thông báo lỗi cũ nếu CRC đúng
        lcd_show_string(10, 100, "CRC OK     ", GREEN, BLACK, 24, 0);
    }

    // 5. DECODE
    pb_istream_t stream = pb_istream_from_buffer(payload, header.packet_length);
    if (!pb_decode(&stream, SensorData_fields, data)) return false;

    return true;
}
