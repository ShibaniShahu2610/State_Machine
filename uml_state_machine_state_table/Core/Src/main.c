/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "main.h"
#include <stdio.h>
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
UART_HandleTypeDef hlpuart1;

/* USER CODE BEGIN PV */
static protimer_t protimer;
uint8_t rx_data;
uint8_t flag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LPUART1_UART_Init(void);
/* USER CODE BEGIN PFP */
static event_status_t protimer_event_dispatcher(protimer_t *const mobj, event_t const *const e);
int millis(void);
static void protimer_state_table_init(protimer_t *const mobj);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_LPUART1_UART_Init();
  /* USER CODE BEGIN 2 */
  protimer_state_table_init(&protimer);
  protimer_init(&protimer);

  HAL_UART_Receive_IT(&hlpuart1, &rx_data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  if (flag)
	  {
	    event_t evt;
	    switch (rx_data)
	    {
	    case '1':
	      evt.sig = INC_TIME;
	      break;
	    case '2':
	      evt.sig = DEC_TIME;
	      break;
	    case '3':
	      evt.sig = START_PAUSE;
	      break;
//	    case '4':
//	      evt.sig = TIME_TICK;
//	      break;
	    case '4':
	      evt.sig = ABRT;
	      break;
	    default:
	      HAL_UART_Receive_IT(&hlpuart1, &rx_data, 1);
	      flag = 0;
	      continue;
	    }

	    flag = 0;

	    // Dispatch the event to the state machine
	    protimer_event_dispatcher(&protimer, &evt);

	    // Restart UART receive interrupt
	    HAL_UART_Receive_IT(&hlpuart1, &rx_data, 1);
	  }

	  // Simulate timer tick every 100ms

	  uint32_t currentTick = millis();
	  if (millis() - currentTick >= 100)
	  {
		currentTick = millis();
	    protimer_tick_event_t te;
	    te.super.sig = TIME_TICK;
	    if(++te.ss > 10) te.ss = 1;
	    protimer_event_dispatcher(&protimer, &te.super);
	  }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 10;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief LPUART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_LPUART1_UART_Init(void)
{
  hlpuart1.Instance = LPUART1;
  hlpuart1.Init.BaudRate = 115200;
  hlpuart1.Init.WordLength = UART_WORDLENGTH_8B;
  hlpuart1.Init.StopBits = UART_STOPBITS_1;
  hlpuart1.Init.Parity = UART_PARITY_NONE;
  hlpuart1.Init.Mode = UART_MODE_TX_RX;
  hlpuart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hlpuart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hlpuart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&hlpuart1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOG_CLK_ENABLE();
  HAL_PWREx_EnableVddIO2();
}

/* USER CODE BEGIN 4 */
static event_status_t protimer_event_dispatcher(protimer_t *const mobj, event_t const *const e)
{
    event_status_t status;
    protimer_state_t source, target;
    e_handler_t ehandler;

    source = mobj->active_state;
//    status = protimer_state_machine(mobj, e);
    ehandler = (e_handler_t) mobj->state_table[mobj->active_state * MAX_SIGNALS + e->sig];
    if(ehandler)
    	status = (*ehandler)(mobj,e);
    if (status == EVENT_TRANSITION)
    {
        target = mobj->active_state;
        event_t ee;
        // 1. RUN EXIT ACTION FOR SOURCE STATE
        ee.sig = EXIT;
//        mobj->active_state = source;
//        protimer_state_machine(mobj, &ee);
        ehandler = (e_handler_t) mobj->state_table[source * MAX_SIGNALS + EXIT];
        if(ehandler)
        	(*ehandler)(mobj,&ee);
        // 2. RUN ENTRY ACTION FOR TARGET STATE
        ee.sig = ENTRY;
        ehandler = (e_handler_t) mobj->state_table[target * MAX_SIGNALS + ENTRY];
        if(ehandler)
        	(*ehandler)(mobj,&ee);
    }
    return status;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == LPUART1)
    {
        flag = 1;
    }
}

int millis(void){
	return HAL_GetTick();
}

static void protimer_state_table_init(protimer_t *const mobj)
{
  static e_handler_t protimer_state_table[MAX_STATES][MAX_SIGNALS] = {
    [IDLE]	  	    = {&IDLE_Inc_time, NULL, &IDLE_Time_tick, NULL, NULL, &IDLE_Entry, &IDLE_Exit},
    [TIME_SET] 	    = {&TIME_SET_Inc_time, &TIME_SET_Dec_time, NULL, &TIME_SET_Start_pause, &TIME_SET_Abrt, &TIME_SET_Entry, &TIME_SET_Exit},
    [COUNTDOWN] 	= {NULL, NULL, &COUNTDOWN_Time_tick, &COUNTDOWN_Start_pause, &COUNTDOWN_abrt, &COUNTDOWN_Entry, &COUNTDOWN_Exit},
    [PAUSE] 		= {&PAUSE_Inc_time, &PAUSE_Dec_time, NULL, &PAUSE_Start_pause, &PAUSE_Abrt, &PAUSE_Entry, &PAUSE_Exit},
};

   mobj->state_table = (uintptr_t*) &protimer_state_table[0][0];

}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
}
#endif /* USE_FULL_ASSERT */

