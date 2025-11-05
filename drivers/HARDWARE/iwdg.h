/**
 * @file iwdg.h
 * @author Pudark
 * @version 0.7
 * @date 2025-11-5
 * @brief 看门狗
 */

#ifndef __IWDG_H__
#define __IWDG_H__

#include "stm32f10x.h"

void IWDG_Init_2s(void);
void IWDG_Feed(void);

#endif
