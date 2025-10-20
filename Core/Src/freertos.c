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
#include "semphr.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	motor2006 = 2006,
	motor3508 = 3508,
	motor6020 = 6020,
} MotorType;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define ALPHA 0.3
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
MotorType mt = motor3508;
char buf[128];
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t CAN_TxData[8] = {0};
uint8_t RxData[8];
uint32_t TxMailBox;
int button3State = 0;
volatile uint8_t bUartDmaIdle = 1;
SemaphoreHandle_t xUartMutex;
volatile uint8_t bAllowCan = 1;
typedef struct {
	float fdata[2];
	uint8_t tail[4];
} FrameStruct;
const uint8_t framtail[4] = {0x00,0x00,0x80,0x7f};
/* USER CODE END Variables */
/* Definitions for defaultTask */
//osThreadId_t defaultTaskHandle;
//const osThreadAttr_t defaultTask_attributes = {
//  .name = "defaultTask",
//  .stack_size = 128 * 4,
//  .priority = (osPriority_t) osPriorityNormal,
//};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

void ButtonTask(void *parames);
void MotorTask(void *parames);
void CAN_SendData(void);
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan);
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan);
void CAN_Filter_Init(void);
void CANTask(void *parames);
void NoisyTask(void *parames);
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart);
/* USER CODE END FunctionPrototypes */

//void StartDefaultTask(void *argument);

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
	xUartMutex = xSemaphoreCreateMutex();
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
//  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */

	if(xTaskCreate(ButtonTask,"ButtonTask",128,NULL,osPriorityNormal,NULL) != pdPASS){
		/* 有时间写个错误处理函数 */
	}
	if(xTaskCreate(MotorTask,"MotorTask",256,NULL,osPriorityNormal,NULL) != pdPASS){
		/* 有时间写个错误处理函数 */
	}
	if(xTaskCreate(CANTask,"CANTask",128,NULL,osPriorityNormal,NULL) != pdPASS){
		/* 有时间写个错误处理函数 */
	}
	if(xTaskCreate(NoisyTask,"NoisyTask",384,NULL,osPriorityNormal + 1,NULL) != pdPASS){
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
//void StartDefaultTask(void *argument)
//{
//  /* USER CODE BEGIN StartDefaultTask */
//  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(1);
//  }
//  /* USER CODE END StartDefaultTask */
//}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void ButtonTask(void *parames){
	int curtime;
	for(;;){
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_SET){
			// 记录长按
			curtime = HAL_GetTick();
			vTaskDelay(20);
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) != GPIO_PIN_SET){
				vTaskDelay(50);
				continue;
			}
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_SET){
				while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3) == GPIO_PIN_SET) taskYIELD();
				if(HAL_GetTick() - curtime >= 2000) g_button_cnt += 125;
				else g_button_cnt -= 125;
			}
		}
		if(g_button_cnt > 1000) g_button_cnt = 125 ;
		if(g_button_cnt < 125) g_button_cnt = 1000;
		vTaskDelay(50);
	}
}
void MotorTask(void *parames){
	for(;;){
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_SET){
			//消抖
			vTaskDelay(20);
			//如果是抖动
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) != GPIO_PIN_SET){
				vTaskDelay(50);
				continue;
			}
			while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5) == GPIO_PIN_SET) taskYIELD();
			switch(mt){
				case motor3508 :
					mt = motor6020;
					snprintf(buf,sizeof(buf),"当前类型:%d\r\n",mt);
					break;
				case motor6020 :
					mt = motor2006;
					snprintf(buf,sizeof(buf),"当前类型:%d\r\n",mt);
					break;
				case motor2006 :
					mt = motor3508;
					snprintf(buf,sizeof(buf),"当前类型:%d\r\n",mt);
					break;
			}
			if(xSemaphoreTake(xUartMutex,portMAX_DELAY) == pdPASS){
				while(bUartDmaIdle == 0) taskYIELD();		// dma被占用
				bUartDmaIdle = 0;			// 标志dma状态
				HAL_UART_Transmit_DMA(&huart2,(uint8_t *)buf,strlen(buf));
				xSemaphoreGive(xUartMutex);
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
	CAN_TxData[0] = 1;      			  // 发一个π的小数点
	CAN_TxData[1] = 4;
	CAN_TxData[2] = 1;
	CAN_TxData[3] = 5;
	CAN_TxData[4] = 9;
	CAN_TxData[5] = 2;
	CAN_TxData[6] = 6;
	CAN_TxData[7] = 5;
	if(HAL_CAN_AddTxMessage(&hcan,&TxHeader,CAN_TxData,&TxMailBox) != HAL_OK){
	/* 有时间写个错误处理 */
	};
}
// 收到数据后存到RxHeader和RxData
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
  HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, RxData);
}
// 三个邮箱发送完数据后的中断,这里用来发送下一轮数据
void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	HAL_UART_Transmit(&huart2,CAN_TxData,sizeof(CAN_TxData), 100);
}
void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	HAL_UART_Transmit(&huart2,CAN_TxData,sizeof(CAN_TxData), 100);
}
void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	HAL_UART_Transmit(&huart2,CAN_TxData,sizeof(CAN_TxData), 100);
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
		vTaskDelay(10);
}
void CANTask(void *parames){
	CAN_Filter_Init();						//初始化外设
	for(;;){
		if(bAllowCan){							//当滤波没工作时发消息,防止波变得混乱
			CAN_SendData();
		}
		vTaskDelay(2000);						//2s发一次
	}
}
void NoisyTask(void *parames){
  static double x = 0.0;
	double noise;
  double raw;
	static double last_filtered = 0.0;
	double current_filtered;
	double max = -1e18,min = 1e18;
	double range = 0.0;
	FrameStruct sendFrame;
	srand((unsigned int)HAL_GetTick());						// 通过当前时间获得种子
	for(;;){
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) == SET){
			//软件消抖
		vTaskDelay(20);
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) == SET){
				while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8) == SET) taskYIELD();
				button3State = !button3State;
				bAllowCan = !bAllowCan;		// 关掉/开启can回环防止干扰
			}
		}
		if(button3State){
			// 制造噪音
				noise = 0.1 + (2.0 * rand() / RAND_MAX - 1.0);
				raw = sqrt(x) + noise;
				x += 0.5;
				if(x > 10.0) x = 0.0;
			// 滤波
				current_filtered = ALPHA * raw + (1 - ALPHA) * last_filtered;
				last_filtered = current_filtered;
			// 计算极差
				if(current_filtered > max) max = current_filtered;
				if(current_filtered < min) min = current_filtered;
				range = max - min;
			// 将数据塞进结构体,准备发出去
				sendFrame.fdata[0] = (float)current_filtered;
			  sendFrame.fdata[1] = (float)range;
			// 按照vofa格式的数据尾部
			  memcpy(sendFrame.tail,framtail,sizeof(framtail));
			// 利用互斥量等待空闲
				if(xSemaphoreTake(xUartMutex,portMAX_DELAY) == pdPASS){
					while(bUartDmaIdle == 0) taskYIELD();
					bUartDmaIdle = 0;
					HAL_UART_Transmit_DMA(&huart2,(uint8_t *)&sendFrame,sizeof(sendFrame));
					xSemaphoreGive(xUartMutex);
				}
			}
		vTaskDelay(500);
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
    if(huart == &huart2){
			// 在回调函数中,将dma状态恢复
        bUartDmaIdle = 1;
    }
}
/* USER CODE END Application */
