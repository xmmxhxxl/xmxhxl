/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
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
#include "stm32f1xx_hal.h"
#include "dma.h"
#include "iwdg.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "config.h"
#define ALL_WG_DATA  3
#define WG_LINE_0    0
#define WG_LINE_1    1
#define WG_LINE_2    2
#define SYS_PARA_ADDR  ADDR_FLASH_PAGE_15 
/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/
uint8_t  message_pack[2048];
WgRecvS   g_wiegand_recv[3];             /*韦根接收缓冲区*/
int8_t    g_line;                        /*韦根接收编号*/
int16_t   g_wg_time_over;                /*韦根接收完成超时计时*/ 
uint8_t   g_wg_cmp_flag;                 /*韦根数据比较标志*/
int16_t   g_wg_valid_time ;              /*韦根输入有效时间*/
SysPara    sys_para;                       /* 存储数据*/      
uint16_t   flash_sys_para[sizeof(SysPara)/2] = {0x00};    /* flash读写缓冲区 */  
uint8_t   g_address = 0x01;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

void wgParamReset(int8_t lineno);
uint8_t standardProc(void);

/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration----------------------------------------------------------*/

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
  MX_USART3_UART_Init();
  MX_USART1_UART_Init();
  MX_TIM3_Init();
  MX_IWDG_Init();

  /* USER CODE BEGIN 2 */
  USER_UART_Init();
	wgParamReset(ALL_WG_DATA);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
    printf("start\n");
  while (1)
  {
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
		HAL_IWDG_Refresh(&hiwdg);
		if(g_wg_time_over < 0){	
			 standardProc();
    }		
  }
  /* USER CODE END 3 */

}

/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/*printf重定义*/
int fputc(int ch,FILE *f)
{
#ifdef DEBUG_PRINT
    uint8_t temp[1] = {ch};
  HAL_UART_Transmit(&huart3, temp, 1,2);

#endif
		return ch;
}

/**************************************************
函数名称：系统滴答计时器回调函数
函数功能： 1ms进入一次
入口参数：无
返回参数：无
说明： 库函数定义 “虚函数”
**************************************************/
void HAL_SYSTICK_Callback()
{
    if(g_line != -1){
	      g_wg_time_over--;
	  }
		if(g_wg_cmp_flag == 1){
			  g_wg_valid_time--;
		}
}
/**************************************************
函数名称：外部中断回调函数
函数功能：韦根信号读取
入口参数：中断管脚
返回参数：无
说明：  库函数定义 “虚函数”
**************************************************/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	  g_wg_time_over = 5;

		switch (GPIO_Pin){
					case GPIO_PIN_1:  g_line=WG_LINE_0;
													  g_wiegand_recv[WG_LINE_0].bit[g_wiegand_recv[WG_LINE_0].len] = !HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0);														
													  g_wiegand_recv[WG_LINE_0].len++;	
												 	  break;
					case GPIO_PIN_3:  g_line=WG_LINE_1;
													  g_wiegand_recv[WG_LINE_1].bit[g_wiegand_recv[WG_LINE_1].len] = !HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_2);				
														g_wiegand_recv[WG_LINE_1].len++;	
													  break;
					case GPIO_PIN_14: g_line=WG_LINE_2;
													  g_wiegand_recv[WG_LINE_2].bit[g_wiegand_recv[WG_LINE_2].len] = !HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15);	
													  g_wiegand_recv[WG_LINE_2].len++;	
													  break;
					default:  break;
		}
		
}
/**************************************************
函数名称：standardProc
函数功能：标准函数处理流程
入口参数：无
返回参数：0数据不输出，1数据输出
说明：  库函数定义 “虚函数”
**************************************************/
uint8_t standardProc(void)
{
    CLI();             /*关闭总中断*/
	
    printf("\nline = %d\n",g_line);  
    printf("\nlen = %d\n",g_wiegand_recv[g_line].len);
    for(int i=0;i<=g_wiegand_recv[g_line].len;i++){
    printf("%d ",g_wiegand_recv[g_line].bit[i]);	
    }
    HAL_IWDG_Refresh(&hiwdg);
    
     if(wiegandVerify(g_wiegand_recv[g_line])!= 1){
         printf("Verify Error\n");
    	   wgParamReset(g_line);
    		   return 0;      /*校验失败，清除数据不进行处理*/
     }	
     bitToHex(g_wiegand_recv[g_line],g_wiegand_recv[g_line].hex);
		 
		 
		 /*打印数据*/
     printf("Data = ");
     for(int n=0;n<g_wiegand_recv[g_line].len/8;n++){
        	printf("%x ",g_wiegand_recv[g_line].hex[n]);
     }
     printf("\n");
    printf("send\n");
		HAL_IWDG_Refresh(&hiwdg);
		 
		 /*读卡器直接转韦根输出*/
    switch (g_wiegand_recv[g_line].len)
    {
        case 26 : Weigand26(g_wiegand_recv[g_line].hex);
        		 		  break;
        case 34 : Weigand34(g_wiegand_recv[g_line].hex);
        				  break;
        case 66 : Weigand66(g_wiegand_recv[g_line].hex); 
        					break;
        default : break;
     }	

		wgParamReset(ALL_WG_DATA);
	  SEI();//开启总中断
		return 1;
}
/**************************************************
函数名称：Param_Reset
函数功能：参数重置
入口参数：重置参数路编号
返回参数：无
说明：0-->g_wiegand_recv[0],1-->g_wiegand_recv[1]
			2-->g_wiegand_recv[2],3-->所有 
**************************************************/
void wgParamReset(int8_t lineno)
{
	 HAL_IWDG_Refresh(&hiwdg);
		if((lineno < 3)&&(lineno > -1)){
		    memset(g_wiegand_recv[lineno].hex, 0, 8);
				memset(g_wiegand_recv[lineno].bit, 0, 66);
				g_wiegand_recv[lineno].len	 = 0;
		}else if(lineno == 3){
				for(int j=0; j<3; j++){
				    memset(g_wiegand_recv[j].hex, 0, 8);
				    memset(g_wiegand_recv[j].bit, 0, 66);
				    g_wiegand_recv[j].len	 = 0;
				}
		} 
		
		g_line = -1;
		g_wg_cmp_flag = 0;
    g_wg_time_over = 5;
		g_wg_valid_time = 5000;
		SEI();              /*开启总中断*/
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void _Error_Handler(char * file, int line)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler_Debug */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
