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
#include "buzzer.h"


int main(void)
{
    SystemInit();
    SysTick_Config(SystemCoreClock / 1000);

    delay_init(72);
    LCD_Init();

    LED_Init();
    USART1_Config();
    Buzzer_Init();

    delay_ms(100);

    LCD_Fill(0,0,160,128,GREEN);
    LCD_ShowString(0,0,"Hello world",WHITE,BLUE,16,0);

    Buzzer_SetMode(BUZZER_MODE_SHORT); // 设置一次

    uint32_t last_led = sys_ms;

    while(1)
    {
        uint32_t now = sys_ms;

        LED_Toggle();

        // 蜂鸣器状态机
        Buzzer_Task();

        // 延时防止CPU空转
        delay_ms(500);
    }

}
