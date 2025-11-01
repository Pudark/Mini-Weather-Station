/**
 * @file ledf.h
 * @author Pudark
 * @version 0.2
 * @date 2025-11-1
 * @brief 通过硬件spi加74hc595控制一个流水灯原件，该原件共用VCC，低电平点亮，自带限流电阻
 */


#ifndef __LEDF_H
#define __LEDF_H

#include "stm32f10x.h"
#include "spi_hw.h"
#include "delay.h"

// === 74HC595 接口定义 ===
// SPI1_SCK  -> PA5
// SPI1_MOSI -> PA7
// LATCH     -> PB0
#define LEDF_GPIO_PORT     GPIOB
#define LEDF_PIN_LATCH     GPIO_Pin_0
#define LEDF_GPIO_CLK      RCC_APB2Periph_GPIOB

// === 函数声明 ===
void LEDF_Init(void);
void LEDF_SetPattern(uint8_t pattern);
void LEDF_On(uint8_t index);
void LEDF_Off(uint8_t index);
void LEDF_AllOn(void);
void LEDF_AllOff(void);
void LEDF_RunningLight(uint16_t delay_time, uint8_t direction);

//整活用的呼吸灯
//void LEDF_Breath(uint8_t index, uint16_t step_ms);

#endif
