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
// --- CAC HAM CUA LAYER 1

void hw_init(void) {

#if (BKIT_PHY_INTERFACE == PHY_UART)
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

#elif (BKIT_PHY_INTERFACE == PHY_I2C)
	// [TO DO]
    //SEND BYTE CHO I2C
#endif
}


uint8_t hw_receive_byte(uint8_t* data) {
#if (BKIT_PHY_INTERFACE == PHY_UART)
    // TODO
    return 0;

#elif (BKIT_PHY_INTERFACE == PHY_I2C)
    // TODO
    return 0;
#endif
    return 0;
}
