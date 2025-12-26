/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "pi_control.h"
#include "highlevel_control.h"
#include "stm32f1xx_hal.h"
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


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t adc_buffer[2];
float output_voltage = 0.0f;
extern float light_value;

extern char RxBuffer[RXBUFFERSIZE];  
extern uint8_t aRxBuffer;		
extern uint8_t Uart3_Rx_Cnt;	
extern char my_order[10];
extern char receive_flag;
extern uint8_t my_mode;

extern uint8_t voice_cmd_ready;
//extern uint8_t voice_rx_buffer[5];

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
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_USART3_UART_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_ADC_Start_DMA(&hadc1, (uint32_t *) adc_buffer, 2);

	HAL_UART_Receive_IT(&huart3, (uint8_t *)&aRxBuffer, 1);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	HAL_Delay(1000);
	esp8266_start_trans();
	//baoshan();
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		
		//__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,1440);
				//PI_Control(output_voltage);
				//HAL_Delay(1);

		//DO_lamp();
		//AO_lamp(light_value);
		//huxi_changliang();
		//HAL_Delay(1);
		yaokong();
		yuyin();
		mode_change(my_mode);

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef * hadc){
    HAL_ADC_Stop_DMA(&hadc1);
    if(hadc == &hadc1){
        uint16_t avg = adc_buffer[0];
        output_voltage = calculate(avg);

				uint16_t light_raw = adc_buffer[1];
				light_value = (float)light_raw * VREF / ADC_MAX;

		}
    HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adc_buffer, 2);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart == &huart3) {
    if(Uart3_Rx_Cnt >= 255) 
    {
        Uart3_Rx_Cnt = 0;
        memset(RxBuffer,0x00,sizeof(RxBuffer));
    }
    else
    {
        static uint8_t Uart3_count=0;

        RxBuffer[Uart3_Rx_Cnt] = aRxBuffer;
        if(receive_flag==0)
        {
            if(RxBuffer[Uart3_Rx_Cnt - Uart3_count] == '<')
            {
                Uart3_count++;
                if((RxBuffer[Uart3_Rx_Cnt] == '>') || Uart3_count >= 14)
                {
                    uint8_t My_i=0;
                    for(int i = Uart3_Rx_Cnt - Uart3_count + 1; i < Uart3_Rx_Cnt+1; i++)
                    {
                        my_order[My_i++]=RxBuffer[i];
                        receive_flag=1;
                        Uart3_count=0;
                    }
                }
            }
        }
        Uart3_Rx_Cnt++;
    }
    HAL_UART_Receive_IT(&huart3, (uint8_t *)&aRxBuffer, 1);
	}
}


void HAL_I2C_MemRxCpltCallback(I2C_HandleTypeDef *hi2c){
    if (hi2c->Instance == I2C1){
        voice_cmd_ready = 1;
    }
}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c){
    if (hi2c->Instance == I2C1){
        Voice_I2C_Read_Start();
    }
}

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
