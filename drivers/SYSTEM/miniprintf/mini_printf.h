/**
 * @file mini_printf.h
 * @brief 轻量级格式化输出库（适用于 STM32 等嵌入式环境）
 * @author Pudark
 * @date 2025-10-30
 *
 * 功能简介：
 * - 不依赖 stdio.h / sprintf
 * - 支持 %d / %u / %f / %s
 * - 可输出到 LCD、USART 或用户自定义回调
 */

#ifndef __MINI_PRINTF_H__
#define __MINI_PRINTF_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// === 用户可配置 ===
// 你可以把这个函数绑定到 LCD 或 USART 输出函数
typedef void (*mini_putc_func)(char c);

// === 函数声明 ===
void mini_printf_set_putc(mini_putc_func func);
void mini_printf(const char *fmt, ...);

// === 基础转换函数（如果单独调用也可以）===
void int_to_str(int value, char *buf);
void uint_to_str(uint32_t value, char *buf);
void float_to_str(float value, char *buf, int precision);

#ifdef __cplusplus
}
#endif

#endif // __MINI_PRINTF_H__
