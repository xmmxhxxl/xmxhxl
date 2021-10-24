#ifndef __FALSH_H__
#define __FLASH_H__

#define ADDR_FLASH_PAGE_15     ((uint32_t)0x0800F800) /* Base @ of Page 63, 1 Kbyte */
#define ADDR_FLASH_PAGE_16     ((uint32_t)0x0800FC00) /* Base @ of Page 64, 1 Kbyte */

#define FLASH_USER_START_ADDR   ADDR_FLASH_PAGE_15          /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_PAGE_16  /*((uint32_t)0x08010000)      End @ of user Flash area */



void     init_flash(void);
uint16_t ReadFlashHalfWord (uint32_t faddr );
uint8_t  writeFlash(uint32_t 	write_addr,uint16_t *write_data,uint16_t len);
uint8_t  readFlash (uint32_t 	read_addr,  uint16_t *read_data,uint16_t len);


#endif
