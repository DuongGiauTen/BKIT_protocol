/*
 * sensor.pb.h
 *
 *  Created on: Jan 25, 2026
 *      Author: DUONG DEP TRAI
 */

#ifndef INC_SENSOR_PB_H_
#define INC_SENSOR_PB_H_

#include "pb.h"

#ifdef __cplusplus
extern "C" {
#endif

/* CAU TRUC DU LIEU CHINH*/
typedef struct _SensorData {
    uint32_t sensor_id;
    float temperature;
    uint32_t timestamp;
} SensorData;


// Field numbers
#define SensorData_sensor_id_tag   1
#define SensorData_temperature_tag 2
#define SensorData_timestamp_tag   3

// Struct descriptor
#define SensorData_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   sensor_id,         1) \
X(a, STATIC,   SINGULAR, FLOAT,    temperature,       2) \
X(a, STATIC,   SINGULAR, UINT32,   timestamp,         3)

#define SensorData_CALLBACK NULL
#define SensorData_DEFAULT NULL


#define SensorData_init_zero {0, 0, 0}

extern const pb_msgdesc_t SensorData_msg;
#define SensorData_fields &SensorData_msg

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
