/**
  ******************************************************************************
  * File Name          : USART.h
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
/* 串口接收缓冲区 */
#define BUFFER_SIZE 100
extern uint8_t u1_rxbuf[100];
extern uint8_t u2_rxbuf[100];
extern uint8_t u3_rxbuf[100];
extern uint8_t u4_rxbuf[100];
extern uint8_t u5_rxbuf[100];
extern uint8_t u1_txbuf[100];
extern uint8_t u2_txbuf[100];
extern uint8_t u3_txbuf[100];
extern uint8_t u4_txbuf[100];
extern uint8_t u5_txbuf[100];
/* 串口接收完成标记 */
extern uint8_t bUart1;
extern uint8_t bUart2;
extern uint8_t bUart3;
extern uint8_t bUart4;
extern uint8_t bUart5;
/* 串口接收数据长度 */
extern uint8_t u1_len;
extern uint8_t u2_len;
extern uint8_t u3_len;
extern uint8_t u4_len;
extern uint8_t u5_len;
/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);
void MX_USART2_UART_Init(void);
void MX_USART3_UART_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
