/**
 * @file sensor.h
 * @author Pudark
 * @version 0.6
 * @date 2025-11-5
 * @brief 传感器接口
 */

#ifndef __SENSOR_H
#define __SENSOR_H

#include "stm32f10x.h"
#include <stdint.h>

/**
 * @brief 传感器数据结构体
 * @note light_analog：光敏电阻
 * @note light_digital：光敏电阻数字输入
 * @note md1101_freq：TLC555 输出（频率）
 */
typedef struct {
    float light_analog;  // 光敏电阻 AO 模拟值
    uint8_t  light_digital; // 光敏电阻 DO 数字输入
    uint16_t md1101_freq;   // TLC555 输出（频率）
} SensorData_t;

/**
 * @brief DHT11 数据结构体
 * @note hum_int：湿度整数部分
 * @note hum_dec：湿度小数部分
 * @note temp_int：温度整数部分
 * @note temp_dec：温度小数部分
 * @note checksum：校验和
 * @note valid：1=成功 0=失败
 */
typedef struct {
    uint8_t hum_int;     // 湿度整数部分
    uint8_t hum_dec;     // 湿度小数部分
    uint8_t temp_int;    // 温度整数部分
    uint8_t temp_dec;    // 温度小数部分
    uint8_t checksum;    // 校验和
    uint8_t valid;       // 1=成功 0=失败
} DHT11_Data_t;

/**
 * @brief 初始化DHT11
 */
void DHT11_Init(void);
/**
 * @brief 读取DHT11数据
 * @return DHT11数据结构体
 */
DHT11_Data_t DHT11_Read(void);


/**
 * @brief 初始化DS18B20
 */
void DS18B20_Init(void);
/**
 * @brief 读取DS18B20温度
 * @return 温度值，单位摄氏度
 */
float DS18B20_ReadTemp(void);
/**
 * @brief 设置启动温度转换
 */
void DS18B20_StartConvert(void);
/**
 * @brief 读取DS18B20温度值（不等待转换完成）
 * @return 温度值，单位摄氏度
 */
float DS18B20_ReadTempValue(void);

/**
 * @brief 传感器模块化
 */
void Sensor_Init(void);
/**
 * @brief 读取传感器数据
 * @note light_analog：光敏电阻
 * @note light_digital：光敏电阻数字输入
 * @note md1101_freq：TLC555 输出频率
 */
void Sensor_Read(SensorData_t *data);




#endif
