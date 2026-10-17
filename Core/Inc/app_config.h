/**
 ******************************************************************************
 * @file    app_config.h
 * @brief   Central configuration (pins, constants, timings).
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 */

#ifndef APP_CONFIG_H
#define APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*==============================================================================
 *                              FIRMWARE METADATA
 *============================================================================*/
#define FW_VERSION_MAJOR            (1U)
#define FW_VERSION_MINOR            (0U)
#define FW_VERSION_PATCH            (0U)

/*==============================================================================
 *                              SHT40 SENSOR
 *============================================================================*/
#define SHT40_I2C_ADDR              (0x44 << 1)     /**< 7-bit addr, HAL-shifted */
#define SHT40_CMD_MEASURE           (0xFDU)         /**< High-precision measure  */

#define I2C_GPIO_PORT               GPIOB
#define I2C_SCL_PIN                 GPIO_PIN_8      /**< PB8 = I2C1_SCL          */
#define I2C_SDA_PIN                 GPIO_PIN_9      /**< PB9 = I2C1_SDA          */

/** Alternate function index for I2C1 on PB8/PB9 (AF6 in STM32G0). */
#define I2C_AF_I2C1                 GPIO_AF6_I2C1

/** CRC polynomial for the SHT40 (x^8 + x^5 + x^4 + 1). */
#define SHT40_CRC_POLYNOMIAL        (0x31U)
#define SHT40_CRC_INIT              (0xFFU)

/** SHT40 conversion coefficients (datasheet formula). */
#define SHT40_TEMP_OFFSET_C         (-45.0f)
#define SHT40_TEMP_SPAN_C           (175.0f)
#define SHT40_HUMI_SPAN_PCT         (100.0f)
#define SHT40_RAW_MAX               (65535.0f)

/*==============================================================================
 *                              PERIPHERAL PINS
 *============================================================================*/
#define CHARGE_EN                   GPIOC, GPIO_PIN_7
#define BURGLAR_ALARM               GPIOA, GPIO_PIN_1

#define LED_STATUS_PORT             GPIOB
#define LED_STATUS_PIN              GPIO_PIN_7

#define BUTTON_MODE_PORT            GPIOB
#define BUTTON_MODE_PIN             GPIO_PIN_6

/*==============================================================================
 *                              EXTERNAL INTERRUPTS
 *============================================================================*/
#define EXTI_MODE_BUTTON_PIN        GPIO_PIN_6

/*==============================================================================
 *                              TIMING CONSTANTS
 *============================================================================*/
/** LED blink duration (ms) for the mode indicator. */
#define LED_BLINK_MS                (200U)

/** Main loop transmission period (ms). */
#define TX_PERIOD_MS                (1500U)

/** Debounce hold time for the mode button (in TIM3 ticks). */
#define BUTTON_HOLD_TICKS           (3U)

/** nRF24L01+ power-up delays (ms). */
#define NRF_POWER_UP_DELAY_MS       (10U)
#define NRF_SETTLE_DELAY_MS         (5U)
#define NRF_READY_DELAY_MS          (150U)

/** I2C delays. */
#define I2C_SOFT_RESET_DELAY_MS     (10U)
#define I2C_RECOVERY_DELAY_MS       (2U)
#define I2C_RECOVERY_PULSES         (9)
#define I2C_TRANSFER_TIMEOUT_MS     (100U)
#define SHT40_MEASURE_DELAY_MS      (10U)

/*==============================================================================
 *                              MODE / PROFILE CONSTANTS
 *============================================================================*/
#define NRF_MODE_MIN                (1U)
#define NRF_MODE_MAX                (10U)
#define NRF_MODE_DEFAULT            (1U)

#ifdef __cplusplus
}
#endif

#endif /* APP_CONFIG_H */
