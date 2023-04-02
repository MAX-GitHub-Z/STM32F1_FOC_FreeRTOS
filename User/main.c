/**
  *********************************************************************
  * @file    main.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   FreeRTOS v9.0.0 + STM32 +FOC驱动
  *********************************************************************
  */ 
 
/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 

/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"//二值信号量头文件

/* 开发板硬件bsp头文件 */
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_iic.h"


/*常用C语言头文件*/
#include <stdio.h>
#include <string.h>
/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
 /* 创建任务句柄 */
static TaskHandle_t AppTaskCreate_Handle = NULL;
/* FOC任务句柄 */
static TaskHandle_t FOC_PID_Task_Handle = NULL;
/* UASRT任务句柄 */
 TaskHandle_t USART_Task_Handle = NULL;
/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */


/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
 //角度滤波（一阶滤波）
#define first_fliter   0.98

//串口接收缓存
unsigned char Usart_Rx_Buf[20];//串口接收数据数组
/*
*************************************************************************
*                             函数声明
*************************************************************************
*/
static void AppTaskCreate(void);/* 用于创建任务 */

static void FOC_PID_Task(void* pvParameters);/* FOC_PID_Task任务实现 */
static void USART_Task(void* pvParameters);/* LED2_Task任务实现 */

static void BSP_Init(void);/* 用于初始化板载相关资源 */

/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */

  /* 开发板硬件初始化 */
  BSP_Init();
  printf("这是一个FOC-FreeRTOS-多任务实验!\r\n");
   /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )128,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
  else
    return -1;  
  
  while(1);   /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
  
  /* 创建Sampl_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )FOC_PID_Task, /* 任务入口函数 */
                        (const char*    )"FOC_PID_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&FOC_PID_Task_Handle);/* 任务控制块指针 */
//  if(pdPASS == xReturn)
//    printf("创建FOC_PID_Task任务成功!\r\n");
  
	/* USART_Task */
  xReturn = xTaskCreate((TaskFunction_t )USART_Task, /* 任务入口函数 */
                        (const char*    )"USART_Task",/* 任务名字 */
                        (uint16_t       )128,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )3,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&USART_Task_Handle);/* 任务控制块指针 */
//  if(pdPASS == xReturn)
//    printf("创建USART_Task任务成功!\r\n");
  
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}



/**********************************************************************
  * @ 函数名  ： FOC_PID_Task
  * @ 功能说明： FOC_PID_Task任务主体,主要实现FOC与PID算法
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void FOC_PID_Task(void* parameter)
{	
	uint16_t angle_new=0;
	uint16_t angle_old=AS5600_ReadTwoByte();
    while (1)
    {
			//读取角度值
      angle_new=AS5600_ReadTwoByte();
			
			//进行一阶滤波Y(n)=a*X(n)+(1-a)*Y(n-1)
			//α=滤波系数；X(n)=本次采样值；Y(n-1)=上次滤波输出值；Y(n)=本次滤波输出值。 
			angle_new=(first_fliter*angle_new)+((1-first_fliter)*angle_old);
			//更新旧的值
			angle_old=angle_new;
			//printf("当前角度值：%d\r\n",angle_old);
			
			
        vTaskDelay(500);   /* 延时30个tick */		 		
        
    }
}

/**********************************************************************
  * @ 函数名  ： USART_Task
  * @ 功能说明： USART_Task任务主体,主要实现获取串口数据并进行处理
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void USART_Task(void* parameter)
{	
	unsigned char* data=Usart_Rx_Buf;
    while (1)
    {
				ulTaskNotifyTake(pdTRUE,portMAX_DELAY);//获取信号通知
				printf("数据:%s",data);
				memset(Usart_Rx_Buf, 0, sizeof(Usart_Rx_Buf));//对字符串进行清零操作 		
       
    }
}
/***********************************************************************
  * @ 函数名  ： BSP_Init
  * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED 初始化 */
	LED_GPIO_Config();

	/* 串口初始化	*/
	USART1_Init();
	
	/*IIC引脚初始化*/
	IIC_GPIO_Config();
	
  
}

/********************************END OF FILE****************************/
