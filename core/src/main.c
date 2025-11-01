/**
 * @file main.c
 * @author Pudark
 * @version 0.2
 * @date 2025.10.30
 * @brief 调试流水灯
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
#include "mini_printf.h"
#include "delay.h"


int main(void)
{
    // 先初始化系统时钟
    SystemInit();
    
    // 先初始化LCD相关的外设
    delay_init(72);  // 初始化延时函数
    LCD_Init();
    
    // 再初始化其他不冲突的外设
    LED_Init();
    USART1_Config();
    LEDF_Init();

    // 添加启动延时，让外设稳定
    delay_ms(100);

    LEDF_RunningLight(100,0);

    LCD_Fill(0,0,160,128,GREEN);

    LCD_ShowString(0,0,"Hello world",WHITE,BLUE,16,0);
    

    while(1)
    {
        LED_Toggle();
        delay_ms(500);  // 使用准确的延时函数
    }
}