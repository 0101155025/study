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
#include <string.h>
#include "timers.h"
#include "usart.h"
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
/* USER CODE BEGIN Variables */
// 软件定时器句柄
TimerHandle_t myTimer = NULL;
// 用于标识定时器是否启动的标志
BaseType_t xTimerStarted = pdFALSE;
volatile uint8_t dmaState = 1;
uint8_t buffer[128];
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
void vMyTimerCallback(TimerHandle_t xTimer);
void vCreateAndStartTimer(void);
void vApplyTask(void *parames);
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
  xTaskCreate(vApplyTask,"applyTask",256,NULL,osPriorityNormal,NULL);
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
void vMyTimerCallback(TimerHandle_t xTimer)
{
  uint8_t txData[] = "timer";
	uint8_t len = sizeof(txData);
	memcpy(buffer,txData,len);
  taskENTER_CRITICAL();
  if (dmaState == 1 )
  {
    dmaState = 0;
    taskEXIT_CRITICAL();
    HAL_UART_Transmit_DMA(&huart2,buffer,sizeof(txData) - 1);
  }else taskEXIT_CRITICAL();
}
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2)
  {
    __HAL_UART_CLEAR_OREFLAG(huart);
    HAL_UART_Init(huart);
    dmaState = 1;
  }
}
void vCreateAndStartTimer(void)
{
  uint8_t wrongData1[] = "2";
  uint8_t wrongData2[] = "3";
  uint8_t rightData[] = "1";
  // 创建一个周期为1000ms的软件定时器
  myTimer = xTimerCreate("MyTimer", pdMS_TO_TICKS(1000), pdTRUE, (void *)0, vMyTimerCallback);
  if (myTimer != NULL)
  {
    // 启动定时器
    xTimerStarted = xTimerStart(myTimer, pdMS_TO_TICKS(200));
    if (xTimerStarted != pdPASS)
    {
      // 定时器启动失败处理
      taskENTER_CRITICAL();
      if (dmaState == 1)
      {
        dmaState = 0;
        taskEXIT_CRITICAL();
        HAL_UART_Transmit_DMA(&huart2,wrongData1,sizeof(wrongData1) - 1);
      }else taskEXIT_CRITICAL();
    }else
    {
      taskENTER_CRITICAL();
      if (dmaState == 1)
      {

        dmaState = 0;
        taskEXIT_CRITICAL();
        HAL_UART_Transmit_DMA(&huart2,rightData,sizeof(rightData) - 1);
      }else taskEXIT_CRITICAL();
    }
  }else
  {
    taskENTER_CRITICAL();
    if (dmaState == 1)
    {

      dmaState = 0;
      taskEXIT_CRITICAL();
      HAL_UART_Transmit_DMA(&huart2,wrongData2,sizeof(wrongData2) - 1);
    }else taskEXIT_CRITICAL();
  }
}
void vApplyTask(void *parames)
{
  vCreateAndStartTimer();
  for (;;)
  {
    vTaskDelay(500);
  }
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2) dmaState = 1;
}
/* USER CODE END Application */

