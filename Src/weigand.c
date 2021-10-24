
//==========================================
//== �� �� ����wiegand.c
//== author	 ��
//== ˵    ����Τ��Э�麯��
//== �޸���־��2010/01 �����ļ�
//== 
//==========================================
/*
�����0��ʱ��DATA0(ͨ��Ϊ��ɫ) ���ϳ��ָ����壻
�����1��ʱ��DATA1(ͨ��Ϊ��ɫ) ���ϳ��ָ����壻
40us�������ȡ�200us��200us����������2ms
�����ȣ�50��s����������1ms��
���ż�����ڵ���2.5ms 
*/

#include "config.h"
#include "tim.h"
//========================================
//== �� �� ����SendaBit
//== ��   �� ������һ�� bit
//== ˵   �� ��
//== ʱ   �� ��2018��11 ��������
//== ��ڲ�����
//== ���ڲ�������
//== �� �� ֵ��
//=========================================
void SendaBit(unsigned char wByte)
{
	CLI();//�ر����ж�
	if((wByte&0x01)== 1) {
			HAL_GPIO_WritePin(WEIGAND,DATA1,GPIO_PIN_SET); 	 Delay_us(50); 
			HAL_GPIO_WritePin(WEIGAND,DATA1,GPIO_PIN_RESET); Delay_us(1600);
	}else{
			HAL_GPIO_WritePin(WEIGAND,DATA0,GPIO_PIN_SET);   Delay_us(50); 
			HAL_GPIO_WritePin(WEIGAND,DATA0,GPIO_PIN_RESET); Delay_us(1600);
	}
	
	SEI();//�������ж�
}


//========================================
//== �� �� ����SendaByte
//== ��   �� ������һ�� Byte
//== ˵   �� ���ȷ���λ
//== ʱ   �� ��2018��11 ��������
//== ��ڲ�����
//== ���ڲ�������
//== �� �� ֵ��
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
//== �� �� ����Weigand26
//== ��   �� ��
//== ˵   �� ��
//	weigand26 format: 	E CCCC CCCC CCCC CCCC CCCC CCCC O
//	Τ�� 26 λ�����ʽ��E XXXX XXXX XXXX XXXX XXXX XXXX O
//						ǰ 12BIT żУ�� ǰ 12 λ �� 12 λ �� 12BIT ��У��
//	�������ݴ�������˳���͡���λ��ǰ
/*
	�� 1 λ�� Ϊ�������2-13λ��żУ��λ 
	�� 2-9 λ�� ID����HID��ĵ�8λ 
	��10-25λ�� ID����PID���� 
	�� 26 λ�� Ϊ�������14-25λ����У��λ
*/
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
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
	HAL_Delay(3);	//==ʹWeigand֡���>2ms
}
//========================================
//== �� �� ����weigand34
//== ��   �� ��
//== ˵   �� ��
//	weigand34 format: E CCCC CCCC CCCC CCCC , CCCC CCCC CCCC CCCC O
//== ʱ   �� ��2010��01 ��������
//== ��ڲ�����
//== ���ڲ�����
//== �� �� ֵ��
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
	HAL_Delay(3);	//==ʹWeigand֡���>2ms
}

//========================================
//== �� �� ����Weigand66
//== ��   �� ��
//== ˵   �� ��			
//== ��ڲ�����wData, Ҫ���͵�����
//== ���ڲ�����
//== �� �� ֵ��
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
	HAL_Delay(3);	//==ʹWeigand֡���>2ms
}

/**************************************************
�������ƣ�Wiegand_Verify
�������ܣ�Τ������У��
��ڲ�����linenoΤ��������
���ز�����У��״̬
					1----�ɹ�          0-----��ʽ��֧��
					-1----żУ�����   -2----��У�����
˵����
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
						    return-1;          /* evenż��%2=0  У��λΪ0��even����%2 =1  У��λΪ1����������*/
    			  }	
    			
    			  for(k=13;k<=24;k++){
    				    if(wiegand_recv.bit[k])odd++;
    			  }
    			  if(odd%2 == wiegand_recv.bit[25]){
					    	return-2;           /*oddż��%2=0  У��λΪ1��odd����%2 =1  У��λΪ0���������*/
    			  }
    			  break;
    		}
        case 34:{         //0--even1~16     33--odd17~32	
    	      for(k=1;k<=16;k++){
    			      if(wiegand_recv.bit[k])even++;
    			  }
    			  if(even%2 != wiegand_recv.bit[0]){
    				     return-1;         /*evenż��%2=0  У��λΪ0��even����%2 =1  У��λΪ1����������*/
    			  } 
    			
    			  for(k=17;k<=32;k++){
    				    if(wiegand_recv.bit[k])odd++;
    			  }
    			  if(odd%2 == wiegand_recv.bit[33]){
                return-2;					/*oddż��%2=0  У��λΪ1��odd����%2 =1  У��λΪ0��������� */
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
    				    return-2;         /*oddż��%2=0  У��λΪ1��odd����%2 =1  У��λΪ0��������� */
    			  }
    			  break;
    		}
        default: return 0;
    }
    return 1;
}

/**************************************************
�������ƣ�Bit_To_Hex
�������ܣ�Τ������BITתHEX
��ڲ�����linenoΤ��������
���ز�������
˵����
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
