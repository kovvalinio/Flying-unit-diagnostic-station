/**
 ******************************************************************************
 * File Name          : app_mems.c
 * Description        : This file provides code for the configuration
 *                      of the STMicroelectronics.X-CUBE-MEMS1.9.1.0 instances.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under Software License Agreement SLA0077,
 * the “License�?. You may not use this component except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        www.st.com/sla0077
 *
 ******************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mems_configuration.h"
#include "main.h"
#include <stdio.h>

#include "iks02a1_motion_sensors.h"
#include "stm32f7xx_nucleo.h"
#include "math.h"
#include "usart.h"

/* Private typedef -----------------------------------------------------------*/
typedef struct displayFloatToInt_s {
  int8_t sign; /* 0 means positive, 1 means negative*/
  uint32_t  out_int;
  uint32_t  out_dec;
} displayFloatToInt_t;

/* Private define ------------------------------------------------------------*/
#define MAX_BUF_SIZE 256

/* Private variables ---------------------------------------------------------*/
static volatile uint8_t PushButtonDetected = 0;
static uint8_t verbose = 1;  /* Verbose output to UART terminal ON/OFF. */
static IKS02A1_MOTION_SENSOR_Capabilities_t MotionCapabilities[IKS02A1_MOTION_INSTANCES_NBR];
static char dataOut[MAX_BUF_SIZE];
static int32_t PushButtonState = GPIO_PIN_RESET;
static uint16_t databufforlentgh;

/* Private function prototypes -----------------------------------------------*/
static void floatToInt(float in, displayFloatToInt_t *out_value, int32_t dec_prec);
static void Accelero_Sensor_Handler(uint32_t Instance);
static void Gyro_Sensor_Handler(uint32_t Instance);
static void Magneto_Sensor_Handler(uint32_t Instance);
static void MX_IKS02A1_DataLogTerminal_Init(void);
static void MX_IKS02A1_DataLogTerminal_Process(void);
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
//
//}

void MX_MEMS_Init(void)
{
  /* USER CODE BEGIN SV */

  /* USER CODE END SV */

  /* USER CODE BEGIN MEMS_Init_PreTreatment */

  /* USER CODE END MEMS_Init_PreTreatment */

  /* Initialize the peripherals and the MEMS components */

  MX_IKS02A1_DataLogTerminal_Init();

  /* USER CODE BEGIN MEMS_Init_PostTreatment */

  /* USER CODE END MEMS_Init_PostTreatment */
}

/*
 * LM background task
 */
void MX_MEMS_Process(void)
{
  /* USER CODE BEGIN MEMS_Process_PreTreatment */

  /* USER CODE END MEMS_Process_PreTreatment */

  MX_IKS02A1_DataLogTerminal_Process();

  /* USER CODE BEGIN MEMS_Process_PostTreatment */

  /* USER CODE END MEMS_Process_PostTreatment */
}

/**
  * @brief  Initialize the DataLogTerminal application
  * @retval None
  */
void MX_IKS02A1_DataLogTerminal_Init(void)
{
  displayFloatToInt_t out_value_odr;
  int i;

  /* Initialize LED */
  BSP_LED_Init(LED2);

  /* Initialize button */
  BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Check what is the Push Button State when the button is not pressed. It can change across families */
  PushButtonState = (BSP_PB_GetState(BUTTON_KEY)) ?  0 : 1;

  /* Initialize Virtual COM Port */


  IKS02A1_MOTION_SENSOR_Init(IKS02A1_ISM330DHCX_0, MOTION_ACCELERO | MOTION_GYRO);

  IKS02A1_MOTION_SENSOR_Init(IKS02A1_IIS2DLPC_0, MOTION_ACCELERO);

  IKS02A1_MOTION_SENSOR_Init(IKS02A1_IIS2MDC_0, MOTION_MAGNETO);


}

/**
  * @brief  BSP Push Button callback
  * @param  Button Specifies the pin connected EXTI line
  * @retval None.
  */
void BSP_PB_Callback(Button_TypeDef Button)
{
  PushButtonDetected = 1;
}

/**
  * @brief  Process of the DataLogTerminal application
  * @retval None
  */
void MX_IKS02A1_DataLogTerminal_Process(void)
{
  int i;

//  if (PushButtonDetected != 0U)
//  {
//    /* Debouncing */
//    HAL_Delay(50);
//
//    /* Wait until the button is released */
//    while ((BSP_PB_GetState( BUTTON_KEY ) == PushButtonState));
//
//    /* Debouncing */
//    HAL_Delay(50);
//
//    /* Reset Interrupt flag */
//    PushButtonDetected = 0;
//
//    /* Do nothing */
//  }

  for(i = 0; i < 2 ; i++)
  {

      Accelero_Sensor_Handler(i);
      HAL_Delay( 1000 );

  }


}

/**
  * @brief  Splits a float into two integer values.
  * @param  in the float value as input
  * @param  out_value the pointer to the output integer structure
  * @param  dec_prec the decimal precision to be used
  * @retval None
  */
static void floatToInt(float in, displayFloatToInt_t *out_value, int32_t dec_prec)
{
  if(in >= 0.0f)
  {
    out_value->sign = 0;
  }else
  {
    out_value->sign = 1;
    in = -in;
  }

  in = in + (0.5f / pow(10, dec_prec));
  out_value->out_int = (int32_t)in;
  in = in - (float)(out_value->out_int);
  out_value->out_dec = (int32_t)trunc(in * pow(10, dec_prec));
}

/**
  * @brief  Handles the accelerometer axes data getting/sending
  * @param  Instance the device instance
  * @retval None
  */
static void Accelero_Sensor_Handler(uint32_t Instance)
{
  float odr;
  int32_t fullScale;
  IKS02A1_MOTION_SENSOR_Axes_t acceleration;
  displayFloatToInt_t out_value;
  uint8_t whoami;

  if (IKS02A1_MOTION_SENSOR_GetAxes(Instance, MOTION_ACCELERO, &acceleration))
  {
    databufforlentgh = sprintf(dataOut, "\r\nACC[%d]: Error\r\n", Instance);
  }
  else
  {
//    databufforlentgh = sprintf(dataOut, "%d\r", acceleration.z);
    databufforlentgh = sprintf(dataOut, "ACC_X[%d]: %d, ACC_Y[%d]: %d, ACC_Z[%d]: %d\r", Instance,
             acceleration.x, Instance, acceleration.y, Instance, acceleration.z);
HAL_UART_Transmit_IT(&huart3, (uint8_t*)dataOut, databufforlentgh);
  }




}

/**
  * @brief  Handles the gyroscope axes data getting/sending
  * @param  Instance the device instance
  * @retval None
  */


/**
  * @brief  Handles the magneto axes data getting/sending
  * @param  Instance the device instance
  * @retval None
  */


#ifdef __cplusplus
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
