/*
 * hw_driver.h
 *
 *  Created on: Jan 25, 2026
 *      Author: my pc
 */

#ifndef INC_HW_DRIVER_H_
#define INC_HW_DRIVER_H_

#include <stdint.h>

void hw_init(void);
void hw_send_byte(uint8_t data);
uint8_t hw_receive_byte(uint8_t* data);

#endif /* INC_HW_DRIVER_H_ */
