/**
 * @file adc_sensor.h
 * @author Pudark
 * @version 0.4
 * @date 2025-11-4
 * @brief ADC 传感器接口
 */

 #ifndef __ADC_SENSOR_H
#define __ADC_SENSOR_H

#include "stm32f10x.h"
#include <stdint.h>

// 传感器数据结构体
typedef struct {
    uint16_t light_analog;  // 光敏电阻 AO 模拟值
    uint8_t  light_digital; // 光敏电阻 DO 数字输入
    uint16_t timer_555;     // TLC555 输出（电压）
} SensorData_t;

void Sensor_Init(void);
void Sensor_Read(SensorData_t *data);

#endif
