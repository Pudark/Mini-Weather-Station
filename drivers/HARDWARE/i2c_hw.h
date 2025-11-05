/**
 * @file i2c_hw.h
 * @author Pudark
 * @version 0.6
 * @date 2025-11-5
 * @brief I2C
 */


#ifndef __I2C_HW_H
#define __I2C_HW_H

#include "stm32f10x.h"
#include <stdint.h>

#define I2C_TIMEOUT  5000 // 超时循环次数上限
extern uint8_t I2C_DeviceList[10];
extern uint8_t I2C_DeviceCount;

void I2C_HW_Init(void);
void I2C_HW_Write(uint8_t devAddr, uint8_t regAddr, uint8_t data);
uint8_t I2C_HW_Read(uint8_t devAddr, uint8_t regAddr);
static uint8_t I2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t event);
void I2C_Scan_Bus(void);
uint8_t I2C_HW_CheckDev(uint8_t devAddr);

#endif
