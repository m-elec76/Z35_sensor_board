/**
 ******************************************************************************
 * @file    app_types.h
 * @brief   Global data structures and enums.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */
#ifndef APP_TYPES_H
#define APP_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*==============================================================================
 *                          WIRELESS SENSOR PAYLOAD
 *============================================================================*/
/**
 * @brief  Payload transmitted over the nRF24L01+ link.
 * @note   The struct is packed so its byte layout matches the main board.
 */
typedef struct __attribute__((packed))
{
    uint8_t  system_id;         /**< Network identifier of the sender       */
    uint8_t  sensor_num;        /**< Logical sensor node number             */
    uint16_t Temperature;       /**< Temperature value multiplied by 10     */
    uint16_t humidity;          /**< Relative humidity multiplied by 10     */
    uint16_t gas;               /**< Raw ADC reading from the MQ-135        */
    uint8_t  burglar_alarm;     /**< PIR alarm flag (0 = idle, 1 = alarm)   */
    uint16_t ldr;               /**< Raw ADC reading from the LDR           */
} SensorData_t;

/*==============================================================================
 *                          SENSOR RESULT CODES
 *============================================================================*/
/**
 * @brief  Result codes returned by Read_SHT40().
 */
typedef enum
{
    SENSOR_OK             = 0,  /**< Success                          */
    SENSOR_ERR_TRANSMIT   = 1,  /**< I2C transmit error               */
    SENSOR_ERR_RECEIVE    = 2,  /**< I2C receive error                */
    SENSOR_ERR_TEMP_CRC   = 3,  /**< Temperature CRC mismatch         */
    SENSOR_ERR_HUMI_CRC   = 4   /**< Humidity CRC mismatch            */
} SensorResult_t;

#ifdef __cplusplus
}
#endif

#endif /* APP_TYPES_H */
