#include "main.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "gpio.h"
#include "hmi_driver.h"
#include "cmd_queue.h"
#include "cmd_process.h"
#include "flash.h"
#include "main.h"
uint8_t cmd_buffer[CMD_MAX_SIZE];                                               //��ʾ��ָ���
uint8_t  lgd_size;
uint8_t current_screen_id = 0;
void Cmd_ProcessMessage( PCTRL_MSG msg, uint16_t size );

/*! 
 *  \brief  ��Ϣ��������
 *  \param msg ��������Ϣ
 *  \param size ��Ϣ����
 */
 void Usart1_Process()
 {
   
   /*
   >�����������ı����ݺ�ᵼ�½��������쳣  
�ڴ������Դ���ָ��C�ļ�cmd_queue�У�һ������������������þ��ǶԴ����ָ�������dma���͵�rx_buffer�е����ݽ���ѹջ����ջ����
�������£�
1.��rx_buffer�е�ָ��ѹջ queue_push����������que.headջͷ������ָ��ĳ�������
2.�ж��Ƿ��������queue_size(),����������ݣ��������ݵĴ�С
3.�����ݽ��н��������ȳ�ջ����ջʱque.tail��ջ�׵������ȳ�������ʱque.tail�Ļ����ӣ����Ƕ����QUEUE_MAX_SIZE����ȡ�࣬����������ȡ��״̬��ȣ�����Ψһ�Ŀ����ԡ��ᵼ��ѹջ���߳�ջ��ʱ�����ݴ��ҡ�
4.queue_find_cmd�������ٽ������ݽ�����
>û��ָ�����֮ǰ����һ�Ρ�
   
   */
      bUart1 = 0;
      for( uint8_t i=0;i<=u1_len;i++)                      //�ѽ��ճ���Ϊrx_len�����������ջ����
             {
			        uint8_t data = u1_rxbuf[i];
              queue_push(data);
             }		 
          lgd_size = queue_find_cmd(u1_rxbuf,CMD_MAX_SIZE);                              //�ӻ������л�ȡһ��ָ��   
            queue_reset();  //��仰����ӵ����λ��
          if(lgd_size>0&&u1_rxbuf[1]!=0x07)                                              //���յ�ָ�� �����ж��Ƿ�Ϊ������ʾ
            {                                                                           
              Cmd_ProcessMessage((PCTRL_MSG)u1_rxbuf, lgd_size);                             //ָ���  
            }             
          else if(lgd_size>0&&u1_rxbuf[1]==0x07)                                         //���Ϊָ��0x07��������STM32  
            {                                                                           
               __disable_fault_irq();                                                   
               NVIC_SystemReset();                                                                                                                                          
            }                      

 }

void Cmd_ProcessMessage( PCTRL_MSG msg, uint16_t size )
{
	uint8_t cmd_type = msg->cmd_type;//ָ������
	uint8_t ctrl_msg = msg->ctrl_msg;   //��Ϣ������
	uint8_t control_type = msg->control_type;//�ؼ�����
	uint16_t screen_id = PTR2U16(&msg->screen_id);//����ID
	uint16_t control_id = PTR2U16(&msg->control_id);//�ؼ�ID
	uint32_t value = PTR2U32(msg->param);//��ֵ

	switch(cmd_type)
	{		
	case NOTIFY_TOUCH_PRESS://����������
	case NOTIFY_TOUCH_RELEASE://�������ɿ�
		NotifyTouchXY(cmd_buffer[1],PTR2U16(cmd_buffer+2),PTR2U16(cmd_buffer+4));
		break;	
	case NOTIFY_WRITE_FLASH_OK://дFLASH�ɹ�
		NotifyWriteFlash(1);
		break;
	case NOTIFY_WRITE_FLASH_FAILD://дFLASHʧ��
		NotifyWriteFlash(0);
		break;
	case NOTIFY_READ_FLASH_OK://��ȡFLASH�ɹ�
		NotifyReadFlash(1,cmd_buffer+2,size-6);//ȥ��֡ͷ֡β
		break;
	case NOTIFY_READ_FLASH_FAILD://��ȡFLASHʧ��
		NotifyReadFlash(0,0,0);
		break;
	case NOTIFY_READ_RTC://��ȡRTCʱ��
		NotifyReadRTC(cmd_buffer[1],cmd_buffer[2],cmd_buffer[3],cmd_buffer[4],cmd_buffer[5],cmd_buffer[6],cmd_buffer[7]);
		break;
	case NOTIFY_TIMER:
		SetScreen(1);
	case NOTIFY_CONTROL:
		{
			if(ctrl_msg==MSG_GET_CURRENT_SCREEN)//����ID�仯֪ͨ
			{
				NotifyScreen(screen_id);
			}
			else
			{
				switch(control_type)
				{
				case kCtrlButton: //��ť�ؼ�
					NotifyButton1(screen_id,control_id,msg->param[1]);
					break;
				case kCtrlText://�ı��ؼ�
					NotifyText(screen_id,control_id,msg->param);
					break;
				case kCtrlProgress: //�������ؼ�                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
					NotifyProgress(screen_id,control_id,value);
					break;
				case kCtrlSlider: //�������ؼ�
					NotifySlider(screen_id,control_id,value);
					break;
				case kCtrlMeter: //�Ǳ�ؼ�
					NotifyMeter(screen_id,control_id,value);
					break;
				case kCtrlMenu://�˵��ؼ�
					NotifyMenu(screen_id,control_id,msg->param[0],msg->param[1]);
					break;
				case kCtrlSelector://ѡ��ؼ�
					NotifySelector(screen_id,control_id,msg->param[0]);
					break;
				case kCtrlRTC://����ʱ�ؼ�
					NotifyTimer(screen_id,control_id);
					break;
				default:
					break;
				}
			}			
		}
		break;
	default:
		break;
	}
}



/*! 
 *  \brief  �����л�֪ͨ
 *  \details  ��ǰ����ı�ʱ(�����GetScreen)��ִ�д˺���
 *  \param screen_id ��ǰ����ID
 */
void NotifyScreen(uint16_t screen_id)
{
	//TODO: ����û�����
	current_screen_id = screen_id;//�ڹ��������п��������л�֪ͨ����¼��ǰ����ID

if(current_screen_id==1)//�ı����ú���ʾ
	{

	
	}
else if(current_screen_id==3)//���ú���ʾ
	{
	}
}

/*! 
 *  \brief  ���������¼���Ӧ
 *  \param press 1���´�������3�ɿ�������
 *  \param x x����
 *  \param y y����
 */
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y)
{
	//TODO: ����û�����
}



/*! 
 *  \brief  ��ť�ؼ�֪ͨ
 *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param state ��ť״̬��0����1����
 */
void NotifyButton1(uint16_t screen_id, uint16_t control_id, uint8_t  state)
{
  if(screen_id==1)
  {
    if(System_state==1&&state==1)
    {
       HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_RESET);//�ر����� �����궨ģʽ
       System_state = 0;
    }
    if(System_state==0&&state==0)
    {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, GPIO_PIN_SET);//���� ����ģʽ
      System_state = 1;
    }
  }
}
//void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t  state)
//{
//  
//}

/*! 
 *  \brief  �ı��ؼ�֪ͨ
 *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param str �ı��ؼ�����
 */
void NotifyText(uint16_t screen_id, uint16_t control_id, uint_least8_t *str)
{
	//TODO: ����û�����
 if(screen_id == 3)//�궨����
  {
    float value;
    value = string_to_float(str);
    store_value(control_id,value);
  }
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�IDv
 *  \param value ֵ
 */
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �������ؼ�֪ͨ
 *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: ����û�����
	
}

/*! 
 *  \brief  �Ǳ�ؼ�֪ͨ
 *  \details  ����GetControlValueʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param value ֵ
 */
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  �˵��ؼ�֪ͨ
 *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item �˵�������
 *  \param state ��ť״̬��0�ɿ���1����
 */
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ѡ��ؼ�֪ͨ
 *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 *  \param item ��ǰѡ��
 */
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ʱ����ʱ֪ͨ����
 *  \param screen_id ����ID
 *  \param control_id �ؼ�ID
 */
void NotifyTimer(uint16_t screen_id, uint16_t control_id)
{
  SetScreen(1);//����ID
}

/*! 
 *  \brief  ��ȡ�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 *  \param _data ��������
 *  \param length ���ݳ���
 */
void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  д�û�FLASH״̬����
 *  \param status 0ʧ�ܣ�1�ɹ�
 */
void NotifyWriteFlash(uint8_t status)
{
	//TODO: ����û�����
}

/*! 
 *  \brief  ��ȡRTCʱ�䣬ע�ⷵ�ص���BCD��
 *  \param year �꣨BCD��
 *  \param month �£�BCD��
 *  \param week ���ڣ�BCD��
 *  \param day �գ�BCD��
 *  \param hour ʱ��BCD��
 *  \param minute �֣�BCD��
 *  \param second �루BCD��
 */
void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second)
{
}

