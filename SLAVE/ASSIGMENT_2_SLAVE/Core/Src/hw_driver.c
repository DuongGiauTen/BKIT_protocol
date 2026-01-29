///*
// * hw_driver.c
// *
// *  Created on: Jan 25, 2026
// *      Author: DUONG DEP TRAI
// */
//#include "hw_driver.h"
//#include "bkit_config.h"
//#include "main.h"
//#include "usart.h"
//#include "i2c.h"
//
//extern UART_HandleTypeDef huart6;
//extern I2C_HandleTypeDef hi2c2;
//
//// CAU HINH RING BUFFER
//#define RX_BUFFER_SIZE 256
//
//volatile uint8_t dma_rx_buffer[RX_BUFFER_SIZE];
//volatile uint16_t rx_tail = 0;
//
//#define HW_TIMEOUT 1000
//// --- CAC HAM CUA LAYER 1
//
//void hw_init(void) {
//
//#if (BKIT_PHY_INTERFACE == PHY_UART)
//	// [TO DO]
//	// UART DMA Circular
//	HAL_UART_Receive_DMA(&huart6, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
//
//#elif (BKIT_PHY_INTERFACE == PHY_I2C)
//	// [TO DO]
//	HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
//#endif
//}
//
//// --- HÀM GỬI 1 BYTE ---
//void hw_send_byte(uint8_t data) {
//#if (BKIT_PHY_INTERFACE == PHY_UART)
//	// [TO DO]
//    // SEND BYTE CHO UART
//    HAL_UART_Transmit(&huart6, &data, 1, HW_TIMEOUT);
//
//#elif (BKIT_PHY_INTERFACE == PHY_I2C)
//	// [TO DO]
//    //SEND BYTE CHO I2C
//    HAL_I2C_Slave_Transmit(&hi2c2, &data, 1, HW_TIMEOUT);
//#endif
//}
//
//// HAM NHAN DUNG DMA
//
//uint8_t hw_receive_byte(uint8_t* data) {
//    uint16_t rx_head = 0;
//
//#if (BKIT_PHY_INTERFACE == PHY_UART)
//    rx_head = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart6.hdmarx);
//
//#elif (BKIT_PHY_INTERFACE == PHY_I2C)
//    rx_head = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(hi2c2.hdmarx);
//#endif
//
//    // LOGIC RING BUFFER CHUNG
//    if (rx_head == rx_tail) {
//        return 0; // buffer rong, chua co du lieu moi
//    }
//
//    *data = dma_rx_buffer[rx_tail];
//    rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
//
//    return 1; // da lay duoc 1 byte
//}
//
//uint32_t hw_get_tick_ms(void) {
//    return HAL_GetTick();
//}
//
//// 1. Xử lý khi UART gặp lỗi (Nhiễu, Overrun...) -> Khởi động lại DMA
//void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart) {
//    if (huart->Instance == USART6) {
//        // Xóa cờ lỗi (Thường HAL tự làm, nhưng làm thêm cho chắc)
//        __HAL_UART_CLEAR_OREFLAG(huart);
//        __HAL_UART_CLEAR_NEFLAG(huart);
//        __HAL_UART_CLEAR_FEFLAG(huart);
//        __HAL_UART_CLEAR_PEFLAG(huart);
//
//        // Kích hoạt lại DMA Receive vào đúng buffer cũ
//        HAL_UART_Receive_DMA(&huart6, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
//    }
//}
//
//// 2. Xử lý khi I2C gặp lỗi (Bus Error, ACK fail...) -> Khởi động lại DMA
//void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
//    if (hi2c->Instance == I2C2) {
//        // Kích hoạt lại DMA Receive
//        HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
//    }
//}
//
//// 3. (Tùy chọn) Xử lý khi I2C nhận xong (Transfer Complete)
//// Một số dòng chip I2C Slave Circular cần được kích lại sau khi Master STOP
//void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
//     // Nếu bạn thấy I2C vẫn bị treo sau 1 lần, hãy bỏ comment dòng dưới:
//     // HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
//}


/*
 * hw_driver.c - PHIÊN BẢN KHẮC PHỤC LỖI DỪNG DMA (FINAL ATTEMPT)
 */

#include "hw_driver.h"
#include "bkit_config.h"
#include "main.h"
#include "usart.h"
#include "i2c.h"

// --- KHAI BÁO BIẾN ---
extern UART_HandleTypeDef huart6;
extern I2C_HandleTypeDef hi2c2;
// extern DMA_HandleTypeDef hdma_usart6_rx; // Bỏ comment nếu cần debug DMA handle
// extern DMA_HandleTypeDef hdma_i2c2_rx;   // Bỏ comment nếu cần debug DMA handle

#define RX_BUFFER_SIZE 256
// __attribute__((section(".dma_buffer"))) // Nếu dùng chip có nhiều vùng RAM (như F7/H7) cần dòng này
volatile uint8_t dma_rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t rx_tail = 0;
#define HW_TIMEOUT 1000

// --- 1. KHỞI TẠO (HW INIT) ---
void hw_init(void) {
#if (BKIT_PHY_INTERFACE == PHY_UART)
    // Bật DMA UART lần đầu
    HAL_UART_Receive_DMA(&huart6, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
#elif (BKIT_PHY_INTERFACE == PHY_I2C)
    // Bật DMA I2C lần đầu
    //HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);

    // Bật thêm ngắt lắng nghe (Listen) cho I2C - Rất quan trọng để Slave tự reset
    HAL_I2C_EnableListen_IT(&hi2c2);
#endif
}

// --- 2. HÀM GỬI (TX) ---
void hw_send_byte(uint8_t data) {
#if (BKIT_PHY_INTERFACE == PHY_UART)
    HAL_UART_Transmit(&huart6, &data, 1, HW_TIMEOUT);
#elif (BKIT_PHY_INTERFACE == PHY_I2C)
    HAL_I2C_Slave_Transmit(&hi2c2, &data, 1, HW_TIMEOUT);
#endif
}

// --- 3. HÀM NHẬN (RX) ---
uint8_t hw_receive_byte(uint8_t* data) {
    uint16_t rx_head = 0;

#if (BKIT_PHY_INTERFACE == PHY_UART)
    rx_head = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart6.hdmarx);

    // [WATCHDOG CẤP CỨU]
    // Nếu UART báo READY (nghĩa là đã TẮT DMA), ta bật lại ngay lập tức
    if(huart6.RxState == HAL_UART_STATE_READY) {
         HAL_UART_Receive_DMA(&huart6, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
    }

#elif (BKIT_PHY_INTERFACE == PHY_I2C)
    rx_head = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(hi2c2.hdmarx);

    // [WATCHDOG CẤP CỨU CHO I2C]
    // Nếu I2C báo READY (đã nhận xong gói trước), bật lại ngay
    if(HAL_I2C_GetState(&hi2c2) == HAL_I2C_STATE_READY) {
        //HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
    	HAL_I2C_EnableListen_IT(&hi2c2);
    }
#endif

    if (rx_head == rx_tail) {
        return 0; // Buffer rỗng
    }

    *data = dma_rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
    return 1;
}

// --- 4. HÀM HỖ TRỢ ---
uint32_t hw_get_tick_ms(void) {
    return HAL_GetTick();
}
