/*
 * bkit_app.h
 *
 *  Created on: Jan 27, 2026
 *      Author: DUONG DEP TRAI
 */

#ifndef INC_BKIT_APP_H_
#define INC_BKIT_APP_H_

#include "sensor.pb.h" // DUNG STRUCT TU NANOPB
#include <stdbool.h>

// DINH NGHIA LAI TEN STRUCT
typedef SensorData sensor_data_t;

// CAC API CAP CAO
bool bkit_send_message(const sensor_data_t* data);
bool bkit_receive_message(sensor_data_t* data);

#endif
