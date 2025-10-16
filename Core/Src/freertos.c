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
#include "queue.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	none,
	motor3508,
	motor6020,
	motor2006,
} MotorType;
struct DMAMessage{
	int type; // type = 0,传电机
	MotorType motortype;
	float filtered_data;
	float range; 
};
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
MotorType mt = motor3508;
QueueHandle_t xMessageQueue;
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

void ButtonTask(void *parames){
	
	for(;;){
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_SET){
			vTaskDelay(2000);
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_SET) g_button_cnt--;
			else g_button_cnt++;
		}
		if(g_button_cnt > 8) g_button_cnt -= 8;
		if(g_button_cnt <= 0)g_button_cnt += 8;
		vTaskDelay(50);
	}
}
void MotorTask(void *parames){
	for(;;){
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_SET){
			switch(mt){
				case motor3508 :
					mt = motor6020;
					break;
				case motor6020 :
					mt = motor2006;
					break;
				case motor2006 :
					mt = motor3508;
					break;
				default:
					break;
			}
			struct DMAMessage Mdata;
			Mdata.type = 0;
			Mdata.motortype = mt;
			Mdata.filtered_data = 0;
			Mdata.range = 0;
			if(xQueueSend(xMessageQueue,&Mdata,portMAX_DELAY) != pdPASS){
				/* 错误情况,有时间写个处理错误的任务 */
			}
			
		}
		vTaskDelay(50);
	}
}
void DMA_TASK(void *parames){
	struct DMAMessage Ddata;
	for(;;){
		xQueueReceive(xMessageQueue,&Ddata,portMAX_DELAY);
		if(Ddata.type == 0){
			strlen();
			HAL_UART_Transmit_DMA()
		}
		vTaskDelay(50);
	}
}
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
	
	if(xTaskCreate(ButtonTask,"ButtonTask",128,NULL,osPriorityNormal,NULL) != pdPASS){
		
	}
	
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

/* USER CODE END Application */

