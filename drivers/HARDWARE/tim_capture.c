/**
 * @file tim_capture.c
 * @author Pudark
 * @version 0.5
 * @date 2025-11-4
 * @brief 定时器捕获接口
 */

#include "tim_capture.h"
#include <stdint.h>

static volatile float cap_freq = 0;

/**
 * @brief 定时器捕获初始化
 */
void TIM_Capture_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    TIM_BaseInitStructure.TIM_Prescaler = 71;  // 1MHz计时
    TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_BaseInitStructure.TIM_Period = 0xFFFF;
    TIM_TimeBaseInit(TIM3, &TIM_BaseInitStructure);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x03;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    TIM_ITConfig(TIM3, TIM_IT_CC4, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM3, ENABLE);
}

/**
 * @brief 定时器3中断服务程序
 */
void TIM3_IRQHandler(void)
{
    static uint16_t cap_val1 = 0;
    static uint16_t cap_val2 = 0;
    static uint8_t  flag = 0;

    if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);

        if (flag == 0)
        {
            cap_val1 = TIM_GetCapture4(TIM3);
            flag = 1;
        }
        else
        {
            cap_val2 = TIM_GetCapture4(TIM3);
            uint16_t diff;
            if (cap_val2 > cap_val1)
                diff = cap_val2 - cap_val1;
            else
                diff = 0xFFFF - cap_val1 + cap_val2;

            cap_freq = 1000000.0f / diff;  // 单位 Hz
            flag = 0;
        }
    }
}

/**
 * @brief 获取捕获频率
 * @return 频率，单位 Hz
 */
float TIM_GetFrequency(void)
{
    return cap_freq;
}
