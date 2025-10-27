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
#include "queue.h"
#include "semphr.h"
#include "usart.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ID 110
#define MSG 110
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
QueueHandle_t xQueue;
typedef struct
{
  uint8_t idData;
  uint8_t msg;
} MyData;
uint8_t rx_buffer[2];
uint8_t tx_buffer[2];
volatile uint8_t DmaRxCplt = 1;
volatile uint8_t DmaTxCplt = 1;
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
void ReceiveFromUartTask(void *parames);
void ProcessQueueTask(void *parames);
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
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
  xQueue = xQueueCreate(5,sizeof(MyData));
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  xTaskCreate(ReceiveFromUartTask,"ReceiveFromUartTask",128,NULL,2,NULL);
  xTaskCreate(ProcessQueueTask,"ProcessQueueTask",128,NULL,2,NULL);
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
void ReceiveFromUartTask(void *parames)
{
  MyData Rxdata;
  Rxdata.idData = ID;
  for (;;)
  {
    DmaRxCplt = 0;
    HAL_UART_Receive_DMA(&huart2,rx_buffer,sizeof(MyData));
    while (DmaRxCplt == 0) taskYIELD();
    if (rx_buffer[0] ==  ID)
    {
      Rxdata.idData = rx_buffer[0];
      Rxdata.msg = rx_buffer[1];
      xQueueSend(xQueue,&Rxdata,portMAX_DELAY);
    }
    vTaskDelay(50);
  }
}
void ProcessQueueTask(void *parames)
{
  MyData PqData;
  for (;;)
  {
    while(xQueueReceive(xQueue,&PqData,portMAX_DELAY) == pdFALSE) taskYIELD();
    tx_buffer[0] = PqData.idData;
    tx_buffer[1] = PqData.msg;
    while (DmaTxCplt == 0) taskYIELD();
    DmaTxCplt = 0;
    HAL_UART_Transmit_DMA(&huart2,tx_buffer,sizeof(MyData));
    while (DmaTxCplt == 0) taskYIELD();
    vTaskDelay(50);
  }
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2) DmaRxCplt = 1;
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart == &huart2) DmaTxCplt = 1;
}
/* USER CODE END Application */

