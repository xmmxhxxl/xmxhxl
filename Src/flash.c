#include "flash.h"
#include "hmi_driver.h"
#include "main.h"

uint16_t k;/*����Kֵ*/
uint16_t b;/*����bֵ*/
uint16_t sysSet[100];
/*
  �������������� ���ڴ洢��Ļ���ݵ�flash�� ������ ���ݵ���ʱ�����ڴ��н����ݵ��ó���
  @param��Alarm����������ֵ
  @param��Lnd����Ũ������
  @param��Hnd����Ũ������
  @param��Xlnd����Ũ�ȵ�����ֵ
  @param��Xhnd����Ũ�ȵ�����ֵ
  @param��K������б��K
  @param��B�������ص�B
*/
union flash
{
  float flash;
  uint16_t sflash[4];
}Alarm,Lnd,Hnd,Xlnd,Xhnd,K,B ;
//�������ܣ�flash�Ķ�ȡ��д��
//ע��flash�Ĳ���������ҳ������bank�Ĳ���
//1Page=1Kbit

//FLASHд��
//@param  TypeProgram : TYPEPROGRAM_HALFWORD/TYPEPROGRAM_WORD/TYPEPROGRAM_DOUBLEWORD
void writeFlash(uint32_t StartAddr, uint16_t*Data, uint8_t len)
{
  //1������FLASH
  HAL_FLASH_Unlock();

  //2������FLASH
  //��ʼ��FLASH_EraseInitTypeDef
  FLASH_EraseInitTypeDef f;
  f.TypeErase = FLASH_TYPEERASE_PAGES;//������ҳ
  f.PageAddress = StartAddr;
  f.NbPages = 1;
  //����PageError
  uint32_t PageError = 0;
  //���ò�������
  HAL_FLASHEx_Erase(&f, &PageError);

  //3����FLASH��д
	while(len)
  {
		HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD, StartAddr, *Data);
		StartAddr+=2;
		Data++;
		len--;
	}
  //4����סFLASH
  HAL_FLASH_Lock();
}

//FLASH��ȡ,len<=64
void readFlash(uint32_t StartAddr, uint16_t*Data, uint8_t len)
{
	for(uint8_t i=0; i<len; i++)
	{
		Data[i] = (*(__IO uint16_t*)(StartAddr));
		StartAddr+=2;
	}
}
/*!
  @void Ini_Data();
  @brief ��ʼ�����ݣ���ȡflash�д洢������
  @param ���ҽ��丳ֵ�������壬���͸���ʾ��
*/
void Ini_Data()
{
    readFlash(Page1,sysSet,64);
    Lnd.sflash[0] =  sysSet[0];
    Lnd.sflash[1] =  sysSet[1];
    Lnd.sflash[2] =  sysSet[2];
    Lnd.sflash[3] =  sysSet[3];
  
    Hnd.sflash[0] = sysSet[4];
    Hnd.sflash[1] = sysSet[5];
    Hnd.sflash[2] = sysSet[6];
    Hnd.sflash[3] = sysSet[7];
  
    Xlnd.sflash[0] = sysSet[8];
    Xlnd.sflash[1] = sysSet[9];
    Xlnd.sflash[2] = sysSet[10];
    Xlnd.sflash[3] = sysSet[11];
  
    Xhnd.sflash[0] = sysSet[12];
    Xhnd.sflash[1] = sysSet[13];
    Xhnd.sflash[2] = sysSet[14];
    Xhnd.sflash[3] = sysSet[15];
  
    Alarm.sflash[0] = sysSet[16];
    Alarm.sflash[1] = sysSet[17];
    Alarm.sflash[2] = sysSet[18];
    Alarm.sflash[3] = sysSet[19];
    
    K.sflash[0] = sysSet[20];
    K.sflash[1] = sysSet[21];
    K.sflash[2] = sysSet[22];
    K.sflash[3] = sysSet[23];
    
    B.sflash[0] = sysSet[24];
    B.sflash[1] = sysSet[25];
    B.sflash[2] = sysSet[26];
    B.sflash[3] = sysSet[27];
  
    SetTextValueFloat1(3,4,Lnd.flash);
    SetTextValueFloat1(3,6,Hnd.flash);
    SetTextValueFloat1(3,5,Xlnd.flash);
    SetTextValueFloat1(3,7,Xhnd.flash);
    SetTextValueFloat1(3,10,Alarm.flash);
    SetTextValueFloat1(3,8,K.flash);
    SetTextValueFloat1(3,9,B.flash);
}
/*!
  @void Calculate()//����Kbֵ 
  @brief��ʼ�����ݣ���ȡflash�д洢������
  @param���ҽ��丳ֵ�������壬���͸���ʾ��
*/
void Calculate()//����Kbֵ 
{
  k = (Xhnd.flash-Xlnd.flash)/(Hnd.flash-Lnd.flash);
  b=Xlnd.flash - k*Lnd.flash;
  K.flash = k;
  B.flash = b;
  
    sysSet[20] = K.sflash[0];
    sysSet[21] = K.sflash[1];
    sysSet[22] = K.sflash[2];
    sysSet[23] = K.sflash[3];
  
    
    sysSet[24]=B.sflash[0];  
    sysSet[25]=B.sflash[1]; 
    sysSet[26]=B.sflash[2]; 
    sysSet[27]=B.sflash[3]; 
  
   SetTextValueFloat1(3,8,K.flash);
   SetTextValueFloat1(3,9,B.flash);
}
/*!
  @brief����б�ʼ������պ���ֵ�Ľ��
  @param Value ��ǰŨ��ֵ
*/
float  Span_Cal(float Vaule)
{
  k = K.flash ;
  b = B.flash ;
  Vaule=k*Vaule+b;
  return Vaule;
}
/*!
  @void store_value(uint8_t control_id,float value)
  @brief ����Ļд���ĺ���ֵ�洢��flash��
  @param��control_id ��Ļ�ؼ�
  @param��value ��Ļд����ֵ
*/
void store_value(uint8_t control_id,float value)
{
  
     if(control_id==4)     //����Ũ��                                                      
        {           
            Lnd.flash = value;
         // for(int i=0;i==3;i++)
           // {
              sysSet[0]=Lnd.sflash[0];  //0��Ӧ���������λ 3��Ӧ���������λ
              sysSet[1]=Lnd.sflash[1];
              sysSet[2]=Lnd.sflash[2];
              sysSet[3]=Lnd.sflash[3];
            //}
        }                  
           
        if(control_id==6)    //����Ũ��
        {
          Hnd.flash = value;
              sysSet[4]=Hnd.sflash[0];  //0��Ӧ���������λ 3��Ӧ���������λ
              sysSet[5]=Hnd.sflash[1];
              sysSet[6]=Hnd.sflash[2];
              sysSet[7]=Hnd.sflash[3];
        }
         if(control_id==5)    //�͵�����
        {
              Xlnd.flash = value;
              sysSet[8]=Xlnd.sflash[0];  //0��Ӧ���������λ 3��Ӧ���������λ
              sysSet[9]=Xlnd.sflash[1];
              sysSet[10]=Xlnd.sflash[2];
              sysSet[11]=Xlnd.sflash[3];
      
        }
         if(control_id==7)   //�ߵ�����
        {
            Xhnd.flash = value;
              sysSet[12]=Xhnd.sflash[0];  //0��Ӧ���������λ 3��Ӧ���������λ
              sysSet[13]=Xhnd.sflash[1];
              sysSet[14]=Xhnd.sflash[2];
              sysSet[15]=Xhnd.sflash[3];
            
        }
         if(control_id==10)   //����ֵ
        {
            Alarm.flash =  value;
            sysSet[16]=Alarm.sflash[0];  //0��Ӧ���������λ 3��Ӧ���������λ
            sysSet[17]=Alarm.sflash[1];
            sysSet[18]=Alarm.sflash[2];
            sysSet[19]=Alarm.sflash[3];
        }
        Calculate();//��ȡ�궨ֵ ���Ҽ���k  bֵ 
        writeFlash(Page1,sysSet,64);//���洢����д��Flash��
}
