/**
 * @file keys.h
 * @author Pudark
 * @version 0.7
 * @date 2025-11-5
 * @brief 按键
 */

#ifndef __KEYS_H__
#define __KEYS_H__

#include "stm32f10x.h"
#include <stdbool.h>

/**
 * @brief 按键事件枚举
 */
typedef enum {
    KEY_NONE = 0,
    KEY_PREV_PAGE,
    KEY_NEXT_PAGE,
    KEY_BOTH_LONG,   // 同时长按
    REED_TOGGLE_LCD
} KeyEvent;

KeyEvent Keys_GetEvent(void);
void Keys_Init(void);
bool Keys_LCDOn(void);


#endif
