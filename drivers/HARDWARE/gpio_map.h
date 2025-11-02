#ifndef __GPIO_MAP_H
#define __GPIO_MAP_H

#include "stm32f10x.h"

/* ======================== 输出类 ======================== */
// 蜂鸣器（PB1）
#define BUZZER_GPIO_PORT      GPIOB
#define BUZZER_GPIO_PIN       GPIO_Pin_1
#define BUZZER_ON()           GPIO_SetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)
#define BUZZER_OFF()          GPIO_ResetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)
#define BUZZER_TOGGLE()       GPIO_WriteBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN, \
                                   (BitAction)(1 - GPIO_ReadOutputDataBit(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)))

/* ======================== 传感器接口 ======================== */
// DHT11 温湿度传感器（PA6）
#define DHT11_GPIO_PORT       GPIOA
#define DHT11_GPIO_PIN        GPIO_Pin_6

// DS18B20 温度探头（PB2）
#define DS18B20_GPIO_PORT     GPIOB
#define DS18B20_GPIO_PIN      GPIO_Pin_2

// HW611 气压传感器 ADC 输入（PA8 -> ADC1_CH8）
#define HW611_ADC_CHANNEL     ADC_Channel_8
#define HW611_ADC_GPIO_PORT   GPIOA
#define HW611_ADC_GPIO_PIN    GPIO_Pin_8

// 光敏电阻 ADC 输入（PA9 -> ADC1_CH9）
#define LIGHT_ADC_CHANNEL     ADC_Channel_9
#define LIGHT_ADC_GPIO_PORT   GPIOA
#define LIGHT_ADC_GPIO_PIN    GPIO_Pin_9

// MD1101 + TLC555 输出 ADC 输入（PA10 -> ADC1_CH10）
#define HUM555_ADC_CHANNEL    ADC_Channel_10
#define HUM555_ADC_GPIO_PORT  GPIOA
#define HUM555_ADC_GPIO_PIN   GPIO_Pin_10

/* ======================== 输入类 ======================== */
// 干簧管（PB10） → 检测磁场开关状态
#define REED_GPIO_PORT        GPIOB
#define REED_GPIO_PIN         GPIO_Pin_10
#define REED_READ()           GPIO_ReadInputDataBit(REED_GPIO_PORT, REED_GPIO_PIN)

// 按键1（PB11） → 上一页
#define KEY1_GPIO_PORT        GPIOB
#define KEY1_GPIO_PIN         GPIO_Pin_11
#define KEY1_READ()           GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN)

// 按键2（PB12） → 下一页
#define KEY2_GPIO_PORT        GPIOB
#define KEY2_GPIO_PIN         GPIO_Pin_12
#define KEY2_READ()           GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN)

/* ======================== 逻辑定义（便于阅读） ======================== */
#define KEY_PRESSED           0      // 按下时为低电平
#define REED_CLOSED           0      // 干簧管闭合为低电平

#endif  // __GPIO_MAP_H
