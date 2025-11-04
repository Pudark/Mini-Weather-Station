/**
 * @file buzzer.c
 * @author Pudark
 * @version 0.3
 * @date 2025-11-2
 * @brief 蜂鸣器驱动
 */

#include "buzzer.h"
#include "delay.h"
#include "gpio_map.h"
#include <stdint.h>

/**
 * @brief 蜂鸣器告警函数
 * @param ms_on  每次鸣叫时间（ms）
 * @param ms_off 每次间隔时间（ms）
 * @param times  鸣叫次数
 */
void BUZZER_Alert(uint16_t ms_on, uint16_t ms_off, uint8_t times)
{
    for(uint8_t i = 0; i < times; i++)
    {
        BUZZER_ON();
        delay_ms(ms_on);
        BUZZER_OFF();
        delay_ms(ms_off);
    }
}

/**
 * @brief 预设蜂鸣器模式
 * @param mode 蜂鸣器模式
 */

 void Buzzer_SetMode(BuzzerMode_t mode)
 {
    switch(mode)
    {
        case BUZZER_MODE_OFF:
            BUZZER_OFF();
            break;
        case BUZZER_MODE_SHORT:
            BUZZER_Alert(30, 20, 1);
            break;
        case BUZZER_MODE_DOUBLE:
            BUZZER_Alert(30, 20, 2);
            break;
        case BUZZER_MODE_ALARM:
            BUZZER_Alert(10, 10, 5);
            break;
        case BUZZER_MODE_ERROR:
            BUZZER_Alert(200, 200, 5);
            break;
        default:
            break;
    }
 }