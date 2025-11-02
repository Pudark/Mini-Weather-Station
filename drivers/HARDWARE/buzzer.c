/**
 * @file buzzer.c
 * @author Pudark
 * @version 0.3
 * @date 2025-11-2
 * @brief 蜂鸣器驱动
 */

#include "buzzer.h"
#include "sys.h"

// 外部定义的全局ms计时变量（在SysTick中断中递增）
extern volatile uint32_t sys_ms;
#define millis() sys_ms

static BuzzerMode_t buzzer_mode = BUZZER_MODE_OFF;
static uint32_t buzzer_timer = 0;
static uint8_t buzzer_step = 0;

void Buzzer_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = BUZZER_GPIO_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(BUZZER_GPIO_PORT, &GPIO_InitStructure);

    BUZZER_OFF();
}

void Buzzer_SetMode(BuzzerMode_t mode)
{
    buzzer_mode = mode;
    buzzer_step = 0;
    buzzer_timer = millis();
}

/**
 * @brief 非阻塞蜂鸣器任务（在主循环中周期调用）
 */
void Buzzer_Task(void)
{
    uint32_t now = millis();

    switch (buzzer_mode)
    {
        case BUZZER_MODE_OFF:
            BUZZER_OFF();
            break;

        case BUZZER_MODE_SHORT:
            if (buzzer_step == 0) {
                BUZZER_ON();
                buzzer_timer = now + 100;
                buzzer_step = 1;
            } else if (now > buzzer_timer) {
                BUZZER_OFF();
                buzzer_mode = BUZZER_MODE_OFF;
            }
            break;

        case BUZZER_MODE_DOUBLE:
            if (buzzer_step == 0) {
                BUZZER_ON();
                buzzer_timer = now + 100;
                buzzer_step = 1;
            } else if (buzzer_step == 1 && now > buzzer_timer) {
                BUZZER_OFF();
                buzzer_timer = now + 100;
                buzzer_step = 2;
            } else if (buzzer_step == 2 && now > buzzer_timer) {
                BUZZER_ON();
                buzzer_timer = now + 100;
                buzzer_step = 3;
            } else if (buzzer_step == 3 && now > buzzer_timer) {
                BUZZER_OFF();
                buzzer_mode = BUZZER_MODE_OFF;
            }
            break;

        case BUZZER_MODE_ALARM:
            if (now > buzzer_timer) {
                static uint8_t toggle = 0;
                toggle = !toggle;
                if (toggle) BUZZER_ON(); else BUZZER_OFF();
                buzzer_timer = now + 150;
            }
            break;

        case BUZZER_MODE_ERROR:
            if (now > buzzer_timer) {
                static uint8_t toggle = 0;
                toggle = !toggle;
                if (toggle) BUZZER_ON(); else BUZZER_OFF();
                buzzer_timer = now + 500;
            }
            break;
    }
}
