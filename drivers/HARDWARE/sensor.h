/**
 * @file sensor.h
 * @author Pudark
 * @version 0.5
 * @date 2025-11-4
 * @brief 传感器接口
 */

#ifndef __SENSOR_H
#define __SENSOR_H

#include "stm32f10x.h"
#include <stdint.h>

// 传感器数据结构体
typedef struct {
    uint16_t light_analog;  // 光敏电阻 AO 模拟值
    uint8_t  light_digital; // 光敏电阻 DO 数字输入
    uint16_t md1101_freq;   // TLC555 输出（频率）
} SensorData_t;

// DHT11 数据结构体
typedef struct {
    uint8_t hum_int;     // 湿度整数部分
    uint8_t hum_dec;     // 湿度小数部分
    uint8_t temp_int;    // 温度整数部分
    uint8_t temp_dec;    // 温度小数部分
    uint8_t checksum;    // 校验和
    uint8_t valid;       // 1=成功 0=失败
} DHT11_Data_t;

void DHT11_Init(void);
DHT11_Data_t DHT11_Read(void);

void DS18B20_Init(void);
float DS18B20_ReadTemp(void);
void DS18B20_StartConvert(void);
float DS18B20_ReadTempValue(void);


void Sensor_Init(void);
void Sensor_Read(SensorData_t *data);

#endif
