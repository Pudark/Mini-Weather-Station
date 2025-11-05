/**
 * @file iwdg.c
 * @author Pudark
 * @version 0.7
 * @date 2025-11-5
 * @brief 看门狗
 */

#include "stm32f10x.h"
#include "iwdg.h"

/* ======================== 配置 ======================== */
#define IWDG_PRESCALER   IWDG_Prescaler_32   // 计数频率 ~ 1.25 kHz
#define IWDG_RELOAD      2500                 // 2500 / 1250 Hz ≈ 2 秒

/**
 * @brief 启动两秒的看门狗
 */
void IWDG_Init_2s(void)
{
    RCC_LSICmd(ENABLE);                             // 使能 LSI
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);   // 允许写访问
    IWDG_SetPrescaler(IWDG_Prescaler_32);           // 设置预分频
    IWDG_SetReload(IWDG_RELOAD);                    // 设置重装载
    IWDG_ReloadCounter();                            // 装载计数器
    IWDG_Enable();                                  // 启动看门狗
}

/**
 * @brief 喂狗
 */
void IWDG_Feed(void)
{
    IWDG_ReloadCounter();                            // 喂狗
}
