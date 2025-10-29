#include "lcd.h"

#define LCD_RST_LOW()   GPIO_ResetBits(GPIOA, GPIO_Pin_2)
#define LCD_RST_HIGH()  GPIO_SetBits(GPIOA, GPIO_Pin_2)
#define LCD_DC_LOW()    GPIO_ResetBits(GPIOA, GPIO_Pin_3)
#define LCD_DC_HIGH()   GPIO_SetBits(GPIOA, GPIO_Pin_3)
#define LCD_CS_LOW()    GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define LCD_CS_HIGH()   GPIO_SetBits(GPIOA, GPIO_Pin_4)

void LCD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void LCD_WR_REG(uint8_t reg)
{
    LCD_DC_LOW();
    LCD_CS_LOW();
    SPI1_WriteByte(reg);
    LCD_CS_HIGH();
}

void LCD_WR_DATA(uint8_t data)
{
    LCD_DC_HIGH();
    LCD_CS_LOW();
    SPI1_WriteByte(data);
    LCD_CS_HIGH();
}

uint16_t LCD_ReadID(void)
{
    uint16_t id = 0;
    LCD_WR_REG(0x04);  // 通用读显示驱动器ID命令（ST77xx兼容）
    SPI1_ReadByte();   // dummy read
    id = SPI1_ReadByte() << 8;
    id |= SPI1_ReadByte();
    return id;
}

void LCD_Init(void)
{
    LCD_GPIO_Init();
    SPI1_HW_Init();

    LCD_RST_LOW();
    delay_ms(50);
    LCD_RST_HIGH();
    delay_ms(120);
}
