/**
 * @file ledf.c
 * @author Pudark
 * @version 0.2
 * @date 2025-11-1
 * @brief 通过硬件spi加74hc595控制一个流水灯原件，该原件共用VCC，低电平点亮，自带限流电阻
 */


#include "ledf.h"
#include "spi_hw.h"
#include "delay.h"


static uint8_t led_state = 0x00; // 当前LED状态缓存

/**
 * @brief 初始化 LEDF 模块（SPI + LATCH）
 */
void LEDF_Init(void)
{
    // 初始化 SPI1 硬件
    SPI1_HW_Init();

    // 初始化 LATCH 引脚
    RCC_APB2PeriphClockCmd(LEDF_GPIO_CLK, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = LEDF_PIN_LATCH;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LEDF_GPIO_PORT, &GPIO_InitStructure);

    GPIO_ResetBits(LEDF_GPIO_PORT, LEDF_PIN_LATCH);

    LEDF_AllOff();
}

/**
 * @brief 向74HC595发送字节（通过SPI1）
 */
static void LEDF_SendByte(uint8_t data)
{
    SPI1_WriteByte(data);
}

/**
 * @brief 锁存输出（RCLK/STCP 上升沿）
 */
static void LEDF_Latch(void)
{
    GPIO_SetBits(LEDF_GPIO_PORT, LEDF_PIN_LATCH);
    delay_us(1);
    GPIO_ResetBits(LEDF_GPIO_PORT, LEDF_PIN_LATCH);
}

/**
 * @brief 设置8位LED状态（低电平点亮）
 * @param pattern 8位数据，每bit对应一个LED（1=亮，0=灭）
 */
void LEDF_SetPattern(uint8_t pattern)
{
    // 低电平点亮 → 取反
    pattern = ~pattern;

    led_state = pattern;
    LEDF_SendByte(led_state);
    LEDF_Latch();
}

/**
 * @brief 点亮单个LED
 */
void LEDF_On(uint8_t index)
{
    if (index > 7) return;
    uint8_t pattern = (~led_state); // 恢复原始逻辑状态
    pattern |= (1 << index);
    LEDF_SetPattern(pattern);
}

/**
 * @brief 熄灭单个LED
 */
void LEDF_Off(uint8_t index)
{
    if (index > 7) return;
    uint8_t pattern = (~led_state);
    pattern &= ~(1 << index);
    LEDF_SetPattern(pattern);
}

/**
 * @brief 全亮
 */
void LEDF_AllOn(void)
{
    LEDF_SetPattern(0xFF);
}

/**
 * @brief 全灭
 */
void LEDF_AllOff(void)
{
    LEDF_SetPattern(0x00);
}

/**
 * @brief 流水灯效果
 * @param delay_time 每个LED点亮时间（ms）
 * @param direction 0=左移, 1=右移
 */
void LEDF_RunningLight(uint16_t delay_time, uint8_t direction)
{
    if (direction == 0)  // 左移
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            LEDF_SetPattern(1 << (7 - i));
            delay_ms(delay_time);
        }
    }
    else  // 右移
    {
        for (uint8_t i = 0; i < 8; i++)
        {
            LEDF_SetPattern(1 << i);
            delay_ms(delay_time);
        }
    }
}


/**
 * @brief 使用软件PWM实现单灯呼吸效果
 * @param index LED编号 (0~7)
 * @param step_ms 呼吸的速度（步进时间）
 * @attention 警告：此模式严重占用CPU资源，仅娱乐使用，不建议，也最好不要在项目里使用
 */
void LEDF_Breath(uint8_t index, uint16_t step_ms)
{
    if (index > 7) return;
    const uint8_t bitmask = 1 << index;

    // 从暗到亮
    for (uint8_t duty = 0; duty < 100; duty++) {
        for (uint8_t i = 0; i < 100; i++) {
            if (i < duty) {
                LEDF_SetPattern(bitmask);  // 点亮
            } else {
                LEDF_AllOff();              // 熄灭
            }
            delay_us(100); // 控制PWM周期
        }
        delay_ms(step_ms);
    }

    // 从亮到暗
    for (int duty = 100; duty > 0; duty--) {
        for (uint8_t i = 0; i < 100; i++) {
            if (i < duty) {
                LEDF_SetPattern(bitmask);
            } else {
                LEDF_AllOff();
            }
            delay_us(100);
        }
        delay_ms(step_ms);
    }
}


