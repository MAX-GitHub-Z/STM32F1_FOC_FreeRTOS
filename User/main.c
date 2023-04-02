/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 +FOC����
  *********************************************************************
  */ 
 
/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 

/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"//��ֵ�ź���ͷ�ļ�

/* ������Ӳ��bspͷ�ļ� */
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_iic.h"


/*����C����ͷ�ļ�*/
#include <stdio.h>
#include <string.h>
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
 /* ���������� */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* FOC������ */
static TaskHandle_t FOC_PID_Task_Handle = NULL;
/* UASRT������ */
 TaskHandle_t USART_Task_Handle = NULL;
/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */


/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
 //�Ƕ��˲���һ���˲���
#define first_fliter   0.98

//���ڽ��ջ���
unsigned char Usart_Rx_Buf[20];//���ڽ�����������
/*
*************************************************************************
*                             ��������
*************************************************************************
*/
static void AppTaskCreate(void);/* ���ڴ������� */

static void FOC_PID_Task(void* pvParameters);/* FOC_PID_Task����ʵ�� */
static void USART_Task(void* pvParameters);/* LED2_Task����ʵ�� */

static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */

  /* ������Ӳ����ʼ�� */
  BSP_Init();
  printf("����һ��FOC-FreeRTOS-������ʵ��!\r\n");
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )128,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
  else
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  
  /* ����Sampl_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )FOC_PID_Task, /* ������ں��� */
                        (const char*    )"FOC_PID_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&FOC_PID_Task_Handle);/* ������ƿ�ָ�� */
//  if(pdPASS == xReturn)
//    printf("����FOC_PID_Task����ɹ�!\r\n");
  
	/* USART_Task */
  xReturn = xTaskCreate((TaskFunction_t )USART_Task, /* ������ں��� */
                        (const char*    )"USART_Task",/* �������� */
                        (uint16_t       )128,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&USART_Task_Handle);/* ������ƿ�ָ�� */
//  if(pdPASS == xReturn)
//    printf("����USART_Task����ɹ�!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}



/**********************************************************************
  * @ ������  �� FOC_PID_Task
  * @ ����˵���� FOC_PID_Task��������,��Ҫʵ��FOC��PID�㷨
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void FOC_PID_Task(void* parameter)
{	
	uint16_t angle_new=0;
	uint16_t angle_old=AS5600_ReadTwoByte();
    while (1)
    {
			//��ȡ�Ƕ�ֵ
      angle_new=AS5600_ReadTwoByte();
			
			//����һ���˲�Y(n)=a*X(n)+(1-a)*Y(n-1)
			//��=�˲�ϵ����X(n)=���β���ֵ��Y(n-1)=�ϴ��˲����ֵ��Y(n)=�����˲����ֵ�� 
			angle_new=(first_fliter*angle_new)+((1-first_fliter)*angle_old);
			//���¾ɵ�ֵ
			angle_old=angle_new;
			//printf("��ǰ�Ƕ�ֵ��%d\r\n",angle_old);
			
			
        vTaskDelay(500);   /* ��ʱ30��tick */		 		
        
    }
}

/**********************************************************************
  * @ ������  �� USART_Task
  * @ ����˵���� USART_Task��������,��Ҫʵ�ֻ�ȡ�������ݲ����д���
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void USART_Task(void* parameter)
{	
	unsigned char* data=Usart_Rx_Buf;
    while (1)
    {
				ulTaskNotifyTake(pdTRUE,portMAX_DELAY);//��ȡ�ź�֪ͨ
				printf("����:%s",data);
				memset(Usart_Rx_Buf, 0, sizeof(Usart_Rx_Buf));//���ַ�������������� 		
       
    }
}
/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED ��ʼ�� */
	LED_GPIO_Config();

	/* ���ڳ�ʼ��	*/
	USART1_Init();
	
	/*IIC���ų�ʼ��*/
	IIC_GPIO_Config();
	
  
}

/********************************END OF FILE****************************/
