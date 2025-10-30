/**
 * @file mini_printf.c
 * @brief 轻量级格式化输出库（适用于 STM32 等嵌入式环境）
 * @author Pudark
 * @date 2025-10-30
 *
 * 功能简介：
 * - 不依赖 stdio.h / sprintf
 * - 支持 %d / %u / %f / %s
 * - 可输出到 LCD、USART 或用户自定义回调
 */

#include "mini_printf.h"
#include <stdarg.h>  // 用于 va_list
#include <stdbool.h>

static mini_putc_func putc_func = 0; // 输出函数指针

// === 用户配置接口 ===
void mini_printf_set_putc(mini_putc_func func)
{
    putc_func = func;
}

/**
 * @brief 输出单个字符（若未设置输出函数，则忽略）
 */
static void mini_putc(char c)
{
    if (putc_func)
        putc_func(c);
}

/**
 * @brief 整数转字符串
 */
void int_to_str(int value, char *buf)
{
    char tmp[12];
    int i = 0, j = 0;
    bool neg = false;

    if (value == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    if (value < 0)
    {
        neg = true;
        value = -value;
    }

    while (value)
    {
        tmp[i++] = (value % 10) + '0';
        value /= 10;
    }

    if (neg)
        buf[j++] = '-';

    while (i)
        buf[j++] = tmp[--i];

    buf[j] = '\0';
}

/**
 * @brief 无符号整数转字符串
 */
void uint_to_str(uint32_t value, char *buf)
{
    char tmp[12];
    int i = 0, j = 0;

    if (value == 0)
    {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }

    while (value)
    {
        tmp[i++] = (value % 10) + '0';
        value /= 10;
    }

    while (i)
        buf[j++] = tmp[--i];

    buf[j] = '\0';
}

/**
 * @brief 浮点数转字符串
 */
void float_to_str(float value, char *buf, int precision)
{
    int int_part = (int)value;
    float frac = value - (float)int_part;
    if (frac < 0) frac = -frac;

    int_to_str(int_part, buf);

    // 拼接小数点
    int i = 0;
    while (buf[i] != '\0') i++;
    buf[i++] = '.';

    for (int p = 0; p < precision; p++)
    {
        frac *= 10;
        int d = (int)frac;
        buf[i++] = '0' + d;
        frac -= d;
    }
    buf[i] = '\0';
}

/**
 * @brief 精简版 printf，仅支持 %d %u %f %s
 */
void mini_printf(const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char buf[32];
    char c;

    while ((c = *fmt++) != '\0')
    {
        if (c != '%')
        {
            mini_putc(c);
            continue;
        }

        // 处理格式控制符
        c = *fmt++;
        switch (c)
        {
        case 'd': // 有符号整数
            int_to_str(va_arg(args, int), buf);
            for (char *p = buf; *p; p++) mini_putc(*p);
            break;

        case 'u': // 无符号整数
            uint_to_str(va_arg(args, unsigned int), buf);
            for (char *p = buf; *p; p++) mini_putc(*p);
            break;

        case 'f': // 浮点数
            float_to_str((float)va_arg(args, double), buf, 2);
            for (char *p = buf; *p; p++) mini_putc(*p);
            break;

        case 's': // 字符串
            for (char *p = va_arg(args, char *); *p; p++) mini_putc(*p);
            break;

        case '%': // 转义 %%
            mini_putc('%');
            break;

        default:
            mini_putc('?');
            break;
        }
    }

    va_end(args);
}
