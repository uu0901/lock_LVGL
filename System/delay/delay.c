#include "delay.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//如果需要使用OS,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
/* 添加公共头文件 (FreeRTOS 需要用到) */
#include "FreeRTOS.h"
#include "task.h"
#endif
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//使用SysTick的普通计数模式对延迟进行管理（适合STM32F10x系列）
//包括delay_us,delay_ms
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2010/1/1
//版本：V1.8
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.2修改说明
//修正了中断中调用出现死循环的错误
//防止延时不准确,采用do while结构!
//V1.3修改说明
//增加了对UCOSII延时的支持.
//如果使用ucosII,delay_init会自动设置SYSTICK的值,使之与ucos的TICKS_PER_SEC对应.
//delay_ms和delay_us也进行了针对ucos的改造.
//delay_us可以在ucos下使用,而且准确度很高,更重要的是没有占用额外的定时器.
//delay_ms在ucos下,可以当成OSTimeDly来用,在未启动ucos时,它采用delay_us实现,从而准确延时
//可以用来初始化外设,在启动了ucos之后delay_ms根据延时的长短,选择OSTimeDly实现或者delay_us实现.
//V1.4修改说明 20110929
//修改了使用ucos,但是ucos未启动的时候,delay_ms中中断无法响应的bug.
//V1.5修改说明 20120902
//在delay_us加入ucos上锁，防止由于ucos打断delay_us的执行，可能导致的延时不准。
//V1.6修改说明 20150109
//在delay_ms加入OSLockNesting判断。
//V1.7修改说明 20150319
//修改OS支持方式,以支持任意OS(不限于UCOSII和UCOSIII,理论上任意OS都可以支持)
//添加:delay_osrunning/delay_ostickspersec/delay_osintnesting三个宏定义
//添加:delay_osschedlock/delay_osschedunlock/delay_ostimedly三个函数
//V1.8修改说明 20150519
//修正UCOSIII支持时的2个bug：
//delay_tickspersec改为：delay_ostickspersec
//delay_intnesting改为：delay_osintnesting
//////////////////////////////////////////////////////////////////////////////////  

static u8  fac_us=0;							//us延时倍乘数			   
//static u16 fac_ms=0;							//ms延时倍乘数,在ucos下,代表每个节拍的ms数
//	
//	
//#if SYSTEM_SUPPORT_OS							//如果SYSTEM_SUPPORT_OS定义了,说明要支持OS了(不限于UCOS).
////当delay_us/delay_ms需要支持OS的时候需要三个与OS相关的宏定义和函数来支持
////首先是3个宏定义:
////    delay_osrunning:用于表示OS当前是否正在运行,以决定是否可以使用相关函数
////delay_ostickspersec:用于表示OS设定的时钟节拍,delay_init将根据这个参数来初始哈systick
//// delay_osintnesting:用于表示OS中断嵌套级别,因为中断里面不可以调度,delay_ms使用该参数来决定如何运行
////然后是3个函数:
////  delay_osschedlock:用于锁定OS任务调度,禁止调度
////delay_osschedunlock:用于解锁OS任务调度,重新开启调度
////    delay_ostimedly:用于OS延时,可以引起任务调度.

////本例程仅作UCOSII和UCOSIII的支持,其他OS,请自行参考着移植
////支持UCOSII
//#ifdef 	OS_CRITICAL_METHOD						//OS_CRITICAL_METHOD定义了,说明要支持UCOSII				
//#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
//#define delay_ostickspersec	OS_TICKS_PER_SEC	//OS时钟节拍,即每秒调度次数
//#define delay_osintnesting 	OSIntNesting		//中断嵌套级别,即中断嵌套次数
//#endif

////支持UCOSIII
//#ifdef 	CPU_CFG_CRITICAL_METHOD					//CPU_CFG_CRITICAL_METHOD定义了,说明要支持UCOSIII	
//#define delay_osrunning		OSRunning			//OS是否运行标记,0,不运行;1,在运行
//#define delay_ostickspersec	OSCfg_TickRate_Hz	//OS时钟节拍,即每秒调度次数
//#define delay_osintnesting 	OSIntNestingCtr		//中断嵌套级别,即中断嵌套次数
//#endif


////us级延时时,关闭任务调度(防止打断us级延迟)
//void delay_osschedlock(void)
//{
//#ifdef CPU_CFG_CRITICAL_METHOD   				//使用UCOSIII
//	OS_ERR err; 
//	OSSchedLock(&err);							//UCOSIII的方式,禁止调度，防止打断us延时
//#else											//否则UCOSII
//	OSSchedLock();								//UCOSII的方式,禁止调度，防止打断us延时
//#endif
//}

////us级延时时,恢复任务调度
//void delay_osschedunlock(void)
//{	
//#ifdef CPU_CFG_CRITICAL_METHOD   				//使用UCOSIII
//	OS_ERR err; 
//	OSSchedUnlock(&err);						//UCOSIII的方式,恢复调度
//#else											//否则UCOSII
//	OSSchedUnlock();							//UCOSII的方式,恢复调度
//#endif
//}

////调用OS自带的延时函数延时
////ticks:延时的节拍数
//void delay_ostimedly(u32 ticks)
//{
//#ifdef CPU_CFG_CRITICAL_METHOD
//	OS_ERR err; 
//	OSTimeDly(ticks,OS_OPT_TIME_PERIODIC,&err);	//UCOSIII延时采用周期模式
//#else
//	OSTimeDly(ticks);							//UCOSII延时
//#endif 
//}
 
extern void xPortSysTickHandler(void); 
//systick中断服务函数,使用ucos时用到
void SysTick_Handler(void)
{	
	/* OS 开始跑了,才执行正常的调度处理 */
     if (xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
     {
        xPortSysTickHandler();
     }
}


			   
//初始化延迟函数
//当使用OS的时候,此函数会初始化OS的时钟节拍
//SYSTICK的时钟固定为HCLK时钟的1/8
//SYSCLK:系统时钟
void delay_init()
{
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	u32 reload;
#endif
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);//选择外部时钟  HCLK
	fac_us=SystemCoreClock/1000000;				//不论是否使用OS,fac_us都需要使用
#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.
	reload=SystemCoreClock/1000000;				//每秒钟的计数次数 单位为M  
	reload*=1000000/configTICK_RATE_HZ;			//根据configTICK_RATE_HZ设定溢出时间
												//reload为24位寄存器,最大值:16777216,在72M下,约合0.233s左右	

	SysTick->CTRL|=SysTick_CTRL_TICKINT_Msk;   	//开启SYSTICK中断
	SysTick->LOAD=reload; 						//每1/configTICK_RATE_HZ秒中断一次	
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;   	//开启SYSTICK    


#endif
}								    

#if SYSTEM_SUPPORT_OS  							//如果需要支持OS.

//延时nus
//nus:要延时的us数.	
//nus:0~204522252(最大值即2^32/fac_us@fac_us=168)	    								   
void delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;				//LOAD的值	    	 
	ticks=nus*fac_us; 						//需要的节拍数 
	told=SysTick->VAL;        				//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};										    
} 
void delay_ms(uint16_t nms)
{
 uint32_t i;
 
 for (i=0; i<nms; i++)
 {
 delay_us(1000);
 }
}
#else //不用OS时
//延时nus
//nus为要延时的us数.		    								   
void delay_us(u32 nus)
{		
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; 					//时间加载	  		 
	SysTick->VAL=0x00;        					//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;	//开始倒数	  
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));		//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;	//关闭计数器
	SysTick->VAL =0X00;      					 //清空计数器	 
}
//延时nms,不会引起任务调度
//nms:要延时的ms数
void delay_xms(u32 nms)
{
	u32 i;
	for(i=0;i<nms;i++) delay_us(1000);
}

#endif






































