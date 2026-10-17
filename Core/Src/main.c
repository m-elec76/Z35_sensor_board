/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body for the Sensor Board.
 * @author         : Mohammadali Abbasi <m.elec76a@gmail.com>
 * @date           : 2026
 * @license        : Apache-2.0
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "app_config.h"
#include "app_types.h"
#include "app_globals.h"
#include "app_delay.h"
#include "app_i2c.h"
#include "app_nrf.h"
#include "app_button.h"
#include "app_sensors.h"
#include "nrf24.h"
#include "string.h"
#include <stdbool.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

	/*
	 * Node Identification:
	 * Each sensor board in a multi-node setup must have a unique ID.
	 * This ID (sensor_num) is transmitted in the payload to allow the
	 * Main Board to distinguish between data coming from different
	 * physical sensors in the same hall.
	 *
	 * Future: Replace this hardcoded value with a dynamic
	 * assignment method (e.g., configured via the long-press button
	 * or stored in non-volatile memory).
	 */
	txData.sensor_num = 1;

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_TIM17_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Init NRF24L01+ <<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
    HAL_Delay(NRF_POWER_UP_DELAY_MS);

    (void)nRF24_Init();
    nRF24_SetPowerMode(nRF24_PWR_UP);
    HAL_Delay(NRF_SETTLE_DELAY_MS);
    apply_mode(current_mode);
    nRF24_SetDataRate(nRF24_DR_250kbps);
    nRF24_SetTXPower(nRF24_TXPWR_0dBm);
    nRF24_SetCRCScheme(nRF24_CRC_2byte);
    nRF24_SetAddrWidth(5U);
    nRF24_SetAutoRetr(nRF24_ARD_1000us, 5U);
    nRF24_SetRXPipe(0U, nRF24_AA_ON, (uint8_t)sizeof(SensorData_t));
    nRF24_SetOperationalMode(nRF24_MODE_TX);
    HAL_Delay(NRF_READY_DELAY_MS);
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>> Init NRF24L01+ <<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    while (1)
    {
    	(void)HAL_GPIO_WritePin(CHARGE_EN, GPIO_PIN_SET);

        /* Sample all on-board sensors. */
        Sensors_ReadAdc();
        Sensors_ReadPir();

        /* Read SHT40 and pack the values into the TX payload. */
        sensor_result = Read_SHT40(&temperature, &humidity);
        if (sensor_result == 0U)
        {
            txData.Temperature = (uint16_t)(temperature * 10.0f);
            txData.humidity    = (uint16_t)(humidity    * 10.0f);
        }
        else
        {
            txData.Temperature = 0U;
            txData.humidity    = 0U;
        }

        /* Transmit the payload. */
        nRF24_TransmitPacket((uint8_t *)&txData, (uint8_t)sizeof(SensorData_t));

        /* Blink the status LED "current_mode" times. */
        Sensors_BlinkModeLed();

        HAL_Delay(TX_PERIOD_MS);

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 12;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    __disable_irq();
    while (1)
    {
        /* Stay here until reset. */
    }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
    (void)file;
    (void)line;
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
