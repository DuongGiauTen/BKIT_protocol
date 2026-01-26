/*
 * app_main.c
 *
 *  Created on: Jan 25, 2026
 *      Author: DUONG DEP TRAI
 *
 *
 */
#include "bkit_app.h"
#include "bkit_protocol.h"
#include <stdio.h>
#include "lcd.h"
/* * HAM GUI: sensor_data_t -> Protobuf Encode -> Header/CRC -> Hardware */
bool bkit_send_message(const sensor_data_t* data) {
    // GOI XUONG LAYER 2
    if (bkit_protocol_send(data)) {
        // THEM LOG GUI THANH CONG O DAY
    	lcd_show_string(10, 120, "GUI THANH CONG", GREEN, WHITE, 16, 0);
        return true;
    } else {
        // XU LY LOI GUI
    	lcd_show_string(10, 120, "GUI THAT BAI", RED, WHITE, 16, 0);
    	// THEM LOG GUI THAT BAI O DAY
        return false;
    }
}

/* * HAM NHAN: Hardware -> Parse Header -> Check CRC -> Protobuf Decode */
bool bkit_receive_message(sensor_data_t* data) {
    //GOI XUONG LAYER 2
    if (bkit_protocol_receive(data)) {
        // NEU NHAN THANH CONG VA CRC KHOP

        return true;
    } else {
        /* XU LY LOI VA GHI LOG */
        // IN RA LOI "INVALID CRC" OR "CRC FAILED" O DAY

        return false;
    }
}

