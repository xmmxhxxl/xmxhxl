
#include "config.h"
extern void rs485Send(unsigned char *tx485,uint16_t len);
/*************************************************
函数名称：rsHkCheckFormat
函数功能：海康报文格式检验
入口参数：报文BUFF以及长度
返回参数：-1校验不成功，1校验成功
说明：  
报文格式检验
preamble,5:0xAA 0xAA,0xAA,0x96,0x69
len1,len2,2:Data数据长度
CMD,1:    0x30对应数据段为卡信息
para,1:   &0x80否比对通过  &0x40是否为黑名单     
Data,<3000:   数据
chk_sum,1：校验位
*************************************************/

int HkCheckFormat(uint8_t *buff, uint16_t buff_len)
{
	  unsigned char chk_sum = 0x00;
	  

	  if(memcmp(buff,"\xAA\xAA\xAA\x96\x69",5) != 0) {
			  printf("start error\n");
				return -1;
	  }
	/*	printf("buff_len=%d\n",buff_len);
		for(int i=5;i<buff_len;i++){
		    chk_sum=chk_sum^buff[i];
		    printf("buff[%d]=%x  chk[%d]=%x   ",i,buff[i],i,chk_sum);
	  }*/
	  printf("\n");
	  if(chk_sum){  
				printf("chk_sum error\n");			
		    return -2;	
	  }
	  if((buff[7] != 0x30)&&(buff[8] != 0x81)){ 
				printf("card error\n");				
		    return -3;  
		}
	
	  return 0;
}

/*******************************************
报文转韦根输出
buff：报文体 韦根输出口数据起始位
len：报文体 韦根长度
*******************************************/
uint8_t  messageToWg(uint8_t *buff,uint8_t len)
{	
	  uint8_t send_buff[8] = {0};
    printf("wgoutput\n");
	  switch (len)
		{   
			  case 0x00 : break;
				case 0x01 : memcpy(&send_buff[2],&buff[0],1);
			              Weigand26(send_buff);  /*0~3byte send by wg26*/
      				    	break;
				case 0x02 : 
			              memcpy(&send_buff[1],&buff[0],2);
			              Weigand26(send_buff);  /*0~3byte send by wg26*/
      				   	  break;
        case 0x03 : memcpy(&send_buff[0],&buff[0],3);
			              Weigand26(send_buff);  /*0~3byte send by wg26*/
      				    	 break;
      	case 0x04 : Weigand34(&buff[0]);  /*4byte send by wg34*/
      					    break;
				case 0x05 : memcpy(&send_buff[3],&buff[0],5);
								    Weigand66(send_buff);  /*5~8byte send by wg66*/
      					    break;
				case 0x06 : memcpy(&send_buff[2],&buff[0],6);
			              Weigand66(send_buff);  /*5~8byte send by wg66*/
      					    break;
				case 0x07 : memcpy(&send_buff[1],&buff[0],7);
								    Weigand66(send_buff);  /*5~8byte send by wg66*/
      					    break;
        case 0x08 : memcpy(&send_buff[0],&buff[0],8);
					          Weigand66(send_buff);  /*5~8byte send by wg66*/
      					    break;
        default: Weigand34(&buff[0]);
								 break;
     }	
		 return 1;		
}


/*********//*************************************************
函数名称：standFormat
函数功能： 一卡通报文格式校验
入口参数：报文BUFF以及长度
返回参数：-1校验不成功，1校验成功
说明：  
报文格式检验
preamble,5:0xAA 0xAA, 
CMD,1:    0x01 
len1,len2,2:Data数据长度
para,1:   待确认
Data,<1024:   数据
chk_sum,1：校验位
*************************************************/

int standFormat(uint8_t *buff, uint16_t buff_len)
{
	  unsigned char chk_sum = 0x00;
	  
  printf("wgcmp\n");
	  if(memcmp(buff,"\xAA\x66",2) != 0) {
			  printf("head error\n");
				return -1;
	  }
		printf("buff_len=%d\n",buff_len);  
		
		/*  除AA 66全部校验   */
		for(int i=2;i<buff_len;i++){      
		    chk_sum=chk_sum^buff[i];
		    printf("buff[%d]=%x  chk[%d]=%x   ",i,buff[i],i,chk_sum);
	  }
	  printf("\n");
	  if(chk_sum){  
				printf("chk_sum error\n");			
		    return -2;	
	  }
	
	  return 0;
}
/************************************************************
发送后台数据组包
messages[]：出参，最后输出报文
buff:组包数据段
buff_len:组包数据段长度
type:设备类型0x01二维码，0x02人脸设备
*************************************************************/
void  combeMessages(unsigned char messages[],uint8_t *buff,uint16_t buff_len,uint8_t type)
{
		uint16_t datalen=0x00; 
	  messages[0] = 0xAA;
	  messages[1] = 0xAA;	
	  messages[2] = type;	
	
		messages[3] = 0xFF&(buff_len>>8);
	  messages[4] = 0xFF&buff_len;
		memcpy(messages+5,buff,buff_len);
		datalen = 5 + buff_len;	
	
		messages[datalen]=0x00;	
		for(uint16_t n=0;n<buff_len+3;n++){
       	messages[datalen] = messages[datalen]^messages[n+2];
		}			
	 
}

/*******************************************
FACE转韦根输出
buff：报文体 韦根输出口数据起始位
len：报文体 韦根长度
*******************************************/
int  faceMesToWg(uint8_t *card,uint8_t len)
{	
    if(len>16)  return -1;
   printf("len = %d\n",len);
   int i,j;
   unsigned char buff[8];
   unsigned long long int id=0;
   for(i = 0 ;i < len; i++){
      card[i] = card[i] - '0';
      id = id*10 + card[i];
   }    
   printf("id = %lld\n",id);

   for(j = 7;j >= 0;j--){
      buff[7-j] = (id>>(8*j)) & 0xff;
      printf("buff[%d] = %x\n",7-j,buff[7-j]);
   }

   if(id > 0xffffff){
      if(id > 0xffffffff){
         printf("wg66 = %x,%x,%x,%x,%x,%x,%x,%x\n",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7]);
           Weigand66(buff);
      }
      else{
         printf("wg34 = %x,%x,%x,%x\n",buff[4],buff[5],buff[6],buff[7]);
				 Weigand34(buff+4);
     }
   }
   else{
      printf("wg26 = %x,%x,%x\n",buff[5],buff[6],buff[7]);	
		  Weigand26(buff+4);
   }
		 return 1;		
}
