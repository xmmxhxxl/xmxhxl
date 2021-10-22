#include "lgd_cmd.h"
#include "usart.h"
#include "stdio.h"
#include "flash.h"
#include "hmi_driver.h"
uint8_t Times = 0;
uint8_t Temp_Flag = 0;
float temp_1;
float temp_2;
/*************************************************************************************
指令解析
红外人体体温传感器指令解析
0：帧头: 固定为0x5A (Uart)  0x60 0x7n  (RF rx dongle)
1: CMD Code 0xFF
2: TA Data:ambient temperature(环境温度) 0x00~0xFF  Ambient_Temperature<0x80  Ambient_Temperature=0x19(25℃)+Ambient_Temperature
                                                    Ambient_Temperature≥0x80  Ambient_Temperature=0x19(25℃)+!(Ambient_Temperature)
3: TB Data:human temperature(人体温度) 0x00~0xFF  human_Temperature<0x80  human_Temperature=0x24(36℃)+human_Temperature
                                                  human_Temperature≥0x80  human_Temperature=0x24(36℃)+!(human_Temperature)
4：Status Data:  0x00：no error
                 0x20: overflow
                 0x40: device error 
5:帧尾：固定为0xA5                                                  
*/

/*定义联合体 存放采集上来的传感器数值*/
typedef  union 
	{
          float FLOAT;
	        uint8_t character;
} ITOF;
ITOF Ambient_Temp,human_Temp; 
char    ddl,ddh;
/*声明定义联合体 */
extern union flash
{
  float flash;
  uint16_t sflash[4];
}Alarm,Lnd,Hnd,Xlnd,Xhnd,K,B ;
uint8_t Error,Fault1;
void HexToStr(uint8_t *pbDest, uint8_t *pbSrc, int nLen);
void Usart2_Process()
{
       bUart2 = 0;                           //将标志位置零                     
       if(u2_rxbuf[0]==0x5A&&u2_rxbuf[5]==0xA5&&u2_rxbuf[1]==0xFF)  //判断接收的指令是否为数据指令
				{
            if(u2_rxbuf[2]>127)//环境温度
            {
              Ambient_Temp.character = u2_rxbuf[2];
              Ambient_Temp.FLOAT = (Ambient_Temp.character-0xFF)*1.0/10.0+25.0;
              printf("%.1f",Ambient_Temp.FLOAT);
            }
            else
            {
              Ambient_Temp.character = u2_rxbuf[2];
              Ambient_Temp.FLOAT = Ambient_Temp.character*1.0/10.0 + 25.0;
              printf("%.1f",Ambient_Temp.FLOAT);
            }
            if(u2_rxbuf[3]>127)//人体温度
            {
              human_Temp.character = u2_rxbuf[3];
              human_Temp.FLOAT = (human_Temp.character-0xFF)*1.0/10.0+36.0;
              printf("%.1f",human_Temp.FLOAT);
            }
            else
            {
              human_Temp.character = u2_rxbuf[3];
              human_Temp.FLOAT = human_Temp.character*1.0/10.0 + 36.0;
              printf("%.1f",human_Temp.FLOAT);
            }
 /*
  算法逻辑：
          1.检测到人的存在
          2.确定温度输出（相信传感器的输出值 即2S后稳定输出）
          3.检测到人的离开
          
*/
//          if(Temp_Flag==0&&human_Temp.FLOAT>35&&human_Temp.FLOAT<40) //检测到人员存在并且
//          {
//            Times++;
//            if(Times==8)
//            {
//               Times=0;
//               Temp_Flag = 1;
//               if(Temp_Flag == 1)
//               {
//                 printf("%.1f",human_Temp.FLOAT);
//               }
//            }
//          }

//          if(human_Temp.FLOAT<35)//人员离开
//          {
//            if(Temp_Flag == 1){Temp_Flag = 0;
//            }//人员测量完毕离开
//            if(Temp_Flag == 0){Times = 0;
//            }//人员未测量完毕 离开 从新计数
//          }	

        }	
      }
void Usart3_Process()
{
       bUart2 = 0;                           //将标志位置零                     
       if(u3_rxbuf[0]==0x5A&&u3_rxbuf[5]==0xA5&&u3_rxbuf[1]==0xFF)  //判断接收的指令是否为数据指令
				{
          if(u3_rxbuf[2]>127)//环境温度
          {
            Ambient_Temp.character = u3_rxbuf[2];
            Ambient_Temp.FLOAT = (Ambient_Temp.character-0xFF)*1.0/10.0+25.0;
            printf("%.1f",Ambient_Temp.FLOAT);
          }
          else
          {
            Ambient_Temp.character = u3_rxbuf[2];
            Ambient_Temp.FLOAT = Ambient_Temp.character*1.0/10.0 + 25.0;
            printf("%.1f",Ambient_Temp.FLOAT);
          }
          if(u3_rxbuf[3]>127)//人体温度
          {
            human_Temp.character = u3_rxbuf[3];
            human_Temp.FLOAT = (human_Temp.character-0xFF)*1.0/10.0+36.0;
            printf("%.1f",human_Temp.FLOAT);
          }
          else
          {
            human_Temp.character = u3_rxbuf[3];
            human_Temp.FLOAT = human_Temp.character*1.0/10.0 + 36.0;
            printf("%.1f",human_Temp.FLOAT);
          }

 }
        }
/*!
* @prototype : void HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen)
* @ameter(s): [OUT] pbDest - 存放目标字符串
  @[IN] pbSrc - 输入16进制数的起始地址
  @[IN] nLen - 16进制数的字节数
  @return value: 
  @remarks : 将16进制数转化为字符串
*/
void HexToStr(uint8_t *pbDest, uint8_t *pbSrc, int nLen)
{
  int i;
for (i=0; i<nLen; i++)
{
ddh = 48 + pbSrc[i] / 16;
ddl = 48 + pbSrc[i] % 16;
if (ddh > 57) ddh = ddh + 7;
if (ddl > 57) ddl = ddl + 7;
pbDest[i*2] = ddh;
pbDest[i*2+1] = ddl;
}

pbDest[nLen*2] = '\0';
}
