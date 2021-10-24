#include "config.h"

FLASH_EraseInitTypeDef EraseInitStruct;

void init_flash()
{
     /* Fill EraseInit structure*/
    EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInitStruct.PageAddress = FLASH_USER_START_ADDR;
    EraseInitStruct.NbPages = (FLASH_USER_END_ADDR - FLASH_USER_START_ADDR) / FLASH_PAGE_SIZE;
}

/**
  * ��������: ��ȡָ����ַ�İ���(16λ����)
  * �������: faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
  * �� �� ֵ: ����ֵ:��Ӧ����.
  * ˵    ������
  */
uint16_t ReadFlashHalfWord ( uint32_t faddr )
{
    return *(__IO uint16_t*)faddr;
}

uint8_t  writeFlash(uint32_t 	write_addr,uint16_t *write_data,uint16_t len)
{
    if ( len > 512)
		{ 
			printf("len error");
      return 1;
    }
		
		
		uint16_t index = 0;
	  HAL_FLASH_Unlock();
		uint32_t PageError;  //error adress
    if (HAL_FLASHEx_Erase(&EraseInitStruct, &PageError) != HAL_OK)
    {
        return 2;
    }

	  while(len --){
	
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, write_addr, write_data[index]) == HAL_OK) {
           write_addr += 2;
  				 index ++;
        }

    }
		HAL_FLASH_Lock();
		return 0;
}

uint8_t readFlash(uint32_t 	read_addr,uint16_t *read_data,uint16_t len)
{
     uint16_t i;
     for(i = 0; i < len; i++)
     {
        read_data[i] = ReadFlashHalfWord(read_addr);//��ȡ2���ֽ�.
        read_addr += 2;//ƫ��2���ֽ�.        
     }
     return 0;
}

