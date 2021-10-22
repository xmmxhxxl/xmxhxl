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
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmd_process.h"
#include "flash.h"
#include "honewell.h"
#include "lgd_cmd.h"
#include "RC522.h"
#include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint8_t System_state =1;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
 /**SPI1 GPIO Configuration    
    PA4     ------> SPI1_NSS
    PA5     ------> SPI1_SCK
    PA6     ------> SPI1_MISO
    PA7     ------> SPI1_MOSI 
		PB0     ------> RESET
		PB10    ------> Usart3 Tx
    */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
uint8_t Card_Type1[2];  
uint8_t Card_ID[4]; 
uint8_t Card_KEY[6] = {0xff,0xff,0xff,0xff,0xff,0xff};    //{0x11,0x11,0x11,0x11,0x11,0x11};   //?ü??
uint8_t Card_Data[16];
uint8_t status;
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
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
    //开启串口空闲中断
	//__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
  //__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	//__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);

  /*开启串口接收DMA及中断*/
//	HAL_UART_Receive_DMA(&huart1,u1_rxbuf,BUFFER_SIZE);
//	HAL_UART_Receive_DMA(&huart2,u2_rxbuf,BUFFER_SIZE);	
//	HAL_UART_Receive_DMA(&huart3,u3_rxbuf,BUFFER_SIZE);	
	
	
	Card_Type1[0]=0x04;
	Card_Type1[1]=0x00;
	
	PcdReset();  //复位RC522
	PcdAntennaOff();  //关闭天线
	HAL_Delay(100);
	PcdAntennaOn();  //打开天线
	printf(" ");  //"唤醒蓝牙"
	printf("开始寻卡");  //"Card Type(0x%04X):"
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
		HAL_Delay(10);
		if(MI_OK==PcdRequest(0x52, Card_Type1))  //寻卡函数，如果成功返回MI_OK  打印多次卡号
//		if(MI_OK==PcdRequest(0x26, Card_Type1))  //寻卡函数，如果成功返回MI_OK  打印多次卡号
		{
			uint16_t cardType = (Card_Type1[0]<<8)|Card_Type1[1];//读不同卡的类型
			printf("卡类型：(0x%04X)\r\n",cardType);  //"Card Type(0x%04X):"
			switch(cardType){
			case 0x4400:
					printf("Mifare UltraLight\r\n");
					break;
			case 0x0400:
					printf("Mifare One(S50)\r\n");
					break;
			case 0x0200:
					printf("Mifare One(S70)\r\n");
					break;
			case 0x0800:
					printf("Mifare Pro(X)\r\n");
					break;
			case 0x4403:
					printf("Mifare DESFire\r\n");
					break;
			default:
					printf("Unknown Card\r\n");
					break;
			}
			
			status = PcdAnticoll(Card_ID);//防冲撞 如果成功返回MI_OK
			if(status != MI_OK){
					printf("Anticoll Error\r\n");
			}else{
					printf("Serial Number:%02X%02X%02X%02X\r\n",Card_ID[0],Card_ID[1],Card_ID[2],Card_ID[3]);
			}
			
			status = PcdSelect(Card_ID);  //选卡 如果成功返回MI_OK
			if(status != MI_OK){
					printf("Select Card Error\r\n");
			}
			else
				printf("Select Card OK\r\n");
			status = PcdAuthState(PICC_AUTHENT1A,5,Card_KEY,Card_ID);//验证卡密码 如果成功返回MI_OK
			if(status != MI_OK){
					printf("Auth State Error\r\n");
					continue;}
			status = PcdAuthState(PICC_AUTHENT1A,5,Card_KEY,Card_ID);//验证卡密码 如果成功返回MI_OK
			if(status != MI_OK){
					printf("Auth State Error\r\n");
					continue;
			}
//			
//			memset(Card_ID,1,4);  //函数用于为变量开辟空间，或将变量都赋值为一个值
//			memset(Card_Data,1,16);
//			Card_Data[0]=0xaa;
//			status = PcdWrite(5,Card_Data);                   //写入0XAA,0X01,0X01……选择块地址写入数据，成功则返回MI_OK
//			if(status != MI_OK){
//					printf("Card Write Error\r\n");
//					continue;
//			}
//			memset(Card_Data,0,16);  //清零
//			delay_us(8);
//			
//			
//			status = PcdRead(5,Card_Data);                    //再一次把它读取出来16字节的卡片数据，选择块地址读出数据，成功则返回MI_OK
//			if(status != MI_OK){
//					printf("Card Read Error\r\n");
//					continue;
//			}else{
//				for(i=0;i<16;i++){
//					printf("%02X ",Card_Data[i]);
//				}
//				printf("\r\n");
//			}
//			
//			memset(Card_Data,2,16);
//			Card_Data[0]=0xbb;
//			delay_us(8);
//			status = PcdWrite(5,Card_Data);                   //写入0Xbb,0X02,0X02……
//			if(status != MI_OK){
//					printf("Card Write Error\r\n");
//					continue;
//			}
//			delay_us(8);
//			
//			status = PcdRead(5,Card_Data);                    //再一次把它读取出来16字节的卡片数据
//			if(status != MI_OK){
//					printf("Card Read Error\r\n");
//					continue;
//			}else{
//				for(i=0;i<16;i++){
//					printf("%02X ",Card_Data[i]);
//				}
//				printf("\r\n");
//			}
			
			//memset(Card_Data,0,16);
		//	PcdHalt();  //卡片进入休眠状态
			
			status = PcdHalt();  //卡片进入休眠状态
			if(status != MI_OK){
					printf("PcdHalt Error\r\n");			
			}
			else
			{
					printf("PcdHalt OK\r\n");	
			}
	
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
		 if(bUart1==1)
    {
      bUart1 = 0;
      HAL_UART_Transmit(&huart2,u1_rxbuf,u1_len,0XFF);
      Usart1_Process();
  
      HAL_UART_Receive_DMA(&huart1,u1_rxbuf,BUFFER_SIZE);
    }
    if(bUart2==1)
    {
      bUart2 = 0;
      Usart2_Process();
      HAL_UART_Receive_DMA(&huart2,u2_rxbuf,BUFFER_SIZE);
    }
   if(bUart3==1)
    {
      bUart3 = 0;
      Usart3_Process();
      HAL_UART_Receive_DMA(&huart3,u3_rxbuf,BUFFER_SIZE);
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

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
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
