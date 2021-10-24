
//==========================================
//== 文 件 名：wiegand.c
//== author	 ：
//== 说    明：韦根协议函数
//== 修改日志：2010/01 创建文件
//== 
//==========================================
/*
输出‘0’时：DATA0(通常为绿色) 线上出现负脉冲；
输出‘1’时：DATA1(通常为白色) 线上出现负脉冲；
40us≤脉冲宽度≤200us，200us≤脉冲间隔≤2ms
脉冲宽度：50μs；脉冲间隔：1ms。
卡号间隔大于等于2.5ms 
*/

#include "config.h"
#include "tim.h"
//========================================
//== 函 数 名：SendaBit
//== 功   能 ：发送一个 bit
//== 说   明 ：
//== 时   间 ：2018、11 创建函数
//== 入口参数：
//== 出口参数：无
//== 返 回 值：
//=========================================
void SendaBit(unsigned char wByte)
{
	CLI();//关闭总中断
	if((wByte&0x01)== 1) {
			HAL_GPIO_WritePin(WEIGAND,DATA1,GPIO_PIN_SET); 	 Delay_us(50); 
			HAL_GPIO_WritePin(WEIGAND,DATA1,GPIO_PIN_RESET); Delay_us(1600);
	}else{
			HAL_GPIO_WritePin(WEIGAND,DATA0,GPIO_PIN_SET);   Delay_us(50); 
			HAL_GPIO_WritePin(WEIGAND,DATA0,GPIO_PIN_RESET); Delay_us(1600);
	}
	
	SEI();//开启总中断
}


//========================================
//== 函 数 名：SendaByte
//== 功   能 ：发送一个 Byte
//== 说   明 ：先发高位
//== 时   间 ：2018、11 创建函数
//== 入口参数：
//== 出口参数：无
//== 返 回 值：
//=========================================
void SendaByte(unsigned char wByte)
{
	int i; 
	unsigned char Byte;
	for( i = 0 ; i < 8; i ++ ) {
		Byte = wByte & 0x80;
		Byte = Byte>>7;
		SendaBit(Byte);
		wByte <<= 1;
	}
}
//========================================
//== 函 数 名：Weigand26
//== 功   能 ：
//== 说   明 ：
//	weigand26 format: 	E CCCC CCCC CCCC CCCC CCCC CCCC O
//	韦根 26 位输出格式：E XXXX XXXX XXXX XXXX XXXX XXXX O
//						前 12BIT 偶校验 前 12 位 后 12 位 后 12BIT 奇校验
//	以上数据从左至右顺序发送。高位在前
/*
	第 1 位： 为输出数据2-13位的偶校验位 
	第 2-9 位： ID卡的HID码的低8位 
	第10-25位： ID卡的PID号码 
	第 26 位： 为输出数据14-25位的奇校验位
*/
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//=========================================
void Weigand26(unsigned char *wData)
{
	unsigned char one_num = 0;
	unsigned char even = 0;
	unsigned char odd = 0;
	unsigned char check_temp,i;
	
	check_temp = *wData;
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)	one_num++;
		check_temp >>= 1;
	}
	check_temp = (*(wData + 1))&0xf0;
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)		one_num++;
		check_temp >>= 1;
	}
	if(one_num % 2 )even = 1;
	else even = 0;

	one_num = 0;
	check_temp = (*(wData + 1))&0x0F;
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)		one_num++;
		check_temp >>= 1;
	}
	check_temp = *(wData + 2);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)		one_num++;
		check_temp >>= 1;
	}
	
	if(one_num % 2 )odd = 0;
	else odd = 1;

	//==Send the 26 Bits
	SendaBit(even);
	SendaByte(wData[0]);
	SendaByte(wData[1]);
	SendaByte(wData[2]);
	SendaBit(odd);	 
	 printf("WG26\n");
	HAL_Delay(3);	//==使Weigand帧间隔>2ms
}
//========================================
//== 函 数 名：weigand34
//== 功   能 ：
//== 说   明 ：
//	weigand34 format: E CCCC CCCC CCCC CCCC , CCCC CCCC CCCC CCCC O
//== 时   间 ：2010、01 创建函数
//== 入口参数：
//== 出口参数：
//== 返 回 值：
//=========================================
void Weigand34(unsigned char *wData)
{
	unsigned char one_num = 0;
	unsigned char even = 0;
	unsigned char odd = 0;
	unsigned char check_temp,i;
	check_temp = *wData;
	
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)	one_num++;
		check_temp >>= 1;
	}
	
	check_temp = *(wData + 1);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)		one_num++;
		check_temp >>= 1;
	}
	if(one_num % 2 )even = 1;
	else even = 0;
	
	one_num = 0;
	check_temp = *(wData + 2);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)	one_num++;
		check_temp >>= 1;
	}
	check_temp = *(wData + 3);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)one_num++;
		check_temp >>= 1;
	}
	if(one_num % 2 )odd = 0;
	else odd = 1;
	
	//Send the 34 Bits
	SendaBit(even);
	SendaByte(wData[0]);
	SendaByte(wData[1]);
	SendaByte(wData[2]);
	SendaByte(wData[3]);
	SendaBit(odd);		
  printf("WG34\n");	
	HAL_Delay(3);	//==使Weigand帧间隔>2ms
}

//========================================
//== 函 数 名：Weigand66
//== 功   能 ：
//== 说   明 ：			
//== 入口参数：wData, 要发送的数据
//== 出口参数：
//== 返 回 值：
//=========================================
void Weigand66(unsigned char *wData)
{	
	unsigned char one_num = 0;
	unsigned char even = 0;
	unsigned char odd = 0;
	unsigned char check_temp,i;
	check_temp = *wData;
	
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)	one_num++;
		check_temp >>= 1;
	}
	
	check_temp = *(wData + 1);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)		one_num++;
		check_temp >>= 1;
	}
	check_temp = *(wData + 2);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)		one_num++;
		check_temp >>= 1;
	}
	check_temp = *(wData + 3);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)		one_num++;
		check_temp >>= 1;
	}
	if(one_num % 2 )even = 1;
	else even = 0;
	
	one_num = 0;
	check_temp = *(wData + 4);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)	one_num++;
		check_temp >>= 1;
	}
	check_temp = *(wData + 5);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)one_num++;
		check_temp >>= 1;
	}
	check_temp = *(wData + 6);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)	one_num++;
		check_temp >>= 1;
	}
	check_temp = *(wData + 7);
	for(i = 0;i < 8;i++){
		if(check_temp & 0x01)one_num++;
		check_temp >>= 1;
	}
	if(one_num % 2 )odd = 0;
	else odd = 1;
	

	//Send the 66 Bits
	SendaBit(even);
	SendaByte(wData[0]);
	SendaByte(wData[1]);
	SendaByte(wData[2]);
	SendaByte(wData[3]);
	SendaByte(wData[4]);
	SendaByte(wData[5]);
	SendaByte(wData[6]);
	SendaByte(wData[7]);
	SendaBit(odd);	
printf("WG66\n");
	HAL_Delay(3);	//==使Weigand帧间隔>2ms
}

/**************************************************
函数名称：Wiegand_Verify
函数功能：韦根数据校验
入口参数：lineno韦根输入编号
返回参数：校验状态
					1----成功          0-----格式不支持
					-1----偶校验错误   -2----奇校验错误
说明：
**************************************************/
int wiegandVerify(WgRecvS wiegand_recv)
{
    int k;
    int even=0,odd=0;

    switch (wiegand_recv.len){   
        case 26:{           /*0--even1~12    25--odd13~24 */ 
    		    for(k=1;k<=12;k++){
    				    if(wiegand_recv.bit[k])even++;
    			  }
    			  if(even%2 != wiegand_recv.bit[0]){
						    return-1;          /* even偶数%2=0  校验位为0；even奇数%2 =1  校验位为1；不等有误*/
    			  }	
    			
    			  for(k=13;k<=24;k++){
    				    if(wiegand_recv.bit[k])odd++;
    			  }
    			  if(odd%2 == wiegand_recv.bit[25]){
					    	return-2;           /*odd偶数%2=0  校验位为1；odd奇数%2 =1  校验位为0；相等有误*/
    			  }
    			  break;
    		}
        case 34:{         //0--even1~16     33--odd17~32	
    	      for(k=1;k<=16;k++){
    			      if(wiegand_recv.bit[k])even++;
    			  }
    			  if(even%2 != wiegand_recv.bit[0]){
    				     return-1;         /*even偶数%2=0  校验位为0；even奇数%2 =1  校验位为1；不等有误*/
    			  } 
    			
    			  for(k=17;k<=32;k++){
    				    if(wiegand_recv.bit[k])odd++;
    			  }
    			  if(odd%2 == wiegand_recv.bit[33]){
                return-2;					/*odd偶数%2=0  校验位为1；odd奇数%2 =1  校验位为0；相等有误 */
    			  } 
    			  break;
    		}	
        case 66:{         /*0--even1~32     65--odd33~64*/
    	      for(k=1;k<=32;k++){
    			      if(wiegand_recv.bit[k])even++;
    			  }   
    			  if(even%2 != wiegand_recv.bit[0]){
    				    return-1;
    			  }
    			
    		   	for(k=33;k<=64;k++){
    				    if(wiegand_recv.bit[k])odd++;
    		  	}
    			  if(odd%2 == wiegand_recv.bit[65]){
    				    return-2;         /*odd偶数%2=0  校验位为1；odd奇数%2 =1  校验位为0；相等有误 */
    			  }
    			  break;
    		}
        default: return 0;
    }
    return 1;
}

/**************************************************
函数名称：Bit_To_Hex
函数功能：韦根数据BIT转HEX
入口参数：lineno韦根输入编号
返回参数：无
说明：
**************************************************/
void bitToHex(WgRecvS wiegand_recv, uint8_t *hexdata)
{
    int i = 0,j = 0; 
    switch (wiegand_recv.len){				
    	  case 26:for(i=0;i<3;i++){
										for(j=1;j<9;j++){
											  hexdata[i] += wiegand_recv.bit[i*8+j]<<(8-j);		
										}	
									}
								break;
    	  case 34:for(i=0;i<4;i++){
										for(j=1;j<9;j++){
											  hexdata[i] += wiegand_recv.bit[i*8+j]<<(8-j);		
										}													
								}
    					  break;
    	  case 66:for(i=0;i<8;i++){
										for(j=1;j<9;j++){
												hexdata[i] += wiegand_recv.bit[i*8+j]<<(8-j);
										}
    					  }
						  	break;
    	  default:break;
    }
}
