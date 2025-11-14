#include <xc.h>
#include <stdint.h>
#include "can.h"
#include "clcd.h"
#include "msg_id.h"
#include "msg_handler.h"
#include "timer_0.h"

/*---------------------------------------------------------
 * Initialize LEDs (RB2 = output, RB3 = input, rest output)
 *---------------------------------------------------------*/
static void init_leds(void)
{
    TRISB = 0x08;            /* RB3 input, others output */
    PORTB = 0x00;
}

/*---------------------------------------------------------
 * System Configuration:
 *  - Initialize CLCD
 *  - Initialize CAN bus
 *  - Initialize LEDs
 *  - Enable interrupts
 *  - Initialize Timer0
 *---------------------------------------------------------*/
static void init_system(void)
{
    init_clcd();
    init_can();
    init_leds();

    /* Enable global + peripheral interrupts */
    PEIE = 1;
    GIE  = 1;

    init_timer0();
}

/*---------------------------------------------------------
 * Main Application Entry Point
 *---------------------------------------------------------*/
void main(void)
{
    init_system();
    display_labels();             /* Draw fixed UI on LCD */

    while (1)
    {
        process_canbus_data();    /* Read & handle CAN messages */
    }
}
