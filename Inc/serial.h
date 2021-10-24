#ifndef __SERIAL_H__
#define __SERIAL_H__

#define HK_REGSITER   "\xAA\xAA\xAA\x96\x69\x00\x03\x10\x00\x13"
#define HK_REG_ACK 		"\xAA\xAA\xAA\x96\x69\x00\x03\x06\x01\x04"
#define SET_ACK 		  "\xAA\xAD\x4F\x4B "
#define INQURE_ACK    "\xAA\xAD\x00\x4F\x4B"


#define QR_DEVICE      0x01
#define FACE_DEVICE    0x02

int HkCheckFormat(uint8_t *buff, uint16_t buff_len);
int standFormat(uint8_t *buff, uint16_t buff_len);
void  combeMessages(unsigned char messages[],uint8_t *buff,uint16_t buff_len,uint8_t type);
uint8_t  messageToWg(uint8_t *buff,uint8_t len);
int  faceMesToWg(uint8_t *card,uint8_t len);


#endif
