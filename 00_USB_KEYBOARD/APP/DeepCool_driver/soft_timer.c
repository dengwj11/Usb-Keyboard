#include "soft_timer.h"


SOFT_TIMER_EVENT_STRUCT g_soft_timer_events[MAX_SOFT_TIMER_AMOUNT];

//软计时器初始化
void SoftTimerInit(void)
{
    unsigned char i = 0;

    for( i = 0; i < MAX_SOFT_TIMER_AMOUNT; i ++ )
    {
			g_soft_timer_events[i].event = NULL;
			g_soft_timer_events[i].delay = 0;
			g_soft_timer_events[i].period = 0;
			g_soft_timer_events[i].runme = 0;
    }
}

void SoftTimerToSleep()
{
	//__WFI();
}
/*删除调度组中的事件*/
void SoftTimerDeleteEvent(const unsigned char index)
{
	if(g_soft_timer_events[index].event == NULL)
		return ;
	
	g_soft_timer_events[index].event = NULL;
	g_soft_timer_events[index].delay = 0;
	g_soft_timer_events[index].period = 0;
	g_soft_timer_events[index].runme = 0;
}
/*主函数while循环中根据标志位启动事件*/
void SoftTimerHandler(void)
{
	unsigned char i;
	
	for(i = 0; i < MAX_SOFT_TIMER_AMOUNT; i++)
	{
		if(g_soft_timer_events[i].runme > 0)
		{
			if(g_soft_timer_events[i].event != NULL)
				(*g_soft_timer_events[i].event)();
			
			g_soft_timer_events[i].runme -=1;
			
			if(g_soft_timer_events[i].period == 0)
				SoftTimerDeleteEvent(i);
		}
	}
	
	//SchedulerGoToSleep();
}
/*添加函数事件进入调度组*/
unsigned char SoftTimerActiveEvent(void (* pFunction)(void), const unsigned short delay, const unsigned short period)
{
	unsigned char i = 0;
	
	if(pFunction == NULL)
		return SOFT_TIMER_ERROR;
	
	while((g_soft_timer_events[i].event != NULL) && (i < MAX_SOFT_TIMER_AMOUNT))
	{
		i++;
	}
		
	
	if(i == MAX_SOFT_TIMER_AMOUNT)
		return SOFT_TIMER_ERROR;
	
	g_soft_timer_events[i].event = pFunction;
	g_soft_timer_events[i].delay = delay;
	g_soft_timer_events[i].period = period;
	g_soft_timer_events[i].runme = 0;
	
	return i;
}


void SoftTimerISR(void)//定时中断中使用，进行计数以控制标志位
{
	unsigned char i;
	
	for(i = 0; i < MAX_SOFT_TIMER_AMOUNT; i++)
	{
		if(g_soft_timer_events[i].event != NULL)
		{
			if(g_soft_timer_events[i].delay == 0)
			{
				g_soft_timer_events[i].runme += 1;
				if(g_soft_timer_events[i].period > 0)
					g_soft_timer_events[i].delay = g_soft_timer_events[i].period;
			}
			else
				g_soft_timer_events[i].delay -= 1;
		}
	}
}

