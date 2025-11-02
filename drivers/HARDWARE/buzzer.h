/**
 * @file buzzer.h
 * @author Pudark
 * @version 0.3
 * @date 2025-11-2
 * @brief 蜂鸣器驱动
 */

#ifndef __BUZZER_H
#define __BUZZER_H

#include "stm32f10x.h"

// === 蜂鸣器GPIO配置 ===
#define BUZZER_GPIO_PORT    GPIOB
#define BUZZER_GPIO_PIN     GPIO_Pin_0
#define BUZZER_ON()         GPIO_ResetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)
#define BUZZER_OFF()        GPIO_SetBits(BUZZER_GPIO_PORT, BUZZER_GPIO_PIN)

// === 蜂鸣器模式 ===
typedef enum {
    BUZZER_MODE_OFF = 0,     // 关闭
    BUZZER_MODE_SHORT,       // 单次短鸣
    BUZZER_MODE_DOUBLE,      // 双短鸣
    BUZZER_MODE_ALARM,       // 快速连续鸣（报警）
    BUZZER_MODE_ERROR        // 慢速循环鸣（错误）
} BuzzerMode_t;

void Buzzer_Init(void);
void Buzzer_SetMode(BuzzerMode_t mode);
void Buzzer_Task(void);

#endif
