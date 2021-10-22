#include "flash.h"
#include "hmi_driver.h"
#include "main.h"

uint16_t k;/*定义K值*/
uint16_t b;/*定义b值*/
uint16_t sysSet[100];
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
union flash
{
  float flash;
  uint16_t sflash[4];
}Alarm,Lnd,Hnd,Xlnd,Xhnd,K,B ;
//函数功能：flash的读取与写入
//注：flash的擦除智能整页或者整bank的擦除
//1Page=1Kbit

//FLASH写入
//@param  TypeProgram : TYPEPROGRAM_HALFWORD/TYPEPROGRAM_WORD/TYPEPROGRAM_DOUBLEWORD
void writeFlash(uint32_t StartAddr, uint16_t*Data, uint8_t len)
{
  //1、解锁FLASH
  HAL_FLASH_Unlock();

  //2、擦除FLASH
  //初始化FLASH_EraseInitTypeDef
  FLASH_EraseInitTypeDef f;
  f.TypeErase = FLASH_TYPEERASE_PAGES;//擦除整页
  f.PageAddress = StartAddr;
  f.NbPages = 1;
  //设置PageError
  uint32_t PageError = 0;
  //调用擦除函数
  HAL_FLASHEx_Erase(&f, &PageError);

  //3、对FLASH烧写
	while(len)
  {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, StartAddr, *Data);
		StartAddr+=2;
		Data++;
		len--;
	}
  //4、锁住FLASH
  HAL_FLASH_Lock();
}

//FLASH读取,len<=64
void readFlash(uint32_t StartAddr, uint16_t*Data, uint8_t len)
{
	for(uint8_t i=0; i<len; i++)
	{
		Data[i] = (*(__IO uint16_t*)(StartAddr));
		StartAddr+=2;
	}
}
/*!
  @void Ini_Data();
  @brief 初始化数据，读取flash中存储的数据
  @param 并且将其赋值给联合体，发送给显示屏
*/
void Ini_Data()
{
    readFlash(Page1,sysSet,64);
    Lnd.sflash[0] =  sysSet[0];
    Lnd.sflash[1] =  sysSet[1];
    Lnd.sflash[2] =  sysSet[2];
    Lnd.sflash[3] =  sysSet[3];
  
    Hnd.sflash[0] = sysSet[4];
    Hnd.sflash[1] = sysSet[5];
    Hnd.sflash[2] = sysSet[6];
    Hnd.sflash[3] = sysSet[7];
  
    Xlnd.sflash[0] = sysSet[8];
    Xlnd.sflash[1] = sysSet[9];
    Xlnd.sflash[2] = sysSet[10];
    Xlnd.sflash[3] = sysSet[11];
  
    Xhnd.sflash[0] = sysSet[12];
    Xhnd.sflash[1] = sysSet[13];
    Xhnd.sflash[2] = sysSet[14];
    Xhnd.sflash[3] = sysSet[15];
  
    Alarm.sflash[0] = sysSet[16];
    Alarm.sflash[1] = sysSet[17];
    Alarm.sflash[2] = sysSet[18];
    Alarm.sflash[3] = sysSet[19];
    
    K.sflash[0] = sysSet[20];
    K.sflash[1] = sysSet[21];
    K.sflash[2] = sysSet[22];
    K.sflash[3] = sysSet[23];
    
    B.sflash[0] = sysSet[24];
    B.sflash[1] = sysSet[25];
    B.sflash[2] = sysSet[26];
    B.sflash[3] = sysSet[27];
  
    SetTextValueFloat1(3,4,Lnd.flash);
    SetTextValueFloat1(3,6,Hnd.flash);
    SetTextValueFloat1(3,5,Xlnd.flash);
    SetTextValueFloat1(3,7,Xhnd.flash);
    SetTextValueFloat1(3,10,Alarm.flash);
    SetTextValueFloat1(3,8,K.flash);
    SetTextValueFloat1(3,9,B.flash);
}
/*!
  @void Calculate()//计算Kb值 
  @brief初始化数据，读取flash中存储的数据
  @param并且将其赋值给联合体，发送给显示屏
*/
void Calculate()//计算Kb值 
{
  k = (Xhnd.flash-Xlnd.flash)/(Hnd.flash-Lnd.flash);
  b=Xlnd.flash - k*Lnd.flash;
  K.flash = k;
  B.flash = b;
  
    sysSet[20] = K.sflash[0];
    sysSet[21] = K.sflash[1];
    sysSet[22] = K.sflash[2];
    sysSet[23] = K.sflash[3];
  
    
    sysSet[24]=B.sflash[0];  
    sysSet[25]=B.sflash[1]; 
    sysSet[26]=B.sflash[2]; 
    sysSet[27]=B.sflash[3]; 
  
   SetTextValueFloat1(3,8,K.flash);
   SetTextValueFloat1(3,9,B.flash);
}
/*!
  @brief根据斜率计算最终函数值的结果
  @param Value 当前浓度值
*/
float  Span_Cal(float Vaule)
{
  k = K.flash ;
  b = B.flash ;
  Vaule=k*Vaule+b;
  return Vaule;
}
/*!
  @void store_value(uint8_t control_id,float value)
  @brief 将屏幕写进的函数值存储到flash中
  @param：control_id 屏幕控件
  @param：value 屏幕写进的值
*/
void store_value(uint8_t control_id,float value)
{
  
     if(control_id==4)     //低气浓度                                                      
        {           
            Lnd.flash = value;
         // for(int i=0;i==3;i++)
           // {
              sysSet[0]=Lnd.sflash[0];  //0对应浮点数最高位 3对应浮点数最低位
              sysSet[1]=Lnd.sflash[1];
              sysSet[2]=Lnd.sflash[2];
              sysSet[3]=Lnd.sflash[3];
            //}
        }                  
           
        if(control_id==6)    //高气浓度
        {
          Hnd.flash = value;
              sysSet[4]=Hnd.sflash[0];  //0对应浮点数最高位 3对应浮点数最低位
              sysSet[5]=Hnd.sflash[1];
              sysSet[6]=Hnd.sflash[2];
              sysSet[7]=Hnd.sflash[3];
        }
         if(control_id==5)    //低点修正
        {
              Xlnd.flash = value;
              sysSet[8]=Xlnd.sflash[0];  //0对应浮点数最高位 3对应浮点数最低位
              sysSet[9]=Xlnd.sflash[1];
              sysSet[10]=Xlnd.sflash[2];
              sysSet[11]=Xlnd.sflash[3];
      
        }
         if(control_id==7)   //高点修正
        {
            Xhnd.flash = value;
              sysSet[12]=Xhnd.sflash[0];  //0对应浮点数最高位 3对应浮点数最低位
              sysSet[13]=Xhnd.sflash[1];
              sysSet[14]=Xhnd.sflash[2];
              sysSet[15]=Xhnd.sflash[3];
            
        }
         if(control_id==10)   //报警值
        {
            Alarm.flash =  value;
            sysSet[16]=Alarm.sflash[0];  //0对应浮点数最高位 3对应浮点数最低位
            sysSet[17]=Alarm.sflash[1];
            sysSet[18]=Alarm.sflash[2];
            sysSet[19]=Alarm.sflash[3];
        }
        Calculate();//读取标定值 并且计算k  b值 
        writeFlash(Page1,sysSet,64);//将存储数据写入Flash中
}
