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

extern UART_HandleTypeDef huart6;
extern I2C_HandleTypeDef hi2c2;

// CAU HINH RING BUFFER
#define RX_BUFFER_SIZE 256

volatile uint8_t dma_rx_buffer[RX_BUFFER_SIZE];
volatile uint16_t rx_tail = 0;

#define HW_TIMEOUT 1000
// --- CAC HAM CUA LAYER 1

void hw_init(void) {

#if (BKIT_PHY_INTERFACE == PHY_UART)
	// [TO DO]
	// UART DMA Circular
	HAL_UART_Receive_DMA(&huart6, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);

#elif (BKIT_PHY_INTERFACE == PHY_I2C)
	// [TO DO]
	HAL_I2C_Slave_Receive_DMA(&hi2c2, (uint8_t*)dma_rx_buffer, RX_BUFFER_SIZE);
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
    HAL_I2C_Slave_Transmit(&hi2c2, &data, 1, HW_TIMEOUT);
#endif
}

// HAM NHAN DUNG DMA

uint8_t hw_receive_byte(uint8_t* data) {
    uint16_t rx_head = 0;

#if (BKIT_PHY_INTERFACE == PHY_UART)
    rx_head = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart6.hdmarx);

#elif (BKIT_PHY_INTERFACE == PHY_I2C)
    rx_head = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(hi2c2.hdmarx);
#endif

    // LOGIC RING BUFFER CHUNG
    if (rx_head == rx_tail) {
        return 0; // buffer rong, chua co du lieu moi
    }

    *data = dma_rx_buffer[rx_tail];
    rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;

    return 1; // da lay duoc 1 byte
}

uint32_t hw_get_tick_ms(void) {
    return HAL_GetTick();
}

