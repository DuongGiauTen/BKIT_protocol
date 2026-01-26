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

#define SLAVE_ADDR_I2C  (0x30 << 1) //CAU HINH DIA CHI SLAVE I2C : 0x30

#define HW_TIMEOUT 100


// --- CAC HAM CUA LAYER 1

void hw_init(void) {

#if (BKIT_PHY_INTERFACE == PHY_UART) //
	// [TO DO]
   // DOAN NAY KHOI TAO UART
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

// HAM MASTER NHAN 1 BYTE VE
uint8_t hw_receive_byte(uint8_t* data) {
#if (BKIT_PHY_INTERFACE == PHY_UART)
    // TODO
	if (HAL_UART_Receive(&huart6, data, 1, HW_TIMEOUT) == HAL_OK){
		return 1;//success
	}
    return 0;//fail

#elif (BKIT_PHY_INTERFACE == PHY_I2C)
    // TODO
    // MASTER YEU CAU DOC DU LIEU TU SLAVE
    if (HAL_I2C_Master_Receive(&hi2c2, SLAVE_ADDR_I2C, data, 1, HW_TIMEOUT) == HAL_OK){
    	return 1;
    }
    return 0;
#endif
    return 0;
}
