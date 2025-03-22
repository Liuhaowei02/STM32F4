#include "public.h"


/*�������--------------------------*/
/*���ȼ� ��ջ��С ������ ������*/
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
	������У��������ݴ���
	�����ֵ�ź�����adc�ɼ���־
*/
QueueHandle_t Soil_Queue;
QueueHandle_t Light_Queue;
QueueHandle_t Temp_Queue;
QueueHandle_t Humi_Queue;

SemaphoreHandle_t AdcSemaphore;

/* �����¼���־�� */
EventGroupHandle_t xEventGroup;
#define BIT_0 (1<<0)
#define BIT_1 (1<<1)
#define BIT_2 (1<<2)

int main(void)
{
		//����ϵͳ�ж����ȼ�����4(FreeRTOS�е�Ĭ�Ϸ�ʽ��)
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
		USART3WIFI_Init_Config();
	
//	ESP8266_Init();


	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

//��ʼ����������  ������������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���

	USART_Init_Config();
	IIC_GPIO_Init();     //  ����
	OLED_Init();         //  ��Ļ
	PWM_LED_Init();      //  ���� led
	ADC_Init_Config();   //  �������ADC

	
	// ������ֵ�ź���
    AdcSemaphore = xSemaphoreCreateBinary();
	/*�������д�������
	ADC�ɼ�����
	��ǿ����
	��ʪ������
	*/
	Soil_Queue=xQueueCreate(1,sizeof(uint16_t));	
	Light_Queue=xQueueCreate(1,sizeof(uint16_t));
	Temp_Queue=xQueueCreate(1,sizeof(uint8_t));	
	Humi_Queue=xQueueCreate(1,sizeof(uint8_t));	
	
    if (AdcSemaphore == NULL)
    {
        while (1);
    }
	
		//����ʱ���־��
		xEventGroup = xEventGroupCreate();	
    if (xEventGroup == NULL) {

        while (1);
    }
		
    //����Light����
    xTaskCreate((TaskFunction_t )Light_task,             
                (const char*    )"Light_task",           
                (uint16_t       )Light_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )Light_TASK_PRIO,        
                (TaskHandle_t*  )&LightTask_Handler);   

    //����DHT11����
    xTaskCreate((TaskFunction_t )DHT11_task,             
                (const char*    )"DHT11_task",           
                (uint16_t       )DHT11_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )DHT11_TASK_PRIO,        
                (TaskHandle_t*  )&DHT11Task_Handler);   
    //����FC28����
    xTaskCreate((TaskFunction_t )FC28_task,     
                (const char*    )"FC28_task",   
                (uint16_t       )FC28_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )FC28_TASK_PRIO,
                (TaskHandle_t*  )&FC28Task_Handler); 		

		/*������������*/
		 xTaskCreate((TaskFunction_t )HC04_task,     
                (const char*    )"HC04_task",   
                (uint16_t       )HC04_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )HC04_TASK_PRIO,
                (TaskHandle_t*  )&HC04Task_Handler); 							
									
								
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}




/*
	���ռ��ģ��
	ÿ�ζ�ȡ���ݣ���������ʾ��OLED��Ļ��ÿ�β���OLED��Ļ���ղ�������д��
	�����ݷŵ�Light�Ķ��У�������־λ������������������wifi
	�жϹ���ǿ��ִ�п���/�ص�
*/
void Light_task(void *pvParameters)
{
	uint16_t Light;
	while(1)
	{
		taskENTER_CRITICAL();   //�ٽ籣������ֹLight��ȡ���ݱ����
		Light = BH1750_ReadData();   //���ܼ������
	
		
	/*	���ݷ��͵�Light���з���wifi��������ȡ����
			����д������Ϊwifi�������ݱȽ�����ÿ�θ��Ǳ�֤wifi��ȡ���������µ�
	*/	
	xQueueOverwrite(Light_Queue, &Light);		
	/*����ǿ�ȼ����ʾ */
	OLED_ClearArea(0,36,128,8);
  OLED_ShowString(0,36,"Light intensity:",OLED_6X8);
	OLED_ShowNum(96,36,Light,5,OLED_6X8);
  OLED_Update();     //����Ϣ��ʾ����ʾ����
	
	xEventGroupSetBits(xEventGroup, BIT_0); // �����¼�λ BIT_0
		
	if(Light<=200)
	{
		TIM_SetCompare4(TIM5,500);
	}
	else
	{
		TIM_SetCompare4(TIM5,0);
	}
	taskEXIT_CRITICAL();

	portYIELD();               //ִ����һ��Ѱ��������������

	}
}


/*
	��ʪ�ȼ��ģ��
	ÿ�ε��ú������Ѿ������ݷŵ�Temp���к�Humi����
	Temp�ĸ�λ���¶���������λ��С�� Humiͬ��
	����ȡ�����ݷŵ���ʾ��������ֻ������
	���ݶ�ȡ���ݿ�����
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
	taskENTER_CRITICAL();   //�ٽ籣������ֹ��ʪ�ȶ�ȡ���ݱ����
	DHT11_REC_Data();     
	/*	
			���ݷ��͵�Humi\Temp���з���wifi��������ȡ����
			����д������Ϊwifi�������ݱȽ�����ÿ�θ��Ǳ�֤wifi��ȡ���������µ�
	*/	
	xQueueOverwrite(Humi_Queue, &HUMI);	 
	xQueueOverwrite(Temp_Queue, &TEMP);		
			/*��ʪ����ʾ*/
	OLED_ClearArea(0,0,128,20);	
  OLED_ShowString(0,0,"Humidity:",OLED_6X8);    
	OLED_ShowString(0,12,"Temperature:",OLED_6X8);
	OLED_ShowNum(54,0,Temp_h,3,OLED_6X8);
	OLED_ShowNum(72,12,Humi_h,3,OLED_6X8);
	OLED_Update();
	
	xEventGroupSetBits(xEventGroup, BIT_1); // �����¼�λ BIT_1
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
	����ʪ�ȼ��ģ��
	
*/

void FC28_task(void *pvParameters)
{

	float Soil = 0;
uint16_t AdcData;
	while(1)
	{
 /*  ��⵽�ź������ͷ�ִ��*/
	if (xSemaphoreTake(AdcSemaphore, portMAX_DELAY) == pdPASS)
    {
			xQueueReceive(Soil_Queue,&AdcData,0);
		}
	if(AdcData<=1750)
	AdcData=1750;
	if(AdcData>=3550)
	AdcData=3550;
	Soil = (float)(4950-AdcData)/(4950-1750)*100;
	
  xEventGroupSetBits(xEventGroup, BIT_2); // �����¼�λ BIT_2
	
	OLED_ClearArea(0,24,128,8);
	OLED_ShowString(0,24,"Soil moisture:",OLED_6X8);
	OLED_ShowNum(84,24,Soil,3,OLED_6X8);
	OLED_ShowString(102,24,"%",OLED_6X8);
  OLED_Update();     //����Ϣ��ʾ����ʾ����


	portYIELD();
	}

}

/*
	����ģ��
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
  // �ȴ� BIT_0, BIT_1, BIT_2 ȫ����λ
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
    ADC�ɼ����ݺ��ͷ��ź������������ݴ��͵�Soil����
*/	
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==SET)          
	{				
		AdcData=ADC_GetConversionValue(ADC1);		
		xQueueSendToFrontFromISR(Soil_Queue, &AdcData, 0);       

	}
	xSemaphoreGiveFromISR(AdcSemaphore,&xHigherPriorityTaskWoken);
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
}

