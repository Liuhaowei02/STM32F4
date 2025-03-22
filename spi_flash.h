#ifndef __SPI_FLASH_H
#define __SPI_FLASH_H
#include "public.h"

static uint16_t SPI_TIMEOUT_USERCALLBACK(uint8_t errorCode);
void SPI_Flash_Init(void);
uint8_t SPI_Flash_SendReadByte(uint8_t byte);
uint32_t SPI_Flash_ReadID(void);
void SPI_Flash_WaitForWriteEnd(void);
void SPI_Flash_WriteEnable(void);
void SPI_Flash_SectorErase(uint32_t SectorAddr);
void SPI_Flash_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void SPI_Flash_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);


#endif

