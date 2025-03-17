#include "lv_cb.h"
#include "lv_mainstart.h"
#include "lvgl.h"
#include "led.h"
#include "delay.h"
#include "math.h"
#define Password 1234


extern lv_obj_t *btn_enter;      /* ���밴ť */
void enter_event_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);      /* ��ȡ����Դ */

    if(target == btn_enter)                      /* ���밴ť */
    {
        Refresh_And_CreateScren();
        lv_lock_sys(); // ���¼�������Ļ
    }

}
extern uint8_t my_key;                       //kk���水��ֵ
uint8_t last_my_key = 1;                    //kk���水��ֵ
extern lv_obj_t *keyboard ;
extern lv_obj_t *textarea;
uint8_t mynummap[]={0,1,2,3,4,5,6,7,8,9};   //�ҵİ�������
int8_t p=0;                                  //�ҵİ�����������
uint8_t mynum=0;                             //���Լ��ļ������鱣��ֵ
uint8_t NUM  = 0;                            //������������ֵ
uint8_t LAST_NUM = 255;                     //��һ�εļ�ֵ
uint8_t OK_FLAG = 0;                        //���̴������±�־λ��δ�õ�
uint8_t OK_LEFT = 0;                        //���̷��ذ��±�־λ��δ�õ�������ֻ��ֱ���л���Ļ
uint32_t SUM = 0;                           //����ֵ


extern uint8_t correct_flag;               //������ȷ��־λ
extern uint8_t incorrect_flag;             //���벻��ȷ��־λ
uint8_t i = 0;                              //�л������ַ���־λ
uint8_t pp = 0;                             //����������������
uint8_t buff[50];                           //������������
uint32_t BackSum = 0;                       //���ذ����ļ���ʱ��Ļ�������
int8_t uu;                                  //��¼��ǰ��ֵ�����������
uint8_t flag = 0;                           //�������صı�־λ
uint8_t t = 0;                              //�������صĴ���
uint8_t temp;                               //�������صĴ����Ļ������
uint8_t temp_2;                             //�������صĴ����Ļ������
void keyboard_event_cb(lv_event_t *e)
{     
    
    lv_event_code_t code = lv_event_get_code(e);                     /* ��ȡ�¼����� */
    lv_obj_t *target = lv_event_get_current_target(e);              /* ��ȡ����Դ */
    if(target == keyboard)//����ԴΪ���̣�Ŀ���ǲ��ϵı�������my_key
    {
        if(my_key != 0)//�ж��´ΰ�������
        {
            printf("my_key  %d\r\n",my_key);//kk��3�������ķ���ֵ����lv_port_indev_template�еļ������뺯���У�my_key�����е�һ����������
            switch(my_key)
            {
                printf("my_key  %d\r\n",my_key);//my_key��3�������ķ���ֵ����lv_port_indev_template�еļ������뺯���У�my_key�����е�һ����������
                case LV_KEY_NEXT:  {p++;if(p>9)p=0; printf("next\r\n"); my_key = 0;break;}//+   LV_KEY_NEXT ���ڱ�������
                case LV_KEY_PREV:   {p--;if(p<0)p=9; printf("prev\r\n"); my_key = 0;break;}//-  LV_KEY_PREV ���ڱ�������
                case LV_KEY_ENTER:  {//ȷ���������������飬�����ֵ��mynum����ӡ����Ļ�ϣ����kk������һ�ΰ������£�
                                        printf("enter\r\n");  
                                        mynum =mynummap[p];
                                        printf("mynum  %d\r\n",mynum);//my_key��3�������ķ���ֵ����lv_port_indev_template�еļ������뺯���У�my_key�����е�һ����������

                                        SUM = SUM *10 + mynum;//ͨ���۳�ʵ�ֱ��������ֵ
                                        printf("                    SUM= %d\r\n", SUM);
                                        switch(p)
                                        {
                                            case 0:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('0'));my_key = 0;break;}
                                            case 1:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('1'));my_key = 0;break;}
                                            case 2:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('2'));my_key = 0;break;}
                                            case 3:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('3'));my_key = 0;break;}
                                            case 4:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('4'));my_key = 0;break;}
                                            case 5:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('5'));my_key = 0;break;}
                                            case 6:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('6'));my_key = 0;break;}
                                            case 7:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('7'));my_key = 0;break;}
                                            case 8:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('8'));my_key = 0;break;}
                                            case 9:{lv_textarea_add_char(textarea, _lv_txt_unicode_to_encoded('9'));my_key = 0;break;}
                                            default:break;
                                        }
                                    }
                default:break;
            }
        }
    }
    if(code == LV_EVENT_VALUE_CHANGED)//�����¼�Ϊ��ֵ�ı�
    {
        uint16_t id = lv_btnmatrix_get_selected_btn(target);                /* ��ȡ���̰�ť���� */
        const char *txt = lv_btnmatrix_get_btn_text(target, id);            /* ��ȡ��ť�ı���ȡָ��������ASC����ַ� */
        int num = *txt - '0';                                                  //���ַ�ת����ʮ����

        //������
//        printf("%d\n", *txt);                                                 //ֱ���ַ���Ӧ��ʮ���ƴ�ӡ
//        printf("%d\n", num);                                                  //���ַ�ת����ʮ��������ӡ
        //������
        
        printf("correct_flag = %d          incorrect_flag = %d\r\n", correct_flag, incorrect_flag);
        if(lv_keyboard_get_mode(target) == LV_KEYBOARD_MODE_TEXT_LOWER)    //��ǰ����ģʽΪСд��ĸʱ�򣬽��л������ַ�������־��0
        {
            printf("LV_KEYBOARD_MODE_TEXT_LOWER \r\n");
            i = 0;//���л������ַ�������־��0
        }
        if(lv_keyboard_get_mode(target) == LV_KEYBOARD_MODE_SPECIAL)        //��ǰ����ģʽΪ�����ַ���ʱ��
        {
            printf("LV_KEYBOARD_MODE_SPECIAL \r\n");
            if(i == 0)//�����ַ�������־��0�����Դ˴μ�ֵ
            {
                if(num == 1) num = 0;   //��һ��Ĭ�ϰ��£���ֵ��ֵΪ0����Ϊ�����ַ��л���������ʱʵ�����Ƿ���1�����������л���ʱ�򲢲���ѷ��ص�1���룬����Ĭ�ϵ�һ���л������ַ��ĵ�һ��ֵ��0
            }
            i = 1;                      //��־��ֵ1�����������֮��ļ�ֵ
        }

        if( (num<=9 && num >=0) || (strcmp(txt, LV_SYMBOL_BACKSPACE) == 0) )//������ּ��̣�����0��9�ͼ��̳���
        {
            if(num<=9 && num >=0) 
            {
                NUM  = num;                     //������̰���ֵΪNum
                p = NUM;                        //���̴���ֵ ͬ��  �ҵİ�����������
                printf("                        NUM = %d\r\n", NUM);
                SUM = SUM *10 + NUM;            //ͨ���۳�ʵ�ֱ��������ֵ
                printf("                        SUM = %d\r\n", SUM);
                if(flag == 0)//���ر�־λΪ0 ������������
                {
                    buff[pp] = NUM;             //����ֵ���浽����buff
                    pp++;                       //�������������´α���
                }
                else         //���ر�־λΪ1��������һ�ζ����ǰ������أ����볷�ز�ȫ
                {
                    //����2��
                    //t = 2     temp=2       temp_2 = 2
                    //temp=1    ��������      temp=0    ��������    �˳�����
                    
                    temp = t;                   //���ش�����ֵ��temp
                    temp_2 = t;                 //���ش�����ֵ��temp_2
                    
                    temp --;                    //temp�Լ�����Ϊ��¼�³��صĴ������������д��ȥͬ���Ĵ�������temp����0����ζ�Ų�ȫ�����飬���ر�־λ��0���˳����ز�ȫ
                    buff[pp - 1*temp_2] = NUM;//����ֵ���浽����buff��pp-1ͬ�����棬��ΪNUM����������Զ�ƫ����1�����´�д�룬��������Ҫ��ȥ1�ҵ���Ӧ��Ԫ�ؽ���д�룬�����*temp2��Ϊ��ƫ��д�룬��Ϊ���ǳ����˶��ٴξ�ƫ�ƶ��ٴ�д��
                    pp++;                        //�������������´α���
                    temp_2 ++;                  //temp_2��������ΪҪ����д��ȥ���飬���ض��ٴΣ���д��ȥ���ٴΣ������buff�����۳��˽�������ƫ��
                }   
                if(temp == 0 )  {flag = 0;}     //���ر�־λ��0,�˳����ز�ȫ
                //��ӡ������ԣ�uint32_t   SUM         ���10λ��  ���浽buff��ֻ��0~9
                printf("                        buff[0] = %d\r\n", buff[0]);
                printf("                        buff[1] = %d\r\n", buff[1]);
                printf("                        buff[2] = %d\r\n", buff[2]);
                printf("                        buff[3] = %d\r\n", buff[3]);
                printf("                        buff[4] = %d\r\n", buff[4]);
                printf("                        buff[5] = %d\r\n", buff[5]);
                printf("                        buff[6] = %d\r\n", buff[6]);
                printf("                        buff[7] = %d\r\n", buff[7]);
                printf("                        buff[8] = %d\r\n", buff[8]);
                printf("                        buff[9] = %d\r\n", buff[9]);

                if(LAST_NUM != NUM || LAST_NUM ==NUM )//��ǰֵ����һ��ֵ��һ����˵�����̰��£������һ�ΰ������µ�������ֵ��uu��uu���ڼ��̳�����������
                {
                    //����
                    static uint8_t a = 0;
                    a++;
                    printf("a=%d\r\n\r\n",a);
                    //����
                    uu = pp - 1;//�����1����Ϊ��κ���������ʱ��pp�Ѿ�������1��������Ϊ�˷��������飬����������Ϊ�˴�����������̳���������������Ҫ������ȥ�����Ҫ��1
                }
                
                LAST_NUM = NUM;//�����ֵ����¼��ֵ
            }
            if(strcmp(txt, LV_SYMBOL_BACKSPACE) == 0)       /* �ж��ǲ���<xͼ�걻���� */
            {
                flag = 1;//���ر�־λ��1
                t ++;      //��¼���ش���
                printf("BACKSPACE\r\n");
                if(uu >= 0)//����������λ��ֻ��������0���ϣ�������SUM����
                {     
                    printf("                    uu= %d\r\n", uu);                    
                    printf("                        buff[uu] = %d\r\n", buff[uu]);
                    BackSum =  SUM - buff[uu];//11223344--->11223340
                    BackSum =  BackSum/10;//1122334
                    SUM = BackSum;//1122334
                }
                if(SUM == 0)                            //��SUM����0��
                {
                    uu = -1;                            //������λΪ-1��Ŀ���ǵ�SUM����0�����Լ������������SUM��������Ϊ����λ�����ǻ�������ļ������³�������
                    memset(buff, 0, sizeof(buff));     //�������
                    pp = 0;                             //�����̱���������ֵΪ0��ȷ����պ󡣼��̼�ֵ�����Ǵ�buff[0]��buff[9]
                }
                uu --;                                  //ÿ����һ�γ��������������Ͷ����������������´�������
                printf("                    SUM= %d\r\n", SUM);                
                printf("                    uu= %d\r\n", uu);
            }

        }
 
        if(strcmp(txt, LV_SYMBOL_KEYBOARD) == 0)                            /* �ж��ǲ��Ǽ���ͼ�걻���� */
        {
            printf("LV_SYMBOL_KEYBOARD \r\n");
            if(lv_keyboard_get_mode(target) == LV_KEYBOARD_MODE_NUMBER)     /* ��ȡ��ǰ����ģʽ���ж��Ƿ�Ϊ����ģʽ */
            {
                lv_keyboard_set_mode(target, LV_KEYBOARD_MODE_TEXT_LOWER);  /* ���������ģʽ�����л�ΪСд��ĸģʽ */
                printf("Switch to the LV_KEYBOARD_MODE_TEXT_LOWER\r\n");
                i = 0;
            }
            else
            {
                lv_keyboard_set_mode(target, LV_KEYBOARD_MODE_NUMBER);      /* ��������ģʽ�����л�Ϊ����ģʽ */   
                printf("Switch to the LV_KEYBOARD_MODE_NUMBER\r\n");
            }
        }
        
        if(strcmp(txt, LV_SYMBOL_OK) == 0)           /* �ж��ǲ��ǡ�ͼ�걻���� */
        {
            printf("LV_SYMBOL_OK\r\n");
            printf("                    SUM= %d\r\n", SUM);
            OK_FLAG = 1;//δʹ��
            if(SUM == Password)
            {
                correct_flag = 1;           //������ȷ��־λ��1
                incorrect_flag = 0;         //���벻��ȷ��־λ��0
                printf("Success!     correct_flag = %d", correct_flag);
                SUM = 0;                    //������0
                p = 0;                      //�ҵİ�������������ֵΪ0
            }
            else
            {
                incorrect_flag = 1;//���벻��ȷ��־λ��1
                correct_flag = 0;//������ȷ��־λ��0
                printf("Fail!    incorrect_flag = %d\r\n", incorrect_flag);
                SUM = 0;//������0
                p = 0;//�ҵİ�������������ֵΪ0
            }
            Refresh_And_CreateScren();
            lv_loading(); // ���¼�������Ļ
        }
        if(strcmp(txt, LV_SYMBOL_LEFT) == 0)         /* �ж��ǲ���<ͼ�걻���� */
        {
            printf("LV_SYMBOL_LEFT\r\n");   
            OK_LEFT = 1;//δʹ��
            Refresh_And_CreateScren();
            lv_mainstart(); // ���¼�������Ļ
        }

    }
}  

extern lv_obj_t *DC_Motor_label_speed;       /* �ٶ���ʾ�ı� */
extern lv_obj_t *DC_Motor_btn_speed_up;      /* ���ٰ�ť */
extern lv_obj_t *DC_Motor_btn_speed_down;    /* ���ٰ�ť */
extern lv_obj_t *DC_Motor_btn_stop;          /* ��ͣ��ť */
extern lv_obj_t *DC_Motor_btn_back;          /* ���ذ�ť */
extern int32_t DC_Motor_speed_val;       /* �ٶ�ֵ */

extern lv_obj_t *Step_Motor_label_speed;       /* �ٶ���ʾ�ı����� */
extern lv_obj_t *Step_Motor_btn_speed_up;      /* ���ٰ�ť����*/
extern lv_obj_t *Step_Motor_btn_speed_down;    /* ���ٰ�ť����*/
extern lv_obj_t *Step_Motor_btn_stop;          /* ��ͣ��ť����*/
extern lv_obj_t *Step_Motor_btn_back;          /* ���ذ�ť����*/
extern int32_t Step_Motor_speed_val;       /* �ٶ�ֵ����*/

extern lv_obj_t *Bldc_Motor_label_speed;       /* �ٶ���ʾ�ı����� */
extern lv_obj_t *Bldc_Motor_btn_speed_up;      /* ���ٰ�ť����*/
extern lv_obj_t *Bldc_Motor_btn_speed_down;    /* ���ٰ�ť����*/
extern lv_obj_t *Bldc_Motor_btn_stop;          /* ��ͣ��ť����*/
extern lv_obj_t *Bldc_Motor_btn_back;          /* ���ذ�ť����*/
extern int32_t Bldc_Motor_speed_val;       /* �ٶ�ֵ����*/

extern lv_obj_t *Servo_label_speed;       /* �ٶ���ʾ�ı����� */
extern lv_obj_t *Servo_btn_speed_up;      /* ���ٰ�ť����*/
extern lv_obj_t *Servo_btn_speed_down;    /* ���ٰ�ť����*/
extern lv_obj_t *Servo_btn_stop;          /* ��ͣ��ť����*/
extern lv_obj_t *Servo_btn_back;          /* ���ذ�ť����*/
extern int32_t Servo_speed_val;       /* �ٶ�ֵ����*/

void sys_btn_event_cb(lv_event_t * e)
{
    lv_obj_t *target = lv_event_get_target(e);      /* ��ȡ����Դ */
    //DC_Motor
    if(target == DC_Motor_btn_speed_up)                      /* ���ٰ�ť */
    {
        DC_Motor_speed_val += 30;
        LED1 = !LED1;
    }
    else if(target == DC_Motor_btn_speed_down)               /* ���ٰ�ť */
    {
        DC_Motor_speed_val -= 30;
        LED1 = !LED1;
    }
    else if(target == DC_Motor_btn_stop)                     /* ��ͣ��ť */
    {
        DC_Motor_speed_val = 0;
        LED1 = !LED1;
    }
    else if(target == DC_Motor_btn_back)                     /* ���ذ�ť */
    {
        LED1 = !LED1;
        Refresh_And_CreateScren();
        lv_menu(); // ���¼�������Ļ
        i = 0;
        return;
    }
    //Step_Motor
    else if(target == Step_Motor_btn_speed_up)                      /* ���ٰ�ť */
    {
        Step_Motor_speed_val += 30;
        LED1 = !LED1;
    }
    else if(target == Step_Motor_btn_speed_down)               /* ���ٰ�ť */
    {
        Step_Motor_speed_val -= 30;
        LED1 = !LED1;
    }
    else if(target == Step_Motor_btn_stop)                     /* ��ͣ��ť */
    {
        Step_Motor_speed_val = 0;
        LED1 = !LED1;
    }
    else if(target == Step_Motor_btn_back)                     /* ���ذ�ť */
    {
        LED1 = !LED1;
        Refresh_And_CreateScren();
        lv_menu(); // ���¼�������Ļ
        i = 0;
        return;
    }
    //Bldc_Motor
    else if(target == Bldc_Motor_btn_speed_up)                      /* ���ٰ�ť */
    {
        Bldc_Motor_speed_val += 30;
        LED1 = !LED1;
    }
    else if(target == Bldc_Motor_btn_speed_down)               /* ���ٰ�ť */
    {
        Bldc_Motor_speed_val -= 30;
        LED1 = !LED1;
    }
    else if(target == Bldc_Motor_btn_stop)                     /* ��ͣ��ť */
    {
        Bldc_Motor_speed_val = 0;
        LED1 = !LED1;
    }
    else if(target == Bldc_Motor_btn_back)                     /* ���ذ�ť */
    {
        LED1 = !LED1;
        Refresh_And_CreateScren();
        lv_menu(); // ���¼�������Ļ
        i = 0;
        return;
    }
    //Servo
    else if(target == Servo_btn_speed_up)                      /* ���ٰ�ť */
    {
        Servo_speed_val += 30;
        LED1 = !LED1;
    }
    else if(target == Servo_btn_speed_down)               /* ���ٰ�ť */
    {
        Servo_speed_val -= 30;
        LED1 = !LED1;
    }
    else if(target == Servo_btn_stop)                     /* ��ͣ��ť */
    {
        Servo_speed_val = 0;
        LED1 = !LED1;
    }
    else if(target == Servo_btn_back)                     /* ���ذ�ť */
    {
        LED1 = !LED1;
        Refresh_And_CreateScren();
        lv_menu(); // ���¼�������Ļ
        i = 0;
        return;
    }
        lv_label_set_text_fmt(DC_Motor_label_speed, "DC_Motor_Speed : %d RPM", DC_Motor_speed_val);    /* �����ٶ�ֵ */
        lv_label_set_text_fmt(Step_Motor_label_speed, "Step_Motor_Speed : %d RPM", Step_Motor_speed_val);    /* �����ٶ�ֵ */
        lv_label_set_text_fmt(Bldc_Motor_label_speed, "Bldc_Motor_Speed : %d RPM", Bldc_Motor_speed_val);    /* �����ٶ�ֵ */
        lv_label_set_text_fmt(Servo_label_speed, "Servo_Speed : %d RPM", Servo_speed_val);    /* �����ٶ�ֵ */
}
extern int16_t val;                         /* ��ǰֵ */
extern lv_obj_t *label_per;                     /* �ٷֱȱ�ǩ */
extern lv_obj_t *bar;                           /* ������ */
extern lv_obj_t *correct_bmp;
extern lv_obj_t *incorrect_bmp;
extern uint8_t loading_finish_flag;        //������ɱ�־λ
extern uint8_t pass_flag ;              //ͨ����־λ
extern uint8_t nopass_flag;            //��ͨ����־λ
extern lv_timer_t *timer;

void timer_cb(lv_timer_t *timer)
{
    if(val < 100)                                                           /* ��ǰֵС��100 */
    {
        val ++;
        lv_bar_set_value(bar, val, LV_ANIM_ON);                             /* ���õ�ǰֵ */
        lv_label_set_text_fmt(label_per, "%d %%", lv_bar_get_value(bar));   /* ��ȡ��ǰֵ��������ʾ */
    }
    else                                                                    /* ��ǰֵ���ڵ���100 */
    {
        if(loading_finish_flag == 0)//���ȼ�����ɣ�����У��
        {
            if(correct_flag == 1)//������ȷ
            {
                lv_label_set_text(label_per, "finished!");                          /* ������� */
                //��ʾ��ȷͼ��
                lv_obj_clear_flag(correct_bmp, LV_OBJ_FLAG_HIDDEN);
                lv_obj_align(correct_bmp,LV_ALIGN_CENTER,180, 30);
                //У�����
                loading_finish_flag = 1;

            }
            else if(incorrect_flag == 1)//�������
            {
                lv_label_set_text(label_per, "finished!");                          /* ������� */
                //��ʾ����ȷͼ��
                lv_obj_clear_flag(incorrect_bmp, LV_OBJ_FLAG_HIDDEN);
                lv_obj_align(incorrect_bmp,LV_ALIGN_CENTER,180, 30);
                //У�����
                loading_finish_flag = 1;
  
            }

        }
       else if(loading_finish_flag == 1)//У����ɣ��˳��л���Ļ
       {
            if(correct_flag == 1)//�л�ϵͳҳ��
            {
                delay_ms(1000);
                Refresh_And_CreateScren();
                lv_menu();
                loading_finish_flag = 2;
                lv_timer_del(timer);
                correct_flag = 0;               //�����־λ���˳�
            }  
            else if(incorrect_flag == 1)//�л�����ҳ��
            {
                delay_ms(1000);
                Refresh_And_CreateScren();
                lv_lock_sys();
                loading_finish_flag = 2;
                lv_timer_del(timer);
                incorrect_flag = 0;             //�����־λ���˳�
            }                    
       }
    }
}
extern lv_obj_t *list;              /* �����б� */
extern lv_obj_t *list_label;        /* �����ǩ */
void list_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *list_btn = lv_event_get_target(e);                                   /* ��ȡ����Դ */
//    lv_obj_add_state(list_btn, LV_STATE_FOCUS_KEY);                                /* ���״̬���۽��� */
    lv_label_set_text(list_label, lv_list_get_btn_text(list, list_btn));           /* ��ȡ��ť�ı�����ʾ */

    const char *txt = lv_list_get_btn_text(list, list_btn);
    printf("%s\r\n", txt);//�������һ��ָ���ַ�����ָ�룬�����ʹ��printf������ϸ�ʽ���ַ���"%s"����ӡ����ַ�����
    //�Ƚ������ַ����Ƿ���Ȳ��ܼ򵥵�ʹ�� == ��������
    //��Ϊ��ֻ��Ƚ�����ָ���Ƿ�ָ��ͬһ���ڴ�λ�ã������ǱȽ��ַ��������ݡ�Ҫ�Ƚ��ַ��������ݣ�����Ҫʹ�ñ�׼�⺯�� strcmp �������ַ��Ƚϡ�
    if (strcmp(txt, "MotorControl") == 0)
    {
        Refresh_And_CreateScren();
        lv_motor_control();
    }
    else if (strcmp(txt, "back") == 0)
    {
        Refresh_And_CreateScren();
        lv_lock_sys();
    }

}

