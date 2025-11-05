/**
 * @file i2c_hw.c
 * @author Pudark
 * @version 0.6
 * @date 2025-11-5
 * @brief I2C
 */


#include "i2c_hw.h"
#include <stdint.h>
#include "led.h"

/**
 * @brief 初始化 I2C 硬件
 */
void I2C_HW_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef I2C_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    // PB6 -> SCL, PB7 -> SDA
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    I2C_DeInit(I2C1);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;  // 100kHz
    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}

/**
 * @brief 写入 1 字节
 * @param devAddr 设备地址
 * @param regAddr 寄存器地址
 * @param data 要写入的数据
 */
void I2C_HW_Write(uint8_t devAddr, uint8_t regAddr, uint8_t data)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));

    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    I2C_SendData(I2C1, regAddr);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTING));

    I2C_SendData(I2C1, data);
    while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    I2C_GenerateSTOP(I2C1, ENABLE);
}

/**
 * @brief 读取 1 字节
 * @param devAddr 设备地址
 * @param regAddr 寄存器地址
 * @return 读取的数据
 */
uint8_t I2C_HW_Read(uint8_t dev_addr, uint8_t regAddr)
{
    uint8_t data = 0;

    //起始信号
    I2C_GenerateSTART(I2C1, ENABLE);
    if (!I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) return 0;

    //发送设备地址 + 写
    I2C_Send7bitAddress(I2C1, dev_addr << 1, I2C_Direction_Transmitter);
    if (!I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) return 0;

    //发送寄存器地址
    I2C_SendData(I2C1, regAddr);
    if (!I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED)) return 0;

    //重新起始信号（重复开始）
    I2C_GenerateSTART(I2C1, ENABLE);
    if (!I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) return 0;

    //发送设备地址 + 读
    I2C_Send7bitAddress(I2C1, dev_addr << 1, I2C_Direction_Receiver);
    if (!I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) return 0;

    //关闭ACK（只读一个字节）
    I2C_AcknowledgeConfig(I2C1, DISABLE);

    //生成停止信号
    I2C_GenerateSTOP(I2C1, ENABLE);

    //等待接收完成
    if (!I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)) return 0;

    //读取数据
    data = I2C_ReceiveData(I2C1);

    //重新打开ACK，供下次读用
    I2C_AcknowledgeConfig(I2C1, ENABLE);

    return data;
}

/**
 * @brief 防超时系统
 * @param I2Cx I2C通道
 * @param event 事件
 * @return 0/1（超时/成功）
 */
static uint8_t I2C_WaitEvent(I2C_TypeDef* I2Cx, uint32_t event)
{
    uint32_t timeout = 0;
    while (!I2C_CheckEvent(I2Cx, event))
    {
        if (++timeout > I2C_TIMEOUT)
            return 0; // 超时
    }
    return 1; // 成功
}

uint8_t I2C_DeviceList[10];  // 最多记录10个设备地址
uint8_t I2C_DeviceCount = 0; // 实际找到的数量

/**
 * @brief 扫描I2C总线上的设备
 */
void I2C_Scan_Bus(void)
{
    I2C_DeviceCount = 0; // 清空计数
    for (uint8_t addr = 1; addr < 255; addr++)
    {
        if (I2C_HW_CheckDev(addr))
        {
            LED_Toggle();
            if (I2C_DeviceCount < 10)
                I2C_DeviceList[I2C_DeviceCount++] = addr;
        }
    }
}

/**
 * @brief 检测I2C设备是否存在
 * @param devAddr 设备地址
 * @return 1=存在 0=不存在
 */
uint8_t I2C_HW_CheckDev(uint8_t devAddr)
{
    while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));

    I2C_GenerateSTART(I2C1, ENABLE);
    if (!I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT)) return 0;

    I2C_Send7bitAddress(I2C1, devAddr, I2C_Direction_Transmitter);
    if (I2C_WaitEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        I2C_GenerateSTOP(I2C1, ENABLE);
        return 1; // 设备存在
    }
    else
    {
        I2C_GenerateSTOP(I2C1, ENABLE);
        return 0; // 设备不存在
    }
}

