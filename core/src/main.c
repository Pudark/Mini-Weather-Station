#include "led.h"

int main(void)
{
    LED_Init();

    while(1)
    {
        LED_Toggle();
        for(volatile int i=0; i<500000; i++); // 简单延时
    }
}
