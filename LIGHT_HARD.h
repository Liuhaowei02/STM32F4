#ifndef __LIGHT_HARD_H
#define __LIGHT_HARD_H

#include "public.h"

#define  BH1750_ADDRESS 0x23  //BH1750的IIC通讯地址
#define	 BH1750_CMD_RESOLUTION 0x10   //BH1750分辨率
#define  BH1750_LIGHT_COLLECTION_TIME 120  //光照采样时间

void BH1750_Write(void);

uint16_t BH1750_READ(void);
#endif


