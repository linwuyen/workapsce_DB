#include "g_timer.h"

/**
  * @brief      Open Timer with Operate Mode and Frequency
  *
  * @param[in]  timer       The pointer of the specified Timer module. It could be TIMER0, TIMER1, TIMER2, TIMER3.
  * @param[in]  u32Mode     Operation mode. Possible options are
  *                         - \ref TIMER_ONESHOT_MODE
  *                         - \ref TIMER_PERIODIC_MODE
  *                         - \ref TIMER_TOGGLE_MODE
  *                         - \ref TIMER_CONTINUOUS_MODE
  * @param[in]  u32Freq     Target working frequency
  *
  * @return     Real timer working frequency
  *
  * @details    This API is used to configure timer to operate in specified mode and frequency.
  *             If timer cannot work in target frequency, a closest frequency will be chose and returned.
  * @note       After calling this API, Timer is \b NOT running yet. But could start timer running be calling
  *             \ref TIMER_Start macro or program registers directly.
  */
uint32_t openTimer(TIMER_T *timer, uint32_t u32Mode, uint32_t u32Freq)
{
    uint32_t u32Clk = TIMER_GetModuleClock(timer);
    uint32_t u32Cmpr = 0, u32Prescale = 0;

    /* Fastest possible timer working freq is (u32Clk / 2). While cmpr = 2, pre-scale = 0. */
    if(u32Freq >= (u32Clk >> 1))
    {
        u32Cmpr = 2;
    }
    else
    {
        u32Cmpr = u32Clk / u32Freq;
        u32Prescale = (u32Cmpr >> 24);  /* for 24 bits CMPDAT */
        if (u32Prescale > 0)
            u32Cmpr = u32Cmpr / (u32Prescale + 1);
    }

    timer->CTL = u32Mode | u32Prescale;
    timer->CMP = u32Cmpr;
		
		timer->CTL |= TIMER_CTL_CNTEN_Msk;

    return(u32Clk / (u32Cmpr * (u32Prescale + 1)));
}

uint32_t getTimerCounts(TIMER_T *timer)
{
    return TIMER_GetCounter(timer);
}


