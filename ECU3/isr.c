#include <xc.h>

/* External timing counter */
extern unsigned long int timer_count;

/*---------------------------------------------------------
 * Timer0 Interrupt Service Routine
 *---------------------------------------------------------*/
void __interrupt() isr(void)
{
    if (TMR0IF)                             /* Timer0 overflow interrupt */
    {
        TMR0 = TMR0 + 9;                    /* Reload value (preserves timing) */

        if (timer_count++ == 20000)         /* 20,000 ticks rollover */
        {
            timer_count = 0;
        }

        TMR0IF = 0;                         /* Clear interrupt flag */
    }
}
