/**
 ******************************************************************************
 * @file    app_i2c.h
 * @brief   I2C bus recovery helpers and SHT40 driver.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 * @note    The hardware I2C bus is I2C1, mapped on PB8 (SCL) and PB9 (SDA).
 *          The original function names (I2C2_*) are preserved to keep the
 *          public API compatible with the previous firmware revision.
 ******************************************************************************
 */

#ifndef APP_I2C_H
#define APP_I2C_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Standard / project includes ------------------------------------- */
#include <stdint.h>
#include <stdbool.h>
#include "main.h"
#include "i2c.h"
#include "app_types.h"      /* Provides SensorResult_t */

/*==============================================================================
 *                          I2C BUS RECOVERY
 *============================================================================*/
void I2C2_soft_reset(void);
bool IsI2C2BusStuck(void);
void I2C2_BusRecovery(void);

/*==============================================================================
 *                          ROBUST I2C TRANSFERS
 *============================================================================*/
HAL_StatusTypeDef I2C1_Robust_Transmit(uint16_t DevAddress,
                                       uint8_t *pData,
                                       uint16_t Size,
                                       uint32_t Timeout);

HAL_StatusTypeDef I2C1_Robust_Receive(uint16_t DevAddress,
                                      uint8_t *pData,
                                      uint16_t Size,
                                      uint32_t Timeout);

/*==============================================================================
 *                          SHT40 SENSOR
 *============================================================================*/
uint8_t SHT40_CalculateCRC(uint8_t *data);
uint8_t Read_SHT40(float *temperature, float *humidity);

#ifdef __cplusplus
}
#endif

#endif /* APP_I2C_H */
