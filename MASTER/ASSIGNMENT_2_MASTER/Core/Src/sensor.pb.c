/*
 * sensor.pb.c
 *
 *  Created on: Jan 25, 2026
 *      Author: DUONG DEP TRAI
 */
#include "sensor.pb.h"

/* PB_BIND sẽ dựa vào SensorData_FIELDLIST ở file .h
   để tự động tạo ra bảng tra cứu nhị phân */
PB_BIND(SensorData, SensorData, AUTO)
