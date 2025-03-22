#include "public.h"

uint16_t BH1750_ReadData(void)
{
	uint16_t H_Data;  //�߰�λ
	uint8_t L_Data;  //�Ͱ�λ	
	uint16_t  R_Data;  //����16λ����
	
	IIC_Start();           //���Ϳ�ʼ�ź�
	
	IIC_SendByte(BH1750_ADDRESS<<1 | 0);   //����BH1750��λ��ַ�Ӷ�дλ��д����
	
	if(IIC_WaitACK()==0)     //����0Ӧ��ɹ�
	{
		IIC_SendByte(BH1750_CMD_RESOLUTION);  //���ͷֱ�������ָ��ֱ���λ1
		if(IIC_WaitACK()==0)   //Ӧ��ɹ�
		{
			IIC_Stop();
		}
	}
	Delay_ms(BH1750_LIGHT_COLLECTION_TIME);  //�ȴ����մ������ɼ�����ǿ��
	
	IIC_Start();   //���·��Ϳ�ʼ�ź�
	IIC_SendByte(BH1750_ADDRESS<<1 | 1);   //����BH1750��λ��ַ�Ӷ�дλ��������
	
	if(IIC_WaitACK()==0)
	{
		H_Data = IIC_ReadByte(0);  //��ȡ�߰�λ��Ӧ�� 0��ʾ��Ҫ���Ŷ�ȡ
		L_Data = IIC_ReadByte(1);  //��ȡ�Ͱ�λ��Ӧ��
	
		IIC_Stop();                //��ȡ��ϣ��ͷ�����
	}
	
	R_Data = H_Data<<8 | L_Data; //����ʮ��λ����
	
	return R_Data;
}
