/*
 * hw_driver.c
 *
 *  Created on: Jan 25, 2026
 *      Author: DUONG DEP TRAI
 */
#include "hw_driver.h"
#include "bkit_config.h"
#include "main.h"
#include "usart.h"
#include "i2c.h"

// KHAI BAO BIEN NGOAI VI
extern UART_HandleTypeDef huart6; // PC6, PC7 LA USART6
extern I2C_HandleTypeDef hi2c2; // PB10, PB11 LA I2C2

// --- CẤU HÌNH RING BUFFER (CHỈ DÀNH CHO UART) ---
// Vì Master I2C phải chủ động đọc, không dùng Ring Buffer thụ động được.
#define RX_BUFFER_SIZE 256
volatile uint8_t master_rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t rx_tail = 0;

#define SLAVE_ADDR_I2C  (0x30 << 1) //CAU HINH DIA CHI SLAVE I2C : 0x30

#define HW_TIMEOUT 100


// --- CAC HAM CUA LAYER 1

void hw_init(void) {

#if (BKIT_PHY_INTERFACE == PHY_UART) //
	// [TO DO]
	HAL_UART_Receive_DMA(&huart6, (uint8_t*)master_rx_buffer, RX_BUFFER_SIZE);
#elif (BKIT_PHY_INTERFACE == PHY_I2C)
	// [TO DO]
    // DOAN NAY KHOI TAO I2C
#endif
}

// --- HÀM GỬI 1 BYTE ---
void hw_send_byte(uint8_t data) {
#if (BKIT_PHY_INTERFACE == PHY_UART)
	// [TO DO]
    // SEND BYTE CHO UART
	HAL_UART_Transmit(&huart6, &data, 1, HW_TIMEOUT);

#elif (BKIT_PHY_INTERFACE == PHY_I2C)
	// [TO DO]
    //SEND BYTE CHO I2C
	HAL_I2C_Master_Transmit(&hi2c2, SLAVE_ADDR_I2C, &data, 1, HW_TIMEOUT);
#endif
}

// --- 3. HÀM NHẬN 1 BYTE (RX) ---
uint8_t hw_receive_byte(uint8_t* data) {
#if (BKIT_PHY_INTERFACE == PHY_UART)
    // [UART MASTER] UPGRADE: Dùng Ring Buffer DMA (Non-blocking)

    uint16_t rx_head = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart6.hdmarx);

    if (rx_head == rx_tail) {
        return 0; // Buffer rỗng
    }

    *data = master_rx_buffer[rx_tail];

    rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;

    return 1; // Thành công

#elif (BKIT_PHY_INTERFACE == PHY_I2C)
    // [I2C MASTER]
    // Không thể dùng Ring Buffer thụ động.

    if (HAL_I2C_Master_Receive(&hi2c2, SLAVE_ADDR_I2C, data, 1, 10) == HAL_OK) {
        return 1;
    }
    return 0;
#endif
    return 0;
}

// --- 4. HÀM THỜI GIAN ---
uint32_t hw_get_tick_ms(void) {
    return HAL_GetTick();
}
