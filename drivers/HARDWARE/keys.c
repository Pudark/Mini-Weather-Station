/**
 * @file keys.c
 * @author Pudark
 * @version 0.7
 * @date 2025-11-5
 * @brief 按键
 */

#include "keys.h"
#include "gpio_map.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h" // NVIC_Config
#include "stdbool.h"
#include "delay.h"

/* ================= 全局状态变量 ================= */
volatile bool lcd_on = true;           // 干簧管布尔值
volatile KeyEvent key_event = KEY_NONE;

/* ================= 简单防抖宏 ================= */
#define DEBOUNCE_MS 20

/**
 * @brief 中断回调
 */
static void Key_Handle(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
    delay_ms(DEBOUNCE_MS); // 简单防抖
    if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin) == KEY_PRESSED)
    {
        if (GPIO_Pin == KEY1_GPIO_PIN && KEY2_READ() == 1)
            key_event = KEY_PREV_PAGE;
        else if (GPIO_Pin == KEY2_GPIO_PIN && KEY1_READ() == 1)
            key_event = KEY_NEXT_PAGE;
        else if (GPIO_Pin == KEY1_GPIO_PIN && KEY2_READ() == KEY_PRESSED)
            key_event = KEY_BOTH_LONG; // 简单处理：同时按下
    }
    else if (GPIO_Pin == REED_GPIO_PIN)
    {
        lcd_on = !lcd_on; // 翻转 LCD 状态
        key_event = REED_TOGGLE_LCD;
    }
}

/**
 * @brief EXTI中断函数
 */
void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line11) != RESET) // PB11 干簧管
    {
        Key_Handle(REED_GPIO_PORT, REED_GPIO_PIN);
        EXTI_ClearITPendingBit(EXTI_Line11);
    }
    if(EXTI_GetITStatus(EXTI_Line12) != RESET) // PB12 按键1
    {
        Key_Handle(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
        EXTI_ClearITPendingBit(EXTI_Line12);
    }
    if(EXTI_GetITStatus(EXTI_Line14) != RESET) // PB14 按键2
    {
        Key_Handle(KEY2_GPIO_PORT, KEY2_GPIO_PIN);
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}

/**
 * @brief 按键初始化
 */
void Keys_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* --- GPIO 输入 --- */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN | KEY2_GPIO_PIN | REED_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* --- AFIO 连接 EXTI --- */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource12);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);

    /* --- EXTI 配置 --- */
    EXTI_InitStructure.EXTI_Line = EXTI_Line11 | EXTI_Line12 | EXTI_Line14;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 按下低电平
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* --- NVIC 配置 --- */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 读取事件
 */
KeyEvent Keys_GetEvent(void)
{
    KeyEvent tmp = key_event;
    key_event = KEY_NONE; // 取一次就清空
    return tmp;
}

/**
 * @brief 读取干簧管
 */
bool Keys_LCDOn(void)
{
    return lcd_on;
}
