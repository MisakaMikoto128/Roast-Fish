/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "crc.h"
#include "rtc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "Sys.h"
#include "UI.h"
#include "Key.h"
#include "Keydef.h"
#include "Counter.h"
#include <stdio.h>
#include "TimeSetting.h"
#include "LED.h"
#include "Optocoupler.h"
#include "Relay.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
//当前UI的设计模式一部分是UI更新值到设备，另�??部分是不断更新sysState的�?�到UI设备
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void User_KeyInit()
{
  // only one key examle,read pin pullup,and key com pin connect to GND
  KeyPin_t keyinpins[4] = {{GPIOA, GPIO_PIN_0}, {GPIOA, GPIO_PIN_1}, {GPIOA, GPIO_PIN_2}, {GPIOA, GPIO_PIN_3}};
  KeyPin_t keysetpins[4] = {{GPIOA, GPIO_PIN_4}, {GPIOA, GPIO_PIN_5}, {GPIOA, GPIO_PIN_6}, {GPIOA, GPIO_PIN_7}};
  KeyInit(4, 4, keyinpins, keysetpins);
};

Counter key_calitime_cnt = {.count_max = 2, .count_min = 0, .count = 0, .step = 1};
Counter key_mode_cnt = {.count_max = 2, .count_min = 0, .count = 0, .step = 1};

Counter key_fodder_cnt = {.count_max = MAX_FODDER, .count_min = MIN_FODDER, .count = 1, .step = 1};
Counter key_interval_cnt = {.count_max = MAX_INTERVAL, .count_min = MIN_INTERVAL, .count = 1, .step = 1};
Counter key_output_cnt = {.count_max = MAX_OUTPOUT, .count_min = MIN_OUTPOUT, .count = 1, .step = 1};
Counter key_area_cnt = {.count_max = MAX_AREA, .count_min = MIN_AREA, .count = 1, .step = 1};

Counter key_run_time_set_cnt = {.count_max = 120, .count_min = 5, .count = 5, .step = 5};

Counter key_period_cnt = {.count_max = 8 * 2, .count_min = 0, .count = 0, .step = 1};
Counter set_rtc_cnt = {.count_max = 400, .count_min = 0, .count = 0, .step = 1};

Counter vibrator_interval_time_cnt = {0};
Counter vibrator_running_time_cnt = {0};

void RTC_Time_Minute_Increament(RTC_TimeTypeDef *time)
{
  if (time->Minutes == 59)
  {
    time->Minutes = 0;
  }
  else
  {
    time->Minutes++;
  }
}

void RTC_Time_Minute_Decreament(RTC_TimeTypeDef *time)
{
  if (time->Minutes == 0)
  {
    time->Minutes = 59;
  }
  else
  {
    time->Minutes--;
  }
}

void RTC_Time_Hour_Increament(RTC_TimeTypeDef *time)
{
  if (time->Hours == 23)
  {
    time->Hours = 0;
  }
  else
  {
    time->Hours++;
  }
}

void RTC_Time_Hour_Decreament(RTC_TimeTypeDef *time)
{
  if (time->Hours == 0)
  {
    time->Hours = 23;
  }
  else
  {
    time->Hours--;
  }
}

void RTC_Time_ResetHMS(RTC_TimeTypeDef *time)
{
  time->Hours = 0;
  time->Minutes = 0;
  time->Seconds = 0;
}

void onKeyMinuteDown()
{
  sysState.runState = SYS_STOP;
  if (sysState.mode == MOD_TIMING)
  {
    Counter_increment_circle(&key_run_time_set_cnt);
    sysState.run_time_set_value = CounterGET(&key_run_time_set_cnt);
  }
  else
  {
    switch (timeSettingMode)
    {
    case SET_INVALID:
      break;
    case SET_RTC_TIME:
      RTC_Time_Minute_Increament(&stimestructureget);
      Counter_reset(&set_rtc_cnt);
      Lcd_Clock_Show(stimestructureget.Hours, stimestructureget.Minutes);
      if (HAL_RTC_SetTime(&hrtc, &stimestructureget, RTC_FORMAT_BCD) != HAL_OK)
      {
        Error_Handler();
      }
      break;
    case SET_PERIOD_START:
    {
      SysPeriodNode *pSysNode = &sysState.period[CounterGET(&key_period_cnt) / 2 + CounterGET(&key_period_cnt) % 2 - 1];
      RTC_Time_Minute_Increament(&pSysNode->start);
      if (!pSysNode->isOpen)
      {
        pSysNode->isOpen = true;
      }
      globalSettingTimeObj = pSysNode->start;
      UI_SendMessage(SET_CLOCK_SHOW_GLOBAL_SETTING_TIME, NULL);
    }
    break;
    case SET_PERIOD_END:
    {
      SysPeriodNode *pSysNode = &sysState.period[CounterGET(&key_period_cnt) / 2 + CounterGET(&key_period_cnt) % 2 - 1];
      if (!pSysNode->isOpen)
      {
        pSysNode->isOpen = true;
      }
      RTC_Time_Minute_Increament(&pSysNode->end);
      globalSettingTimeObj = pSysNode->end;
      UI_SendMessage(SET_CLOCK_SHOW_GLOBAL_SETTING_TIME, NULL);
    }
    break;

    default:
      break;
    }
  }
}

void onKeyHourDown()
{
  sysState.runState = SYS_STOP;

  switch (timeSettingMode)
  {
  case SET_INVALID:
    break;
  case SET_RTC_TIME:
    RTC_Time_Hour_Increament(&stimestructureget);
    Counter_reset(&set_rtc_cnt);
    Lcd_Clock_Show(stimestructureget.Hours, stimestructureget.Minutes);
    if (HAL_RTC_SetTime(&hrtc, &stimestructureget, RTC_FORMAT_BCD) != HAL_OK)
    {
      Error_Handler();
    }
    break;
  case SET_PERIOD_START:
  {
    SysPeriodNode *pSysNode = &sysState.period[CounterGET(&key_period_cnt) / 2 + CounterGET(&key_period_cnt) % 2 - 1];
    RTC_Time_Hour_Increament(&pSysNode->start);
    if (!pSysNode->isOpen)
    {
      pSysNode->isOpen = true;
    }
    globalSettingTimeObj = pSysNode->start;
    UI_SendMessage(SET_CLOCK_SHOW_GLOBAL_SETTING_TIME, NULL);
  }
  break;
  case SET_PERIOD_END:
  {
    SysPeriodNode *pSysNode = &sysState.period[CounterGET(&key_period_cnt) / 2 + CounterGET(&key_period_cnt) % 2 - 1];
    if (!pSysNode->isOpen)
    {
      pSysNode->isOpen = true;
    }
    RTC_Time_Hour_Increament(&pSysNode->end);
    globalSettingTimeObj = pSysNode->end;
    UI_SendMessage(SET_CLOCK_SHOW_GLOBAL_SETTING_TIME, NULL);
  }
  break;

  default:
    break;
  }
}

void KeyDriver()
{
  static KeyState_t key = KEY_NONE;

  if (!isKeyFIFOEmpty())
  {
    key = Key_FIFO_Get();

    switch (key)
    {

    case KEY_FODDDER_P_Down:
      Counter_increment(&key_fodder_cnt);
      sysState.fodder_num = CounterGET(&key_fodder_cnt);
      Counter_init(&vibrator_running_time_cnt, GET_FODDER_TIME(sysState.fodder_num),0, 1);
      break;
    case KEY_FODDDER_P_LongPress:
      Counter_increment(&key_fodder_cnt);
      sysState.fodder_num = CounterGET(&key_fodder_cnt);
     Counter_init(&vibrator_running_time_cnt, GET_FODDER_TIME(sysState.fodder_num),0, 1);
      break;
    case KEY_FODDDER_N_Down:
      Counter_decrement(&key_fodder_cnt);
      sysState.fodder_num = CounterGET(&key_fodder_cnt);
      Counter_init(&vibrator_running_time_cnt, GET_FODDER_TIME(sysState.fodder_num),0, 1);
      break;
    case KEY_FODDDER_N_LongPress:
      Counter_decrement(&key_fodder_cnt);
      sysState.fodder_num = CounterGET(&key_fodder_cnt);
      Counter_init(&vibrator_running_time_cnt, GET_FODDER_TIME(sysState.fodder_num),0, 1);
      break;
    case KEY_INTERVAL_P_Down:
      Counter_increment(&key_interval_cnt);
      sysState.interval_num = CounterGET(&key_interval_cnt);
      Counter_init(&vibrator_interval_time_cnt, GET_FODDER_TIME(sysState.interval_num),0, 1);
      break;
    case KEY_INTERVAL_P_LongPress:
      Counter_increment(&key_interval_cnt);
      sysState.interval_num = CounterGET(&key_interval_cnt);
      Counter_init(&vibrator_interval_time_cnt, GET_FODDER_TIME(sysState.interval_num),0, 1);
      break;
    case KEY_INTERVAL_N_Down:
      Counter_decrement(&key_interval_cnt);
      sysState.interval_num = CounterGET(&key_interval_cnt);
      Counter_init(&vibrator_interval_time_cnt, GET_FODDER_TIME(sysState.interval_num),0, 1);
      break;
    case KEY_INTERVAL_N_LongPress:
      Counter_decrement(&key_interval_cnt);
      sysState.interval_num = CounterGET(&key_interval_cnt);
      Counter_init(&vibrator_interval_time_cnt, GET_FODDER_TIME(sysState.interval_num),0, 1);
      break;
    case KEY_OUTPUT_P_Down:
      Counter_increment(&key_output_cnt);
      sysState.output_num = CounterGET(&key_output_cnt);
      break;
    case KEY_OUTPUT_P_LongPress:
      Counter_increment(&key_output_cnt);
      sysState.output_num = CounterGET(&key_output_cnt);
      break;
    case KEY_OUTPUT_N_Down:
      Counter_decrement(&key_output_cnt);
      sysState.output_num = CounterGET(&key_output_cnt);
      break;
    case KEY_OUTPUT_N_LongPress:
      Counter_decrement(&key_output_cnt);
      sysState.output_num = CounterGET(&key_output_cnt);
      break;
    case KEY_AREA_P_Down:
      Counter_increment(&key_area_cnt);
      sysState.area_num = CounterGET(&key_area_cnt);
      break;
    case KEY_AREA_P_LongPress:
      Counter_increment(&key_area_cnt);
      sysState.area_num = CounterGET(&key_area_cnt);
      break;
    case KEY_AREA_N_Down:
      Counter_decrement(&key_area_cnt);
      sysState.area_num = CounterGET(&key_area_cnt);
      break;
    case KEY_AREA_N_LongPress:
      Counter_decrement(&key_area_cnt);
      sysState.area_num = CounterGET(&key_area_cnt);
      break;

    case KEY_OFF_Down:
      sysState.runState = SYS_STOP;
      break;
    case KEY_ON_Down:
      sysState.runState = SYS_RUN;
      sysState.run_time = sysState.run_time_set_value;
      break;
    case KEY_MODE_Down:
      Counter_increment_circle(&key_mode_cnt);
      sysState.mode = CounterGET(&key_mode_cnt);
      sysState.runState = SYS_STOP;
      __TDOT_TEXT_Off(); // Elimination of residual display
      Counter_reset(&key_period_cnt);
      timeSettingMode = SET_INVALID;
      break;

    case KEY_MINUTE_Down:
      onKeyMinuteDown();

      break;
    case KEY_MINUTE_LongPress:
      onKeyMinuteDown();
      break;
    case KEY_HOUR_Down:
      onKeyHourDown();
      break;
    case KEY_HOUR_LongPress:
      onKeyHourDown();
      break;

    case KEY_PERIOD_Down:
      sysState.runState = SYS_STOP;
      if (sysState.mode == MOD_NORMAL_AOTO)
      {
        Counter_increment_circle(&key_period_cnt);
        if (CounterGET(&key_period_cnt) == 0)
        {
          UI_SendMessage(SET_CLOCK_NORMAL_SHOW, NULL);
          break;
        }
        SysPeriodNode *pSysNode = &sysState.period[CounterGET(&key_period_cnt) / 2 + CounterGET(&key_period_cnt) % 2 - 1];

        if (IS_ODD(CounterGET(&key_period_cnt)))
        {
          __CLOSE_TEXT_Off(); // Elimination of residual display
          __OPEN_TEXT_On();   // Elimination of residual display
          // 1,2->0
          // 3,4->1
          timeSettingMode = SET_PERIOD_START;
          if (pSysNode->isOpen)
          {
            globalSettingTimeObj = pSysNode->start;
            UI_SendMessage(SET_CLOCK_SHOW_GLOBAL_SETTING_TIME, NULL);
          }
          else
          {
            UI_SendMessage(SET_CLOCK_SHOW_NONE, NULL);
          }
        }
        else
        {
          __OPEN_TEXT_Off(); // Elimination of residual display
          __CLOSE_TEXT_On(); // Elimination of residual display

          // 1,2->0
          // 3,4->1
          timeSettingMode = SET_PERIOD_END;
          if (pSysNode->isOpen)
          {
            globalSettingTimeObj = pSysNode->end;
            UI_SendMessage(SET_CLOCK_SHOW_GLOBAL_SETTING_TIME, NULL);
          }
          else
          {
            UI_SendMessage(SET_CLOCK_SHOW_NONE, NULL);
          }
        }
      }

      break;

    case KEY_CaliTime_Down:
      if (sysState.mode == MOD_NORMAL_OPEN)
      {
        timeSettingMode = SET_RTC_TIME;
      }
      break;
    case KEY_CANCEL_Down:
      if (sysState.mode == MOD_TIMING)
      {
        Counter_increment_circle(&key_run_time_set_cnt);
        sysState.run_time_set_value = CounterGET(&key_run_time_set_cnt);
      }
      else if (sysState.mode == MOD_NORMAL_AOTO)
      {
        switch (timeSettingMode)
        {
        case SET_INVALID:
          break;
        case SET_RTC_TIME:
          break;
        case SET_PERIOD_START:
        {
          SysPeriodNode *pSysNode = &sysState.period[CounterGET(&key_period_cnt) / 2 + CounterGET(&key_period_cnt) % 2 - 1];
          RTC_Time_Minute_Increament(&pSysNode->start);
          pSysNode->isOpen = false;
          RTC_Time_ResetHMS(&pSysNode->start);
          UI_SendMessage(SET_CLOCK_SHOW_NONE, NULL);
        }
        break;
        case SET_PERIOD_END:
        {
          SysPeriodNode *pSysNode = &sysState.period[CounterGET(&key_period_cnt) / 2 + CounterGET(&key_period_cnt) % 2 - 1];
          RTC_Time_Minute_Increament(&pSysNode->start);
          pSysNode->isOpen = false;
          RTC_Time_ResetHMS(&pSysNode->end);
          UI_SendMessage(SET_CLOCK_SHOW_NONE, NULL);
        }
        break;

        default:
          break;
        }
      }
      break;

    default:
      break;
    }
  }
}


void Update_SyaState_2_Counter(){
Counter_set_cnt(&key_fodder_cnt,sysState.fodder_num);
Counter_set_cnt(&key_interval_cnt,sysState.interval_num);
Counter_set_cnt(&key_output_cnt,sysState.output_num);
Counter_set_cnt(&key_area_cnt,sysState.area_num);
Counter_init(&vibrator_interval_time_cnt, GET_FODDER_TIME(sysState.fodder_num),0, 1);
Counter_init(&vibrator_running_time_cnt, GET_FODDER_TIME(sysState.fodder_num),0, 1);
}

void oneMilliSecCallback()
{
  KeyScan();
  Sys_Running_Scan();
  UI_Scan();
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
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM14_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
  reloadSysStateFromFlash();
  Update_SyaState_2_Counter();
  LED_Init();
  Relay_Init();
  RELAY_ON();
  Optocoupler_Init();
  UI_Init();
  User_KeyInit();
  UserPIDInit();
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Stop_IT(&htim14, TIM_CHANNEL_1);
  
	HAL_TIM_Base_Start_IT(&htim17);
	HAL_TIM_Base_Start_IT(&htim16);
  //HAL_TIM_Base_Start_IT(&htim17);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    KeyDriver();
    HAL_Delay(0);
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_LSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the peripherals clocks
  */
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInit.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
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
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
