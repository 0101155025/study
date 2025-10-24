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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum{
	NONE,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE,
	SIX,
	SEVEN,
	EIGHT,
	WRONG,
	COMPLETE
} State;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
int g_pinNum = 0;
State state = NONE;
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
void buttonTask(void *parames){
	uint8_t curTime = 0;
	uint8_t lastTime = 0;
	int pinNum = 0;
	for(;;){
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4) == GPIO_PIN_SET){
			vTaskDelay(20);
			if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4) == GPIO_PIN_SET){
				while(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4) == GPIO_PIN_SET) taskYIELD();
				lastTime = HAL_GetTick();
				pinNum++;
			}else vTaskDelay(50);
		}else{
			curTime = HAL_GetTick();
			if(curTime - lastTime > 2000){
				taskENTER_CRITICAL();
				g_pinNum = pinNum;
				taskEXIT_CRITICAL();
				pinNum = 0;
			}
		}
		vTaskDelay(50);
	}
}
void stateTask(void *parames){
	int pinNum;
	int curStep = 0;
	int step[8] = {4,6,8,1,5,3,2,7};
	for(;;){
		taskENTER_CRITICAL();
		pinNum = g_pinNum;
		taskEXIT_CRITICAL();
		if(curStep >= 8){
			curStep = 0;
			taskENTER_CRITICAL();
			state = COMPLETE;
			taskEXIT_CRITICAL();
		}else{
			if(pinNum == step[curStep]){
				taskENTER_CRITICAL();
				switch(pinNum){
					case 1: state = ONE;break;
					case 2: state = TWO;break;
					case 3: state = THREE;break;
					case 4: state = FOUR;break;
					case 5: state = FIVE;break;
					case 6: state = SIX;break;
					case 7: state = SEVEN;break;
					case 8: state = EIGHT;break;
				}
				taskEXIT_CRITICAL();
				curStep++;
			}else{
				taskENTER_CRITICAL();
				state = WRONG;
				taskEXIT_CRITICAL();
				curStep = 0;
			}
		}
		vTaskDelay(50);
	}
}
void LEDTask(void *parames){
	State tempState = NONE;
	int isWrong = 0;
	for(;;){
		taskENTER_CRITICAL();
		tempState = state;
		taskEXIT_CRITICAL();
		if(HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6) == GPIO_PIN_SET){
			switch(tempState){
				case EIGHT: 
				case SEVEN:
				case SIX:
				case FIVE:
				case FOUR:
				case THREE:
				case TWO:
				case ONE:
					for(int i = 0;i < tempState;i++){
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_SET);
						vTaskDelay(1000);
						HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,GPIO_PIN_RESET);
						vTaskDelay(1000);
					}
					break;
				case WRONG:
						while(tempState == WRONG){
							HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
							vTaskDelay(1000);
							HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
							vTaskDelay(1000);
						}
					break;
				default :
					break;
				}
		}
		vTaskDelay(50);
	}
}
/* USER CODE END Application */

