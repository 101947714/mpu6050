#ifndef __MYIIC_H
#define __MYIIC_H

#include "stm32f10x.h"

void IIC_Init(void);//IIC��ʼ��
void IIC_Start(void);//IIC��ʼ�ź�
void IIC_Stop(void);//IIC�����ź�
u8 IIC_Wait_Ack(void);//��ȡӦ���ź�
void IIC_Send_Byte(u8 txd);//ͨ��IICдһ�ֽ�����
u8 IIC_Read_Byte(u8 ack);    //��ȡIICһ���ֽڵ�����
void IIC_NAck(void);
void IIC_Ack(void);

//IO��������
void SDA_OUT(void); //����sda��Ϊ���
void SDA_IN(void);//����sda��Ϊ����

//IO��������
void WRITE_SDA(u8 dat);		//����SDA����ߵ͵�ƽ
void WRITE_SCL(u8 dat);		//����SCL����ߵ͵�ƽ
u8 	 READ_SDA(void);		//��ȡSDA�ĵ�ƽ


#endif

