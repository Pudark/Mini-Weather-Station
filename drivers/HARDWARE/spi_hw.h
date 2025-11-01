/**
 * @file spi_hw.h
 * @author Pudark
 * @version 0.2
 * @date 2025-11-1
 * @brief 硬件spi
 */

#ifndef __SPI_HW_H
#define __SPI_HW_H

#include "stm32f10x.h"

void SPI1_HW_Init(void);
void SPI1_WriteByte(uint8_t data);
uint8_t SPI1_ReadByte(void);

#endif
