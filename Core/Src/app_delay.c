/**
 ******************************************************************************
 * @file    app_delay.c
 * @brief   Millisecond blocking delay based on TIM17.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */

#include "app_delay.h"
#include "tim.h"

void delay_ms(uint16_t ms)
{
    /* Clear any pending update flag before starting the timer. */
    __HAL_TIM_CLEAR_FLAG(&htim17, TIM_FLAG_UPDATE);

    /* Set the reload value (period = ticks - 1) and reset the counter. */
    __HAL_TIM_SET_AUTORELOAD(&htim17, (uint32_t)(ms - 1U));
    __HAL_TIM_SET_COUNTER(&htim17, 0U);

    /* Start the timer in blocking mode. */
    (void)HAL_TIM_Base_Start(&htim17);

    /* Wait until the update flag fires (delay complete). */
    while (__HAL_TIM_GET_FLAG(&htim17, TIM_FLAG_UPDATE) == 0U)
    {
        /* busy-wait */
    }

    /* Stop the timer and clear the flag for the next call. */
    (void)HAL_TIM_Base_Stop(&htim17);
    __HAL_TIM_CLEAR_FLAG(&htim17, TIM_FLAG_UPDATE);
}
