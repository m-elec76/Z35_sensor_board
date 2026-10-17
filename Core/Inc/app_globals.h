/**
 ******************************************************************************
 * @file    app_globals.h
 * @brief   Declaration and definition of shared runtime variables.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */

#ifndef APP_GLOBALS_H
#define APP_GLOBALS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Standard / project includes ------------------------------------- */
#include "app_types.h"
#include "main.h"
#include "tim.h"

/*==============================================================================
 *                          SENSOR PAYLOAD (TX)
 *============================================================================*/
extern SensorData_t txData;

/*==============================================================================
 *                          BUTTON / MODE STATE
 *============================================================================*/
extern volatile uint8_t button_pressed;
extern volatile uint8_t button_counter;
extern volatile uint8_t current_mode;

/*==============================================================================
 *                          SHT40 STATE
 *============================================================================*/
extern float    temperature;
extern float    humidity;
extern uint16_t realTemp;
extern uint16_t realHumi;
extern int8_t   shtResetCounter;
extern int8_t   calibTemp;
extern int8_t   calibHumi;
extern uint8_t  reset_cmd;
extern uint8_t  sensorStatus;
extern uint8_t  sensor_result;

#ifdef __cplusplus
}
#endif

#endif /* APP_GLOBALS_H */
