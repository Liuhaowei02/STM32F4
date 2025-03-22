#include "public.h"

uint16_t SPITimeOut = 0;
 /**
  * @brief  SPI_FLASH��ʼ��
  * @param  ��
  * @retval ��
  */
void SPI_Flash_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef	  SPI_InitStructure;
	
	/* ʹ�� GPIO ʱ�� PB3  PB4 PB5 PB14*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/*spi ʱ��ʹ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	/*�������ţ�SCK MISO MOSI  PB3 PB4 PB5*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF; //���Ÿ���
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN; 
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*�������ţ�CS PB14*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*�������Ÿ���*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
	
	/* FLASH_SPI ģʽ���� */
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//ͨѶ����˫��ȫ˫��
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;//����ģʽ
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;//����֡��С��8λ
	/* FLASHоƬ֧��SPIģʽ0��ģʽ3 ������CPOL=1 CPHA=1 */
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;//Ƭѡ�ź���������������ǿ����б��
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;//ʱ�ӷ�Ƶ��84Mhz/2=42Mhz
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//��λ����
	SPI_Init(SPI1,&SPI_InitStructure);
	
	/*ʹ��FLASH_SPI*/
	SPI_Cmd(SPI1,ENABLE);	
}

 /**
  * @brief  ʹ��SPI���ͺͽ���һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
uint8_t SPI_Flash_SendReadByte(uint8_t byte)
{
	
	SPITimeOut = 0x1000;
	
	/*�ȴ����ͻ�����Ϊ�գ�TXE��־Ϊ1����˵�����ͻ�����Ϊ�գ�����д����*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
	{
		/*����һ���ȴ���ʱ���������һֱ�ȴ��ͷ��ش���ֵ*/
		if((SPITimeOut--)==0)
			/*��дһ���ص��������������������Ϣ*/
			return SPI_TIMEOUT_USERCALLBACK(0);
	}
	/*д����������Ҫ���͵�����д�뷢�ͻ�����*/
	SPI_I2S_SendData(SPI1,byte);
	
	
	
	/*
	SPIû�ж���д��˵������Ϊʵ����ÿ��SPI�������豸�ڽ������ݡ�
    Ҳ����˵���㷢һ�����ݱ�Ȼ���յ�һ�����ݣ�
    ��Ҫ��һ�����ݱ���ҲҪ�ȷ�һ�����ݡ�
    ��ˣ��ڳ�����һ�㶼���� SPI ���������ݺ�
    ȥ��ȡ SPI �豸�������, 
    ��ʹ��Щ����(0xFF)�����ǵĳ����������õ�
    (��Ȼ���ͺ�����ŵĶ�ȡ��������ģ�����Ȼ��Ҫ�ӼĴ����ж�����)��
   */
	
	SPITimeOut = 0x1000;
	/*�ȴ����ջ�����Ϊ�ǿգ�RXNE��־Ϊ1ʱ����ʾ���ջ��������յ��������ݣ�����ȥ��*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeOut--)==0)
			/*��дһ���ص��������������������Ϣ*/
			return SPI_TIMEOUT_USERCALLBACK(1);
	}
	return  SPI_I2S_ReceiveData(SPI1);
}

/**
  * @brief  ��ȡFlash ID
  * @param 	��
  * @retval FLASH ID
  */
uint32_t SPI_Flash_ReadID(void)
{
	uint32_t temp = 0, temp0=0 , temp1=0, temp2=0;
	/*��ʼͨѶ��CS�͵�ƽ*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*����0x9F ָ���ȡID*/
	SPI_Flash_SendReadByte(0x9F);
	
	/*��ȡһ���ֽ�����*/
	temp0=SPI_Flash_SendReadByte(0xFF);
	
	/*��ȡһ���ֽ�����*/
	temp1=SPI_Flash_SendReadByte(0xFF);
	
	/*��ȡһ���ֽ�����*/
	temp2=SPI_Flash_SendReadByte(0xFF);
	
	/*����ͨѶ��CS�ߵ�ƽ*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	temp=(temp0<<16)|(temp1<<8)| temp2;
	
	return temp;	
}

 /**
  * @brief  �ȴ�BUSY��־����0�����ȴ���FLASH�ڲ�����д�����
  * @param  none
  * @retval none
  */
void SPI_Flash_WaitForWriteEnd(void)
{
	uint8_t FLASH_Status;
	/*��ʼͨѶ��CS�͵�ƽ*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*���Ͷ�״̬�Ĵ������� 0x05*/
	SPI_Flash_SendReadByte(0x05);
	
	SPITimeOut = 0x1000;
	 /* ��FLASHæµ����ȴ� */
	do
	{
		if((SPITimeOut--)==0)
		{
			SPI_TIMEOUT_USERCALLBACK(3);
			return ;
		}
		
		FLASH_Status = SPI_Flash_SendReadByte(0xFF);	
		
	}while((FLASH_Status & 0x01)==SET);   /*flash��æ��־λ*/
	
	/*����ͨѶ��CS�ߵ�ƽ*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}


 /**
  * @brief  ��FLASH���� дʹ��0x06 ����
  * @param  none
  * @retval none
  */
void SPI_Flash_WriteEnable(void)
{
	/*��ʼͨѶ��CS�͵�ƽ*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*����дʹ������ 0x06*/
	SPI_Flash_SendReadByte(0x06);

	/*����ͨѶ��CS�ߵ�ƽ*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

 /**
  * @brief  ����FLASH������һ�β���4KB
  * @param  SectorAddr��Ҫ������������ַ
  * @retval ��
  */
void SPI_Flash_SectorErase(uint32_t SectorAddr)
{
	/*����дʹ������*/
	SPI_Flash_WriteEnable();
	SPI_Flash_WaitForWriteEnd();
	
	/*��������*/
	/*��ʼͨѶ��CS�͵�ƽ*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*���Ͳ������� 0x20*/
	SPI_Flash_SendReadByte(0x20);
	
	/*���Ͳ��������ĸ�λ��ַ*/ 
	SPI_Flash_SendReadByte((SectorAddr & 0xFF0000)>>16);
	/*���Ͳ�����������λ��ַ*/
	SPI_Flash_SendReadByte((SectorAddr & 0xFF00)>>8);
	/*���Ͳ��������ĵ�λ��ַ*/
	SPI_Flash_SendReadByte(SectorAddr & 0xFF);
	
	/*����ͨѶ��CS�ߵ�ƽ*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	/*�ȴ��������*/
	SPI_Flash_WaitForWriteEnd();
	
}

/**
  * @brief  ��FLASH��ҳд�����ݣ����ñ�����д������ǰ��Ҫ�Ȳ�������
  * @param	pBuffer��Ҫд�����ݵ�ָ��
  * @param  WriteAddr��д���ַ
  * @param  NumByteToWrite��д�����ݳ��ȣ�����С�ڵ���256
  * @retval ��
  */
void SPI_Flash_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	/*����дʹ������*/
	SPI_Flash_WriteEnable();
	
	/*��ʼͨѶ��CS�͵�ƽ*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*дҳ��ָ�� 02*/
	SPI_Flash_SendReadByte(0x02);
	
	/*����дҳ��ĸ�λ��ַ*/ 
	SPI_Flash_SendReadByte((WriteAddr & 0xFF0000)>>16);
	/*����дҳ�����λ��ַ*/
	SPI_Flash_SendReadByte((WriteAddr & 0xFF00)>>8);
	/*����дҳ��ĵ�λ��ַ*/
	SPI_Flash_SendReadByte(WriteAddr & 0xFF);
	
	/*д������*/
	
	while(NumByteToWrite--)
	{
		/*���͵�ǰҪд���ֽ�����*/
		SPI_Flash_SendReadByte(*pBuffer);
		
		/*ָ����һ���ֽ�*/
		pBuffer++;
	}
	
	/*����ͨѶ��CS�ߵ�ƽ*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	/*�ȴ�д�����*/
	SPI_Flash_WaitForWriteEnd();
}


/**
  * @brief  ��ȡFLASH����
  * @param 	 pBuffer���洢�������ݵ�ָ��
  * @param   ReadAddr����ȡ��ַ
  * @param   NumByteToRead����ȡ���ݳ���
  * @retval  ��
  */
void SPI_Flash_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	/*��ʼͨѶ��CS�͵�ƽ*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*���Ͷ�ָ�� 0x03*/
	SPI_Flash_SendReadByte(0x03);
	
	/*���Ͷ�ҳ��ĸ�λ��ַ*/ 
	SPI_Flash_SendReadByte((ReadAddr & 0xFF0000)>>16);
	/*���Ͷ�ҳ�����λ��ַ*/
	SPI_Flash_SendReadByte((ReadAddr & 0xFF00)>>8);
	/*���Ͷ�ҳ��ĵ�λ��ַ*/
	SPI_Flash_SendReadByte(ReadAddr & 0xFF);
	
	
	while(NumByteToRead--)
	{
		/*��ȡһ���ֽ�*/
		*pBuffer = SPI_Flash_SendReadByte(0xFF);
		
		/*ָ����һ���ֽڵĿհ׻�����*/
		pBuffer++;
	}
	
	/*����ͨѶ��CS�ߵ�ƽ*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}



/**
  * @brief  ������ʾ�ص�����
  * @param  None.
  * @retval None.
  */
static uint16_t SPI_TIMEOUT_USERCALLBACK(uint8_t errorCode)
{
	/*�ȴ���ʱ��Ĵ�����Ϣ��ӡ*/
	printf("errorCode:%d\r\n",errorCode);
	return errorCode;
}


