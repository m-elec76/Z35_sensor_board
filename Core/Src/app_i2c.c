/**
 ******************************************************************************
 * @file    app_i2c.c
 * @brief   I2C bus recovery helpers and SHT40 driver.
 *
 * @author  Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date    2026
 * @license Apache-2.0
 ******************************************************************************
 * @note    The I2C bus is I2C1 on PB8 (SCL) / PB9 (SDA).
 *          The original function names (I2C2_*) are preserved verbatim so
 *          the public API remains compatible with the previous firmware.
 ******************************************************************************
 */

#include "app_i2c.h"
#include "app_config.h"
#include "app_globals.h"
#include "app_delay.h"
#include "app_types.h"

/*==============================================================================
 *                          I2C SOFT RESET
 *============================================================================*/
void I2C2_soft_reset(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* De-initialise the hardware I2C peripheral. */
    (void)HAL_I2C_DeInit(&hi2c1);

    /* Enable GPIOB clock and configure SCL/SDA as open-drain outputs. */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitStruct.Pin   = I2C_SCL_PIN | I2C_SDA_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    (void)HAL_GPIO_Init(I2C_GPIO_PORT, &GPIO_InitStruct);

    /* Release both lines (idle high). */
    (void)HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
    (void)HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
    delay_ms(I2C_SOFT_RESET_DELAY_MS);

    /* If SDA is held low, issue up to nine SCL pulses to free it. */
    if (HAL_GPIO_ReadPin(I2C_GPIO_PORT, I2C_SDA_PIN) == GPIO_PIN_RESET)
    {
        for (int i = 0; i < I2C_RECOVERY_PULSES; i++)
        {
            (void)HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_RESET);
            delay_ms(I2C_SOFT_RESET_DELAY_MS);
            (void)HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
            delay_ms(I2C_SOFT_RESET_DELAY_MS);

            if (HAL_GPIO_ReadPin(I2C_GPIO_PORT, I2C_SDA_PIN) == GPIO_PIN_SET)
            {
                break;
            }
        }
    }

    /* Emit a manual STOP condition: SDA low -> high while SCL is high. */
    (void)HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SCL_PIN, GPIO_PIN_SET);
    (void)HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_RESET);
    delay_ms(I2C_SOFT_RESET_DELAY_MS);
    (void)HAL_GPIO_WritePin(I2C_GPIO_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
    delay_ms(I2C_SOFT_RESET_DELAY_MS);

    /* Release pins and re-enable the hardware I2C peripheral. */
    (void)HAL_GPIO_DeInit(I2C_GPIO_PORT, I2C_SCL_PIN | I2C_SDA_PIN);
    (void)HAL_I2C_Init(&hi2c1);
}

/*==============================================================================
 *                          BUS-STUCK DETECTION
 *============================================================================*/
bool IsI2C2BusStuck(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Read line levels as inputs with pull-up. */
    GPIO_InitStruct.Pin  = I2C_SDA_PIN | I2C_SCL_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    (void)HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_PinState sda = HAL_GPIO_ReadPin(GPIOB, I2C_SDA_PIN);
    GPIO_PinState scl = HAL_GPIO_ReadPin(GPIOB, I2C_SCL_PIN);

    /* Restore pins to the I2C alternate function (AF6 = I2C1 on PB8/PB9). */
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = I2C_AF_I2C1;
    (void)HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    return (sda == GPIO_PIN_RESET) || (scl == GPIO_PIN_RESET);
}

/*==============================================================================
 *                          BUS RECOVERY
 *============================================================================*/
void I2C2_BusRecovery(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    (void)HAL_I2C_DeInit(&hi2c1);
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* Configure SCL (PB8) and SDA (PB9) as open-drain outputs. */
    GPIO_InitStruct.Pin   = I2C_SDA_PIN | I2C_SCL_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    (void)HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Drive both lines high (idle). */
    (void)HAL_GPIO_WritePin(GPIOB, I2C_SDA_PIN | I2C_SCL_PIN, GPIO_PIN_SET);
    delay_ms(I2C_RECOVERY_DELAY_MS);

    /* Generate nine clock pulses on SCL to free up SDA. */
    for (int i = 0; i < I2C_RECOVERY_PULSES; i++)
    {
        (void)HAL_GPIO_WritePin(GPIOB, I2C_SCL_PIN, GPIO_PIN_RESET);
        delay_ms(I2C_RECOVERY_DELAY_MS);
        (void)HAL_GPIO_WritePin(GPIOB, I2C_SCL_PIN, GPIO_PIN_SET);
        delay_ms(I2C_RECOVERY_DELAY_MS);
    }

    /* Manual STOP condition: SDA low -> high while SCL is high. */
    (void)HAL_GPIO_WritePin(GPIOB, I2C_SDA_PIN, GPIO_PIN_RESET);
    delay_ms(I2C_RECOVERY_DELAY_MS);
    (void)HAL_GPIO_WritePin(GPIOB, I2C_SCL_PIN, GPIO_PIN_SET);
    delay_ms(I2C_RECOVERY_DELAY_MS);
    (void)HAL_GPIO_WritePin(GPIOB, I2C_SDA_PIN, GPIO_PIN_SET);
    delay_ms(I2C_RECOVERY_DELAY_MS);

    /* Restore pins to the I2C alternate function. */
    GPIO_InitStruct.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull      = GPIO_PULLUP;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = I2C_AF_I2C1;
    (void)HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    (void)HAL_I2C_Init(&hi2c1);
}

/*==============================================================================
 *                          ROBUST I2C TRANSFERS
 *============================================================================*/
HAL_StatusTypeDef I2C1_Robust_Transmit(uint16_t DevAddress,
                                       uint8_t *pData,
                                       uint16_t Size,
                                       uint32_t Timeout)
{
    if (IsI2C2BusStuck())
    {
        I2C2_BusRecovery();
    }

    HAL_StatusTypeDef ret =
        HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, Timeout);

    if (ret != HAL_OK)
    {
        if (IsI2C2BusStuck())
        {
            I2C2_BusRecovery();
            ret = HAL_I2C_Master_Transmit(&hi2c1, DevAddress, pData, Size, Timeout);
        }
    }
    return ret;
}

HAL_StatusTypeDef I2C1_Robust_Receive(uint16_t DevAddress,
                                      uint8_t *pData,
                                      uint16_t Size,
                                      uint32_t Timeout)
{
    if (IsI2C2BusStuck())
    {
        I2C2_BusRecovery();
    }

    HAL_StatusTypeDef ret =
        HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, Size, Timeout);

    if (ret != HAL_OK)
    {
        if (IsI2C2BusStuck())
        {
            I2C2_BusRecovery();
            ret = HAL_I2C_Master_Receive(&hi2c1, DevAddress, pData, Size, Timeout);
        }
    }
    return ret;
}

/*==============================================================================
 *                          SHT40 CRC
 *============================================================================*/
uint8_t SHT40_CalculateCRC(uint8_t *data)
{
    uint8_t crc = SHT40_CRC_INIT;

    for (int i = 0; i < 2; i++)
    {
        crc ^= data[i];
        for (uint8_t bit = 0U; bit < 8U; bit++)
        {
            if ((crc & 0x80U) != 0U)
            {
                crc = (uint8_t)((crc << 1U) ^ SHT40_CRC_POLYNOMIAL);
            }
            else
            {
                crc = (uint8_t)(crc << 1U);
            }
        }
    }
    return crc;
}

/*==============================================================================
 *                          SHT40 MEASUREMENT
 *============================================================================*/
uint8_t Read_SHT40(float *temperature, float *humidity)
{
    uint8_t           cmd = SHT40_CMD_MEASURE;
    uint8_t           data[6] = {0};
    HAL_StatusTypeDef status;

    /* Perform bus recovery if the bus appears stuck. */
    if (IsI2C2BusStuck())
    {
        I2C2_BusRecovery();
    }

    /* Send the measure command. */
    status = I2C1_Robust_Transmit(SHT40_I2C_ADDR, &cmd, 1U, I2C_TRANSFER_TIMEOUT_MS);
    if (status != HAL_OK)
    {
        return SENSOR_ERR_TRANSMIT;
    }

    /* Wait for the measurement to complete. */
    delay_ms(SHT40_MEASURE_DELAY_MS);

    /* Receive 6 bytes: temp(2)+CRC + hum(2)+CRC. */
    status = I2C1_Robust_Receive(SHT40_I2C_ADDR, data, 6U, I2C_TRANSFER_TIMEOUT_MS);
    if (status != HAL_OK)
    {
        return SENSOR_ERR_RECEIVE;
    }

    /* CRC check for temperature. */
    if (SHT40_CalculateCRC(&data[0]) != data[2])
    {
        return SENSOR_ERR_TEMP_CRC;
    }

    /* CRC check for humidity. */
    if (SHT40_CalculateCRC(&data[3]) != data[5])
    {
        return SENSOR_ERR_HUMI_CRC;
    }

    /* Convert raw words to engineering values. */
    uint16_t raw_temp = (uint16_t)(((uint16_t)data[0] << 8U) | (uint16_t)data[1]);
    uint16_t raw_hum  = (uint16_t)(((uint16_t)data[3] << 8U) | (uint16_t)data[4]);

    *temperature = SHT40_TEMP_OFFSET_C
                 + (SHT40_TEMP_SPAN_C * ((float)raw_temp / SHT40_RAW_MAX));
    *humidity    = SHT40_HUMI_SPAN_PCT * ((float)raw_hum / SHT40_RAW_MAX);

    return (uint8_t)SENSOR_OK;
}
