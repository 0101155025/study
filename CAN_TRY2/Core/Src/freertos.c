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
#include "can.h"
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
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t CAN_TxData[2] = {0};
uint8_t RxData[2];
uint32_t TxMailBox;
int dmaState = 0;
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
void CAN_SendData(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void CAN_Filter_Init(void);
void CANTask(void *parames);
void dataDealTask(void *parames);
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
	xTaskCreate(CANTask,"CanTask",256,NULL,osPriorityNormal,NULL);
	xTaskCreate(dataDealTask,"dataDealTask",256,NULL,osPriorityNormal,NULL);
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
void CAN_SendData(void){
	uint32_t lastLightOffTime = HAL_GetTick();
	uint32_t current_time;
		for (int i = 0;i < 5;i++)
		{
			current_time = HAL_GetTick();
			if (current_time - lastLightOffTime < 2000) vTaskDelay(2000 - (current_time - lastLightOffTime));
			TxHeader.StdId = 0x300 + i;         // 标准帧id
			TxHeader.DLC = 2;  							// 数据长度
			TxHeader.ExtId = 0;							// 扩展帧id
			TxHeader.IDE = CAN_ID_STD;      // 标准帧还是扩展帧
			TxHeader.RTR = CAN_RTR_DATA;	 // 是标准帧还是远程帧
			if(HAL_CAN_AddTxMessage(&hcan,&TxHeader,CAN_TxData,&TxMailBox) != HAL_OK){
			/* 有时间写个错误处理 */
			};
			lastLightOffTime = HAL_GetTick();
		}

}
// 收到数据后存到RxHeader和RxData
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef* hcan){
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
	UBaseType_t uxSavedInterruptStatus; //创建变量
	uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
	dmaState = 1;
	taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}
// 三个邮箱发送完数据后的中断,这里用来发送下一轮数据
void CAN_Filter_Init(void) {
    CAN_FilterTypeDef sFilterConfig;      								// 定义过滤器
    sFilterConfig.FilterBank = 0;										// 过滤器组编号
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0 ;			   // 匹配成功后消息存入哪个fifo
    sFilterConfig.FilterIdHigh = 0x0000;			    	// 过滤器id高16位    期望收到数据的id
    sFilterConfig.FilterIdLow = 0x0000;									// 过滤器id低16位
    sFilterConfig.FilterMaskIdHigh = 0x0000;							// 过滤器掩码高16位  掩码告诉你哪些位需要严格匹配
    sFilterConfig.FilterMaskIdLow = 0x0000;								// 过滤器掩码低16位
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;					// 过滤器模式(掩码模式/列表模式)
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;					// 过滤器位宽(用32位过滤器还是16位)
    sFilterConfig.SlaveStartFilterBank = 14;							// 划分主,从过滤器(一共14个),主从can1接收,从can2
	  sFilterConfig.FilterActivation = ENABLE;							// 过滤器使能
    HAL_CAN_ConfigFilter(&hcan, &sFilterConfig);						// 配置过滤器
	  uint32_t can_it = CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_TX_MAILBOX_EMPTY;
    HAL_CAN_ActivateNotification(&hcan, can_it);                // 激活can中断
    HAL_CAN_Start(&hcan);        // 启动can
		HAL_CAN_GetState(&hcan);     // 获取can状态
		vTaskDelay(10);
}
void CANTask(void *parames){
	CAN_Filter_Init();				//初始化外设
	for(;;){
		CAN_SendData();
		vTaskDelay(50);
	}
}
void dataDealTask(void *parames)
{
	for (;;)
	{
		taskENTER_CRITICAL();
		if (dmaState == 1)
		{
			taskEXIT_CRITICAL();
			switch (RxHeader.StdId)
			{
			case 0x300:
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
				vTaskDelay(1000);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
				break;
			case 0x301:
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
				vTaskDelay(1000);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
				break;
			case 0x303:
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8 | GPIO_PIN_10,GPIO_PIN_SET);
				vTaskDelay(1000);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8 | GPIO_PIN_10,GPIO_PIN_RESET);
				break;
			}
			taskENTER_CRITICAL();
			dmaState = 0;
			taskEXIT_CRITICAL();
		}
		else taskEXIT_CRITICAL();
		vTaskDelay(50);
	}
}
/* USER CODE END Application */

