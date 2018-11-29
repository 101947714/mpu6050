#include "mpu6050.h"
#include "delay.h"

void MPU6050_Init(void)
{
	u8 res;
	IIC_Init();							//1.��ʼ��IIC��
	
	delay_ms(100);
	
	MPU6050_Write_OneByte(MPU6050ADDR,POWERREG1ADDR,0x80);  //��mpu6050��λ��
	delay_ms(100);
	MPU6050_Write_OneByte(MPU6050ADDR,POWERREG1ADDR,0x00);  //��mpu6050���ѡ�
	delay_ms(100);
	delay_ms(100);
	
	GYRO_Range(3);						
		delay_ms(100);
	
	ACC_Range(0);
		delay_ms(100);
	
	MPU6050_Frequency(50);								//4.�������� �����������ǲ���Ƶ��Ϊ50HZ
		delay_ms(100);
	


	
	MPU6050_Write_OneByte(MPU6050ADDR,INTREGADDR,0x00);//����Ϊ�ر������жϡ�
		delay_ms(100);
	
	MPU6050_Write_OneByte(MPU6050ADDR,AUXIICREGADDR,0X00);//�ر�AUX2�Ĵ�����IIC2ͨ��
		delay_ms(100);

MPU6050_Write_OneByte(MPU6050ADDR,FIFOREGADDR,0x00);//����Ϊ�ر�����FIFO��
		delay_ms(100);


	
	MPU6050_Write_OneByte(MPU6050ADDR,INTBPREGADDR,0x80);//����INT�͵�ƽ��Ч
		delay_ms(100);
		
	res=MPU6050_Read_OneByte(MPU6050ADDR,DEVICEID);
	if(res==MPU6050ADDR)
	{

	MPU6050_Write_OneByte(MPU6050ADDR,POWERREG1ADDR,0x01);					//5.����ϵͳʱ��Դ,ʹ��PLL��X�����������ο����ر��¶ȴ�����.
			delay_ms(100);
	
	MPU6050_Write_OneByte(MPU6050ADDR,POWERREG2ADDR,0x00);					//ʹ��ACC��GYRO������
			delay_ms(100);
		
			MPU6050_Frequency(50);
		delay_ms(100);
	
	MPU6050_Write_OneByte(MPU6050ADDR,DIGREGADDR,0x04);	//���ֵ�ͨ�˲���
		delay_ms(100);
	
	}
	

}










/*����MPU6050�Ĳ���Ƶ��*/
void MPU6050_Frequency(u8 xx)
{	
	u8 data;
	data=1000/xx-1;
	MPU6050_Write_OneByte(MPU6050ADDR,FREQREGADDR,data);
}


/*���������ǵ����̡�xx����Ϊ0,1,2,3���ֱ��Ӧ+-250,500,1000,2000������*/
void GYRO_Range(u8 xx)
{
	MPU6050_Write_OneByte(MPU6050ADDR,GYROREGADDR,xx<<3);
}

/*���ü��ٶȼƵ����̡�xx����Ϊ0,1,2,3���ֱ��Ӧ+-2g,4g,8g,16g������*/
void ACC_Range(u8 xx)
{
	MPU6050_Write_OneByte(MPU6050ADDR,ACCREGADDR,xx<<3);
}

/*MPU6050д��һ���ֽ�*/
void MPU6050_Write_OneByte(u8 addr,u8 regaddr,u8 dat)
{
	IIC_Start();
	IIC_Send_Byte(addr<<1|0);
	IIC_Wait_Ack();
	IIC_Send_Byte(regaddr);
	IIC_Wait_Ack();
	IIC_Send_Byte(dat);
	IIC_Wait_Ack();
	IIC_Stop();
}

/*MPU6050��ȡһ���ֽ�*/
u8 MPU6050_Read_OneByte(u8 addr,u8 regaddr)
{
	u8 tempdat;
	IIC_Start();
	IIC_Send_Byte(addr<<1|0);
	IIC_Wait_Ack();
	IIC_Send_Byte(regaddr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((addr<<1)|1);
	IIC_Wait_Ack();
	tempdat=IIC_Read_Byte(0);
	IIC_Stop();
	return tempdat;
}
















u8 MPU6050_Read_Len(u8 addr,u8 regaddr,u8 len,u8 *buf)
{	
	IIC_Start();
	IIC_Send_Byte(addr<<1);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}	
	IIC_Send_Byte(regaddr);
	IIC_Wait_Ack();
	IIC_Start();
	IIC_Send_Byte((addr<<1)+1);
	IIC_Wait_Ack();
	while(len)
	{
		if(len==1)
		{
			*buf=IIC_Read_Byte(0);			//�����ݣ����Ҳ�����Ӧ
		}
		else
		{
			*buf=IIC_Read_Byte(1);
		}
		len--;
		buf++;
	}
	IIC_Stop();
	return 0;
}


u8 MPU6050_Write_Len(u8 addr,u8 regaddr,u8 len,u8 *buf)
{
	u8 i;
	IIC_Start();
	IIC_Send_Byte(addr<<1);
	if(IIC_Wait_Ack())
	{
		IIC_Stop();
		return 1;
	}	
	IIC_Send_Byte(regaddr);
	IIC_Wait_Ack();
	for(i=0;i<len;i++)
	{
		IIC_Send_Byte(buf[i]);		
		if(IIC_Wait_Ack())
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_Stop();
	return 0;
}



