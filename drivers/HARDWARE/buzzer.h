/**
 * @file buzzer.h
 * @author Pudark
 * @version 0.3
 * @date 2025-11-2
 * @brief 蜂鸣器驱动
 */

#include <stdint.h>

#ifndef __BUZZER_H
#define __BUZZER_H

// === 蜂鸣器模式 ===
typedef enum {
    BUZZER_MODE_OFF = 0,     // 关闭
    BUZZER_MODE_SHORT,       // 单次短鸣
    BUZZER_MODE_DOUBLE,      // 双短鸣
    BUZZER_MODE_ALARM,       // 快速连续鸣（报警）
    BUZZER_MODE_ERROR        // 慢速循环鸣（错误）
} BuzzerMode_t;


void BUZZER_Alert(uint16_t ms_on, uint16_t ms_off, uint8_t times);
 void Buzzer_SetMode(BuzzerMode_t mode);

#endif
