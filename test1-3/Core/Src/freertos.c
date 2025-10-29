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
#include "tim.h"
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
int fla = 0;
int k = 0;
LEDState tempState;
LEDState tempState2;
LEDState ledState = NONE;
int flash = 0;
int pwmVal = 0;
int pwmIncrease = 1;
char CTRLData[] = "CTRL_ACTIVE";
char K1Data[] = "K1_PRESSED";
char PWMData[] = "PWM";
char K2Data[] = "K2_PRESSED";
char K3Data[] = "K3_PRESSED";
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
void buttonTask(void *parames);
void stateTask(void *parames);
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
	xTaskCreate(buttonTask,"buttonTask",128,NULL,osPriorityNormal+1,NULL);
	xTaskCreate(stateTask,"stateTask",128,NULL,osPriorityNormal,NULL);
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
	uint32_t lastTime = 0;
	uint32_t curTime = 0;
	for(;;){
		if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET){
			lastTime = HAL_GetTick();
			vTaskDelay(20);
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET){
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) == GPIO_PIN_RESET);
				curTime = HAL_GetTick();
				if(curTime - lastTime >= 2000){
					ledState = PWM;
				}else{
					ledState = FLA;
					fla = !fla;
				}
			}
		}else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET){
			vTaskDelay(20);
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET){
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_4) == GPIO_PIN_RESET);
				ledState = FLAPWM;
			}
		}else if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == GPIO_PIN_RESET){
			vTaskDelay(20);
			if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == GPIO_PIN_RESET){
				tempState = ledState;
				while(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_6) == GPIO_PIN_RESET){
					if(fla != 0){
						fla = 0;
						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
						k = 1;
					}
					ledState = STOP;
					vTaskDelay(50);
				}
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_RESET);
				if(k == 1){
					fla = 1;
					__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1000);
				}
				ledState = tempState;
			}
		}
		vTaskDelay(50);
	}
}
void stateTask(void *parames){
	for(;;){
		switch(ledState){
			case FLA:
				if(fla != 0){
						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1000);
					}else{
						__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
					}
					break;
			case NONE:
				break;
			case PWM:
				if(pwmIncrease){
					pwmVal += 50;
					if(pwmVal >= 1000){
						pwmVal = 990;
						pwmIncrease = 0;
					}
				}else{
					pwmVal -= 50;
					if(pwmVal <= 0){
						pwmVal = 10;
						pwmIncrease = 1;
					}
				}
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,pwmVal);
				break;
			case FLAPWM:
				HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_9);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1000);
				vTaskDelay(250);
				HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_9);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				vTaskDelay(250);
				HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_9);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1000);
				vTaskDelay(250);
				HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_9);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				vTaskDelay(250);
			
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1000);
				vTaskDelay(250);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				vTaskDelay(250);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1000);
				vTaskDelay(250);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				vTaskDelay(250);
				
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1000);
				vTaskDelay(250);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				vTaskDelay(250);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,1000);
				vTaskDelay(250);
				__HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,0);
				vTaskDelay(250);
				
				break;
			case STOP:
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_10,GPIO_PIN_SET);
				break;
		}
		vTaskDelay(50);
	}
}
/* USER CODE END Application */

