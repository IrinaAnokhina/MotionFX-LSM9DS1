/**
 ******************************************************************************
 * @file    stm32f4xx_hal_msp.c
 * @author  MEMS Software Solutions Team
 * @brief   HAL MSP module.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2018 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup MOTION_APPLICATIONS MOTION APPLICATIONS
 * @{
 */

/** @addtogroup DATALOG_FUSION DATALOG FUSION
 * @{
 */

extern int UseLSI; /* This "redundant" line is here to fulfil MISRA C-2012 rule 8.4 */
int UseLSI = 0;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  RTC MSP Initialization
 *         This function configures the hardware resources used in this example
 * @param  hrtc: RTC handle pointer
 *
 * @note   Care must be taken when HAL_RCCEx_PeriphCLKConfig() is used to select
 *         the RTC clock source; in this case the Backup domain will be reset in
 *         order to modify the RTC Clock source, as consequence RTC registers (including
 *         the backup registers) and RCC_BDCR register are set to their reset values.
 *
 * @retval None
 */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef        RCC_OscInitStruct;
  RCC_PeriphCLKInitTypeDef  PeriphClkInitStruct;

  /*##-1- Enables the PWR Clock and Enables access to the backup domain ###################################*/
  /* To change the source clock of the RTC feature (LSE, LSI), You have to:
     - Enable the power clock using __HAL_RCC_PWR_CLK_ENABLE()
     - Enable write access using HAL_PWR_EnableBkUpAccess() function before to
       configure the RTC clock source (to be done once after reset).
     - Reset the Back up Domain using __HAL_RCC_BACKUPRESET_FORCE() and
       __HAL_RCC_BACKUPRESET_RELEASE().
     - Configure the needed RTC clock source */
  __HAL_RCC_PWR_CLK_ENABLE();
  HAL_PWR_EnableBkUpAccess();

  /*##-2- Configue LSE/LSI as RTC clock soucre ###############################*/
  if (UseLSI == 0)
  {
    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSEState = RCC_LSE_ON;
    RCC_OscInitStruct.LSIState = RCC_LSI_OFF;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  }
  else
  {
    RCC_OscInitStruct.OscillatorType =  RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
    RCC_OscInitStruct.LSIState = RCC_LSI_ON;
    RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
      Error_Handler();
    }

    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
    PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSI;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
    {
      Error_Handler();
    }
  }

  /*##-2- Enable RTC peripheral Clocks #######################################*/
  /* Enable RTC Clock */
  __HAL_RCC_RTC_ENABLE();

  /*##-4- Configure the NVIC for RTC Alarm ###################################*/
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0x0F, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}

/**
 * @brief  RTC MSP De-Initialization
 *         This function frees the hardware resources used in this example:
 *           - Disable the Peripheral's clock
 * @param  hrtc: RTC handle pointer
 * @retval None
 */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
  __HAL_RCC_RTC_DISABLE();
}

/**
 * @brief  TIM MSP Initialization
 *         This function configures the hardware resources used in this example:
 *            - Peripheral's clock enable
 *            - Peripheral's GPIO Configuration
 * @param  htim: TIM handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
  if (htim_base->Instance == TIM_ALGO)
  {
    /* Peripheral clock enable */
    TIM_ALGO_CLK_ENABLE();

    /* System interrupt init*/
    HAL_NVIC_SetPriority(TIM_ALGO_IRQn, 0x0F, 0);
    HAL_NVIC_EnableIRQ(TIM_ALGO_IRQn);
  }
}

/**
 * @brief  TIM MSP DeInitialization
 *         This function configures the hardware resources used in this example:
 *            - Peripheral's clock enable
 *            - Peripheral's GPIO Configuration
 * @param  htim: TIM handle pointer
 * @retval None
 */
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
{
  if (htim_base->Instance == TIM_ALGO)
  {
    /* Peripheral clock disable */
    TIM_ALGO_CLK_DISABLE();
    /* Peripheral interrupt DeInit*/
    HAL_NVIC_DisableIRQ(TIM_ALGO_IRQn);
  }
}

/**
 * @brief  CRC MSP Initialization
 *         This function configures the hardware resources used in this example:
 *           - Peripheral's clock enable
 * @param  hcrc: CRC handle pointer
 * @retval None
 */
void HAL_CRC_MspInit(CRC_HandleTypeDef *hcrc)
{
  if (hcrc->Instance == CRC)
  {
    /* Peripheral clock enable */
    __HAL_RCC_CRC_CLK_ENABLE();
  }
}

/**
 * @brief  CRC MSP DeInitialization
 *         This function configures the hardware resources used in this example:
 *           - Peripheral's clock disable
 * @param  hcrc: CRC handle pointer
 * @retval None
 */
void HAL_CRC_MspDeInit(CRC_HandleTypeDef *hcrc)
{
  if (hcrc->Instance == CRC)
  {
    /* Peripheral clock disable */
    __HAL_RCC_CRC_CLK_DISABLE();
  }
}

/**
 * @brief  Initializes I2C MSP.
 * @param  hi2c : I2C handler
 * @retval None
 */
void HAL_I2C_MspInit(I2C_HandleTypeDef *hi2c)
{
  GPIO_InitTypeDef  gpio_init;
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
if(hi2c->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C2_MspInit 0 */

  /* USER CODE END I2C2_MspInit 0 */
  
		
		  __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2C3 GPIO Configuration    
    PC9     ------> I2C3_SDA
    PA8     ------> I2C3_SCL 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL ;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull =GPIO_NOPULL ;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		 /* I2C2 clock enable */
    __HAL_RCC_I2C3_CLK_ENABLE();
/*
 gpio_init.Pin = GPIO_PIN_3 | GPIO_PIN_10;
  gpio_init.Mode = GPIO_MODE_AF_OD;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = GPIO_AF9_I2C2;

  HAL_GPIO_Init(GPIOB, &gpio_init);*/
  
  /* USER CODE BEGIN I2C2_MspInit 1 */

  /* USER CODE END I2C2_MspInit 1 */
  }
	else{
  /* Enable I2C clock */
  __HAL_RCC_I2C1_CLK_ENABLE();

  /* Enable GPIO clock */
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Configure I2C Tx, Rx as alternate function */
  gpio_init.Pin = GPIO_PIN_8 | GPIO_PIN_9;
  gpio_init.Mode = GPIO_MODE_AF_OD;
  gpio_init.Pull = GPIO_PULLUP;
  gpio_init.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  gpio_init.Alternate = GPIO_AF4_I2C1;

  HAL_GPIO_Init(GPIOB, &gpio_init);
	}
}

/**
 * @brief  DeInitializes I2C MSP.
 * @param  hi2c : I2C handler
 * @retval None
 */
void HAL_I2C_MspDeInit(I2C_HandleTypeDef *hi2c)
{
	 if(hi2c->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C2_MspDeInit 0 */

  /* USER CODE END I2C2_MspDeInit 0 */
    /* Peripheral clock disable */
      /**I2C3 GPIO Configuration    
    PC9     ------> I2C3_SDA
    PA8     ------> I2C3_SCL 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);

  /* USER CODE BEGIN I2C2_MspDeInit 1 */

  /* USER CODE END I2C2_MspDeInit 1 */
  }
	else{
  /* Disable I2C clock */
  __HAL_RCC_I2C1_CLK_DISABLE();

  /* DeInitialize peripheral GPIOs */
  HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8 | GPIO_PIN_9);
	}
}

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
