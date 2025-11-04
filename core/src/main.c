/**
 * @file main.c
 * @author Pudark
 * @version 0.5
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
#include "sensor.h"


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
    Sensor_Init();
    DHT11_Init();
    DS18B20_Init();

    USART1_Config();

    delay_ms(100);

    LCD_Fill(0,0,160,128,GREEN);
    LCD_ShowString(0,0,"light:",WHITE,BLUE,16,0);

    SensorData_t sensor;
    DHT11_Data_t dht;
    float ds18_temp;


    while(1)
    {

        Sensor_Read(&sensor);

        LED_Toggle();
        DS18B20_StartConvert();

        LCD_ShowIntNum(100,16,sensor.light_analog,4,WHITE,GREEN,12);
//        LCD_ShowFloatNum1(100,32,sensor.md1101_freq,6,WHITE,GREEN,12);

        LEDF_RunningLight(60,1);
        LCD_ShowIntNum(100,16,sensor.light_analog,4,WHITE,GREEN,12);

        LEDF_RunningLight(60,1);
        LCD_ShowIntNum(100,16,sensor.light_analog,4,WHITE,GREEN,12);

        dht = DHT11_Read();

        ds18_temp = DS18B20_ReadTempValue();

        if (dht.valid){
            LCD_ShowString(0,48,"DHT OK",WHITE,GREEN,16,0);
        }else{
            LCD_ShowString(0,48,"DHT fail",WHITE,GREEN,16,0);
        }

        LCD_ShowIntNum(40,48,dht.temp_int,2,BLACK,GREEN,12);
        LCD_ShowIntNum(40,64,ds18_temp * 10,12,BLACK,GREEN,12);
    }

}
