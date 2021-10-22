#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "hmi_driver.h"
#include "honewell.h"
#include "stdlib.h"
uint32_t Sum1=0;                                   //流量计的合计值
float  sum_num=0;                                       //流量计合计数
uint8_t ReadBuffer[4];                                    //流量计数据存储数组
uint16_t Airflow;                                         //气体流量
float IMairflow;                                          //气体流量转换成浮点型数据
uint16_t times = 0;//计数累加防止向屏幕发数据过于频繁

void Honewel_init()
 {
   /*Honewell流量传感器的初始化程序*/
	 HAL_I2C_Master_Receive(&hi2c1 ,0x93, ReadBuffer,2, 0x10);              //向流量计发送读取数据指令
	 HAL_Delay (20);
	 HAL_I2C_Master_Receive(&hi2c1 ,0x93, &ReadBuffer[2],2, 0x10);          //向流量计发送读取数据指令
	 HAL_Delay (20);
	 HAL_I2C_Master_Receive(&hi2c1 ,0x93, &ReadBuffer[4],2, 0x10);          //向流量计发送读取数据指令
	 HAL_Delay (20);	
 }	 
void honewell_read()
{
     HAL_I2C_Master_Receive(&hi2c1 ,0x93, ReadBuffer,2, 0x10);	
     Airflow=(ReadBuffer[0]<<8|ReadBuffer[1]);	
     HAL_Delay(10);
     IMairflow =10*(((float)Airflow /16384)-0.1)/0.8+0.05;
    if(IMairflow>0&&Sum1<100)                             //流量示数平滑处理
    {
      sum_num =sum_num +IMairflow;
      Sum1 ++;
    }	
    else if(Sum1 ==100)
    {
      Sum1=0;
      sum_num =sum_num /100;
     // printf(" %f \r",sum_num);
      SetTextValueFloat2(1,2,sum_num);                     //显示流量值
      SetTextValueFloat2(2,5,sum_num);	
    }	
}
