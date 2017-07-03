 #include "led_IO.h"
 
 void led_init(void)
 {
	 GPIO_InitTypeDef  GPIO_InitStructure;
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //ʹ��PC�˿�ʱ��
		
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				 //LED-->PC.7 �˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
	 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOC.3
	 GPIO_SetBits(GPIOC,GPIO_Pin_13);
 }