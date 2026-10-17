/**
 ******************************************************************************
 * @file    app_delay.h
 * @brief   Millisecond blocking delay based on TIM17.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */

#ifndef APP_DELAY_H
#define APP_DELAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/**
 * @brief  Block the CPU for the requested number of milliseconds using TIM17.
 * @param  ms  Delay in milliseconds (1..65535).
 * @note   Assumes TIM17 is clocked for a 1 ms tick period.
 */
void delay_ms(uint16_t ms);

#ifdef __cplusplus
}
#endif

#endif /* APP_DELAY_H */
