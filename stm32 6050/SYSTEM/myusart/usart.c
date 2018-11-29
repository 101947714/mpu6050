#include "usart.h"






void usart_init(u32 bound)
{
	USART_InitTypeDef USART_InitTypeInstructure;
	GPIO_InitTypeDef GPIO_InitTypeInstructure;
	NVIC_InitTypeDef NVIC_InitTypeInstructure;	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_USART1,ENABLE);				//1.ʹ��GPIOA�봮��1��ʱ�ӣ�
	
	USART_DeInit(USART1);																	//2.����1��λ;
	
	USART_InitTypeInstructure.USART_BaudRate=bound;
	USART_InitTypeInstructure.USART_WordLength=USART_WordLength_8b;							//8λ����
	USART_InitTypeInstructure.USART_StopBits=USART_StopBits_1;								//1λֹͣλ
	USART_InitTypeInstructure.USART_Parity=USART_Parity_No;									//��У��λ��
	USART_InitTypeInstructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;						//����ģʽ-�շ�ģʽ��
	USART_InitTypeInstructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;		//��������λ
	USART_Init(USART1,&USART_InitTypeInstructure);											//3.���ڳ�ʼ��		
	
	GPIO_InitTypeInstructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitTypeInstructure.GPIO_Pin=GPIO_Pin_9;
	GPIO_InitTypeInstructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitTypeInstructure);												//��ʼ��GPIOA9Ϊ�������������50MHZ
	
	GPIO_InitTypeInstructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_InitTypeInstructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitTypeInstructure.GPIO_Speed=GPIO_Speed_50MHz;	
	GPIO_Init(GPIOA,&GPIO_InitTypeInstructure);												//��ʼ��GPIOA10Ϊ�������룬50MHZ
	
	NVIC_InitTypeInstructure.NVIC_IRQChannel=USART1_IRQn;									//ѡ��Ҫ���õ��ж�Ƶ��
	NVIC_InitTypeInstructure.NVIC_IRQChannelCmd=ENABLE;										//�ж�ʹ��;
	NVIC_InitTypeInstructure.NVIC_IRQChannelPreemptionPriority=3;
	NVIC_InitTypeInstructure.NVIC_IRQChannelSubPriority=3;									//���ȼ��趨����ռ���ȼ�3����Ӧ���ȼ�3��
	NVIC_Init(&NVIC_InitTypeInstructure);													//�ж����ú���
	
	USART_Cmd(USART1,ENABLE);																//����ʹ�ܣ�
	
	//USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);											//�򿪴��ڽ�������жϣ�
}




void USART1_IRQHandler(void)
{
	USART_SendData(USART1,USART_ReceiveData(USART1));
}

void usart1_send(u8 xx)
{
	USART_SendData(USART1,xx);
	while( USART_GetFlagStatus(USART1, USART_FLAG_TC) != SET);
}

