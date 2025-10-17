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
#include "stdio.h"
#include "string.h"
#include "tim.h"
#include "can.h"
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	motor3508,
	motor6020,
	motor2006,
} MotorType;

/* USER CODE END PTD */
#define ALPHA 0.3f
#define FILTER_TASK_PERIOD (100 / portTICK_PERIOD_MS)
/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
MotorType mt = motor3508;
char buf[128];
QueueHandle_t xMessageQueue;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8] = {0};
uint8_t RxData[8];
uint32_t TxMailBox;
TaskHandle_t xFilterTaskHandle;
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

void ButtonTask(void *parames);
void MotorTask(void *parames);
void CAN_SendData(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan);
void CAN_Filter_Init(void);
void CANTask(void *parames);
void GenerateNoisyTask(void *parames){
	static double x = 0.0;
	double noise;
	static double raw;
	for(;;){
		noise = 0.1 + (2.0 * rand() / RAND_MAX - 1.0);
		raw = sqrt(x) + noise;
		x += 0.5;
		if(x > 10.0) x = 0.0;
		xTaskNotify(xFilterTaskHandle,(uint32_t)&raw,eSetValueWithOverwrite);
		vTaskDelay(50);
	}
}
void FilterTask(void *parames){
	static double last_filtered = 0.0;
	double current_filtered;
	double max = 0.0,min = 0.0;
	double range = 0.0;
	uint32_t RxData;
	uint8_t TxData[2];
	for(;;){
		xTaskNotifyWait(0,0xffffffff,&RxData,portMAX_DELAY);
		double *RawData = (double *)RxData;
		current_filtered = ALPHA * (*RawData) + (1 - ALPHA) * last_filtered;
		last_filtered = current_filtered;
		if(current_filtered > max) max = current_filtered;
		if(current_filtered < min) min = current_filtered;
		range = max - min;
		
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
		/* 有时间写个错误处理函数 */
	}
	if(xTaskCreate(MotorTask,"MotorTask",128,NULL,osPriorityNormal,NULL) != pdPASS){
		/* 有时间写个错误处理函数 */
	}
	if(xTaskCreate(CANTask,"CANTask",128,NULL,osPriorityNormal,NULL) != pdPASS){
		/* 有时间写个错误处理函数 */
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
void ButtonTask(void *parames){
	int curtime;
	for(;;){
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_SET){
			curtime = HAL_GetTick();
			while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_SET);
			if(HAL_GetTick() - curtime >= 2000) g_button_cnt += 125;
			else g_button_cnt -= 125;
		}
		if(g_button_cnt > 1000) g_button_cnt = 125 ;
		if(g_button_cnt < 125)g_button_cnt = 1000;
		vTaskDelay(50);
	}
}
void MotorTask(void *parames){
	for(;;){
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_SET){
			vTaskDelay(20);
			while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_SET);
			switch(mt){
				case motor3508 :
					mt = motor6020;
					snprintf(buf,sizeof(buf),"当前类型:%d/r/n",6020);
					HAL_UART_Transmit_DMA(&huart2,(uint8_t *)buf,strlen(buf));
					break;
				case motor6020 :
					mt = motor2006;
					snprintf(buf,sizeof(buf),"当前类型:%d/r/n",2006);
					HAL_UART_Transmit_DMA(&huart2,(uint8_t *)buf,strlen(buf));
					break;
				case motor2006 :
					mt = motor3508;
					snprintf(buf,sizeof(buf),"当前类型:%d/r/n",3508);
					HAL_UART_Transmit_DMA(&huart2,(uint8_t *)buf,strlen(buf));
					break;
			}
			
		}
		vTaskDelay(50);
	}
}
void CAN_SendData(void){
	TxHeader.StdId = 0x200;         // 标准帧id
	TxHeader.DLC = 8;  							// 数据长度
	TxHeader.ExtId = 0;							// 扩展帧id
	TxHeader.IDE = CAN_ID_STD;      // 标准帧还是扩展帧
	TxHeader.RTR = CAN_RTR_DATA;    // 是标准帧还是远程帧
	TxData[0] = 1;       // 发一个π的小数点
	TxData[1] = 4;
	TxData[2] = 1;
	TxData[3] = 5;
	TxData[4] = 9;
	TxData[5] = 2;
	TxData[6] = 6;
	TxData[7] = 5;
	if(HAL_CAN_AddTxMessage(&hcan,&TxHeader,TxData,&TxMailBox) == HAL_OK){};
}
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
}
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
    HAL_UART_Transmit(&huart2,TxData,sizeof(TxData), 100);
}
void CAN_Filter_Init(void) {
    CAN_FilterTypeDef sFilterConfig;      											// 定义过滤器
    sFilterConfig.FilterBank = 0;																// 过滤器组编号
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0 ;     // 匹配成功后消息存入哪个fifo
    sFilterConfig.FilterIdHigh = (0x200 << 5) & 0xffff;					// 过滤器id高16位    期望收到数据的id
    sFilterConfig.FilterIdLow = 0x0000;													// 过滤器id低16位
    sFilterConfig.FilterMaskIdHigh = 0xffff;										// 过滤器掩码高16位  掩码告诉你哪些位需要严格匹配
    sFilterConfig.FilterMaskIdLow = 0x0000;											// 过滤器掩码低16位
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;						// 过滤器模式(掩码模式/列表模式)
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;					// 过滤器位宽(用32位过滤器还是16位)
    sFilterConfig.SlaveStartFilterBank = 14;										// 划分主,从过滤器(一共14个),主从can1接收,从can2
	  sFilterConfig.FilterActivation = ENABLE;										// 过滤器使能
    HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);								// 配置过滤器
	  uint32_t can_it = CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY;
    HAL_CAN_ActivateNotification(&hcan, can_it);                // 激活can中断
    HAL_CAN_Start(&hcan);        // 启动can
		HAL_CAN_GetState(&hcan);     // 获取can状态
		HAL_Delay(10);
}
void CANTask(void *parames){
	CAN_Filter_Init();
	for(;;){
		CAN_SendData();
		vTaskDelay(1000);
	}
}
/* USER CODE END Application */

