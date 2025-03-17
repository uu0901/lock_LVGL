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
    delay_init();//��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);//���ڳ�ʼ��Ϊ115200
    FSMC_SRAM_Init();

    LED_Init();//LED�˿ڳ�ʼ��
    LCD_Init();
    KEY_Init();
    my_mem_init(SRAMIN);

    lvgl_demo();

}


