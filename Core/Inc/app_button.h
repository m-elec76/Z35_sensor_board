/**
 ******************************************************************************
 * @file    app_button.h
 * @brief   Long-press button handler for RF mode selection.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */

#ifndef APP_BUTTON_H
#define APP_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Standard / project includes ------------------------------------- */
#include <stdint.h>
#include "main.h"      /* Provides TIM_HandleTypeDef via stm32g0xx_hal.h */

/**
 * @brief  Rising-edge callback: latch the button press and start the hold timer.
 * @param  GPIO_Pin  Pin that triggered the interrupt (PB6 = mode button).
 */
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);

/**
 * @brief  TIM3 period-elapsed callback: advance the RF mode after a long press.
 * @param  htim  Timer handle that generated the interrupt.
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#ifdef __cplusplus
}
#endif

#endif /* APP_BUTTON_H */
