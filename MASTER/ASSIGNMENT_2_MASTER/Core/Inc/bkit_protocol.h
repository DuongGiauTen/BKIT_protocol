/*
 * bkit_protocol.h
 *
 *  Created on: Jan 25, 2026
 *      Author: my pc
 */

#ifndef INC_BKIT_PROTOCOL_H_
#define INC_BKIT_PROTOCOL_H_

#include <stdint.h>
#include <stdbool.h>
#include "sensor.pb.h"

#define BKIT_START_BYTE 0x7E

// CAU TRUC HEADER
typedef struct __attribute__((packed)) {
    uint8_t  start_of_packet; // Offset 0: 0x7E
    uint16_t packet_length;   // Offset 1-2: Độ dài payload (Little-endian)
    uint16_t crc16;           // Offset 3-4: CRC-16 của payload
    uint8_t  flags;           // Offset 5: Cờ tùy chọn
} bkit_header_t;

// API CHO LAYER 3
bool bkit_protocol_send(const SensorData *data);
bool bkit_protocol_receive(SensorData *data);


#endif /* INC_BKIT_PROTOCOL_H_ */

