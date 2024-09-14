#include "at32_timer.h"

crm_clocks_freq_type crm_clocks_freq_struct = {0};
void Timer1msInit(void)//1ms�������
{
	crm_clocks_freq_get(&crm_clocks_freq_struct);
	crm_periph_clock_enable(CRM_TMR1_PERIPH_CLOCK, TRUE);

	tmr_base_init(TMR1, 9, (crm_clocks_freq_struct.ahb_freq / 10000) - 1);//1ms���ʱ
	tmr_cnt_dir_set(TMR1, TMR_COUNT_UP);//���ϼ���
	tmr_interrupt_enable(TMR1, TMR_OVF_INT, TRUE);//��������ж�
	nvic_irq_enable(TMR1_OVF_TMR10_IRQn, 0, 0);//�ж����ȼ�����
	tmr_counter_enable(TMR1, TRUE);//����ʹ��
}


//extern uint8_t breath_pos;
//extern uint8_t b_flag;
void TMR1_OVF_TMR10_IRQHandler(void)//����ж���ں���
{
//	static uint8_t i1=0;
  if(tmr_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
  {
    /* add user code... */
//		i1++;
    SoftTimerISR();//��ʱ1ms���ʱ�����ȡ�����־λ    
//		if(i1>=20)		
//		{
//			i1=0;
//			if(breath_pos<2)  b_flag=1;
//			if(breath_pos>200)	b_flag=0;
//			if(b_flag)  {breath_pos=breath_pos+2;}
//			else  			{breath_pos=breath_pos-2;}//breath_pos����[1,200]
//		}	
		
		tmr_flag_clear(TMR1, TMR_OVF_FLAG);
  }
//	 if(tmr_interrupt_flag_get(TMR1, TMR_OVF_FLAG) != RESET)
//  {
//    /* add user code... */
//    SoftTimerISR();
//    tmr_flag_clear(TMR1, TMR_OVF_FLAG);
//  }
}

