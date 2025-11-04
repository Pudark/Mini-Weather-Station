/**
 * @file main.c
 * @author Pudark
 * @version 0.4
 * @date 2025.11.4
 * @brief 调试全局封装和剩余传感器
 * @note MCU:STM32F103C8T6
 * @note 编译器:ARM-GCC
 * @note IDE:VSCode + CMake
 * Copyrigh (c) 2025 Pudark.
 */


#include "stm32f10x.h"
#include "lcd.h"
#include "usart.h"
#include "led.h"
#include "ledf.h"
#include "delay.h"
#include "buzzer.h"
#include "gpio_map.h"
#include "adc.h"
#include "adc_sensor.h"


int main(void)
{
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);

    delay_init(72);
    LCD_Init();
    LEDF_Init();
    LED_Init();
    ADCx_Init();

    GPIO_All_Init();

    USART1_Config();

    delay_ms(100);

    LCD_Fill(0,0,160,128,GREEN);
    LCD_ShowString(0,0,"light:",WHITE,BLUE,16,0);

    SensorData_t sensor;

    while(1)
    {

        Sensor_Read(&sensor);

        LED_Toggle();

        LCD_ShowIntNum(100,16,sensor.light_analog,4,WHITE,GREEN,16);

        LEDF_RunningLight(60,1);
    }

}
