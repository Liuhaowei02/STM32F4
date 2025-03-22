#ifndef __LIGHT_HARD_H
#define __LIGHT_HARD_H

#include "public.h"

#define  BH1750_ADDRESS 0x23  //BH1750��IICͨѶ��ַ
#define	 BH1750_CMD_RESOLUTION 0x10   //BH1750�ֱ���
#define  BH1750_LIGHT_COLLECTION_TIME 120  //���ղ���ʱ��

void BH1750_Write(void);

uint16_t BH1750_READ(void);
#endif


