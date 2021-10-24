#ifndef __WEIGAND_H__
#define __WEIGAND_H__

#define DATA0                    GPIO_PIN_8   //== DATA0(通常为绿色)
#define DATA1										 GPIO_PIN_9   //== DATA1(通常为白色)
#define WEIGAND                   GPIOB
#define CLI() __set_PRIMASK(1)	//关闭总中断		
#define SEI() __set_PRIMASK(0)  //开启总中断


 void SendaBit(unsigned char wByte);
 void SendaByte(unsigned char wByte);
 void Weigand26(unsigned char *wData);
 void Weigand34(unsigned char *wData);
 void Weigand66(unsigned char *wData);
 
int wiegandVerify(WgRecvS wiegand_recv);
void bitToHex(WgRecvS wiegand_recv, uint8_t *hexdata);
#endif
