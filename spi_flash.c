#include "public.h"

uint16_t SPITimeOut = 0;
 /**
  * @brief  SPI_FLASH初始化
  * @param  无
  * @retval 无
  */
void SPI_Flash_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef	  SPI_InitStructure;
	
	/* 使能 GPIO 时钟 PB3  PB4 PB5 PB14*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	/*spi 时钟使能*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	
	/*配置引脚：SCK MISO MOSI  PB3 PB4 PB5*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_AF; //引脚复用
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_DOWN; 
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*配置引脚：CS PB14*/
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_OUT;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/*设置引脚复用*/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
	
	/* FLASH_SPI 模式配置 */
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//通讯方向双线全双工
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;//主机模式
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;//数据帧大小是8位
	/* FLASH芯片支持SPI模式0及模式3 可设置CPOL=1 CPHA=1 */
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;//片选信号由软件触发，我们可自行编程
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_2;//时钟分频，84Mhz/2=42Mhz
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;//高位先行
	SPI_Init(SPI1,&SPI_InitStructure);
	
	/*使能FLASH_SPI*/
	SPI_Cmd(SPI1,ENABLE);	
}

 /**
  * @brief  使用SPI发送和接收一个字节的数据
  * @param  byte：要发送的数据
  * @retval 返回接收到的数据
  */
uint8_t SPI_Flash_SendReadByte(uint8_t byte)
{
	
	SPITimeOut = 0x1000;
	
	/*等待发送缓冲区为空，TXE标志为1，则说明发送缓冲区为空，可以写数据*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
	{
		/*设置一个等待超时动作，如果一直等待就返回错误值*/
		if((SPITimeOut--)==0)
			/*编写一个回调函数，用作输出错误信息*/
			return SPI_TIMEOUT_USERCALLBACK(0);
	}
	/*写缓冲区，把要发送的数据写入发送缓冲区*/
	SPI_I2S_SendData(SPI1,byte);
	
	
	
	/*
	SPI没有读和写的说法，因为实质上每次SPI是主从设备在交换数据。
    也就是说，你发一个数据必然会收到一个数据；
    你要收一个数据必须也要先发一个数据。
    因此，在程序中一般都会在 SPI 传输完数据后，
    去读取 SPI 设备里的数据, 
    即使这些数据(0xFF)在我们的程序里是无用的
    (虽然发送后紧接着的读取是无意义的，但仍然需要从寄存器中读出来)。
   */
	
	SPITimeOut = 0x1000;
	/*等待接收缓冲区为非空，RXNE标志为1时，表示接收缓冲区接收到了新数据，可以去读*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeOut--)==0)
			/*编写一个回调函数，用作输出错误信息*/
			return SPI_TIMEOUT_USERCALLBACK(1);
	}
	return  SPI_I2S_ReceiveData(SPI1);
}

/**
  * @brief  读取Flash ID
  * @param 	无
  * @retval FLASH ID
  */
uint32_t SPI_Flash_ReadID(void)
{
	uint32_t temp = 0, temp0=0 , temp1=0, temp2=0;
	/*开始通讯，CS低电平*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*发送0x9F 指令，读取ID*/
	SPI_Flash_SendReadByte(0x9F);
	
	/*读取一个字节数据*/
	temp0=SPI_Flash_SendReadByte(0xFF);
	
	/*读取一个字节数据*/
	temp1=SPI_Flash_SendReadByte(0xFF);
	
	/*读取一个字节数据*/
	temp2=SPI_Flash_SendReadByte(0xFF);
	
	/*结束通讯，CS高电平*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	temp=(temp0<<16)|(temp1<<8)| temp2;
	
	return temp;	
}

 /**
  * @brief  等待BUSY标志被置0，即等待到FLASH内部数据写入完毕
  * @param  none
  * @retval none
  */
void SPI_Flash_WaitForWriteEnd(void)
{
	uint8_t FLASH_Status;
	/*开始通讯，CS低电平*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*发送读状态寄存器命令 0x05*/
	SPI_Flash_SendReadByte(0x05);
	
	SPITimeOut = 0x1000;
	 /* 若FLASH忙碌，则等待 */
	do
	{
		if((SPITimeOut--)==0)
		{
			SPI_TIMEOUT_USERCALLBACK(3);
			return ;
		}
		
		FLASH_Status = SPI_Flash_SendReadByte(0xFF);	
		
	}while((FLASH_Status & 0x01)==SET);   /*flash繁忙标志位*/
	
	/*结束通讯，CS高电平*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}


 /**
  * @brief  向FLASH发送 写使能0x06 命令
  * @param  none
  * @retval none
  */
void SPI_Flash_WriteEnable(void)
{
	/*开始通讯，CS低电平*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*发送写使能命令 0x06*/
	SPI_Flash_SendReadByte(0x06);

	/*结束通讯，CS高电平*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

 /**
  * @brief  擦除FLASH扇区，一次擦除4KB
  * @param  SectorAddr：要擦除的扇区地址
  * @retval 无
  */
void SPI_Flash_SectorErase(uint32_t SectorAddr)
{
	/*发送写使能命令*/
	SPI_Flash_WriteEnable();
	SPI_Flash_WaitForWriteEnd();
	
	/*擦除扇区*/
	/*开始通讯，CS低电平*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*发送擦除命令 0x20*/
	SPI_Flash_SendReadByte(0x20);
	
	/*发送擦除扇区的高位地址*/ 
	SPI_Flash_SendReadByte((SectorAddr & 0xFF0000)>>16);
	/*发送擦除扇区的中位地址*/
	SPI_Flash_SendReadByte((SectorAddr & 0xFF00)>>8);
	/*发送擦除扇区的低位地址*/
	SPI_Flash_SendReadByte(SectorAddr & 0xFF);
	
	/*结束通讯，CS高电平*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	/*等待擦除完毕*/
	SPI_Flash_WaitForWriteEnd();
	
}

/**
  * @brief  对FLASH按页写入数据，调用本函数写入数据前需要先擦除扇区
  * @param	pBuffer，要写入数据的指针
  * @param  WriteAddr，写入地址
  * @param  NumByteToWrite，写入数据长度，必须小于等于256
  * @retval 无
  */
void SPI_Flash_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	/*发送写使能命令*/
	SPI_Flash_WriteEnable();
	
	/*开始通讯，CS低电平*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*写页面指令 02*/
	SPI_Flash_SendReadByte(0x02);
	
	/*发送写页面的高位地址*/ 
	SPI_Flash_SendReadByte((WriteAddr & 0xFF0000)>>16);
	/*发送写页面的中位地址*/
	SPI_Flash_SendReadByte((WriteAddr & 0xFF00)>>8);
	/*发送写页面的低位地址*/
	SPI_Flash_SendReadByte(WriteAddr & 0xFF);
	
	/*写入数据*/
	
	while(NumByteToWrite--)
	{
		/*发送当前要写的字节数据*/
		SPI_Flash_SendReadByte(*pBuffer);
		
		/*指向下一个字节*/
		pBuffer++;
	}
	
	/*结束通讯，CS高电平*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
	
	/*等待写入完毕*/
	SPI_Flash_WaitForWriteEnd();
}


/**
  * @brief  读取FLASH数据
  * @param 	 pBuffer，存储读出数据的指针
  * @param   ReadAddr，读取地址
  * @param   NumByteToRead，读取数据长度
  * @retval  无
  */
void SPI_Flash_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
	/*开始通讯，CS低电平*/
	GPIO_ResetBits(GPIOB,GPIO_Pin_14);
	
	/*发送读指令 0x03*/
	SPI_Flash_SendReadByte(0x03);
	
	/*发送读页面的高位地址*/ 
	SPI_Flash_SendReadByte((ReadAddr & 0xFF0000)>>16);
	/*发送读页面的中位地址*/
	SPI_Flash_SendReadByte((ReadAddr & 0xFF00)>>8);
	/*发送读页面的低位地址*/
	SPI_Flash_SendReadByte(ReadAddr & 0xFF);
	
	
	while(NumByteToRead--)
	{
		/*读取一个字节*/
		*pBuffer = SPI_Flash_SendReadByte(0xFF);
		
		/*指向下一个字节的空白缓冲区*/
		pBuffer++;
	}
	
	/*结束通讯，CS高电平*/
	GPIO_SetBits(GPIOB,GPIO_Pin_14);
}



/**
  * @brief  错误显示回调函数
  * @param  None.
  * @retval None.
  */
static uint16_t SPI_TIMEOUT_USERCALLBACK(uint8_t errorCode)
{
	/*等待超时后的错误信息打印*/
	printf("errorCode:%d\r\n",errorCode);
	return errorCode;
}


