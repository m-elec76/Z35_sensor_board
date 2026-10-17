/**
 ******************************************************************************
 * @file    app_button.c
 * @brief   Long-press button handler for RF mode selection.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 * @details PB6 is configured as a rising-edge external interrupt.  When the
 *          button is pressed, TIM3 starts counting.  If the button is still
 *          held after BUTTON_HOLD_TICKS timer ticks, the RF mode is advanced
 *          and TIM3 is stopped.
 ******************************************************************************
 */

#include "app_button.h"
#include "app_config.h"
#include "app_globals.h"
#include "app_nrf.h"
#include "tim.h"
#include "main.h"

void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == EXTI_MODE_BUTTON_PIN)
    {
        if (HAL_GPIO_ReadPin(BUTTON_MODE_PORT, BUTTON_MODE_PIN) == GPIO_PIN_SET)
        {
            button_pressed = 1U;
            button_counter = 0U;
            (void)HAL_TIM_Base_Start_IT(&htim3);
        }
    }
}

/*---------------------------------------------------------------------------*/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        if (HAL_GPIO_ReadPin(BUTTON_MODE_PORT, BUTTON_MODE_PIN) == GPIO_PIN_SET)
        {
            button_counter++;
            if (button_counter >= BUTTON_HOLD_TICKS)
            {
                current_mode++;
                if (current_mode > NRF_MODE_MAX)
                {
                    current_mode = NRF_MODE_MIN;
                }

                apply_mode(current_mode);
                button_pressed = 0U;
                button_counter = 0U;
                (void)HAL_TIM_Base_Stop_IT(&htim3);
            }
        }
        else
        {
            button_pressed = 0U;
            button_counter = 0U;
            (void)HAL_TIM_Base_Stop_IT(&htim3);
        }
    }
}
