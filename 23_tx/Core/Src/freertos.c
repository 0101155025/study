/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	NONE,
	ALLON,
	REDON,
	GREENON,
	ALLOFF,
}LEDState;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
LEDState ledstate = NONE;
int dmaTxState = 1;
int dmaRxState = 1;
	uint8_t rx_buf[4];
/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void RxTask(void *parames);
void TxTask(void *parames);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	xTaskCreate(RxTask,"RxTask",128,NULL,osPriorityNormal,NULL);
	xTaskCreate(TxTask,"TxTask",128,NULL,osPriorityNormal,NULL);
	HAL_UART_Receive_DMA(&huart2,rx_buf,sizeof(rx_buf));
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void RxTask(void *parames){
	for(;;){
		taskENTER_CRITICAL();
		if(dmaRxState == 1){
			dmaRxState = 0;
			taskEXIT_CRITICAL();
			HAL_UART_Receive_DMA(&huart2,rx_buf,sizeof(rx_buf));
		}else{
			taskEXIT_CRITICAL();
			vTaskDelay(10);
			continue;
		}
		if(rx_buf[1] == 0x01){
			taskENTER_CRITICAL();
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == GPIO_PIN_SET) ledstate = ALLON;
			else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == GPIO_PIN_SET && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == GPIO_PIN_RESET) ledstate = GREENON;
			else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == GPIO_PIN_SET) ledstate = REDON;
			else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == GPIO_PIN_RESET && HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == GPIO_PIN_RESET) ledstate = ALLOFF;
			taskEXIT_CRITICAL();
		}else if(rx_buf[1] == 0x02){
			taskENTER_CRITICAL();
			if(rx_buf[2] == 0x01) ledstate = REDON;
			else if(rx_buf[2] == 0x02) ledstate = GREENON;
			else if(rx_buf[2] == 0x03) ledstate = ALLON;
			else if(rx_buf[2] == 0x04) ledstate = ALLOFF;
			taskEXIT_CRITICAL();
		}
		vTaskDelay(50);
	}
}
void TxTask(void *parames){
	LEDState tempstate = NONE;
	char tx_buf[9]="NONE\n";
	for(;;){
		taskENTER_CRITICAL();
		tempstate = ledstate;
		taskEXIT_CRITICAL();
		switch(tempstate){
			case ALLON:
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == GPIO_PIN_RESET) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == GPIO_PIN_RESET) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
				snprintf(tx_buf,sizeof(tx_buf),"ALLON\n");
				break;
			case GREENON:
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == GPIO_PIN_RESET) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == GPIO_PIN_SET) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
				snprintf(tx_buf,sizeof(tx_buf),"GREENON\n");
				break;
			case REDON:
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == GPIO_PIN_SET) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == GPIO_PIN_RESET) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
				snprintf(tx_buf,sizeof(tx_buf),"REDON\n");
				break;
			case ALLOFF:
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8) == GPIO_PIN_SET) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_10) == GPIO_PIN_SET) HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
				snprintf(tx_buf,sizeof(tx_buf),"ALLOFF\n");
				break;
			case NONE:
				break;
		}
		taskENTER_CRITICAL();
		if(dmaTxState == 0){
			taskEXIT_CRITICAL();
			vTaskDelay(50);
		}
		else{
			dmaTxState = 0;
			taskEXIT_CRITICAL();
			HAL_UART_Transmit_DMA(&huart2,(uint8_t *)tx_buf,strlen(tx_buf));
		}
		vTaskDelay(1000);
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart2) dmaTxState = 1;
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart2) dmaRxState = 1;
}
/* USER CODE END Application */

