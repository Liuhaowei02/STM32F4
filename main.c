#include "public.h"


/*任务参数--------------------------*/
/*优先级 堆栈大小 任务句柄 任务函数*/
#define START_TASK_PRIO     1
#define START_STK_SIZE      128  
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define Light_TASK_PRIO     2
#define Light_STK_SIZE      128  
TaskHandle_t LightTask_Handler;
void Light_task(void *pvParameters);

#define DHT11_TASK_PRIO     2
#define DHT11_STK_SIZE      128  
TaskHandle_t DHT11Task_Handler;
void DHT11_task(void *pvParameters);

#define FC28_TASK_PRIO     2
#define FC28_STK_SIZE      128  
TaskHandle_t FC28Task_Handler;
void FC28_task(void *pvParameters);

#define HC04_TASK_PRIO     3
#define HC04_STK_SIZE      256  
TaskHandle_t HC04Task_Handler;
void HC04_task(void *pvParameters);

/*
	定义队列：用于数据传送
	定义二值信号量：adc采集标志
*/
QueueHandle_t Soil_Queue;
QueueHandle_t Light_Queue;
QueueHandle_t Temp_Queue;
QueueHandle_t Humi_Queue;

SemaphoreHandle_t AdcSemaphore;

/* 定义事件标志组 */
EventGroupHandle_t xEventGroup;
#define BIT_0 (1<<0)
#define BIT_1 (1<<1)
#define BIT_2 (1<<2)

int main(void)
{
		//设置系统中断优先级分组4(FreeRTOS中的默认方式！)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
		USART3WIFI_Init_Config();
	
//	ESP8266_Init();


	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数  创建所有任务
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区

	USART_Init_Config();
	IIC_GPIO_Init();     //  光敏
	OLED_Init();         //  屏幕
	PWM_LED_Init();      //  风扇 led
	ADC_Init_Config();   //  土壤检测ADC

	
	// 创建二值信号量
    AdcSemaphore = xSemaphoreCreateBinary();
	/*创建队列传递数据
	ADC采集数据
	光强数据
	温湿度数据
	*/
	Soil_Queue=xQueueCreate(1,sizeof(uint16_t));	
	Light_Queue=xQueueCreate(1,sizeof(uint16_t));
	Temp_Queue=xQueueCreate(1,sizeof(uint8_t));	
	Humi_Queue=xQueueCreate(1,sizeof(uint8_t));	
	
    if (AdcSemaphore == NULL)
    {
        while (1);
    }
	
		//创建时间标志组
		xEventGroup = xEventGroupCreate();	
    if (xEventGroup == NULL) {

        while (1);
    }
		
    //创建Light任务
    xTaskCreate((TaskFunction_t )Light_task,             
                (const char*    )"Light_task",           
                (uint16_t       )Light_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )Light_TASK_PRIO,        
                (TaskHandle_t*  )&LightTask_Handler);   

    //创建DHT11任务
    xTaskCreate((TaskFunction_t )DHT11_task,             
                (const char*    )"DHT11_task",           
                (uint16_t       )DHT11_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )DHT11_TASK_PRIO,        
                (TaskHandle_t*  )&DHT11Task_Handler);   
    //创建FC28任务
    xTaskCreate((TaskFunction_t )FC28_task,     
                (const char*    )"FC28_task",   
                (uint16_t       )FC28_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )FC28_TASK_PRIO,
                (TaskHandle_t*  )&FC28Task_Handler); 		

		/*创建蓝牙任务*/
		 xTaskCreate((TaskFunction_t )HC04_task,     
                (const char*    )"HC04_task",   
                (uint16_t       )HC04_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )HC04_TASK_PRIO,
                (TaskHandle_t*  )&HC04Task_Handler); 							
									
								
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}




/*
	光照检测模块
	每次读取数据，将数据显示到OLED屏幕，每次擦除OLED屏幕光照部分重新写入
	将数据放到Light的队列，并给标志位，方便后续添加蓝牙，wifi
	判断光照强度执行开灯/关灯
*/
void Light_task(void *pvParameters)
{
	uint16_t Light;
	while(1)
	{
		taskENTER_CRITICAL();   //临界保护，防止Light读取数据被打断
		Light = BH1750_ReadData();   //接受检测数据
	
		
	/*	数据发送到Light队列方便wifi和蓝牙读取数据
			覆盖写法，因为wifi传输数据比较慢，每次覆盖保证wifi读取数据是最新的
	*/	
	xQueueOverwrite(Light_Queue, &Light);		
	/*光照强度监测显示 */
	OLED_ClearArea(0,36,128,8);
  OLED_ShowString(0,36,"Light intensity:",OLED_6X8);
	OLED_ShowNum(96,36,Light,5,OLED_6X8);
  OLED_Update();     //将信息显示到显示屏上
	
	xEventGroupSetBits(xEventGroup, BIT_0); // 设置事件位 BIT_0
		
	if(Light<=200)
	{
		TIM_SetCompare4(TIM5,500);
	}
	else
	{
		TIM_SetCompare4(TIM5,0);
	}
	taskEXIT_CRITICAL();

	portYIELD();               //执行完一次寻找其他就绪任务

	}
}


/*
	温湿度检测模块
	每次调用函数，已经将数据放到Temp队列和Humi队列
	Temp的高位是温度整数，低位是小数 Humi同理
	将读取的数据放到显示屏，假设只放整数
	根据读取数据开风扇
*/
	uint8_t Temp_h;
	uint8_t Temp_l;
	uint8_t Humi_h;
	uint8_t Humi_l;	
void DHT11_task(void *pvParameters)
{
	uint8_t	HUMI;
	uint8_t TEMP;
	while(1)
	{
	taskENTER_CRITICAL();   //临界保护，防止温湿度读取数据被打断
	DHT11_REC_Data();     
	/*	
			数据发送到Humi\Temp队列方便wifi和蓝牙读取数据
			覆盖写法，因为wifi传输数据比较慢，每次覆盖保证wifi读取数据是最新的
	*/	
	xQueueOverwrite(Humi_Queue, &HUMI);	 
	xQueueOverwrite(Temp_Queue, &TEMP);		
			/*温湿度显示*/
	OLED_ClearArea(0,0,128,20);	
  OLED_ShowString(0,0,"Humidity:",OLED_6X8);    
	OLED_ShowString(0,12,"Temperature:",OLED_6X8);
	OLED_ShowNum(54,0,Temp_h,3,OLED_6X8);
	OLED_ShowNum(72,12,Humi_h,3,OLED_6X8);
	OLED_Update();
	
	xEventGroupSetBits(xEventGroup, BIT_1); // 设置事件位 BIT_1
	if(Temp_h>=25)
	{
		TIM_SetCompare1(TIM11,500);
	}
	else
		TIM_SetCompare1(TIM11,0);


	taskEXIT_CRITICAL();
		portYIELD();
	}

}

/*
	土壤湿度检测模块
	
*/

void FC28_task(void *pvParameters)
{

	float Soil = 0;
uint16_t AdcData;
	while(1)
	{
 /*  检测到信号量被释放执行*/
	if (xSemaphoreTake(AdcSemaphore, portMAX_DELAY) == pdPASS)
    {
			xQueueReceive(Soil_Queue,&AdcData,0);
		}
	if(AdcData<=1750)
	AdcData=1750;
	if(AdcData>=3550)
	AdcData=3550;
	Soil = (float)(4950-AdcData)/(4950-1750)*100;
	
  xEventGroupSetBits(xEventGroup, BIT_2); // 设置事件位 BIT_2
	
	OLED_ClearArea(0,24,128,8);
	OLED_ShowString(0,24,"Soil moisture:",OLED_6X8);
	OLED_ShowNum(84,24,Soil,3,OLED_6X8);
	OLED_ShowString(102,24,"%",OLED_6X8);
  OLED_Update();     //将信息显示到显示屏上


	portYIELD();
	}

}

/*
	蓝牙模块
*/

void HC04_task(void *pvParameters)
{
	uint16_t Light;
	uint16_t SoilMoisture;
	uint8_t HUMI;
	uint8_t TEMP;
	float Soil;
	while(1)
	{
  // 等待 BIT_0, BIT_1, BIT_2 全部置位
  EventBits_t uxBits = xEventGroupWaitBits(xEventGroup,
                                           BIT_0 | BIT_1 | BIT_2,
                                           pdTRUE,
                                           pdTRUE,
                                         portMAX_DELAY);
		
	xQueueReceive(Soil_Queue,&SoilMoisture,0);
	xQueueReceive(Light_Queue,&Light,0);
	xQueueReceive(Temp_Queue,&HUMI,0);
	xQueueReceive(Humi_Queue,&TEMP,0);
	Soil = (float)(4950-SoilMoisture)/(4950-1750)*100;
	Serial_Printf("Humidity:%u\r\n",HUMI);
	Serial_Printf("Temperature:%u\r\n",TEMP);
	Serial_Printf("Soil moisture:%f\r\n",Soil);	
	Serial_Printf("Light intensity:%d\r\n",Light);

	portYIELD();
	}
}



void ADC_IRQHandler(void)
{
	uint16_t AdcData;
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
/*
    ADC采集数据后释放信号量，并将数据传送到Soil队列
*/	
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==SET)          
	{				
		AdcData=ADC_GetConversionValue(ADC1);		
		xQueueSendToFrontFromISR(Soil_Queue, &AdcData, 0);       

	}
	xSemaphoreGiveFromISR(AdcSemaphore,&xHigherPriorityTaskWoken);
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}

