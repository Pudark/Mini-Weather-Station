/**
 * @file adc_sensor.c
 * @author Pudark
 * @version 0.4
 * @date 2025-11-4
 * @brief ADC 传感器接口
 */

#include "adc_sensor.h"
#include "gpio_map.h"
#include "adc.h"
#include <stm32f10x_adc.h>

// ===================== 初始化 =====================
void Sensor_Init(void)
{
    ADCx_Init();
}


// ===================== 数据读取 =====================
void Sensor_Read(SensorData_t *data)
{
    // 光敏电阻 AO: PB0 -> ADC_Channel8
    data->light_analog = ADC_Read(ADC_Channel_8);

    // 光敏电阻 DO: PB1
    data->light_digital = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1);

    // TLC555: PA9 -> ADC_Channel0（假设）
    data->timer_555 = ADC_Read(ADC_Channel_0);
}
