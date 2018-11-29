#include "myiic.h"
#include "delay.h"

/*IIC��ʼ������*/
void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );//PB ʱ��ʹ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;  //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); //��ʼ�� GPIO
	GPIO_SetBits(GPIOB,GPIO_Pin_6|GPIO_Pin_7);   //PB10,PB11 �����
}

/*IIC��ʼ�ź�*/
void IIC_Start(void)									//����Ķ���
{
	SDA_OUT();				//����SDAΪ�����
	WRITE_SDA(1);			//SDA����ߵ�ƽ
	WRITE_SCL(1);			//SCL����ߵ�ƽ
	delay_us(2);			//�ӳ�2΢��
	WRITE_SDA(0);			//SDA��͵�ƽ����
	delay_us(2);			//�ӳ�2΢��
	WRITE_SCL(0);			//SCL��Ϊ�͵�ƽ��׼����������	
}

/*IIC�����ź�*/
void IIC_Stop(void)												//����Ķ���
{
	SDA_OUT();				//����SDAΪ���
	WRITE_SCL(0);			//SCLΪ�͵�ƽ
	WRITE_SDA(0);			//SDAΪ�͵�ƽ
	delay_us(2);			
	WRITE_SCL(1);
	WRITE_SDA (1);			//SDA��ߵ�ƽ���䣬������ݴ���
	delay_us(2);
}

/*IIC�ȴ�Ӧ���ź�*/
u8 IIC_Wait_Ack(void)
{
	u8 error=255;					//���õȴ���ʱʱ��
	SDA_OUT();
	WRITE_SCL(1);
	delay_us(2);
	WRITE_SDA(1);
	delay_us(2);
	SDA_IN();						//����SDAΪ���룬׼�����ܵ͵�ƽ
	while(READ_SDA()&(error>0))		//�ж�����Ӧ���ź�
	{
		error--;
	}
	if(error==0)					//��ʱ������1
	{
		IIC_Stop();
		return 1;
	}
	else							//��Ӧ���źţ�����0
	{	
		WRITE_SCL(0);					//ʱ�����0	
		return 0;
	}
}

void IIC_Send_Byte(u8 txd)
{
	u8 i=0;
	SDA_OUT();
	WRITE_SCL(0);				//����ʱ�ӣ���ʼ��������
	for(i=0;i<8;i++)
	{
		WRITE_SDA((txd&0x80)>>7);
		txd<<=1;
		WRITE_SCL(1);
		delay_us(2);
		WRITE_SCL(0);
		delay_us(2);			
	}
}


u8 IIC_Read_Byte(u8 ack)			//ackΪ0 ����Ӧ��Ϊ1��Ӧ
{
	u8 i,receivedat=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		WRITE_SCL(0);			//����ʱ��
		delay_us(2);
		WRITE_SCL(1);			//����ʱ��
		receivedat<<=1;
		if(READ_SDA()==1)
		{
			receivedat++;
		}
	}
	if(!ack)								//ackΪ0 ��Ӧ��
		IIC_NAck();
	else
		IIC_Ack();							//Ϊ1��Ӧ��
	return receivedat;
}

/*������Ӧ���ź�*/
void IIC_NAck(void)
{
	WRITE_SCL(0);
	SDA_OUT();
	WRITE_SDA(1);
	delay_us(2);
	WRITE_SCL(1);
	delay_us(2);
	WRITE_SCL(0);
}

/*����Ӧ���ź�*/
void IIC_Ack(void)
{
	WRITE_SCL(0);
	SDA_OUT();
	WRITE_SDA(0);
	delay_us(2);
	WRITE_SCL(1);
	delay_us(2);
	WRITE_SCL(0);
}

/*����SDAΪ���*/
void SDA_OUT(void)										//��SDA����Ϊ�����
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 				//��ʼ�� GPIO;
}

/*����SDAΪ����*/
void SDA_IN(void)										//��SDA����Ϊ���룻
{
	GPIO_InitTypeDef GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 //��������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 				//��ʼ�� GPIO;
}

/*����SDAΪ����ߵ͵�ƽ*/
void WRITE_SDA(u8 dat)
{
	switch(dat)
	{
		case 1:
		GPIO_SetBits(GPIOB,GPIO_Pin_7);break;
		case 0:	
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);break;
	}
}

/*����SCLΪ����ߵ͵�ƽ*/
void WRITE_SCL(u8 dat)
{
	switch(dat)
	{
		case 1:
		GPIO_SetBits(GPIOB,GPIO_Pin_6);break;
		case 0:	
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);break;
	}
}

/*��ȡSCL��ƽ*/
u8 READ_SDA(void)
{	
	return 	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7);
}





