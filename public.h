#ifndef __PUBLIC_H
#define __PUBLIC_H

#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "exti.h"
#include "timer.h"
#include "pwm.h"
//#include "SysTick.h"
#include "capture.h"
#include "iwdg.h"
#include "wwdg.h"
#include "usart.h"
#include <stdio.h>
#include "iic.h"
#include "light.h"
#include "Dht11.h"
#include "adc.h"
#include "spi_flash.h"
#include "rs485.h"
#include "time3.h"
#include "delay.h"

#include "OLED.H"
#include "OLED_Data.h"
#include "esp8266.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h"

/*驱动层数据采集规整*/
typedef struct {
	uint16_t AdcData;   //采集到的ADC值
	float SoilMoisture;//土壤湿度值
}AdcDataStruct, *PAdcDataStruct;

#endif
