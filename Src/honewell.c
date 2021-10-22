#include "stm32f1xx_hal.h"
#include "i2c.h"
#include "hmi_driver.h"
#include "honewell.h"
#include "stdlib.h"
uint32_t Sum1=0;                                   //�����Ƶĺϼ�ֵ
float  sum_num=0;                                       //�����ƺϼ���
uint8_t ReadBuffer[4];                                    //���������ݴ洢����
uint16_t Airflow;                                         //��������
float IMairflow;                                          //��������ת���ɸ���������
uint16_t times = 0;//�����ۼӷ�ֹ����Ļ�����ݹ���Ƶ��

void Honewel_init()
 {
   /*Honewell�����������ĳ�ʼ������*/
	 HAL_I2C_Master_Receive(&hi2c1 ,0x93, ReadBuffer,2, 0x10);              //�������Ʒ��Ͷ�ȡ����ָ��
	 HAL_Delay (20);
	 HAL_I2C_Master_Receive(&hi2c1 ,0x93, &ReadBuffer[2],2, 0x10);          //�������Ʒ��Ͷ�ȡ����ָ��
	 HAL_Delay (20);
	 HAL_I2C_Master_Receive(&hi2c1 ,0x93, &ReadBuffer[4],2, 0x10);          //�������Ʒ��Ͷ�ȡ����ָ��
	 HAL_Delay (20);	
 }	 
void honewell_read()
{
     HAL_I2C_Master_Receive(&hi2c1 ,0x93, ReadBuffer,2, 0x10);	
     Airflow=(ReadBuffer[0]<<8|ReadBuffer[1]);	
     HAL_Delay(10);
     IMairflow =10*(((float)Airflow /16384)-0.1)/0.8+0.05;
    if(IMairflow>0&&Sum1<100)                             //����ʾ��ƽ������
    {
      sum_num =sum_num +IMairflow;
      Sum1 ++;
    }	
    else if(Sum1 ==100)
    {
      Sum1=0;
      sum_num =sum_num /100;
     // printf(" %f \r",sum_num);
      SetTextValueFloat2(1,2,sum_num);                     //��ʾ����ֵ
      SetTextValueFloat2(2,5,sum_num);	
    }	
}
