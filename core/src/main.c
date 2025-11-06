/**
 * @file main.c
 * @author Pudark
 * @version 1.0
 * @date 2025.11.6
 * @brief 摆烂了，就这样吧
 * @note MCU:STM32F103C8T6
 * @note 编译器:ARM-GCC
 * @note IDE:VSCode + CMake
 * Copyrigh (c) 2025 Pudark.
 */

#include "stm32f10x.h"
#include "lcd.h"
#include "usart.h"
#include "mini_printf.h"
#include "led.h"
#include "ledf.h"
#include "delay.h"
#include "buzzer.h"
#include "gpio_map.h"
#include "sensor.h"
#include "i2c_hw.h"
#include "bmp280.h"
#include "keys.h"
#include "iwdg.h"
#include "pic.h"

u8 buff[30];//参数显示缓存数组
u16 Pre;		//气压检测值

static float bmp280_press,bmp280_tmp,bmp280_asl;			


int main(void)
{
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);

    delay_init(72);
    LCD_Init();
    LEDF_Init();
    LED_Init();

    GPIO_All_Init();
    Sensor_Init();
    DHT11_Init();
    DS18B20_Init();
	BMP280Init();
    Keys_Init();

    USART1_Config();

    delay_ms(100);


    SensorData_t sensor;
    DHT11_Data_t dht;
    float ds18_temp;
    float pressure;   // hPa
    float dummy1, dummy2;
    LCD_Fill(0,0,160,128,GREEN);
;


    IWDG_Init_2s();




    while(Keys_LCDOn())
    {
        KeyEvent ev = Keys_GetEvent();

        LCD_ShowChinese16x16(0+50,0,"气",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(17+50,0,"象",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(34+50,0,"数",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(51+50,0,"据",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(0,18,"温",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(17,18,"度",BLACK,GREEN,16,0);
        LCD_ShowString(34,18,"1:",BLACK,GREEN,16,0);
        LCD_ShowChinese12x12(100,20,"℃",BLACK,GREEN,12,0);
        LCD_ShowChinese16x16(0,36,"湿",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(17,36,"度",BLACK,GREEN,16,0);
        LCD_ShowString(34,36,":",BLACK,GREEN,16,0);
        LCD_ShowChinese12x12(100,38,"%",BLACK,GREEN,12,0);
        LCD_ShowChinese16x16(0,54,"气",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(17,54,"压",BLACK,GREEN,16,0);
        LCD_ShowString(34,54,":",BLACK,GREEN,16,0);
        LCD_ShowString(100,56,"hPa",BLACK,GREEN,12,0);
        LCD_ShowChinese16x16(0,72,"光",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(17,72,"强",BLACK,GREEN,16,0);
        LCD_ShowString(34,72,":",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(0,90,"温",BLACK,GREEN,16,0);
        LCD_ShowChinese16x16(17,90,"度",BLACK,GREEN,16,0);
        LCD_ShowString(34,90,"2:",BLACK,GREEN,16,0);
        LCD_ShowChinese12x12(100,92,"℃",BLACK,GREEN,12,0);

        Sensor_Read(&sensor);

        LED_Toggle();
        DS18B20_StartConvert();

        LCD_ShowFloatNum1(50,74,sensor.light_analog,8,RED,GREEN,12);
        LCD_ShowFloatNum1(50,38,sensor.md1101_freq,4,RED,GREEN,12);

        LEDF_RunningLight(60,1);
        LCD_ShowFloatNum1(50,74,sensor.light_analog,8,RED,GREEN,12);

        LEDF_RunningLight(60,1);
        LCD_ShowFloatNum1(50,74,sensor.light_analog,8,RED,GREEN,12);

        dht = DHT11_Read();

        ds18_temp = DS18B20_ReadTempValue();

        LCD_ShowIntNum(50,20,dht.temp_int,2,RED,GREEN,12);
        LCD_ShowFloatNum1(50,92,ds18_temp,4,RED,GREEN,12);

   		BMP280GetData(&bmp280_press,&bmp280_tmp,&bmp280_asl);

        LCD_ShowFloatNum1(50,56,bmp280_press,6,RED,GREEN,12);
        
 
        IWDG_Feed();

        switch(ev)
        {
            //拼手速按这个才能显示，属于是彩蛋了
            case KEY_NEXT_PAGE:    
            LCD_Fill(0,0,160,128,GREEN);
            LCD_ShowChinese16x16(60,0,"作",BLACK,GREEN,16,0);
            LCD_ShowChinese16x16(80,0,"者",BLACK,GREEN,16,0);
            LCD_ShowChinese16x16(0,20,"范",BLACK,GREEN,16,0);
            LCD_ShowChinese16x16(18,20,"轶",BLACK,GREEN,16,0);
            LCD_ShowChinese16x16(0,40,"陶",BLACK,GREEN,16,0);
            LCD_ShowChinese16x16(18,40,"涤",BLACK,GREEN,16,0);
            LCD_ShowChinese16x16(36,40,"华",BLACK,GREEN,16,0);
            LCD_ShowChinese16x16(0,60,"薛",BLACK,GREEN,16,0);
            LCD_ShowChinese16x16(18,60,"振",BLACK,GREEN,16,0);
            LCD_ShowChinese16x16(36,60,"宇",BLACK,GREEN,16,0);
            LCD_ShowString(34,90,"      ",BLACK,GREEN,16,0);
            delay_ms(500);
            LCD_Fill(0,0,160,128,GREEN);
            break;
            case KEY_PREV_PAGE: 
            break;
            case KEY_BOTH_LONG:    break;
            case REED_TOGGLE_LCD:  break;
            default: break;
        }

    }
    BUZZER_Alert(100,100,10);//主程序错误告警

    delay_ms(1000);//没喂狗，会进入自动重启，这里写的程序是防止主循环跑飞了以后没东西可以执行

    return 1;

}
