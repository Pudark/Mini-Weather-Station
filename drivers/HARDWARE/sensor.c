/**
 * @file sensor.c
 * @author Pudark
 * @version 0.5
 * @date 2025-11-4
 * @brief 传感器接口
 */

#include "sensor.h"
#include "gpio_map.h"
#include "adc.h"
#include "tim_capture.h"
#include <stm32f10x_adc.h>
#include "delay.h"


/**
 * @brief 传感器模块化
 */
void Sensor_Init(void)
{
    ADCx_Init();
    TIM_Capture_Init();
}


/**
 * @brief 读取传感器数据
 * @note light_analog：光敏电阻
 * @note light_digital：光敏电阻数字输入
 * @note md1101_freq：TLC555 输出频率
 */
void Sensor_Read(SensorData_t *data)
{
    // 光敏电阻 AO: PB0 -> ADC_Channel8
    data->light_analog = ADC_Read(ADC_Channel_8);

    // 光敏电阻 DO: PB1
    data->light_digital = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);

    // TLC555: PA9 -> ADC_Channel0（假设）
    data->md1101_freq = TIM_GetFrequency();
}

// ======================= DHT11 =======================

/**
 * @brief 设置为输出
 */
static void DHT11_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief 设置为输入
 */
static void DHT11_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    GPIO_Init(DHT11_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief 读取数据的一位比特
 * @return 0 或 1
 */
static uint8_t DHT11_ReadBit(void)
{
    while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == Bit_RESET); // 等待低变高
    delay_us(40);
    if (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == Bit_SET)
        return 1;
    else
        return 0;
}

/**
 * @brief 读取数据的一个字节
 * @return 读取的字节
 */
static uint8_t DHT11_ReadByte(void)
{
    uint8_t i, data = 0;
    for (i = 0; i < 8; i++)
    {
        while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == Bit_RESET);
        delay_us(40);
        if (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN))
        {
            data |= (1 << (7 - i));
            while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN));
        }
    }
    return data;
}

/**
 * @brief 初始化DHT11
 */
void DHT11_Init(void)
{
    DHT11_IO_OUT();
    GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
}

/**
 * @brief 读取DHT11数据
 * @return DHT11数据结构体
 */
DHT11_Data_t DHT11_Read(void)//读取DHT11数据
{
    DHT11_Data_t data = {0};

    DHT11_IO_OUT();
    GPIO_ResetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    delay_ms(20);
    GPIO_SetBits(DHT11_GPIO_PORT, DHT11_GPIO_PIN);
    delay_us(30);
    DHT11_IO_IN();

    if (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == Bit_RESET)
    {
        while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == Bit_RESET);
        while (GPIO_ReadInputDataBit(DHT11_GPIO_PORT, DHT11_GPIO_PIN) == Bit_SET);

        data.hum_int = DHT11_ReadByte();
        data.hum_dec = DHT11_ReadByte();
        data.temp_int = DHT11_ReadByte();
        data.temp_dec = DHT11_ReadByte();
        data.checksum = DHT11_ReadByte();

        uint8_t sum = data.hum_int + data.hum_dec + data.temp_int + data.temp_dec;
        data.valid = (sum == data.checksum);
    }
    else
    {
        data.valid = 0;
    }

    return data;
}

// ======================= DS18B20 =======================

/**
 * @brief 设置为输出
 */
static void DS18B20_IO_OUT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DS18B20_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(DS18B20_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief 设置为输入
 */
static void DS18B20_IO_IN(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = DS18B20_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(DS18B20_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief 复位DS18B20
 * @return 1=存在 0=不存在
 */
static uint8_t DS18B20_Reset(void)
{
    uint8_t res = 0;
    DS18B20_IO_OUT();
    GPIO_ResetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    delay_us(750);
    GPIO_SetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    delay_us(15);
    DS18B20_IO_IN();
    if (!GPIO_ReadInputDataBit(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN))
        res = 1;
    else
        res = 0;
    delay_us(480);
    return res;
}

/**
 * @brief 写入一位比特
 * @param bit 要写入的比特（0或1）
 */
static void DS18B20_WriteBit(uint8_t bit)
{
    DS18B20_IO_OUT();
    GPIO_ResetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    if (bit)
    {
        delay_us(10);
        GPIO_SetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
        delay_us(55);
    }
    else
    {
        delay_us(65);
        GPIO_SetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
        delay_us(5);
    }
}

/**
 * @brief 读取一位比特
 * @return 读取的比特（0或1）
 */
static uint8_t DS18B20_ReadBit(void)
{
    uint8_t bit = 0;
    DS18B20_IO_OUT();
    GPIO_ResetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    delay_us(3);
    GPIO_SetBits(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    DS18B20_IO_IN();
    delay_us(10);
    bit = GPIO_ReadInputDataBit(DS18B20_GPIO_PORT, DS18B20_GPIO_PIN);
    delay_us(53);
    return bit;
}

/**
 * @brief 写入一个字节
 * @param byte 要写入的字节
 */
static void DS18B20_WriteByte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
        DS18B20_WriteBit(byte >> i & 0x01);
}

/**
 * @brief 读取一个字节
 * @return 读取的字节
 */
static uint8_t DS18B20_ReadByte(void)
{
    uint8_t byte = 0;
    for (uint8_t i = 0; i < 8; i++)
        byte |= DS18B20_ReadBit() << i;
    return byte;
}

/**
 * @brief 初始化DS18B20
 */
void DS18B20_Init(void)
{
    DS18B20_Reset();
}

/**
 * @brief 读取DS18B20温度
 * @return 温度值，单位摄氏度
 */
float DS18B20_ReadTemp(void)
{
    uint8_t temp_l, temp_h;
    uint16_t temp;
    float value;

    DS18B20_Reset();
    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0x44);
    delay_ms(750);

    DS18B20_Reset();
    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0xBE);

    temp_l = DS18B20_ReadByte();
    temp_h = DS18B20_ReadByte();

    temp = (temp_h << 8) | temp_l;
    value = (float)temp / 16.0f;
    return value;
}

/**
 * @brief 设置启动温度转换
 */
void DS18B20_StartConvert(void)
{
    DS18B20_Reset();
    DS18B20_WriteByte(0xCC); // 跳过ROM匹配
    DS18B20_WriteByte(0x44); // 启动温度转换
}

/**
 * @brief 返回温度值
 * @return 温度12位浮点数值
 */
float DS18B20_ReadTempValue(void)
{
    uint8_t temp_l, temp_h;
    uint16_t temp;
    float value;

    DS18B20_Reset();
    DS18B20_WriteByte(0xCC);
    DS18B20_WriteByte(0xBE);

    temp_l = DS18B20_ReadByte();
    temp_h = DS18B20_ReadByte();

    temp = (temp_h << 8) | temp_l;
    value = (float)temp / 16.0f;

    return value;
}

