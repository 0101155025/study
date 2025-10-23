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
#include "can.h"
#include <string.h>
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
uint8_t CAN_TxData[8] = {0};
uint8_t RxData[8];
uint32_t TxMailBox;
int canTrans = 1;
volatile int canState = 0;
int angle = 0;
int speed = 0;
int current = 0;
volatile int dmaState = 1;
int a = 0;
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
void CAN_Filter_Init(void);
void CANTask(void *parames);
void TransTask(void *parames);
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
	xTaskCreate(CANTask,"CANTask",128,NULL,osPriorityNormal,NULL);
	xTaskCreate(TransTask,"TransTask",128,NULL,osPriorityNormal,NULL);
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
	if(canTrans == 1){
		TxHeader.StdId = 0x201;         // 标准帧id
		TxHeader.DLC = 8;  							// 数据长度
		TxHeader.ExtId = 0;							// 扩展帧id
		TxHeader.IDE = CAN_ID_STD;      // 标准帧还是扩展帧 
		TxHeader.RTR = CAN_RTR_DATA;    // 是标准帧还是远程帧
		CAN_TxData[0] = 0x11;      			  // 发一个π的小数点
		CAN_TxData[1] = 0x33;
		CAN_TxData[2] = 0x00;
		CAN_TxData[3] = 0x00;
		CAN_TxData[4] = 0xAA;
		CAN_TxData[5] = 0xBB;
		CAN_TxData[6] = 0xCC;
		CAN_TxData[7] = 0x00;
		canTrans = 2;
	}else if(canTrans == 2){
		TxHeader.StdId = 0x202;         // 标准帧id
		TxHeader.DLC = 8;  							// 数据长度
		TxHeader.ExtId = 0;							// 扩展帧id
		TxHeader.IDE = CAN_ID_STD;      // 标准帧还是扩展帧 
		TxHeader.RTR = CAN_RTR_DATA;    // 是标准帧还是远程帧
		CAN_TxData[0] = 0x22;      			  // 发一个π的小数点
		CAN_TxData[1] = 0x44;
		CAN_TxData[2] = 0x00;
		CAN_TxData[3] = 0x11;
		CAN_TxData[4] = 0xAA;
		CAN_TxData[5] = 0xBB;
		CAN_TxData[6] = 0xDD;
		CAN_TxData[7] = 0x00;
		canTrans = 3;
	}else if(canTrans == 3){
		TxHeader.StdId = 0x203;         // 标准帧id
		TxHeader.DLC = 8;  							// 数据长度
		TxHeader.ExtId = 0;							// 扩展帧id
		TxHeader.IDE = CAN_ID_STD;      // 标准帧还是扩展帧 
		TxHeader.RTR = CAN_RTR_DATA;    // 是标准帧还是远程帧
		CAN_TxData[0] = 0x12;      			  // 发一个π的小数点
		CAN_TxData[1] = 0x23;
		CAN_TxData[2] = 0x00;
		CAN_TxData[3] = 0x00;
		CAN_TxData[4] = 0xCF;
		CAN_TxData[5] = 0xA1;
		CAN_TxData[6] = 0xEA;
		CAN_TxData[7] = 0x00;
		canTrans = 1;
	}
	if(HAL_CAN_AddTxMessage(&hcan,&TxHeader,CAN_TxData,&TxMailBox) != HAL_OK){
	/* 有时间写个错误处理 */
		a++;
	};
}
// 收到数据后存到RxHeader和RxData
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
	UBaseType_t uxSavedInterruptStatus; //创建变量
	uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
	canState = 1;
	taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
}
// 三个邮箱发送完数据后的中断,这里用来发送下一轮数据
void CAN_Filter_Init(void) {
    CAN_FilterTypeDef sFilterConfig;      											// 定义过滤器
    sFilterConfig.FilterBank = 0;																// 过滤器组编号
    sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0 ;     // 匹配成功后消息存入哪个fifo
    sFilterConfig.FilterIdHigh = ((0x201 | 0x202 | 0x203) << 5) & 0xffff;					// 过滤器id高16位    期望收到数据的id
    sFilterConfig.FilterIdLow = 0x0000;													// 过滤器id低16位
    sFilterConfig.FilterMaskIdHigh = 0x0000;										// 过滤器掩码高16位  掩码告诉你哪些位需要严格匹配
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
		vTaskDelay(10);
}
void CANTask(void *parames){
	CAN_Filter_Init();						//初始化外设
	for(;;){						
		CAN_SendData();
		vTaskDelay(2000);						//2s发一次
	}
}
void TransTask(void *parames){
	CAN_RxHeaderTypeDef tempHeader;
	int tempAngle = 0;
	int tempCurrent = 0;
	int tempSpeed = 0;
	int tempTemp = 0;
	char TxData[128];
	uint32_t uarTXlen = 0;
	for(;;){
		if(RxHeader.StdId == 0x201 || RxHeader.StdId == 0x202 || RxHeader.StdId == 0x203){
			taskENTER_CRITICAL();
			if(canState == 1){
				canState = 0;
//				tempHeader = RxHeader;
				tempAngle += RxData[0];
				tempAngle -= RxData[1];
				tempSpeed += RxData[2];
				tempSpeed -= RxData[3];
				tempCurrent += RxData[4];
				tempCurrent -= RxData[5];
				tempTemp = RxData[6];
				taskEXIT_CRITICAL(); 
				uarTXlen = snprintf(TxData,sizeof(TxData),
					"Angle=%d\nSpeed=%d\nCurrent=%d\nTemp=%d\n",
					tempAngle,tempSpeed,tempCurrent,tempTemp);
				taskENTER_CRITICAL();
				if(dmaState == 1){
					dmaState = 0;
					taskEXIT_CRITICAL(); 
					HAL_UART_Transmit_DMA(&huart2,(uint8_t *)TxData,uarTXlen);
				}else taskEXIT_CRITICAL(); 
			}else taskEXIT_CRITICAL();
		}else // 红灯亮,理应不可能
		vTaskDelay(50);
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart2){
		UBaseType_t uxSavedInterruptStatus; //创建变量
		uxSavedInterruptStatus = taskENTER_CRITICAL_FROM_ISR();
		dmaState = 1;
		taskEXIT_CRITICAL_FROM_ISR(uxSavedInterruptStatus);
	}
}
/* USER CODE END Application */

