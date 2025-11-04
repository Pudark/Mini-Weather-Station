/**
 * @file tim_capture.h
 * @author Pudark
 * @version 0.5
 * @date 2025-11-4
 * @brief 定时器捕获接口
 */

#ifndef __TIM_CAPTURE_H
#define __TIM_CAPTURE_H

#include "stm32f10x.h"

void TIM_Capture_Init(void);
float TIM_GetFrequency(void);

#endif
