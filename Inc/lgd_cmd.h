#include "usart.h"
void Usart2_Process(void);
void Usart3_Process();

/*!
* @prototype : void Start_Sensor()
* @ameter(s): 开启传感器
*/
void Start_Sensor(void);
/*!
* @prototype : Stop_Sensor()
* @ameter(s): 关闭传感器
*/
void Stop_Sensor(void);
/*!
* @prototype : void HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen)
* @ameter(s): [OUT] pbDest - 存放目标字符串
  @[IN] pbSrc - 输入16进制数的起始地址
  @[IN] nLen - 16进制数的字节数
  @return value: 
  @remarks : 将16进制数转化为字符串
*/
void HexToStr(uint8_t *pbDest, uint8_t *pbSrc, int nLen);
