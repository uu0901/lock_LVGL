#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "sram.h"
#include "lvgl_demo.h"
#include "malloc.h"

int main(void)
{
    delay_init();//延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);//串口初始化为115200
    FSMC_SRAM_Init();

    LED_Init();//LED端口初始化
    LCD_Init();
    KEY_Init();
    my_mem_init(SRAMIN);

    lvgl_demo();

}


