#include "usart.h"
void Usart2_Process(void);
void Usart3_Process();

/*!
* @prototype : void Start_Sensor()
* @ameter(s): ����������
*/
void Start_Sensor(void);
/*!
* @prototype : Stop_Sensor()
* @ameter(s): �رմ�����
*/
void Stop_Sensor(void);
/*!
* @prototype : void HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen)
* @ameter(s): [OUT] pbDest - ���Ŀ���ַ���
  @[IN] pbSrc - ����16����������ʼ��ַ
  @[IN] nLen - 16���������ֽ���
  @return value: 
  @remarks : ��16������ת��Ϊ�ַ���
*/
void HexToStr(uint8_t *pbDest, uint8_t *pbSrc, int nLen);
