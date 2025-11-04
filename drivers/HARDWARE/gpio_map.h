/**
 * @file gpio_map.h
 * @author Pudark
 * @version 0.3
 * @date 2025-11-2
 * @brief gpio 映射定义
 */


#ifndef __GPIO_MAP_H
#define __GPIO_MAP_H

#include "stm32f10x.h"

/* ======================== 输出类 ======================== */
// 蜂鸣器（PB8） - 低电平触发
#define BUZZER_GPIO_PORT      GPIOB
#define BUZZER_GPIO_PIN       GPIO_Pin_8
#define BUZZER_ON()           GPIO_ResetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)
#define BUZZER_OFF()          GPIO_SetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)
#define BUZZER_TOGGLE()       GPIO_WriteBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, \
                                   (BitAction)(1 - GPIO_ReadOutputDataBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)))

/* ======================== 传感器接口 ======================== */
// DHT11 温湿度传感器（PB9）
#define DHT11_GPIO_PORT       GPIOB
#define DHT11_GPIO_PIN        GPIO_Pin_9

// DS18B20 温度探头（PB10）
#define DS18B20_GPIO_PORT     GPIOB
#define DS18B20_GPIO_PIN      GPIO_Pin_10

// HW611 气压传感器（I2C1：PB6=SCL, PB7=SDA）
#define HW611_I2C_SCL_PORT    GPIOB
#define HW611_I2C_SCL_PIN     GPIO_Pin_6
#define HW611_I2C_SDA_PORT    GPIOB
#define HW611_I2C_SDA_PIN     GPIO_Pin_7

// 光敏电阻（AO: PB0-> ADC1_IN8, DO: PB12 -> 数字输出）
#define LIGHT_AO_ADC_CHANNEL  ADC_Channel_8
#define LIGHT_AO_GPIO_PORT    GPIOA
#define LIGHT_AO_GPIO_PIN     GPIO_Pin_8

#define LIGHT_DO_GPIO_PORT    GPIOB
#define LIGHT_DO_GPIO_PIN     GPIO_Pin_12
#define LIGHT_DO_READ()       GPIO_ReadInputDataBit(LIGHT_DO_GPIO_PORT, LIGHT_DO_GPIO_PIN)

// MD1101 + TLC555 输出（PB1） 测频率
#define HUM555_ADC1_GPIO_PORT GPIOB
#define HUM555_ADC1_GPIO_PIN  GPIO_Pin_1


/* ======================== 输入类 ======================== */
// 干簧管（PB11） → 控制LCD亮灭
#define REED_GPIO_PORT        GPIOB
#define REED_GPIO_PIN         GPIO_Pin_11
#define REED_READ()           GPIO_ReadInputDataBit(REED_GPIO_PORT, REED_GPIO_PIN)

// 按键1（PB12） → 上一页
#define KEY1_GPIO_PORT        GPIOB
#define KEY1_GPIO_PIN         GPIO_Pin_12
#define KEY1_READ()           GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN)

// 按键2（PB13） → 下一页
#define KEY2_GPIO_PORT        GPIOB
#define KEY2_GPIO_PIN         GPIO_Pin_13
#define KEY2_READ()           GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN)

/* ======================== 逻辑定义（便于阅读） ======================== */
#define KEY_PRESSED           0      // 按下时为低电平
#define REED_CLOSED           0      // 干簧管闭合为低电平


void GPIO_All_Init(void);

#endif  // __GPIO_MAP_H
