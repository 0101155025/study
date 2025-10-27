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
	MODE1,
	MODE2,
	MODE3,
}MODE;
typedef enum{
	NO,
	UP,
	DOWN,
	STOP,
	SUSPEND,
}LEDstate;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
MODE mode = NONE;
LEDstate ledState = STOP;
int TxState = 1;
int RxState = 1;
int suspend = 0;
LEDstate susState;
char TxData[] = "INIT_OK\n";
char DOWNData[] = "DOWN_ACTIVE";
char STOPData[] = "STOP_ACTIVE";
char FLOWData[] = "FLOW_ACTIVE";
char UPData[] = "UP_ACTIVE";
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
void copeMode1(void);
void stateTask(void *parames);
void buttonTask(void *parames);
void ledLight(void);
void RxTask(void *parames);
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
	xTaskCreate(stateTask,"stateTask",128,NULL,osPriorityNormal,NULL);
	xTaskCreate(buttonTask,"buttonTask",128,NULL,osPriorityNormal+1,NULL);
	xTaskCreate(RxTask,"RxTask",128,NULL,osPriorityNormal,NULL);
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
void stateTask(void *parames){
	MODE tempMode = NONE;
	for(;;){
		taskENTER_CRITICAL();
		if(mode == tempMode){
			taskEXIT_CRITICAL();
			vTaskDelay(50);
			continue;
		}else{
			tempMode = mode;
			taskEXIT_CRITICAL();
		}
		switch(tempMode){
			case MODE1:
				copeMode1();
				break;
			case MODE2:
				ledLight();
				break;
			case MODE3:
				break;
			case NONE:
				break;
		}
		vTaskDelay(50);
	}
}
void ledLight(){
	LEDstate state;
	while(1){
		state = ledState;
		switch(state){
			case UP:
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
				vTaskDelay(125);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
				vTaskDelay(125);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
				vTaskDelay(125);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
				vTaskDelay(125);
				break;
			case DOWN:
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
				vTaskDelay(125);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
				vTaskDelay(125);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
				vTaskDelay(125);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
				vTaskDelay(125);
				break;
			case STOP:
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11,GPIO_PIN_RESET);
				break;
			case SUSPEND:
				state = NO;
				break;
			case NO:
				break;
		}
		vTaskDelay(50);
	}
}
void buttonTask(void *parames){
	uint32_t curTime = 0;
	uint32_t lastTime = 0;
	MODE tempmode;
	for(;;){
		tempmode = mode;
		if(tempmode == NONE){
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET) mode = MODE1;
			else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET) mode = MODE2;
			else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == GPIO_PIN_RESET) mode = MODE3;
		}else{
			if(tempmode == MODE2){
				if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET){
					vTaskDelay(20);
					if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET){
						while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET);
						if(suspend == 0){
							suspend = 1;
							susState = ledState;
							ledState = SUSPEND;
						}else if(suspend == 1){
							suspend = 0;
							ledState = susState;
						}
					}
				}else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET){
					vTaskDelay(20);
					if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET){
						while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET);
										ledState = (ledState == UP) ? DOWN : UP;
						if(ledState == UP){
							if(TxState == 1){
								TxState = 0;
								HAL_UART_Transmit_DMA(&huart2,(uint8_t*)UPData,strlen(UPData));
							}
						}else if(ledState == DOWN){
							if(TxState == 1){
								TxState = 0;
								HAL_UART_Transmit_DMA(&huart2,(uint8_t*)DOWNData,strlen(DOWNData));
							}
						}
					}

				}
			}
		}
		vTaskDelay(50);
	}
}
void copeMode1(void){
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
	vTaskDelay(200);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_SET);
	vTaskDelay(200);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
	vTaskDelay(200);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_SET);
	vTaskDelay(200);
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
	vTaskDelay(200);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_9,GPIO_PIN_RESET);
	vTaskDelay(200);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
	vTaskDelay(200);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_11,GPIO_PIN_RESET);
	vTaskDelay(200);
	
	for(int i = 0;i < 3;i++){
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11,GPIO_PIN_SET);
		vTaskDelay(500);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11,GPIO_PIN_RESET);
		vTaskDelay(500);
	}
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11,GPIO_PIN_SET);
	if(TxState == 1){
		TxState = 0;
		HAL_UART_Transmit_DMA(&huart2,(uint8_t*)TxData,strlen(TxData));
	}else{
		while(TxState == 0) taskYIELD();
		HAL_UART_Transmit_DMA(&huart2,(uint8_t*)TxData,strlen(TxData));
	}
}
void RxTask(void *parames){
	uint8_t Rx_buf[4];
	for(;;){
		if(RxState == 1){
			if(HAL_UART_Receive_DMA(&huart2,Rx_buf,sizeof(Rx_buf)) == HAL_OK){
				if(mode == MODE2){
					if(Rx_buf[1] == 'D'){
						ledState = DOWN;
						if(TxState == 1){
							TxState = 0;
							HAL_UART_Transmit_DMA(&huart2,(uint8_t*)DOWNData,strlen(DOWNData));
						}
					}
					else if(Rx_buf[1] == 'S'){
						ledState = STOP;
						if(TxState == 1){
							TxState = 0;
							HAL_UART_Transmit_DMA(&huart2,(uint8_t*)STOPData,strlen(STOPData));
						}
					}
					else if(Rx_buf[1] == 'U'){
						ledState = UP;
					}
				}else{
					if(Rx_buf[1] == 'F'){
						mode = MODE2;
						ledState = UP;
						if(TxState == 1){
							TxState = 0;
							HAL_UART_Transmit_DMA(&huart2,(uint8_t*)FLOWData,strlen(FLOWData));
						}
					}
					if(Rx_buf[1] =='C'){
						mode = MODE3;
					}
				}
			}
		}
		vTaskDelay(50);
	}
}
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart2){
		TxState = 1;
	}
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	if(huart == &huart2){
		RxState = 1;
	}
}
/* USER CODE END Application */

