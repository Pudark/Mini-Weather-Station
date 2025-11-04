/**
 * @file adc.h
 * @author Pudark
 * @version 0.4
 * @date 2025-11-4
 * @brief ADC 驱动
 */

#ifndef __ADC_H
#define __ADC_H

#include "stm32f10x.h"
#include <stdint.h>

void ADCx_Init(void);
uint16_t ADC_Read(uint8_t channel);

#endif
