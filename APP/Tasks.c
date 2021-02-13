#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "lcd.h"
#include "malloc.h"  
#include "MMC_SD.h" 
#include "ff.h"  
#include "exfuns.h"
#include "fontupd.h"
#include "text.h"	
#include "pstwo.h"
#include "Timer3.h"
#include "Tasks.h"
#include "protocol.h"

uint8_t Bsp_Int_Ok = 0;

/******************************************************************************
����ԭ�ͣ�	void Nvic_Init(void)
��    �ܣ�	NVIC��ʼ��
*******************************************************************************/ 
void Nvic_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//NVIC_PriorityGroup 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ�����
    //Timer3
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;//��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	//����1�ж�
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	
}


/******************************************************************************
����ԭ�ͣ�	void BSP_Int(void)
��    �ܣ�	Ӳ��������ʼ��
*******************************************************************************/ 
void BSP_Int(void)
{
	
	Nvic_Init();//�ж����ȼ���ʼ��	
	delay_init();
	uart_init(115200);//����1��ʼ����������115200��8λ���ݣ�1λֹͣλ��������żУ��
	Timer3_Init(1000);//Timer3�ж�100HZ
	LCD_Init();			    //��ʼ��Һ�� 												    
 	mem_init();				//��ʼ���ڴ��	    
 	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
    f_mount(fs[0],"0:",1); 	//����SD�� 
 	f_mount(fs[1],"1:",1); 	//����FLASH.
	font_init();
	 
    PS2_Init();			 //�����˿ڳ�ʼ��
	PS2_SetInit();		 //�����ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
	                     //������ģʽ

	
	
	Bsp_Int_Ok = 1;
	
}

u8 receive_flag;
/******************************************************************************
����ԭ�ͣ�	void Task_10HZ(void)
��    �ܣ�	��ѭ��������Ƶ��Ϊ10HZ����
*******************************************************************************/ 
void Task_10HZ(void)
{
	
	
//	   pitch=3114;roll=3114;yaw=3114;������
	   //ROV������ʾ 
	   POINT_COLOR=BLACK;       
	   Show_Str(82,35,200,24,"���β��̻�����",24,0);//82				    	 
//	   Show_Str(82,35,200,24,"����ˮ�¹۲������",16,0);				    	 
//	   Show_Str(10,70,200,16,"ROV��̬��",16,0);
	
       Show_Str(10,100,260,24,"�����ǣ�    ��",24,0);   //���¼��û���������ҿ��260
	   LCD_ShowxNum(90,100,pitch/100,3,24,0); 
	   Show_Str(126,100,260,24,".",24,0);     
	   LCD_ShowxNum(131,100,pitch%100,2,24,0);
	   
	   Show_Str(10,125,260,24,"�����㣺    ��",24,0);  
       LCD_ShowxNum(90,125,roll/100,  3,24,0); 
	   Show_Str(126,125,260,24,".",24,0);      
	   LCD_ShowxNum(131,125,roll%100,  2,24,0);
	   
       Show_Str(10,150,260,24,"����ǣ�    ��",24,0);  
       LCD_ShowxNum(90,150,yaw/100,  3,24,0); 
	   Show_Str(126,150,260,24,".",24,0);     
	   LCD_ShowxNum(131,150,yaw%100,  2,24,0);
	
//	   POINT_COLOR=BLACK; 
	   Show_Str(10,65,260,24,"��ȣ�    cm",24,0); 
	   LCD_ShowxNum(67,65,depth,  5,24,0);
	   
	   
//       LCD_ShowxNum(67,65,depth/100,  3,24,0);
//       Show_Str(104,65,200,24,".",24,0);
//	   if((depth%100)<10)
//		  LCD_ShowxNum(105,65,0,2,24,0);	   
//       LCD_ShowxNum(117,65,depth%100,  2,24,0);

	   
//	   POINT_COLOR=BLACK; 
      
//	   Show_Str(10,160,200,16,"��ص�����  %",16,0); 
//	   LCD_ShowxNum(78,160,battery,3,16,0);    
			   
       Send_Command();//����ROV��������
	   Status_Get(); //�õ�ROV״̬
	  receive_flag++;
	if(receive_flag>=10) 
	{		
		POINT_COLOR=RED; 
		Show_Str(70,190,200,24,"ͨѶ�жϣ�����",24,0);
	    POINT_COLOR=BLACK; 
	}else{
	   
		Show_Str(70,190,200,24,"              ",24,0);

	    }
	
	
	
}













