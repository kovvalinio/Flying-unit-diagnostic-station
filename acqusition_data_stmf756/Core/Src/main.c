/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
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

#include "crc.h"

#include "dma.h"

#include "i2c.h"

#include "pdm2pcm.h"

#include "spi.h"

#include "tim.h"

#include "usart.h"

#include "gpio.h"

#include "app_mems.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <accelerometer_config.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PDM_BUF 192
#define PCM_MID_BUF 24
#define PCM_BIG_BUF 48000
#define mic_buffer_bytes 96080

#define ACC_BUFFER_SIZE 2400
#define acc_buffer_bytes 14412

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t PDMbuff[PDM_BUF];
uint16_t pcmSmallBuffer[PCM_MID_BUF];
uint16_t pcmBigBuffer[PCM_BIG_BUF];
uint8_t micRxFlag = 0;
uint32_t fifoIterator = 0;
uint8_t mic_data_to_send[mic_buffer_bytes];

uint8_t espTxFlag = 0;

acceleration acc_vector[ACC_BUFFER_SIZE];
uint32_t acc_iterator = 0;
ism330dhcx_axis3bit16_t data_raw;
uint8_t acc_data_to_send[acc_buffer_bytes];
uint8_t acc_aqcuisition_flag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

//Function storing  sound data in a big buffer
//Whenever the big buffer is full espTxFlag is set
//to start transmiting
void FifoWrite_B(uint16_t data) {

  if (fifoIterator != PCM_BIG_BUF) {

    pcmBigBuffer[fifoIterator] = data;

    fifoIterator++;

  }
  if (fifoIterator == PCM_BIG_BUF) {

    espTxFlag = 1;

  }

}

//Callbacks allowing pipelining in the i2s interface.
//Its used to ensure constant data flow.
void HAL_SPI_RxHalfCpltCallback(SPI_HandleTypeDef * hspi) {

  if (hspi -> Instance == SPI2) {

    micRxFlag = 1;

  }

}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef * hspi) {

  if (hspi -> Instance == SPI2) {

    micRxFlag = 2;

  }

}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef * htim) {

  if (htim -> Instance == TIM1) {

    if (acc_aqcuisition_flag == 0) {

      measure_and_add( & data_raw.i16bit);

    }

  }

}
void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef * hi2c) {

  if (hi2c -> Instance == I2C4) {

    acc_vector[acc_iterator].x = (int16_t)((float)((float)(data_raw.i16bit[0]) * ACC_SENS));
    acc_vector[acc_iterator].y = (int16_t)((float)((float)(data_raw.i16bit[1]) * ACC_SENS));
    acc_vector[acc_iterator].z = (int16_t)((float)((float)(data_raw.i16bit[2]) * ACC_SENS));

    acc_iterator++;

    if (acc_iterator == ACC_BUFFER_SIZE) {

      acc_aqcuisition_flag = 1;

    }

  }

}

void prepare_mic_data_to_send()
{

  uint8_t pack_nr = 0;

  uint16_t * ptr = & pcmBigBuffer[0];

  for (int i = 0; i < mic_buffer_bytes; i = i + 2) {

    if (i % 1201 == 0) {

      mic_data_to_send[i] = pack_nr;

      pack_nr++;

      i++;

    }

    else if (i % 1201 != 0) {

      mic_data_to_send[i] = ( * ptr >> 8);

      mic_data_to_send[i + 1] = ( * ptr & 0xFF);

      if (i < mic_buffer_bytes - 1) {

        ptr++;

      }

    }

  }

}

void prepare_acc_data_to_send()
{
  uint8_t pack_nr = 128;

  acceleration * ptr = & acc_vector[0];

  for (int i = 0; i < acc_buffer_bytes; i = i + 6)
  {

    if (i % 1201 == 0) {

      acc_data_to_send[i] = pack_nr;

      pack_nr++;

      i++;

    }
    else if (i % 1201 != 0){

      acc_data_to_send[i] = (ptr -> x >> 8);

      acc_data_to_send[i + 1] = (ptr -> x & 0xFF);

      acc_data_to_send[i + 2] = (ptr -> y >> 8);

      acc_data_to_send[i + 3] = (ptr -> y & 0xFF);

      acc_data_to_send[i + 4] = (ptr -> z >> 8);

      acc_data_to_send[i + 5] = (ptr -> z & 0xFF);

      if (i < acc_buffer_bytes - 1) {

    	  ptr++;
      }

    }

  }

}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
  /* USER CODE BEGIN 1 */

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
  MX_CRC_Init();
  MX_DMA_Init();
  MX_PDM2PCM_Init();
  MX_SPI2_Init();
  MX_UART4_Init();
  MX_I2C1_Init();
  MX_TIM1_Init();
  MX_MEMS_Init();
  /* USER CODE BEGIN 2 */
  HAL_SPI_Receive_DMA( & hspi2, PDMbuff, PDM_BUF);
  HAL_TIM_Base_Start_IT( & htim1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {

    //Read values mode
    if (espTxFlag == 0 && fifoIterator != PCM_BIG_BUF) {

      if (micRxFlag == 1) {

        if (PDM_Filter( & PDMbuff[0], & pcmSmallBuffer[0], & PDM1_filter_handler) != 0) {

          Error_Handler();

        }

        for (int i = 0; i < PCM_MID_BUF; i++) {

          FifoWrite_B(pcmSmallBuffer[i]);

        }

        micRxFlag = 0;

      }

      if (micRxFlag == 2) {

        if (PDM_Filter( & PDMbuff[PDM_BUF / 2], & pcmSmallBuffer[0], & PDM1_filter_handler) != 0) {

          Error_Handler();

        }

        for (int i = 0; i < PCM_MID_BUF; i++) {

          FifoWrite_B(pcmSmallBuffer[i]);

        }

        micRxFlag = 0;
      }
    }

    //Transmit mode
    if (espTxFlag == 1 && acc_aqcuisition_flag == 1) {

      prepare_mic_data_to_send();
      prepare_acc_data_to_send();

      for (int i = 0; i < 80; i++) {

        HAL_UART_Transmit( & huart4, & mic_data_to_send[1201 * i], 1201, HAL_MAX_DELAY);
        HAL_Delay(1);

      }

      for (int i = 0; i < 12; i++) {

        HAL_UART_Transmit( & huart4, & acc_data_to_send[1201 * i], 1201, HAL_MAX_DELAY);
        HAL_Delay(1);

      }

      espTxFlag = 0;
      fifoIterator = 0;
      acc_aqcuisition_flag = 0;
      acc_iterator = 0;

    }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {
    0
  };
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {
    0
  };

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 14;
  RCC_OscInitStruct.PLL.PLLN = 214;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  if (HAL_RCC_OscConfig( & RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
    RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig( & RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {}
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
void assert_failed(uint8_t * file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
