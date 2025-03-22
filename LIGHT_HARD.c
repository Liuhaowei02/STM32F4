#include "public.h"
//��������
void BH1750_Write(void)
{ 
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));        //�ȴ�IIC���߿���
	I2C_GenerateSTART(I2C1,ENABLE);											 //���Ϳ�ʼ�ź�
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));   //�ȴ���ʼ�źŷ������
	I2C_Send7bitAddress(I2C1,BH1750_ADDRESS<<1,I2C_Direction_Transmitter);  //���ʹ��豸��ַ+д����
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); //�ȴ���ַ�������
	
	I2C_SendData(I2C1,BH1750_CMD_RESOLUTION);                                //��������
	
	
	
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)); //�ȴ���������
	
	
	I2C_GenerateSTOP(I2C1,ENABLE);                                   //����ֹͣ�ź�
}

//��BH1750��������
uint16_t BH1750_READ(void)
{
	uint8_t data[2];
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));                   //�ȴ�IIC���߿���
	I2C_GenerateSTART(I2C1,ENABLE);											 //���Ϳ�ʼ�ź�

	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT));   //�ȴ���ʼ�źŷ������
	I2C_Send7bitAddress(I2C1,BH1750_ADDRESS<<1,I2C_Direction_Receiver);  //���ʹ��豸��ַ+������
	
	while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); //�ȴ���ַ�������	
	
	//���յ�һ���ַ�
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE)==RESET);                  //�ж�SDA�ַ���Ч �ж����ݼĴ����ǿ�
	data[0]=I2C_ReceiveData(I2C1);
	
	//���յڶ����ַ�
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_RXNE)==RESET);                  //�ж�SDA�ַ���Ч �ж����ݼĴ����ǿ�
	data[1]=I2C_ReceiveData(I2C1);
	
	//����ֹͣ�ź�
	I2C_GenerateSTOP(I2C1,ENABLE); 
	
	//�ϲ��߰�λ�͵Ͱ�λ���ݣ�������
	return data[0]<<8 | data[1];

}
