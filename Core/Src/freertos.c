/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
typedef StaticTask_t osStaticThreadDef_t;
typedef StaticSemaphore_t osStaticMutexDef_t;
typedef StaticSemaphore_t osStaticSemaphoreDef_t;
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

/* USER CODE END Variables */
/* Definitions for taskDefault */
osThreadId_t taskDefaultHandle;
uint32_t taskDefaultBuffer[ 128 ];
osStaticThreadDef_t taskDefaultControlBlock;
const osThreadAttr_t taskDefault_attributes = {
  .name = "taskDefault",
  .cb_mem = &taskDefaultControlBlock,
  .cb_size = sizeof(taskDefaultControlBlock),
  .stack_mem = &taskDefaultBuffer[0],
  .stack_size = sizeof(taskDefaultBuffer),
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for taskLux */
osThreadId_t taskLuxHandle;
uint32_t taskLuxBuffer[ 128 ];
osStaticThreadDef_t taskLuxControlBlock;
const osThreadAttr_t taskLux_attributes = {
  .name = "taskLux",
  .cb_mem = &taskLuxControlBlock,
  .cb_size = sizeof(taskLuxControlBlock),
  .stack_mem = &taskLuxBuffer[0],
  .stack_size = sizeof(taskLuxBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for taskDisplay */
osThreadId_t taskDisplayHandle;
uint32_t taskDisplayBuffer[ 128 ];
osStaticThreadDef_t taskDisplayControlBlock;
const osThreadAttr_t taskDisplay_attributes = {
  .name = "taskDisplay",
  .cb_mem = &taskDisplayControlBlock,
  .cb_size = sizeof(taskDisplayControlBlock),
  .stack_mem = &taskDisplayBuffer[0],
  .stack_size = sizeof(taskDisplayBuffer),
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for taskVoltage */
osThreadId_t taskVoltageHandle;
uint32_t taskVoltageBuffer[ 128 ];
osStaticThreadDef_t taskVoltageControlBlock;
const osThreadAttr_t taskVoltage_attributes = {
  .name = "taskVoltage",
  .cb_mem = &taskVoltageControlBlock,
  .cb_size = sizeof(taskVoltageControlBlock),
  .stack_mem = &taskVoltageBuffer[0],
  .stack_size = sizeof(taskVoltageBuffer),
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for mutexLux */
osMutexId_t mutexLuxHandle;
osStaticMutexDef_t mutexLuxControlBlock;
const osMutexAttr_t mutexLux_attributes = {
  .name = "mutexLux",
  .cb_mem = &mutexLuxControlBlock,
  .cb_size = sizeof(mutexLuxControlBlock),
};
/* Definitions for mutexI2c */
osMutexId_t mutexI2cHandle;
osStaticMutexDef_t mutexI2cControlBlock;
const osMutexAttr_t mutexI2c_attributes = {
  .name = "mutexI2c",
  .cb_mem = &mutexI2cControlBlock,
  .cb_size = sizeof(mutexI2cControlBlock),
};
/* Definitions for mutexVoltage */
osMutexId_t mutexVoltageHandle;
osStaticMutexDef_t mutexVoltageControlBlock;
const osMutexAttr_t mutexVoltage_attributes = {
  .name = "mutexVoltage",
  .cb_mem = &mutexVoltageControlBlock,
  .cb_size = sizeof(mutexVoltageControlBlock),
};
/* Definitions for semaphoreDisplay */
osSemaphoreId_t semaphoreDisplayHandle;
osStaticSemaphoreDef_t semaphoreDisplayControlBlock;
const osSemaphoreAttr_t semaphoreDisplay_attributes = {
  .name = "semaphoreDisplay",
  .cb_mem = &semaphoreDisplayControlBlock,
  .cb_size = sizeof(semaphoreDisplayControlBlock),
};
/* Definitions for semaphoreLux */
osSemaphoreId_t semaphoreLuxHandle;
osStaticSemaphoreDef_t semaphoreLuxControlBlock;
const osSemaphoreAttr_t semaphoreLux_attributes = {
  .name = "semaphoreLux",
  .cb_mem = &semaphoreLuxControlBlock,
  .cb_size = sizeof(semaphoreLuxControlBlock),
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void taskDefaultFunction(void *argument);
extern void taskLuxFunction(void *argument);
extern void taskDisplayFunction(void *argument);
extern void taskVoltageFunction(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */
  /* Create the mutex(es) */
  /* creation of mutexLux */
  mutexLuxHandle = osMutexNew(&mutexLux_attributes);

  /* creation of mutexI2c */
  mutexI2cHandle = osMutexNew(&mutexI2c_attributes);

  /* creation of mutexVoltage */
  mutexVoltageHandle = osMutexNew(&mutexVoltage_attributes);

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of semaphoreDisplay */
  semaphoreDisplayHandle = osSemaphoreNew(1, 1, &semaphoreDisplay_attributes);

  /* creation of semaphoreLux */
  semaphoreLuxHandle = osSemaphoreNew(1, 1, &semaphoreLux_attributes);

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
  /* creation of taskDefault */
  taskDefaultHandle = osThreadNew(taskDefaultFunction, NULL, &taskDefault_attributes);

  /* creation of taskLux */
  taskLuxHandle = osThreadNew(taskLuxFunction, NULL, &taskLux_attributes);

  /* creation of taskDisplay */
  taskDisplayHandle = osThreadNew(taskDisplayFunction, NULL, &taskDisplay_attributes);

  /* creation of taskVoltage */
  taskVoltageHandle = osThreadNew(taskVoltageFunction, NULL, &taskVoltage_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_taskDefaultFunction */
/**
  * @brief  Function implementing the taskDefault thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_taskDefaultFunction */
void taskDefaultFunction(void *argument)
{
  /* USER CODE BEGIN taskDefaultFunction */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END taskDefaultFunction */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

