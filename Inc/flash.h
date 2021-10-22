#ifndef _FLASH_H_
#define _FLASH_H_ 
#include "stm32f1xx_hal.h"
#include "main.h"
//64K为0x10000    -2k=F800
#define Page1				0x0800F800//最后一个page1


#define DEVICE_IP				0x0800F000//倒数第二个page2-1
#define DEVICE_SUBNET		0x0800F004//page2-2
#define DEVICE_GATEWAY	0x0800F008//page2-3

void writeFlash(uint32_t StartAddr, uint16_t*Data, uint8_t len);
void readFlash(uint32_t StartAddr, uint16_t*Data, uint8_t len);
/*
  定义联合体数据 用于存储屏幕数据到flash中 联合体 数据调用时将从内存中将数据调用出来
  @param：Alarm：报警数据值
  @param：Lnd：低浓度数据
  @param：Hnd：高浓度数据
  @param：Xlnd：低浓度的修正值
  @param：Xhnd：高浓度的修正值
  @param：K：修正斜率K
  @param：B：修正截点B
*/
extern union flash Alarm,Lnd,Hnd,Xlnd,Xhnd,K,B ;
/*
  void Ini_Data();
  初始化数据，读取flash中存储的数据
  @param：并且将其赋值给联合体，发送给显示屏
*/
void Ini_Data(void);
/*
  void Calculate()//计算Kb值 
  初始化数据，读取flash中存储的数据
  @param：并且将其赋值给联合体，发送给显示屏
*/
void Calculate(void);
/*
  void Calculate()//计算Kb值 
  根据斜率计算最终函数值的结果
  @param：
*/
float  Span_Cal(float Vaule);

/*
  void store_value(uint8_t control_id,float value)
  将屏幕写进的函数值存储到flash中
  @param：control_id 屏幕控件
  @param：value 屏幕写进的值
*/
void store_value(uint8_t control_id,float value);

#endif
