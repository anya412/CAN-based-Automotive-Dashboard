#include <xc.h>
#include "timer_0.h"

/*---------------------------------------------------------
 * Initialize Timer0
 *  - 8-bit mode
 *  - Internal clock (Fosc / 4)
 *  - Prescaler disabled
 *  - Preload value = 6
 *  - Interrupt enabled
 *---------------------------------------------------------*/
void init_timer0(void)
{
    /* Configure Timer0 for 8-bit operation */
    T08BIT = 1;

    /* Use internal instruction clock (Fosc/4) */
    T0CS = 0;

    /* Disable prescaler (Timer0 increments every instruction cycle) */
    PSA = 1;

    /* Load initial timer value */
    TMR0 = 6;

    /* Clear pending overflow interrupt flag */
    TMR0IF = 0;

    /* Enable Timer0 overflow interrupt */
    TMR0IE = 1;

    /* Enable Timer0 module */
    TMR0ON = 1;
}
