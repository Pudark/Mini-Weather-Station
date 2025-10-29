#ifndef __LCD_H
#define __LCD_H

#include "stm32f10x.h"
#include "spi_hw.h"
#include "delay.h"

void LCD_Init(void);
uint16_t LCD_ReadID(void);

void LCD_WR_REG(uint8_t reg);
void LCD_WR_DATA(uint8_t data);

#endif
