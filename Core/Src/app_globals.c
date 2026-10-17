/**
 ******************************************************************************
 * @file    app_globals.c
 * @brief   Declaration and definition of shared runtime variables.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */

#include "app_globals.h"
#include "app_config.h"

/*==============================================================================
 *                          SENSOR PAYLOAD (TX)
 *============================================================================*/
SensorData_t txData = {0};

/*==============================================================================
 *                          BUTTON / MODE STATE
 *============================================================================*/
volatile uint8_t button_pressed = 0U;
volatile uint8_t button_counter = 0U;
volatile uint8_t current_mode   = NRF_MODE_DEFAULT;

/*==============================================================================
 *                          SHT40 STATE
 *============================================================================*/
float    temperature     = 0.0f;
float    humidity        = 0.0f;
uint16_t realTemp        = 0U;
uint16_t realHumi        = 0U;
int8_t   shtResetCounter = 0;
int8_t   calibTemp       = 0;
int8_t   calibHumi       = 0;
uint8_t  reset_cmd       = 0x94U;
uint8_t  sensorStatus    = 1U;      /* 0=OK, 1=TX err, 2=RX err, 3/4=CRC  */
uint8_t  sensor_result   = 0U;
