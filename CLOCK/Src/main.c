/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "myLCD.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define TRUE 1
#define FALSE 0

#define SETUP_NONE		0
#define SETUP_HOUR 		1
#define SETUP_MINUTE 	2
#define SETUP_SECOND 	3

#define SETUP_DAY 		4
#define SETUP_MONTH 	5
#define SETUP_YEAR 		6
#define SETUP_WEEKDAY 7

#define SETUP_END			8
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN PV */
RTC_TimeTypeDef setup_time;
RTC_DateTypeDef setup_date;
uint8_t setupState = SETUP_NONE;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_RTC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void PrintTime(RTC_TimeTypeDef _time)
{
	myLCD_ClearLCDScreen();
	myLCD_Cursor(0,0);
	if(_time.Hours <10) myLCD_PrintStr("0");
	myLCD_PrintInt(_time.Hours);
	myLCD_PrintStr(":");
	if(_time.Minutes <10) myLCD_PrintStr("0");
	myLCD_PrintInt(_time.Minutes);
	myLCD_PrintStr(":");
	if(_time.Seconds <10) myLCD_PrintStr("0");
	myLCD_PrintInt(_time.Seconds);
	
	switch(setupState)
	{
		case SETUP_HOUR:
		{
			myLCD_PrintStr(" HOUR");
			break;
		}
		case SETUP_MINUTE:
		{
			myLCD_PrintStr(" MIN");
			break;
		}
		case SETUP_SECOND:
		{
			myLCD_PrintStr(" SEC");
			break;
		}
		
		
		case SETUP_DAY:
		{
			myLCD_PrintStr(" DAY");
			break;
		}
		case SETUP_MONTH:
		{
			myLCD_PrintStr(" MONTH");
			break;
		}
		case SETUP_YEAR:
		{
			myLCD_PrintStr(" YEAR");
			break;
		}
		case SETUP_WEEKDAY:
		{
			myLCD_PrintStr(" WDAY");
			break;
		}
	}
}

void PrintDate(RTC_DateTypeDef _date)
{
	myLCD_Cursor(1,0);
	if(_date.Date <10) myLCD_PrintStr("0");
	myLCD_PrintInt(_date.Date);
	myLCD_PrintStr(".");
	if(_date.Month <10) myLCD_PrintStr("0");
	myLCD_PrintInt(_date.Month);
	myLCD_PrintStr(".");
	if(_date.Year <10) myLCD_PrintStr("0");
	myLCD_PrintInt(_date.Year);
	
	switch(_date.WeekDay)
	{
		case 0 :
		{
			myLCD_PrintStr("  SUN");
			break;
		}
		case 1 :
		{
			myLCD_PrintStr("  MON");
			break;
		}
		case 2 :
		{                                               
			myLCD_PrintStr("  TUE");                        
			break;                                        
		}                                               
		case 3 :                                        
		{                                               
			myLCD_PrintStr("  WED");                        
			break;
		}
		case 4 :
		{
			myLCD_PrintStr("  THU");
			break;
		}
		case 5 :
		{
			myLCD_PrintStr("  FRI");
			break;
		}
		case 6 :
		{
			myLCD_PrintStr("  SAT");
			break;
		}
	}
}
void SetupButton_Handler()
{
	setupState++;
	if(setupState == SETUP_END)
	{
		HAL_RTC_SetTime(&hrtc,&setup_time,FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc,&setup_date,FORMAT_BIN);
		setupState = SETUP_NONE;
	}
	else if (setupState == SETUP_HOUR)
	{
		HAL_RTC_GetTime(&hrtc,&setup_time,RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc,&setup_date,RTC_FORMAT_BIN);
	}
	
	PrintTime(setup_time);
	PrintDate(setup_date);
}

void PlusMinusButton_Handler(int8_t sign)
{
	if(setupState == SETUP_NONE) return;
	switch(setupState)
	{
			case SETUP_HOUR :
			{
				myLCD_ClearLCDScreen();
				//printf("SETUP_HOUR\n");
				setup_time.Hours+=sign;
				if(setup_time.Hours == 24) setup_time.Hours =0;
				if(setup_time.Hours == 255) setup_time.Hours =23;
				break;
			}
			case SETUP_MINUTE :
			{
				//printf("SETUP_MINUTE\n");
				setup_time.Minutes+=sign;
				if(setup_time.Minutes == 60) setup_time.Minutes =0;
				if(setup_time.Minutes == 255) setup_time.Minutes =59;
				break;
			}
			case SETUP_SECOND :
			{
				//printf("SETUP_SECOND\n");
				setup_time.Seconds+=sign;
				if(setup_time.Seconds == 60) setup_time.Seconds =0;
				if(setup_time.Seconds == 255) setup_time.Seconds =59;
				break;
			}
			case SETUP_YEAR :
			{
				//printf("SETUP_YEAR\n");
				setup_date.Year+=sign;
				if(setup_date.Year == 100) setup_date.Year =0;
				if(setup_date.Year == 255) setup_date.Year =99;
				break;
			}
			case SETUP_MONTH :
			{
				//printf("SETUP_MONTH\n");
				setup_date.Month+=sign;
				if(setup_date.Month == 13) setup_date.Month =0;
				if(setup_date.Month == 255) setup_date.Month =12;
				break;
			}
			case SETUP_DAY :
			{
				//printf("SETUP_DAY\n");
				setup_date.Date+=sign;
				if(setup_date.Date == 32) setup_date.Date =0;
				if(setup_date.Date == 255) setup_date.Date =31;
				break;
			}
			case SETUP_WEEKDAY :
			{
				//printf("SETUP_WEEKDAY\n");
				setup_date.WeekDay+=sign;
				if(setup_date.WeekDay == 7) setup_date.WeekDay =0;
				if(setup_date.WeekDay == 255) setup_date.WeekDay =6;
				break;
			}
	}
	PrintTime(setup_time);
	PrintDate(setup_date);
}

void CheckButtons()
{
	uint8_t pin0 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0);
	uint8_t pin1 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1);
	uint8_t pin2 = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2);
	
	if(pin0==1)
	{
		SetupButton_Handler();
	}
	else if(pin1 == 1)
	{
		PlusMinusButton_Handler(1);
	}
	else if(pin2 == 1)
	{
		PlusMinusButton_Handler(-1);
	}
}

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
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	myLCD_Initialize();
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		if( HAL_GetTick()%1000 >990 && setupState == SETUP_NONE)
		{
			myLCD_ClearLCDScreen();
			HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);
			HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);
			PrintTime(time);
			PrintDate(date);
			//HAL_Delay(500);
		}
		CheckButtons();
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief RTC Initialization Function
  * @param None
  * @retval None
  */
static void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */

  /* USER CODE END RTC_Init 0 */

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */
  /** Initialize RTC Only 
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_ALARM;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */

  /* USER CODE END RTC_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA1 PA2 PA3 
                           PA4 PA5 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
                          |GPIO_PIN_4|GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB1 PB2 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
