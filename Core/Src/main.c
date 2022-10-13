/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "i2s.h"
#include "pdm2pcm.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mems_configuration.h"
#include "iks02a1_motion_sensors.h"
#include "pdm2pcm_glo.h"
#include "test_lib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PDM_BUF 256
#define PCM_MID_BUF 16
#define PCM_BIG_BUF 2*48000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
		uint8_t RxFlag_B = 0; //Flag used in i2c callbacks.

		uint8_t PDM_Rxbuf_B[PDM_BUF]; //Buffer storing pdm sound values
		uint16_t pcmMidBuf_B[PCM_MID_BUF]; //Buffer storing pcm values after conversion from pdm.
		uint16_t pcm_big_buf_B[PCM_BIG_BUF];//Big buffer. Stores 0.1 s sound probe to send.

		uint32_t pdm2pcm_status_half; //Status of conversion - half complete.
		uint32_t pdm2pcm_status_cplt; //Status of conversion - complete.

		uint32_t fifo_w_ptr_B = 0; //Fifo pointer used to iterate in big buffer.

		uint8_t espTxFlag=0;// Esp flag. This flag allows to switch modes between reading values and transimiting to esp.

		  uint8_t tab[2*PCM_BIG_BUF]; //Array of compressed data.

		  uint8_t V = 1; // Compression protocol version.


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */


//Function storing  sound data in a big buffer
//Whenever the big buffer is full espTxFlag is set
//to start transmiting
void FifoWrite_B(uint16_t data) {


	if(fifo_w_ptr_B !=PCM_BIG_BUF){
		pcm_big_buf_B[fifo_w_ptr_B] = data;
		fifo_w_ptr_B++;
	}
	if(fifo_w_ptr_B == PCM_BIG_BUF){
		espTxFlag = 1;
	}
}

//Callbacks allowing pipelining in the i2s interface.
//Its used to ensure constant data flow.
void HAL_I2S_RxHalfCpltCallback (I2S_HandleTypeDef *hi1s) {
	  if(hi1s->Instance ==SPI1){
	  RxFlag_B = 1;
	  }
}

void HAL_I2S_RxCpltCallback (I2S_HandleTypeDef *hi1s) {
	  if(hi1s->Instance ==SPI1){
	  RxFlag_B = 2;
	  }
}


//Uart complete callback.
//This callback informs us that all data has been sent
//and sets the fifo pointer to 0 which allows us to start reading values again.
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart->Instance == USART3){

		fifo_w_ptr_B =0;
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
int main(void)
{
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
  MX_TIM3_Init();
  MX_DMA_Init();
  MX_I2S1_Init();
  MX_USART3_UART_Init();
  MX_CRC_Init();
  MX_PDM2PCM_Init();
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */

  HAL_I2S_Receive_DMA(&hi2s1, (uint8_t*)PDM_Rxbuf_B, PDM_BUF);

  MX_MEMS_Init();
  IKS02A1_MOTION_SENSOR_Axes_t acceleration;
  char dataOut[300];
  uint16_t databufforlentgh;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {



	  //Read values mode
	  if(espTxFlag == 0){
		  if (RxFlag_B == 1) {

			  pdm2pcm_status_half = PDM_Filter(&PDM_Rxbuf_B[0], &pcmMidBuf_B[0], &PDM1_filter_handler);
					 for (int i = 0; i < PCM_MID_BUF; i++) {
						 FifoWrite_B(pcmMidBuf_B[i]);
					 }

			  RxFlag_B= 0;
				//HAL_GPIO_WritePin(TX_OUTPUT_GPIO_Port, TX_OUTPUT_Pin, GPIO_PIN_RESET);
			}
		  if (RxFlag_B == 2) {
			//HAL_GPIO_WritePin(TX_OUTPUT_GPIO_Port, TX_OUTPUT_Pin, GPIO_PIN_SET);
			  pdm2pcm_status_cplt = PDM_Filter(&PDM_Rxbuf_B[192], &pcmMidBuf_B[0], &PDM1_filter_handler);
				 for (int i = 0; i < PCM_MID_BUF; i++) {
					 FifoWrite_B(pcmMidBuf_B[i]);
				 }
			  RxFlag_B = 0;
			//HAL_GPIO_WritePin(TX_OUTPUT_GPIO_Port, TX_OUTPUT_Pin, GPIO_PIN_RESET);
		}
	  }


	  //Transmit mode
	  if(espTxFlag == 1){
		  for(int i = 0 ; i < PCM_BIG_BUF ; i++){


			  	  	  //This function reads values from accelerometer.
//		  	  	  	  Accelero_Get_Values(0, &acceleration);

//		  	  	  	  databufforlentgh = sprintf(dataOut, "%d;%d;%d;%d\r",
//		  	  	               	   	   	   	   	acceleration.x, acceleration.y, acceleration.z, pcm_big_buf_B[i]);

		  	  	  	  //Function compressing data
		  	  	  	  update_val(tab,  pcm_big_buf_B[i], 2*i);

//		  	  	  		  HAL_UART_Transmit_IT(&huart3, tab, 9);
//
//		  			  HAL_Delay(100);

		  }
		  for(int i = 0 ; i < 2*PCM_BIG_BUF ;i=i+8){
			  HAL_UART_Transmit_IT(&huart3, &tab[i], 8);
			  HAL_Delay(1);
		  }


			espTxFlag = 0;

	  }

//	  HAL_Delay(2000);

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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 125;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
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
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

