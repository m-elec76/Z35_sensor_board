/**
 ******************************************************************************
 * @file    app_sensors.h
 * @brief   On-board sensor helpers (ADC sampling, PIR, LED).
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */
#ifndef APP_SENSORS_H
#define APP_SENSORS_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief  Sample the MQ-135 and LDR channels from ADC1 and store the raw
 *         results in txData.gas and txData.ldr.
 */
void Sensors_ReadAdc(void);

/**
 * @brief  Sample the PIR input and store the result in txData.burglar_alarm.
 */
void Sensors_ReadPir(void);

/**
 * @brief  Blink the status LED "current_mode" times to display the mode.
 */
void Sensors_BlinkModeLed(void);

#ifdef __cplusplus
}
#endif

#endif /* APP_SENSORS_H */
