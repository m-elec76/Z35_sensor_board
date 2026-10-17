/**
 ******************************************************************************
 * @file    app_sensors.c
 * @brief   On-board sensor helpers (ADC sampling, PIR, LED).
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */

#include "app_sensors.h"
#include "app_config.h"
#include "app_globals.h"
#include "adc.h"
#include "main.h"

/*==============================================================================
 *                          ADC SAMPLING
 *============================================================================*/
/**
 * @note  The original code configures ADC1 in scan mode with two channels:
 *        MQ-135 first, then LDR.  Both conversions are triggered by a single
 *        HAL_ADC_Start() call.
 */
void Sensors_ReadAdc(void)
{
    (void)HAL_ADC_Start(&hadc1);

    /* First conversion: MQ-135 gas sensor. */
    (void)HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    txData.gas = (uint16_t)HAL_ADC_GetValue(&hadc1);

    /* Second conversion: LDR. */
    (void)HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
    txData.ldr = (uint16_t)HAL_ADC_GetValue(&hadc1);
}

/*==============================================================================
 *                          PIR INPUT
 *============================================================================*/
void Sensors_ReadPir(void)
{
    if (HAL_GPIO_ReadPin(BURGLAR_ALARM) == GPIO_PIN_SET)
    {
        txData.burglar_alarm = 1U;
    }
    else
    {
        txData.burglar_alarm = 0U;
    }
}

/*==============================================================================
 *                          STATUS LED
 *============================================================================*/
void Sensors_BlinkModeLed(void)
{
    for (uint8_t i = 0U; i < current_mode; i++)
    {
        (void)HAL_GPIO_WritePin(LED_STATUS_PORT, LED_STATUS_PIN, GPIO_PIN_SET);
        HAL_Delay(LED_BLINK_MS);
        (void)HAL_GPIO_WritePin(LED_STATUS_PORT, LED_STATUS_PIN, GPIO_PIN_RESET);
        HAL_Delay(LED_BLINK_MS);
    }
}
