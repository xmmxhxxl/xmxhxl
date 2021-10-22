#ifndef _FLASH_H_
#define _FLASH_H_ 
#include "stm32f1xx_hal.h"
#include "main.h"
//64KΪ0x10000    -2k=F800
#define Page1				0x0800F800//���һ��page1


#define DEVICE_IP				0x0800F000//�����ڶ���page2-1
#define DEVICE_SUBNET		0x0800F004//page2-2
#define DEVICE_GATEWAY	0x0800F008//page2-3

void writeFlash(uint32_t StartAddr, uint16_t*Data, uint8_t len);
void readFlash(uint32_t StartAddr, uint16_t*Data, uint8_t len);
/*
  �������������� ���ڴ洢��Ļ���ݵ�flash�� ������ ���ݵ���ʱ�����ڴ��н����ݵ��ó���
  @param��Alarm����������ֵ
  @param��Lnd����Ũ������
  @param��Hnd����Ũ������
  @param��Xlnd����Ũ�ȵ�����ֵ
  @param��Xhnd����Ũ�ȵ�����ֵ
  @param��K������б��K
  @param��B�������ص�B
*/
extern union flash Alarm,Lnd,Hnd,Xlnd,Xhnd,K,B ;
/*
  void Ini_Data();
  ��ʼ�����ݣ���ȡflash�д洢������
  @param�����ҽ��丳ֵ�������壬���͸���ʾ��
*/
void Ini_Data(void);
/*
  void Calculate()//����Kbֵ 
  ��ʼ�����ݣ���ȡflash�д洢������
  @param�����ҽ��丳ֵ�������壬���͸���ʾ��
*/
void Calculate(void);
/*
  void Calculate()//����Kbֵ 
  ����б�ʼ������պ���ֵ�Ľ��
  @param��
*/
float  Span_Cal(float Vaule);

/*
  void store_value(uint8_t control_id,float value)
  ����Ļд���ĺ���ֵ�洢��flash��
  @param��control_id ��Ļ�ؼ�
  @param��value ��Ļд����ֵ
*/
void store_value(uint8_t control_id,float value);

#endif
